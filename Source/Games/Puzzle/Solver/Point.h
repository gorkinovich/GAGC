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

#ifndef __NPUZZLE_POINT_H__
#define __NPUZZLE_POINT_H__

namespace NPuzzle {
    /**
     * This structure represents a point inside a puzzle.
     */
    struct Point {
        //----------------------------------------------------------------------------
        // Fields
        //----------------------------------------------------------------------------

        unsigned int i, j;

        //----------------------------------------------------------------------------
        // Constructors, destructor and operators
        //----------------------------------------------------------------------------

        Point() : i(0), j(0) {}
        Point(unsigned int a, unsigned int b) : i(a), j(b) {}

        bool operator== (const Point & rhs) const {
            return i == rhs.i && j == rhs.j;
        }

        bool operator!= (const Point & rhs) const {
            return i != rhs.i || j != rhs.j;
        }

        //----------------------------------------------------------------------------
        // Methods
        //----------------------------------------------------------------------------

        Point GetUpPoint() const { return Point(i - 1, j); }
        Point GetDownPoint() const { return Point(i + 1, j); }
        Point GetLeftPoint() const { return Point(i, j - 1); }
        Point GetRightPoint() const { return Point(i, j + 1); }
    };
}

#endif