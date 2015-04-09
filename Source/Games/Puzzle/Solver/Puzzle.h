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

#ifndef __NPUZZLE_PUZZLE_H__
#define __NPUZZLE_PUZZLE_H__

#include <vector>

namespace NPuzzle {
    /**
     * This class represents a puzzle.
     */
    class Puzzle {
    public:
        //----------------------------------------------------------------------------
        // Constants
        //----------------------------------------------------------------------------

        static const int EMPTY = 0;
        static const int RANGE = 4;
        static const int LAST_INDEX = RANGE - 1;

        //----------------------------------------------------------------------------
        // Constructors, destructor and operators
        //----------------------------------------------------------------------------

        Puzzle();
        Puzzle(const std::vector<int> & data);
        Puzzle(const Puzzle & rhs);

        Puzzle & operator=(const Puzzle & rhs);
        bool operator==(const Puzzle & rhs) const;

        //----------------------------------------------------------------------------
        // Methods
        //----------------------------------------------------------------------------

        bool Solved() const;

        bool MoveUp();
        bool MoveDown();
        bool MoveLeft();
        bool MoveRight();

        std::vector<int> GetData() const;
        int GetData(unsigned int i, unsigned int j) const;

        int GetEmptyI() const { return _emptyI; }
        int GetEmptyJ() const { return _emptyJ; }

    private:
        //----------------------------------------------------------------------------
        // Fields
        //----------------------------------------------------------------------------

        int _table[RANGE][RANGE];
        int _emptyI, _emptyJ;

        //----------------------------------------------------------------------------
        // Methods
        //----------------------------------------------------------------------------

        void defaultInit();
    };
}

#endif
