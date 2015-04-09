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
     * Constructs a new object.
     */
    Solver::Solver(const Puzzle & data) : _start(data) {
    }

    //********************************************************************************
    // .
    //********************************************************************************

    /**
     * Checks if the puzzle can be solved or not.
     */
    bool Solver::Solve() {
        reset();
        solveDownSide();
        return solveUpSide();
    }

    //--------------------------------------------------------------------------------

    /**
     * Resets the internal table.
     */
    void Solver::reset() {
        _table.Reset(_start);
    }

    //--------------------------------------------------------------------------------

    /**
     * Solves the down side of the puzzle.
     */
    void Solver::solveDownSide() {
        const int START = RANGE * RANGE - 1; // 15
        const int END = RANGE * 2 - 1; // 7
        for(int victim = START; victim > END; --victim) {
            Point victimPos = _table.FindValuePos(victim);
            Point victimDest = findVictimDest(victim);

            if(victimPos == victimDest) {
                _table.data[victimPos.i][victimPos.j].blocked = true;
            } else if(victimDest.j == 0) {
                solveDownSideLeft(victim, victimPos, victimDest);
            } else {
                solveDownSideRight(victim, victimPos, victimDest);
            }
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Solves the up side of the puzzle.
     */
    bool Solver::solveUpSide() {
        const int START = RANGE * 2 - 1; // 7
        const int EXCLUDE = 4;

        std::vector<int> candidates;
        for(int v = RANGE - 1; v >= 0; --v) {
            candidates.push_back(v + RANGE);
            candidates.push_back(v);
        }

        int i = 0, len = candidates.size() - EXCLUDE;
        for(; i < len; ++i) {
            auto victim = candidates[i];

            Point victimPos = _table.FindValuePos(victim);
            Point victimDest = findVictimDest(victim);

            if(victimPos == victimDest) {
                _table.data[victimPos.i][victimPos.j].blocked = true;
            } else {
                if(victim < RANGE) {
                    solveUpSideRight(victim, victimPos, victimDest);
                } else {
                    solveDownSideRight(victim, victimPos, victimDest);
                }
            }
        }

        solveUpSideLeft(candidates[i]);
        return isTableSolved();
    }

    //--------------------------------------------------------------------------------

    /**
     * Finds the destination of a victim value.
     */
    Point Solver::findVictimDest(int value) {
        const int LIMIT = RANGE * RANGE;
        return (0 <= value && value < LIMIT) ?
            Point(value / RANGE, value % RANGE) :
        Point(INVALID_COORD, INVALID_COORD);
    }

    //--------------------------------------------------------------------------------

    /**
     * Checks if the table is solved or not.
     */
    bool Solver::isTableSolved() {
        for(int i = 0, k = 0; i < RANGE; ++i) {
            for(int j = 0; j < RANGE; ++j, ++k) {
                if(_table.data[i][j].value != k) return false;
            }
        }
        return true;
    }

    //--------------------------------------------------------------------------------

    /**
     * Solves the up-left side of the puzzle.
     */
    void Solver::solveUpSideLeft(int victim) {
        Point victimPos = _table.FindValuePos(victim);
        Point victimDest = findVictimDest(victim);
        Point emptyPos = _table.FindValuePos(EMPTY);

        auto move = [&](const Point & next) {
            _table.ForceSwap(emptyPos, next);
            emptyPos = next;
        };

        auto move2 = [&](const Point & next) {
            _table.Block(victimPos);
            longMove(emptyPos, next);
            _table.Unblock(victimPos);
            emptyPos = next;
        };

        if(victimPos == victimDest) {
            // ..
            // .V
            move2(Point(0, 0));

        } else if(victimPos.i == victimDest.i) {
            move2(victimDest);

            // ..
            // VE
            move(emptyPos.GetLeftPoint());
            move(emptyPos.GetUpPoint());

        } else if(victimPos.j == victimDest.j) {
            move2(victimDest);

            // .V
            // .E
            move(emptyPos.GetUpPoint());
            move(emptyPos.GetLeftPoint());

        } else {
            move2(Point(victimDest.i - 1, victimDest.j));

            // VE
            // ..
            move(emptyPos.GetLeftPoint());
            move(emptyPos.GetDownPoint());
            move(emptyPos.GetRightPoint());
            move(emptyPos.GetUpPoint());
            move(emptyPos.GetLeftPoint());
        }
    }

    //--------------------------------------------------------------------------------

    /**
     * Solves the up-right side of the puzzle.
     */
    void Solver::solveUpSideRight(int victim, Point & victimPos, const Point & victimDest) {
        Point emptyPos = _table.FindValuePos(EMPTY), emptyDest;
        auto move = [&]() {
            _table.Block(victimPos);
            longMove(emptyPos, emptyDest);
            _table.Unblock(victimPos);

            _table.Swap(victimPos, emptyDest);
            emptyPos = victimPos;
            victimPos = emptyDest;
        };

        auto move2 = [&]() {
            _table.Block(victimPos);
            longMove(emptyPos, emptyDest);
            _table.Unblock(victimPos);
            emptyPos = emptyDest;
        };

        if(victimPos.i == victimDest.i && victimPos.j + 1 == victimDest.j) {
            if(emptyPos == victimDest) {
                // VE
                // .P
                _table.Swap(victimPos, emptyPos);

            } else {
                emptyDest.i = victimPos.i + 1;
                emptyDest.j = victimPos.j;
                if(emptyPos != emptyDest) move2();

                // V.
                // EP
                loopFinalMoveOnTheTopRight(emptyPos);
            }
        } else {
            if(victimPos.i == 0) {
                emptyDest.i = victimPos.i;
                emptyDest.j = victimPos.j + 1;
                move();

            } else {
                emptyDest.i = victimPos.i - 1;
                emptyDest.j = victimPos.j;
                move();

                if(victimPos.j < victimDest.j - 1) {
                    loopMoveOnTheTopLeft(victimPos, emptyPos);
                }
            }

            // EV. -> Right
            // ...
            while(victimPos.j < victimDest.j - 1) {
                loopMoveToRight(victimPos, emptyPos);
            }

            emptyDest.i = victimPos.i + 1;
            emptyDest.j = victimPos.j;
            if(emptyPos != victimPos) {
                move2();
            }

            // V.
            // EP
            loopFinalMoveOnTheTopRight(emptyPos);
        }

        _table.Block(victimDest);
    }

    //--------------------------------------------------------------------------------

    /**
     * Solves the down-left side of the puzzle.
     */
    void Solver::solveDownSideLeft(int victim, Point & victimPos, const Point & victimDest) {
        Point emptyPos = _table.FindValuePos(EMPTY), emptyDest;
        auto move = [&]() {
            _table.Block(victimPos);
            longMove(emptyPos, emptyDest);
            _table.Unblock(victimPos);

            _table.Swap(victimPos, emptyDest);
            emptyPos = victimPos;
            victimPos = emptyDest;
        };

        auto move2 = [&]() {
            _table.Block(victimPos);
            longMove(emptyPos, emptyDest);
            _table.Unblock(victimPos);
            emptyPos = emptyDest;
        };

        if (emptyPos == victimDest && victimPos.j == victimDest.j && victimPos.i + 1 == victimDest.i) {
            // V.
            // EP
            _table.Swap(emptyPos, victimPos);

        } else {
            if (victimPos.j == victimDest.j && victimPos.i + 1 == victimDest.i) {
                // V.
                // .P
                emptyDest = victimPos.GetUpPoint();
                move();
            }

            emptyDest = victimDest;
            longMove(emptyPos, emptyDest);
            emptyPos = emptyDest;

            Point prevDest = emptyPos.GetRightPoint();
            _table.ForceSwap(emptyPos, prevDest);
            emptyPos = prevDest;

            if(victimPos.j > victimDest.j) {
                emptyDest.i = victimPos.i;
                emptyDest.j = victimPos.j - 1;
                move();

                // ....
                // .VE. <- Left
                // ....
                while(victimPos.j > victimDest.j) {
                    loopMoveToLeft(victimPos, emptyPos);
                }
            }

            if(victimPos.i < victimDest.i - 1) {
                emptyDest.i = victimPos.i + 1;
                emptyDest.j = victimPos.j;
                move();

                while(victimPos.i < victimDest.i - 1) {
                    loopMoveToDown(victimPos, emptyPos);
                }
            }

            emptyDest = prevDest;
            move2();

            loopFinalMoveOnTheLeft(emptyPos);
        }

        _table.Block(victimDest);
    }

    //--------------------------------------------------------------------------------

    /**
     * Solves the down-right side of the puzzle.
     */
    void Solver::solveDownSideRight(int victim, Point & victimPos, const Point & victimDest) {
        Point emptyPos = _table.FindValuePos(EMPTY), emptyDest;
        auto move = [&]() {
            _table.Block(victimPos);
            longMove(emptyPos, emptyDest);
            _table.Unblock(victimPos);

            _table.Swap(victimPos, emptyDest);
            emptyPos = victimPos;
            victimPos = emptyDest;
        };

        if(victimPos.j < victimDest.j) {
            emptyDest.i = victimPos.i;
            emptyDest.j = victimPos.j + 1;
            move();

            // ....
            // .EV. -> Right
            // ....
            while(victimPos.j < victimDest.j) {
                loopMoveToRight(victimPos, emptyPos);
            }
        } else if(victimPos.j > victimDest.j) {
            emptyDest.i = victimPos.i;
            emptyDest.j = victimPos.j - 1;
            move();

            // ....
            // .VE. <- Left
            // ....
            while(victimPos.j > victimDest.j) {
                loopMoveToLeft(victimPos, emptyPos);
            }
        }

        if(victimPos.i < victimDest.i) {
            emptyDest.i = victimPos.i + 1;
            emptyDest.j = victimPos.j;
            move();

            while(victimPos.i < victimDest.i) {
                loopMoveToDown(victimPos, emptyPos);
            }
        }

        _table.Block(victimDest);
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a loop move on the top left side of the puzzle.
     */
    void Solver::loopMoveOnTheTopLeft(Point & victimPos, Point & emptyPos) {
        Point pos = emptyPos;
        auto move = [&](const Point & next) {
            _table.ForceSwap(pos, next);
            pos = next;
        };

        // .V.  .VX  .VE
        // .EX  ..E  ...
        move(pos.GetRightPoint());
        move(pos.GetUpPoint());
        move(pos.GetLeftPoint());

        emptyPos = victimPos;
        ++(victimPos.j);
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a loop final move on the top right side of the puzzle.
     */
    void Solver::loopFinalMoveOnTheTopRight(Point & emptyPos) {
        Point pos = emptyPos;
        auto move = [&](const Point & next) {
            _table.ForceSwap(pos, next);
            pos = next;
        };

        // .V.  XV.  EV.  VEX  V.E  V.P  VXP
        // XEP  E.P  ..P  ..P  ..P  .XE  .E.
        move(pos.GetLeftPoint());
        move(pos.GetUpPoint());
        move(pos.GetRightPoint());
        move(pos.GetRightPoint());
        move(pos.GetDownPoint());
        move(pos.GetLeftPoint());
        move(pos.GetUpPoint());

        // VEP  EVP  .VP  .VP  .VP  .VE
        // ...  X..  EX.  .EX  ..E  ..P
        move(pos.GetLeftPoint());
        move(pos.GetDownPoint());
        move(pos.GetRightPoint());
        move(pos.GetRightPoint());
        move(pos.GetUpPoint());
        move(pos.GetLeftPoint());
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a loop final move on the left side of the puzzle.
     */
    void Solver::loopFinalMoveOnTheLeft(Point & emptyPos) {
        Point pos = emptyPos;
        auto move = [&](const Point & next) {
            _table.ForceSwap(pos, next);
            pos = next;
        };

        // V.  V.
        // PE  EP
        move(pos.GetLeftPoint());
        move(pos.GetUpPoint());
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a loop move to the right.
     */
    void Solver::loopMoveToRight(Point & victimPos, Point & emptyPos) {
        Point pos = emptyPos;
        auto move = [&](const Point & next) {
            _table.Swap(pos, next);
            pos = next;
        };

        // .EV. | .X..
        // .X.. | .EV.
        if(victimPos.i == 0) {
            move(pos.GetDownPoint());
        } else {
            move(pos.GetUpPoint());
        }

        // ..V. | .EX.
        // .EX. | ..V.
        move(pos.GetRightPoint());

        // ..V. | ..EX
        // ..EX | ..V.
        move(pos.GetRightPoint());

        // ..VX | ...E
        // ...E | ..VX
        if(victimPos.i == 0) {
            move(pos.GetUpPoint());
        } else {
            move(pos.GetDownPoint());
        }

        // ..VE | ....
        // .... | ..VE
        move(pos.GetLeftPoint());

        ++(victimPos.j);
        ++(emptyPos.j);
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a loop move to the left.
     */
    void Solver::loopMoveToLeft(Point & victimPos, Point & emptyPos) {
        Point pos = emptyPos;
        auto move = [&](const Point & next) {
            _table.Swap(pos, next);
            pos = next;
        };

        // .VE. | ..X.
        // ..X. | .VE.
        if(victimPos.i == 0) {
            move(pos.GetDownPoint());
        } else {
            move(pos.GetUpPoint());
        }

        // .V.. | .XE.
        // .XE. | .V..
        move(pos.GetLeftPoint());

        // .V.. | XE..
        // XE.. | .V..
        move(pos.GetLeftPoint());

        // XV.. | E...
        // E... | XV..
        if(victimPos.i == 0) {
            move(pos.GetUpPoint());
        } else {
            move(pos.GetDownPoint());
        }

        // EV.. | ....
        // .... | EV..
        move(pos.GetRightPoint());

        --(victimPos.j);
        --(emptyPos.j);
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a loop move to the bottom.
     */
    void Solver::loopMoveToDown(Point & victimPos, Point & emptyPos) {
        Point pos = emptyPos;
        auto move = [&](const Point & next) {
            _table.Swap(pos, next);
            pos = next;
        };

        // XE  E.  ..  ..  ..
        // .V  XV  EV  .V  .V
        // ..  ..  X.  EX  .E

        if(victimPos.j > 0) {
            move(pos.GetLeftPoint());
        } else {
            move(pos.GetRightPoint());
        }

        move(pos.GetDownPoint());
        move(pos.GetDownPoint());

        if(victimPos.j > 0) {
            move(pos.GetRightPoint());
        } else {
            move(pos.GetLeftPoint());
        }

        move(pos.GetUpPoint());

        ++(victimPos.i);
        ++(emptyPos.i);
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a long move.
     */
    void Solver::longMove(const Point & orig, const Point & dest) {
        if(orig == dest) return;
        _table.ClearVisited();
        longMoveStep(orig, dest);
    }

    //--------------------------------------------------------------------------------

    /**
     * Executes a step inside a long move.
     */
    bool Solver::longMoveStep(const Point & orig, const Point & dest) {
        if(orig == dest) return true;

        std::vector<Point> candidates;
        if(orig.i < dest.i) {
            if(orig.j < dest.j) { // UP-LEFT
                candidates.push_back(orig.GetDownPoint());
                candidates.push_back(orig.GetRightPoint());
                candidates.push_back(orig.GetLeftPoint());
                candidates.push_back(orig.GetUpPoint());
            } else { // UP-RIGHT
                candidates.push_back(orig.GetDownPoint());
                candidates.push_back(orig.GetLeftPoint());
                candidates.push_back(orig.GetRightPoint());
                candidates.push_back(orig.GetUpPoint());
            }
        } else if(orig.i > dest.i) {
            if(orig.j < dest.j) { // DOWN-LEFT
                candidates.push_back(orig.GetUpPoint());
                candidates.push_back(orig.GetRightPoint());
                candidates.push_back(orig.GetLeftPoint());
                candidates.push_back(orig.GetDownPoint());
            } else { // DOWN-RIGHT
                candidates.push_back(orig.GetUpPoint());
                candidates.push_back(orig.GetLeftPoint());
                candidates.push_back(orig.GetRightPoint());
                candidates.push_back(orig.GetDownPoint());
            }
        } else {
            if(orig.j < dest.j) { // DOWN-LEFT
                candidates.push_back(orig.GetRightPoint());
                candidates.push_back(orig.GetUpPoint());
                candidates.push_back(orig.GetDownPoint());
                candidates.push_back(orig.GetLeftPoint());
            } else if(orig.j > dest.j) { // DOWN-RIGHT
                candidates.push_back(orig.GetLeftPoint());
                candidates.push_back(orig.GetUpPoint());
                candidates.push_back(orig.GetDownPoint());
                candidates.push_back(orig.GetRightPoint());
            }
        }

        _table.Visit(orig);

        auto i = candidates.begin();
        auto end = candidates.end();
        for(; i != end; ++i) {
            if(_table.IsBlocked(*i) == false && _table.IsVisited(*i) == false) {
                _table.Swap(orig.i, orig.j, i->i, i->j);
                bool result = longMoveStep(*i, dest);
                if(!result) {
                    _table.Swap(orig, *i);
                } else {
                    return true;
                }
            }
        }

        _table.UnVisit(orig);

        return false;
    }
}
