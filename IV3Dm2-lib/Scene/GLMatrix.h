#pragma once

class GLMatrix {
public:
    GLMatrix();
    GLMatrix(const float* rows);
    GLMatrix(const GLMatrix& matrix);

    // cast operators
    operator float*() { return &m[0][0]; }
    operator const float*() const { return &m[0][0]; }

    // subscript operators
    float* operator[](int row) { return m[row]; }
    const float* operator[](int row) const { return m[row]; }

    void scale(float sx, float sy, float sz);
    void translate(float tx, float ty, float tz);
    void rotate(float angle, float x, float y, float z);
    void frustum(float left, float right, float bottom, float top, float nearZ, float farZ);
    void perspective(float fovy, float aspect, float nearZ, float farZ);
    void ortho(float left, float right, float bottom, float top, float nearZ, float farZ);
    void loadMatrix(const GLMatrix& matrix);
    void multiply(const GLMatrix& matrix);
    void multiplyLeft(const GLMatrix& matrix);
    void loadIdentity();
    GLMatrix inverse() const;

private:
    float m[4][4];
};

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
