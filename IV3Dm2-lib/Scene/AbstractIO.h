#pragma once

#include <fstream>

class AbstractReader {
public:
    virtual ~AbstractReader() {}
    virtual bool open(const char* input, size_t size = 0) = 0;
    virtual bool isOpen() = 0;
    virtual std::streamoff bytesAvailable() = 0;
    virtual void close() = 0;
    virtual std::streamoff read(char* buffer, size_t size) = 0;
};

class AbstractWriter {
public:
    virtual ~AbstractWriter() {}
    virtual bool open(const char* output, size_t size = 0) = 0;
    virtual bool isOpen() = 0;
    virtual std::streamoff bytesAvailable() = 0;
    virtual void close() = 0;
    virtual size_t write(const char* buffer, size_t size) = 0;
};

class ReaderFromFile : public AbstractReader {
public:
    ReaderFromFile();
    ReaderFromFile(const char* file);
    virtual ~ReaderFromFile();

    bool open(const char* input, size_t size = 0);
    bool isOpen();
    std::streamoff bytesAvailable();
    void close();
    std::streamoff read(char* buffer, size_t size);

private:
    std::ifstream fs;
};

class ReaderFromMemory : public AbstractReader {
public:
    ReaderFromMemory();
    ReaderFromMemory(const char* m, size_t size);

    bool open(const char* input, size_t size = 0);
    void close();
    bool isOpen();
    std::streamoff bytesAvailable();
    std::streamoff read(char* buffer, size_t size);

private:
    const char* memory;
    size_t bytesRead;
    size_t bytesToRead;
};
