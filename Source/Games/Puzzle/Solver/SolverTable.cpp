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

#include "Solver.h"

namespace NPuzzle {
    //********************************************************************************
    // .
    //********************************************************************************

    /**
     * Resets the table internal data.
     */
    void Solver::Table::Reset(const Puzzle & start) {
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                data[i][j].value = start.GetData(i, j);
                data[i][j].blocked = false;
                visited[i][j] = false;
            }
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Finds the position of a value.
     */
    Point Solver::Table::FindValuePos(int value) const {
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                if(data[i][j].value == value)
                    return Point(i, j);
            }
        }
        return Point(INVALID_COORD, INVALID_COORD);
    }

    //--------------------------------------------------------------------------------

    /**
     * Swaps two cells from the table.
     */
    void Solver::Table::Swap(unsigned int i1, unsigned int j1, unsigned int i2, unsigned int j2) {
        if(i1 < RANGE && j1 < RANGE && i2 < RANGE && j2 < RANGE &&
            !data[i1][j1].blocked && !data[i2][j2].blocked) {
            std::swap(data[i1][j1], data[i2][j2]);
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Forces the swap of two cells from the table.
     */
    void Solver::Table::ForceSwap(unsigned int i1, unsigned int j1, unsigned int i2, unsigned int j2) {
        if(i1 < RANGE && j1 < RANGE && i2 < RANGE && j2 < RANGE) {
            std::swap(data[i1][j1], data[i2][j2]);
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Blocks a cell inside the table.
     */
    void Solver::Table::Block(const Point & coords) {
        if(coords.i < RANGE && coords.j < RANGE) {
            data[coords.i][coords.j].blocked = true;
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Unblocks a cell inside the table.
     */
    void Solver::Table::Unblock(const Point & coords) {
        if(coords.i < RANGE && coords.j < RANGE) {
            data[coords.i][coords.j].blocked = false;
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Checks if a cell is blocked or not.
     */
    bool Solver::Table::IsBlocked(const Point & coords) const {
        return (coords.i < RANGE && coords.j < RANGE) ?
            data[coords.i][coords.j].blocked : true;
    }

    //--------------------------------------------------------------------------------

    /**
     * Marks a cell as visited.
     */
    void Solver::Table::Visit(const Point & coords) {
        if(coords.i < RANGE && coords.j < RANGE) {
            visited[coords.i][coords.j] = true;
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Marks a cell as not visited.
     */
    void Solver::Table::UnVisit(const Point & coords) {
        if(coords.i < RANGE && coords.j < RANGE) {
            visited[coords.i][coords.j] = false;
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Checks if a cell is visited or not.
     */
    bool Solver::Table::IsVisited(const Point & coords) const {
        return (coords.i < RANGE && coords.j < RANGE) ?
            visited[coords.i][coords.j] : true;
    }

    //--------------------------------------------------------------------------------

    /**
     * Clears all the visited flags inside the table.
     */
    void Solver::Table::ClearVisited() {
        for(int i = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j) {
                visited[i][j] = false;
            }
        }
    }
}
