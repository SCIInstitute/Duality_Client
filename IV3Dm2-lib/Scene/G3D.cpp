#include "AbstractIO.h"
#include "G3D.h"

#include <string>

void G3D::writeHeader(AbstractWriter& writer, const GeometryInfo& info, const uint32_t* const vertexType) {
    writer.write((char*)&info.isOpaque, sizeof(bool));
    writer.write((char*)&info.numberPrimitives, sizeof(uint32_t));
    writer.write((char*)&info.primitiveType, sizeof(uint32_t));

    uint32_t numberSemantics = (uint32_t)info.attributeSemantics.size();
    writer.write((char*)&numberSemantics, sizeof(uint32_t));

    writer.write((char*)&info.numberIndices, sizeof(uint32_t));
    writer.write((char*)&info.indexSize, sizeof(uint32_t));
    writer.write((char*)&info.numberVertices, sizeof(uint32_t));
    writer.write((char*)&info.vertexSize, sizeof(uint32_t));

    writer.write((char*)(vertexType ? vertexType : &info.vertexType), sizeof(uint32_t));

    writer.write((char*)&(info.attributeSemantics.at(0)), sizeof(uint32_t) * numberSemantics);
}

void G3D::writeIndices(AbstractWriter& writer, const uint32_t* const indices, const GeometryInfo& info) {
    writer.write((char*)indices, info.numberIndices * info.indexSize);
}

void G3D::writeVertices(AbstractWriter& writer, const float* const vertices, const GeometryInfo& info) {
    writer.write((char*)vertices, info.numberVertices * info.vertexSize);
}

void G3D::writeContent(AbstractWriter& writer, const GeometryAoS& geometry) {
    writeIndices(writer, geometry.indices, geometry.info);
    writeVertices(writer, geometry.vertices, geometry.info);
}

void G3D::write(AbstractWriter& writer, const GeometryAoS* const geometry, uint32_t vertexType) {
    if (writer.isOpen()) {
        if (vertexType == AoS) {
            writeHeader(writer, geometry->info);
            writeContent(writer, *geometry);
        } else if (vertexType == SoA) {
            writeHeader(writer, geometry->info, &vertexType);
            writeIndices(writer, geometry->indices, geometry->info);
            std::vector<float*> vertexAttributes;
            convertVertices(geometry->vertices, vertexAttributes, geometry->info);
            writeVertices(writer, vertexAttributes, geometry->info);
            cleanVertices(vertexAttributes);
        }
    }
}

void G3D::writeVertices(AbstractWriter& writer, const std::vector<float*>& vertexAttributes, const GeometryInfo& info) {
    uint32_t i = 0;
    for (std::vector<uint32_t>::const_iterator it = info.attributeSemantics.begin(); it != info.attributeSemantics.end(); ++it) {
        writer.write((char*)vertexAttributes.at(i), info.numberVertices * floats(*it) * sizeof(float));
        ++i;
    }
}

void G3D::writeContent(AbstractWriter& writer, const GeometrySoA& geometry) {
    writeIndices(writer, geometry.indices, geometry.info);
    writeVertices(writer, geometry.vertexAttributes, geometry.info);
}

void G3D::write(AbstractWriter& writer, const GeometrySoA* const geometry, uint32_t vertexType) {
    if (writer.isOpen()) {
        if (vertexType == SoA) {
            writeHeader(writer, geometry->info);
            writeContent(writer, *geometry);
        } else if (vertexType == AoS) {
            writeHeader(writer, geometry->info, &vertexType);
            writeIndices(writer, geometry->indices, geometry->info);
            float* vertices = NULL;
            convertVertices(geometry->vertexAttributes, vertices, geometry->info);
            writeVertices(writer, vertices, geometry->info);
            cleanVertices(vertices);
        }
    }
}

void G3D::readHeader(AbstractReader& reader, GeometryInfo& info) {
    char* buffer = new char[8 * sizeof(uint32_t) + sizeof(bool)];
    memset(buffer, 0, 8 * sizeof(uint32_t) + sizeof(bool));
    reader.read(buffer, 8 * sizeof(uint32_t) + sizeof(bool));
    info.isOpaque = ((buffer++)[0] == 1);
    info.numberPrimitives = ((uint32_t*)buffer)[0];
    info.primitiveType = ((uint32_t*)buffer)[1];
    uint32_t numberSemantics = ((uint32_t*)buffer)[2];
    info.numberIndices = ((uint32_t*)buffer)[3];
    info.indexSize = ((uint32_t*)buffer)[4];
    info.numberVertices = ((uint32_t*)buffer)[5];
    info.vertexSize = ((uint32_t*)buffer)[6];
    info.vertexType = ((uint32_t*)buffer)[7];
    delete[]-- buffer;

    uint32_t divisor =
        (info.primitiveType == Point) ? 1 : (info.primitiveType == Line) ? 2 : (info.primitiveType == Triangle)
                                                                                   ? 3
                                                                                   : (info.primitiveType == TriangleAdj) ? 6 : 0;
    if (divisor > 0 && (info.numberIndices / divisor) == info.numberPrimitives) {
        buffer = new char[numberSemantics * sizeof(uint32_t)];
        reader.read(buffer, numberSemantics * sizeof(uint32_t));
        for (uint32_t i = 0; i < numberSemantics; ++i) {
            info.attributeSemantics.push_back(((uint32_t*)buffer)[i]);
        }
        delete[] buffer;
    } else
        info.numberVertices = 0;
}

void G3D::readIndices(AbstractReader& reader, uint32_t*& indices, const GeometryInfo& info) {
    indices = (uint32_t*)new char[info.numberIndices * info.indexSize];
    reader.read((char*)indices, info.numberIndices * info.indexSize);
}

void G3D::readVertices(AbstractReader& reader, float*& vertices, const GeometryInfo& info) {
    vertices = (float*)new char[info.numberVertices * info.vertexSize];
    reader.read((char*)vertices, info.numberVertices * info.vertexSize);
}

void G3D::readContent(AbstractReader& reader, GeometryAoS& geometry) {
    readIndices(reader, geometry.indices, geometry.info);
    readVertices(reader, geometry.vertices, geometry.info);
}

void G3D::convertVertices(const std::vector<float*>& vertexAttributes, float*& vertices, const GeometryInfo& info) {
    vertices = (float*)new char[info.numberVertices * info.vertexSize];

    uint32_t vertexFloats = info.vertexSize / sizeof(float);
    for (uint32_t i = 0; i < info.numberVertices; ++i) {
        uint32_t offset = 0;
        uint32_t attributeIndex = 0;
        for (std::vector<uint32_t>::const_iterator it = info.attributeSemantics.begin(); it != info.attributeSemantics.end(); ++it) {
            uint32_t attributeFloats = floats(*it);
            for (uint32_t j = 0; j < attributeFloats; ++j)
                vertices[j + offset + (i * vertexFloats)] = vertexAttributes[attributeIndex][j + (i * attributeFloats)];
            offset += attributeFloats;
            ++attributeIndex;
        }
    }
}

void G3D::read(AbstractReader& reader, GeometryAoS* const geometry) {
    if (reader.isOpen()) {
        readHeader(reader, geometry->info);
        if (geometry->info.numberVertices == 0)
            return;
        if (geometry->info.vertexType == AoS)
            readContent(reader, *geometry);
        else if (geometry->info.vertexType == SoA) {
            geometry->info.vertexType = AoS;
            readIndices(reader, geometry->indices, geometry->info);
            std::vector<float*> vertexAttributes;
            readVertices(reader, vertexAttributes, geometry->info);
            convertVertices(vertexAttributes, geometry->vertices, geometry->info);
            cleanVertices(vertexAttributes);
        }
    }
}

void G3D::readVertices(AbstractReader& reader, std::vector<float*>& vertexAttributes, const GeometryInfo& info) {
    for (uint32_t i = 0; i < info.attributeSemantics.size(); ++i) {
        vertexAttributes.push_back(NULL);
        uint32_t attributeFloats = floats(info.attributeSemantics.at(i));
        vertexAttributes.at(i) = new float[info.numberVertices * attributeFloats];
        reader.read((char*)vertexAttributes.at(i), info.numberVertices * attributeFloats * sizeof(float));
    }
}

void G3D::readContent(AbstractReader& reader, GeometrySoA& geometry) {
    readIndices(reader, geometry.indices, geometry.info);
    readVertices(reader, geometry.vertexAttributes, geometry.info);
}

void G3D::convertVertices(const float* const vertices, std::vector<float*>& vertexAttributes, const GeometryInfo& info) {
    uint32_t i = 0;
    for (std::vector<uint32_t>::const_iterator it = info.attributeSemantics.begin(); it != info.attributeSemantics.end(); ++it) {
        vertexAttributes.push_back(NULL);
        uint32_t attributeFloats = floats(*it);
        vertexAttributes.at(i) = new float[info.numberVertices * attributeFloats];
        ++i;
    }

    uint32_t vertexFloats = info.vertexSize / sizeof(float);
    for (uint32_t i = 0; i < info.numberVertices; ++i) {
        uint32_t offset = 0;
        uint32_t attributeIndex = 0;
        for (std::vector<uint32_t>::const_iterator it = info.attributeSemantics.begin(); it != info.attributeSemantics.end(); ++it) {
            uint32_t attributeFloats = floats(*it);
            for (uint32_t j = 0; j < attributeFloats; ++j)
                vertexAttributes[attributeIndex][j + (i * attributeFloats)] = vertices[j + offset + (i * vertexFloats)];
            offset += attributeFloats;
            ++attributeIndex;
        }
    }
}

void G3D::read(AbstractReader& reader, GeometrySoA* const geometry) {
    if (reader.isOpen()) {
        readHeader(reader, geometry->info);
        if (geometry->info.numberVertices == 0)
            return;
        if (geometry->info.vertexType == SoA)
            readContent(reader, *geometry);
        else if (geometry->info.vertexType == AoS) {
            geometry->info.vertexType = SoA;
            readIndices(reader, geometry->indices, geometry->info);
            float* vertices = NULL;
            readVertices(reader, vertices, geometry->info);
            convertVertices(vertices, geometry->vertexAttributes, geometry->info);
            cleanVertices(vertices);
        }
    }
}

void G3D::cleanIndices(uint32_t* indices) {
    delete[] indices;
    indices = NULL;
}

void G3D::cleanVertices(float* vertices) {
    delete[] vertices;
    vertices = NULL;
}

void G3D::cleanVertices(std::vector<float*>& vertexAttributes) {
    for (std::vector<float*>::iterator it = vertexAttributes.begin(); it != vertexAttributes.end(); ++it)
        delete[] * it;
    vertexAttributes.clear();
}

void G3D::clean(GeometryAoS* geometry) {
    if (geometry) {
        cleanIndices(geometry->indices);
        cleanVertices(geometry->vertices);
        geometry->info.attributeSemantics.clear();
    }
}

void G3D::clean(GeometrySoA* geometry) {
    if (geometry) {
        cleanIndices(geometry->indices);
        cleanVertices(geometry->vertexAttributes);
        geometry->info.attributeSemantics.clear();
    }
}


std::string G3D::printPrimitiveType(const Geometry* const geometry) {
    return ((geometry->info.primitiveType == Point) ? "Point" : (geometry->info.primitiveType == Line)
                                                                    ? "Line"
                                                                    : (geometry->info.primitiveType == Triangle)
                                                                          ? "Triangle"
                                                                          : (geometry->info.primitiveType == TriangleAdj)
                                                                                ? "Triangle with adjacency"
                                                                                : "Unknown");
}

std::string G3D::printVertexType(const Geometry* const geometry) {
    return ((geometry->info.vertexType == AoS) ? "Array of Structs" : (geometry->info.vertexType == SoA) ? "Struct of Arrays" : "Unknown");
}

std::string G3D::printAttributeSemantics(const Geometry* const geometry) {
    std::string as;
    for (size_t i = 0; i < geometry->info.attributeSemantics.size(); ++i) {
        if (i)
            as.append(", ");
        auto const& it = geometry->info.attributeSemantics[i];
        as.append(((it) == Position)
                      ? "Position"
                      : ((it) == Normal) ? "Normal"
                                         : ((it) == Tangent)
                                               ? "Tangent"
                                               : ((it) == Color) ? "Color" : ((it) == Tex) ? "Tex" : ((it) == Float) ? "Float" : "Unknown");
        as.append(" (" + std::to_string(floats(it)) + "f)");
    }
    return as;
}

void G3D::print(const Geometry* const geometry, std::ostream& output) {
    if (geometry) {
        output << "Opaque: " << (geometry->info.isOpaque ? "yes" : "no") << std::endl;
        output << "Number primitives: " << geometry->info.numberPrimitives << std::endl;
        output << "Primitive type: " << printPrimitiveType(geometry) << std::endl;
        output << "Number indices: " << geometry->info.numberIndices << std::endl;
        output << "Index size: " << geometry->info.indexSize << std::endl;
        output << "Number vertices: " << geometry->info.numberVertices << std::endl;
        output << "Vertex size: " << geometry->info.vertexSize << std::endl;
        output << "Vertex type: " << printVertexType(geometry) << std::endl;
        output << "Vertex attribute semantics: " << printAttributeSemantics(geometry) << std::endl;
    }
}

/*
 The MIT License

 Copyright (c) 2012 Interactive Visualization and Data Analysis Group
 Saarland University, DFKI, Intel VCI, MMCI

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */