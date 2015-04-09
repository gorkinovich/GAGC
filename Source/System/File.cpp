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

#include "File.h"

//********************************************************************************
// Methods
//********************************************************************************

void File::Read(std::string & victim) {
    char buffer;
    std::string overkill = "";
    do {
        stream_.read(&buffer, sizeof(char));
        if (buffer != '\0') {
            overkill += buffer;
        }
    } while (buffer != '\0' && !stream_.eof());
    if (buffer == '\0') {
        victim = overkill;
    }
}

//--------------------------------------------------------------------------------

void File::Write(const std::string & victim) {
    auto size = victim.size() + 1;
    auto * buffer = victim.c_str();
    stream_.write(buffer, size);
}
