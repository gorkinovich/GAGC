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

#include "ReversiMiniMax.h"
#include <limits>
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const int INITIAL_ALPHA = std::numeric_limits<int>::min();
const int INITIAL_BETA = std::numeric_limits<int>::max();

const int REVERSI_HEURISTIC[ReversiGameData::BOARD_SIZE][ReversiGameData::BOARD_SIZE] = {
    { 10, 5, 5, 5, 5, 5, 5, 10 },
    {  5, 1, 1, 1, 1, 1, 1,  5 },
    {  5, 1, 1, 1, 1, 1, 1,  5 },
    {  5, 1, 1, 1, 1, 1, 1,  5 },
    {  5, 1, 1, 1, 1, 1, 1,  5 },
    {  5, 1, 1, 1, 1, 1, 1,  5 },
    {  5, 1, 1, 1, 1, 1, 1,  5 },
    { 10, 5, 5, 5, 5, 5, 5, 10 }
};

//********************************************************************************
// Methods
//********************************************************************************

ReversiMove ReversiMiniMax::Execute(ReversiGameData & data) {
    initialize(data.Difficulty());
    return minimax(data, 0, INITIAL_ALPHA, INITIAL_BETA);
}

//--------------------------------------------------------------------------------

void ReversiMiniMax::initialize(int difficulty) {
    if (difficulty == ReversiGameData::HARD_LEVEL) {
        maxDepth = 5;
    } else if (difficulty == ReversiGameData::NORMAL_LEVEL) {
        maxDepth = 3;
    } else {
        maxDepth = 3;
    }
}

//--------------------------------------------------------------------------------

void ReversiMiniMax::getAllMoves(ReversiGameData & data, ReversiMoveVector & moves) {
    // First, get all the candidates on the current board.
    auto & candidates = data.Candidates();
    // For each candidate lets get all the moves of it.
    ForEach(candidates, [&] (const sf::Vector2i & item) {
        moves.push_back(ReversiMove(item));
    });
}

//--------------------------------------------------------------------------------

ReversiMove ReversiMiniMax::minimax(ReversiGameData & data, int depth, int alpha, int beta) {
    ReversiMove result;
    if (!data.GameOver()) {
        // If the game is not over, get all the moves inside the board.
        ReversiMoveVector moves;
        getAllMoves(data, moves);

        // If we have some moves to check, we'll initialize some variables.
        if (!moves.empty()) {
            int actval, maxval = INITIAL_ALPHA;
            auto it = moves.begin(), end = moves.end();
            result = ReversiMove(*it);

            // And then for each move we'll try to get the maximum result.
            for (; it != end; ++it) {
                ReversiGameData future(data);
                it->MakeMove(future);
                actval = minimizer(future, depth + 1, alpha, beta);
                if (maxval < actval) {
                    maxval = actval;
                    alpha = actval;
                    result = ReversiMove(*it);
                }
            }
        }
    }
    return result;
}

//--------------------------------------------------------------------------------

int ReversiMiniMax::minimizer(ReversiGameData & data, int depth, int alpha, int beta) {
    // If we reach the maximum depth o the game is over, we'll evaluate the current
    // state of the game. If the current side does not have any moves the game ends,
    // so is not necesary to check if we have or not moves when there is no game over.
    if (maxDepth < depth || data.GameOver()) {
        return evaluate(data);
    } else {
        // If the game is not over, get all the moves inside the board.
        ReversiMoveVector moves;
        getAllMoves(data, moves);

        // We'll initialize some variables.
        int actval, minval = INITIAL_BETA;
        auto it = moves.begin(), end = moves.end();
        // And then for each move we'll try to get the minimum result.
        for (; it != end; ++it) {
            ReversiGameData future(data);
            it->MakeMove(future);
            actval = maximizer(future, depth + 1, alpha, beta);

            if(minval > actval) {
                minval = actval;
                beta = actval;
            }

            if(alpha > beta) {
                return minval;
            }
        }
        return minval;
    }
}

//--------------------------------------------------------------------------------

int ReversiMiniMax::maximizer(ReversiGameData & data, int depth, int alpha, int beta) {
    // If we reach the maximum depth o the game is over, we'll evaluate the current
    // state of the game. If the current side does not have any moves the game ends,
    // so is not necesary to check if we have or not moves when there is no game over.
    if (maxDepth < depth || data.GameOver()) {
        return evaluate(data);
    } else {
        // If the game is not over, get all the moves inside the board.
        ReversiMoveVector moves;
        getAllMoves(data, moves);

        // We'll initialize some variables.
        int actval, maxval = INITIAL_ALPHA;
        auto it = moves.begin(), end = moves.end();
        // And then for each move we'll try to get the maximum result.
        for (; it != end; ++it) {
            ReversiGameData future(data);
            it->MakeMove(future);
            actval = minimizer(future, depth + 1, alpha, beta);

            if(maxval < actval) {
                maxval = actval;
                alpha = actval;
            }

            if(alpha > beta) {
                return maxval;
            }
        }
        return maxval;
    }
}

//--------------------------------------------------------------------------------

int ReversiMiniMax::evaluate(ReversiGameData & data) {
    if (data.GameOver()) {
        if (data.Winner() == ReversiGameData::AI_WINNER) {
            return INITIAL_BETA;
        } else if (data.Winner() == ReversiGameData::BOTH_SIDES) {
            return 0;
        } else {
            return INITIAL_ALPHA;
        }
    } else {
        int result = 0;
        int machineSide = data.GetOppositeSide(data.PlayerSide());
        if (data.Difficulty() == ReversiGameData::EASY_LEVEL) {
            const int NORMAL_VALUE = 1;
            data.ForEachInBoard([&] (int victim, int i, int j) {
                if (victim == machineSide) {
                    result += NORMAL_VALUE;
                } else {
                    result -= NORMAL_VALUE;
                }
            });
        } else {
            data.ForEachInBoard([&] (int victim, int i, int j) {
                if (victim == machineSide) {
                    result += REVERSI_HEURISTIC[i][j];
                } else {
                    result -= REVERSI_HEURISTIC[i][j];
                }
            });
        }
        return result;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

ReversiMiniMax::ReversiMiniMax() {}

//--------------------------------------------------------------------------------

ReversiMiniMax::~ReversiMiniMax() {}
