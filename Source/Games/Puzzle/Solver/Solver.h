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

#ifndef __NPUZZLE_SOLVER_H__
#define __NPUZZLE_SOLVER_H__

#include "Point.h"
#include "Puzzle.h"

namespace NPuzzle {
    /**
     * This class represents a puzzle solver.
     */
    class Solver {
    public:
        //----------------------------------------------------------------------------
        // Constructor
        //----------------------------------------------------------------------------

        Solver(const Puzzle & data);

        //----------------------------------------------------------------------------
        // Methods
        //----------------------------------------------------------------------------

        bool Solve();

    private:
        //----------------------------------------------------------------------------
        // Constants
        //----------------------------------------------------------------------------

        static const int EMPTY = Puzzle::EMPTY;
        static const int RANGE = Puzzle::RANGE;
        static const int LAST_INDEX = Puzzle::LAST_INDEX;

        static const unsigned int INVALID_COORD = 0XFF;

        //----------------------------------------------------------------------------
        // Types
        //----------------------------------------------------------------------------

        struct Cell {
            int value;
            bool blocked;

            Cell() : value(0), blocked(false) {}
            Cell(int v) : value(v), blocked(false) {}
            Cell(int v, bool b) : value(v), blocked(b) {}
        };

        struct Table {
            Cell data[RANGE][RANGE];
            bool visited[RANGE][RANGE];

            void Reset(const Puzzle & start);

            Point FindValuePos(int value) const;

            void Swap(unsigned int i1, unsigned int j1, unsigned int i2, unsigned int j2);
            void ForceSwap(unsigned int i1, unsigned int j1, unsigned int i2, unsigned int j2);

            void Swap(const Point & c1, const Point & c2) {
                Swap(c1.i, c1.j, c2.i, c2.j);
            }
            void ForceSwap(const Point & c1, const Point & c2) {
                ForceSwap(c1.i, c1.j, c2.i, c2.j);
            }

            void Block(const Point & coords);
            void Unblock(const Point & coords);
            bool IsBlocked(const Point & coords) const;

            void Visit(const Point & coords);
            void UnVisit(const Point & coords);
            bool IsVisited(const Point & coords) const;
            void ClearVisited();
        };

        //----------------------------------------------------------------------------
        // Fields
        //----------------------------------------------------------------------------

        Puzzle _start;
        Table _table;

        //----------------------------------------------------------------------------
        // Methods
        //----------------------------------------------------------------------------

        void reset();

        void solveDownSide();
        bool solveUpSide();

        Point findVictimDest(int value);

        bool isTableSolved();

        void solveUpSideLeft(int victim);
        void solveUpSideRight(int victim, Point & victimPos, const Point & victimDest);
        void solveDownSideLeft(int victim, Point & victimPos, const Point & victimDest);
        void solveDownSideRight(int victim, Point & victimPos, const Point & victimDest);

        void loopMoveOnTheTopLeft(Point & victimPos, Point & emptyPos);
        void loopFinalMoveOnTheTopRight(Point & emptyPos);
        void loopFinalMoveOnTheLeft(Point & emptyPos);

        void loopMoveToRight(Point & victimPos, Point & emptyPos);
        void loopMoveToLeft(Point & victimPos, Point & emptyPos);
        void loopMoveToDown(Point & victimPos, Point & emptyPos);

        void longMove(const Point & orig, const Point & dest);
        bool longMoveStep(const Point & orig, const Point & dest);

    };
}

#endif
