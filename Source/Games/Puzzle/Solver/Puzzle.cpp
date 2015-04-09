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

#include "Puzzle.h"

namespace NPuzzle {
    //********************************************************************************
    // Constructors, destructor and operators
    //********************************************************************************

    /**
     * Constructs a new object.
     */
    Puzzle::Puzzle() {
        defaultInit();
    }

    //--------------------------------------------------------------------------------

    /**
     * Constructs a new object.
     */
    Puzzle::Puzzle(const std::vector<int> & data) {
        auto begin = data.begin();
        auto end = data.end();
        int k = 0;
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                if(begin != end) {
                    _table[i][j] = *begin;
                    ++begin;
                    if(_table[i][j] == EMPTY) {
                        _emptyI = i;
                        _emptyJ = j;
                    }
                } else {
                    _table[i][j] = EMPTY;
                    _emptyI = i;
                    _emptyJ = j;
                    ++k;
                }
            }
        }
        if(k > 1) defaultInit();
    }

    //--------------------------------------------------------------------------------

    /**
     * The copy constructor of the object.
     */
    Puzzle::Puzzle(const Puzzle & rhs) {
        *this = rhs;
    }

    //--------------------------------------------------------------------------------

    /**
     * The assign operator of the object.
     */
    Puzzle & Puzzle::operator=(const Puzzle & rhs) {
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                _table[i][j] = rhs._table[i][j];
            }
        }
        _emptyI = rhs._emptyI;
        _emptyJ = rhs._emptyJ;
        return *this;
    }

    //--------------------------------------------------------------------------------

    /**
     * The equality operator of the object.
     */
    bool Puzzle::operator==(const Puzzle & rhs) const {
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                if(_table[i][j] != rhs._table[i][j])
                    return false;
            }
        }
        return true;
    }

    //********************************************************************************
    // Methods
    //********************************************************************************

    /**
     * Checks if the puzzle is solved or not.
     */
    bool Puzzle::Solved() const {
        const auto MAX_VICTIM = RANGE * RANGE;
        int victim = EMPTY;
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                if(_table[i][j] != victim) {
                    return false;
                } else {
                    victim = (victim + 1) % MAX_VICTIM;
                }
            }
        }
        return true;
    }

    //--------------------------------------------------------------------------------

    /**
     * Moves up the empty cell.
     */
    bool Puzzle::MoveUp() {
        if(_emptyI > 0) {
            auto nextI = _emptyI - 1;
            std::swap(_table[nextI][_emptyJ], _table[_emptyI][_emptyJ]);
            _emptyI = nextI;
            return true;
        }
        return false;
    }

    //--------------------------------------------------------------------------------

    /**
     * Moves down the empty cell.
     */
    bool Puzzle::MoveDown() {
        if(_emptyI < LAST_INDEX) {
            auto nextI = _emptyI + 1;
            std::swap(_table[nextI][_emptyJ], _table[_emptyI][_emptyJ]);
            _emptyI = nextI;
            return true;
        }
        return false;
    }

    //--------------------------------------------------------------------------------

    /**
     * Moves left the empty cell.
     */
    bool Puzzle::MoveLeft() {
        if(_emptyJ > 0) {
            auto nextJ = _emptyJ - 1;
            std::swap(_table[_emptyI][nextJ], _table[_emptyI][_emptyJ]);
            _emptyJ = nextJ;
            return true;
        }
        return false;
    }

    //--------------------------------------------------------------------------------

    /**
     * Moves right the empty cell.
     */
    bool Puzzle::MoveRight() {
        if(_emptyJ < LAST_INDEX) {
            auto nextJ = _emptyJ + 1;
            std::swap(_table[_emptyI][nextJ], _table[_emptyI][_emptyJ]);
            _emptyJ = nextJ;
            return true;
        }
        return false;
    }

    //--------------------------------------------------------------------------------

    /**
     * Gets the internal data of the puzzle.
     */
    std::vector<int> Puzzle::GetData() const {
        std::vector<int> result;
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                result.push_back(_table[i][j]);
            }
        }
        return result;
    }

    //--------------------------------------------------------------------------------

    /**
     * Gets a cell of the puzzle.
     */
    int Puzzle::GetData(unsigned int i, unsigned int j) const {
        return (i < RANGE && j < RANGE) ? _table[i][j] : -1;
    }

    //--------------------------------------------------------------------------------

    /**
     * Initializes the puzzle with the default data.
     */
    void Puzzle::defaultInit() {
        const auto MAX_VICTIM = RANGE * RANGE;
        int victim = EMPTY;
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                _table[i][j] = victim;
                victim = (victim + 1) % MAX_VICTIM;
            }
        }
        _emptyI = _emptyJ = RANGE - 1;
    }
}
