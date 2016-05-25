#include "duality/AbstractIO.h"

// ReaderFromFile

ReaderFromFile::ReaderFromFile() {}

ReaderFromFile::ReaderFromFile(const char* file) {
    open(file);
}

ReaderFromFile::~ReaderFromFile() {
    fs.close();
}

bool ReaderFromFile::open(const char* input, size_t size) {
    fs.open(input, std::ifstream::in | std::ifstream::binary);
    return isOpen();
}

bool ReaderFromFile::isOpen() {
    return fs.is_open();
}

std::streamoff ReaderFromFile::bytesAvailable() {
    std::streamoff bytesRead = fs.tellg();
    fs.seekg(0, std::ifstream::end);
    std::streamoff bytesInFile = fs.tellg();
    fs.seekg(bytesRead);

    return bytesInFile - bytesRead;
}

void ReaderFromFile::close() {
    fs.close();
    fs.clear();
}

std::streamoff ReaderFromFile::read(char* buffer, size_t size) {
    fs.read(buffer, size);
    return fs.gcount();
}

// ReaderFromMemory

ReaderFromMemory::ReaderFromMemory()
    : memory(NULL)
    , bytesRead(0)
    , bytesToRead(0) {}

ReaderFromMemory::ReaderFromMemory(const char* m, size_t size)
    : memory(m)
    , bytesRead(0)
    , bytesToRead(size) {}

bool ReaderFromMemory::open(const char* input, size_t size) {
    memory = input;
    bytesRead = 0;
    bytesToRead = size;
    return isOpen();
}

void ReaderFromMemory::close() {
    memory = NULL;
    bytesRead = 0;
    bytesToRead = 0;
}

bool ReaderFromMemory::isOpen() {
    return (memory != NULL);
}

std::streamoff ReaderFromMemory::bytesAvailable() {
    return bytesToRead;
}

std::streamoff ReaderFromMemory::read(char* buffer, size_t size) {
    if (size > bytesToRead)
        size = bytesToRead;
    memcpy(buffer, memory + bytesRead, size);
    bytesRead += size;
    bytesToRead -= size;
    return size;
}