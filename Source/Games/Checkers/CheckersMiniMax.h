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

#ifndef __CHECKERS_MINIMAX_HEADER__
#define __CHECKERS_MINIMAX_HEADER__

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <Games/Checkers/CheckersGameData.h>

/**
 * This structure represents a step inside a move.
 */
struct CheckersMoveStep {
    sf::Vector2i origin;
    sf::Vector2i destination;
    CheckersMoveStep() {}
    CheckersMoveStep(const sf::Vector2i & orig, const sf::Vector2i & dest)
        : origin(orig), destination(dest) {}
};

/**
 * This type represents a vector steps inside a move.
 */
typedef std::vector<CheckersMoveStep> CheckersMoveStepVector;

/**
 * This structure represents a move in the game.
 */
struct CheckersMove {
    CheckersMoveStepVector steps;
    CheckersGameData finalData;
    CheckersMove() {}
    CheckersMove(const CheckersMoveStepVector & st, const CheckersGameData & fd)
        : steps(st), finalData(fd) {}
};

/**
 * This type represents a vector of moves.
 */
typedef std::vector<CheckersMove> CheckersMoveVector;

/**
 * This class represents the mini-max algorithm.
 */
class CheckersMiniMax {
public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    CheckersMove Execute(CheckersGameData & data);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    CheckersMiniMax();
    virtual ~CheckersMiniMax();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int maxDepth;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void initialize(int difficulty);
    void getAllMoves(CheckersGameData & data, CheckersMoveVector & moves);
    void getAllMoves(CheckersGameData & data, const sf::Vector2i & piece,
        CheckersMoveStepVector & steps, CheckersMoveVector & moves);
    CheckersMove minimax(CheckersGameData & data, int depth, int alpha, int beta);
    int minimizer(CheckersGameData & data, int depth, int alpha, int beta);
    int maximizer(CheckersGameData & data, int depth, int alpha, int beta);
    int evaluate(CheckersGameData & data);
};

#endif
