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

#include "CheckersMiniMax.h"
#include <limits>
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const int INITIAL_ALPHA = std::numeric_limits<int>::min();
const int INITIAL_BETA = std::numeric_limits<int>::max();

//********************************************************************************
// Methods
//********************************************************************************

CheckersMove CheckersMiniMax::Execute(CheckersGameData & data) {
    initialize(data.Difficulty());
    return minimax(data, 0, INITIAL_ALPHA, INITIAL_BETA);
}

//--------------------------------------------------------------------------------

void CheckersMiniMax::initialize(int difficulty) {
    if (difficulty == CheckersGameData::HARD_LEVEL) {
        maxDepth = 3;
    } else if (difficulty == CheckersGameData::NORMAL_LEVEL) {
        maxDepth = 1;
    } else {
        maxDepth = 1;
    }
}

//--------------------------------------------------------------------------------

void CheckersMiniMax::getAllMoves(CheckersGameData & data, CheckersMoveVector & moves) {
    // First, get all the candidates on the current board.
    auto & candidates = data.Candidates();
    // For each candidate lets get all the moves of it.
    CheckersMoveStepVector currentSteps;
    ForEach(candidates, [&] (const sf::Vector2i & item) {
        getAllMoves(data, item, currentSteps, moves);
    });
}

//--------------------------------------------------------------------------------

void CheckersMiniMax::getAllMoves(CheckersGameData & data, const sf::Vector2i & piece,
    CheckersMoveStepVector & steps, CheckersMoveVector & moves) {
    // Get all the possible moves of the current candidate.
    CheckersGameData::CoordsVector possibleMoves;
    data.GetPossibleMoves(possibleMoves, piece);
    // For each possible move, make the move and change the turn.
    ForEach(possibleMoves, [&] (const sf::Vector2i & destination) {
        CheckersGameData future(data);
        future.MakeMove(piece, destination);
        future.NextTurn();
        // If the turn is finished, add it to the vector of moves.
        // Otherwise get deep in the chain of moves.
        steps.push_back(CheckersMoveStep(piece, destination));
        if (future.LastTurnFinished()) {
            moves.push_back(CheckersMove(steps, future));
        } else {
            getAllMoves(future, destination, steps, moves);
        }
        steps.pop_back();
    });
}

//--------------------------------------------------------------------------------

CheckersMove CheckersMiniMax::minimax(CheckersGameData & data, int depth, int alpha, int beta) {
    CheckersMove result;
    if (!data.GameOver()) {
        // If the game is not over, get all the moves inside the board.
        CheckersMoveVector moves;
        getAllMoves(data, moves);

        // If we have some moves to check, we'll initialize some variables.
        if (!moves.empty()) {
            int actval, maxval = INITIAL_ALPHA;
            auto it = moves.begin(), end = moves.end();
            result = CheckersMove(*it);

            // And then for each move we'll try to get the maximum result.
            for (; it != end; ++it) {
                actval = minimizer(it->finalData, depth + 1, alpha, beta);
                if (maxval < actval) {
                    maxval = actval;
                    alpha = actval;
                    result = CheckersMove(*it);
                }
            }
        }
    }
    return result;
}

//--------------------------------------------------------------------------------

int CheckersMiniMax::minimizer(CheckersGameData & data, int depth, int alpha, int beta) {
    // If we reach the maximum depth o the game is over, we'll evaluate the current
    // state of the game. If the current side does not have any moves the game ends,
    // so is not necesary to check if we have or not moves when there is no game over.
    if (maxDepth < depth || data.GameOver()) {
        return evaluate(data);
    } else {
        // If the game is not over, get all the moves inside the board.
        CheckersMoveVector moves;
        getAllMoves(data, moves);

        // We'll initialize some variables.
        int actval, minval = INITIAL_BETA;
        auto it = moves.begin(), end = moves.end();
        // And then for each move we'll try to get the minimum result.
        for (; it != end; ++it) {
            actval = maximizer(it->finalData, depth + 1, alpha, beta);

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

int CheckersMiniMax::maximizer(CheckersGameData & data, int depth, int alpha, int beta) {
    // If we reach the maximum depth o the game is over, we'll evaluate the current
    // state of the game. If the current side does not have any moves the game ends,
    // so is not necesary to check if we have or not moves when there is no game over.
    if (maxDepth < depth || data.GameOver()) {
        return evaluate(data);
    } else {
        // If the game is not over, get all the moves inside the board.
        CheckersMoveVector moves;
        getAllMoves(data, moves);

        // We'll initialize some variables.
        int actval, maxval = INITIAL_ALPHA;
        auto it = moves.begin(), end = moves.end();
        // And then for each move we'll try to get the maximum result.
        for (; it != end; ++it) {
            actval = minimizer(it->finalData, depth + 1, alpha, beta);

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

int CheckersMiniMax::evaluate(CheckersGameData & data) {
    if (data.GameOver()) {
        if (data.Winner() == CheckersGameData::AI_WINNER) {
            return INITIAL_BETA;
        } else {
            return INITIAL_ALPHA;
        }
    } else {
        int result = 0;
        int machineSide = data.GetOppositeSide(data.PlayerSide());
        int normalValue = 1, queenValue = 10;
        if (data.Difficulty() == CheckersGameData::EASY_LEVEL) {
            queenValue = 1;
        }
        data.ForEachInBoard([&] (int victim, int i, int j) {
            if (data.IsNormalPiece(victim)) {
                if (data.IsPieceSide(victim, machineSide)) {
                    result += normalValue;
                } else {
                    result -= normalValue;
                }
            } else if (data.IsQueenPiece(victim)) {
                if (data.IsPieceSide(victim, machineSide)) {
                    result += queenValue;
                } else {
                    result -= queenValue;
                }
            }
        });
        return result;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

CheckersMiniMax::CheckersMiniMax() {}

//--------------------------------------------------------------------------------

CheckersMiniMax::~CheckersMiniMax() {}
