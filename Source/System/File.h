/******************************************************************************
 Copyright (c) 2014 Gorka Suárez García

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
******************************************************************************/

#ifndef __FILE_HEADER__
#define __FILE_HEADER__

#include <string>
#include <fstream>

/**
 * This class represents a file.
 */
class File {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const auto BINARY_MODE = std::fstream::binary;
    static const auto READ_MODE   = std::fstream::in | BINARY_MODE;
    static const auto WRITE_MODE  = std::fstream::out | std::fstream::trunc | BINARY_MODE;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool IsOpen() const { return stream_.is_open(); }
    bool Fail() const { return stream_.fail(); }
    bool Good() const { return stream_.good(); }
    bool EndOfFile() const { return stream_.eof(); }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    bool Open(const std::string & path, int mode) {
        stream_.open(path, mode);
        return IsOpen();
    }

    void Close() {
        stream_.close();
    }

    bool OpenForRead(const std::string & path) { return Open(path, READ_MODE); }
    bool OpenForWrite(const std::string & path) { return Open(path, WRITE_MODE); }

    // Read
    template<typename T>
    void Read(T & victim) {
        if (!stream_.eof()) {
            stream_.read((char *)(&victim), sizeof(T));
        }
    }

    void Read(std::string & victim);

    // Write
    template<typename T>
    void Write(T victim) {
        stream_.write((char *)(&victim), sizeof(T));
    }

    void Write(const std::string & victim);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    File() : stream_() {}
    ~File() {}

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    std::fstream stream_;
};

#endif
