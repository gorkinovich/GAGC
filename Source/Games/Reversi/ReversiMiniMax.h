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

#ifndef __REVERSI_MINIMAX_HEADER__
#define __REVERSI_MINIMAX_HEADER__

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <Games/Reversi/ReversiGameData.h>

/**
 * This structure represents a move in the game.
 */
struct ReversiMove {
    sf::Vector2i step;

    ReversiMove() {}
    ReversiMove(const sf::Vector2i & st) : step(st) {}

    void MakeMove(ReversiGameData & victim) {
        victim.MakeMove(step);
        victim.NextTurn();
    }
};

/**
 * This type represents a vector of moves.
 */
typedef std::vector<ReversiMove> ReversiMoveVector;

/**
 * This class represents the mini-max algorithm.
 */
class ReversiMiniMax {
public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    ReversiMove Execute(ReversiGameData & data);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    ReversiMiniMax();
    virtual ~ReversiMiniMax();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int maxDepth;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void initialize(int difficulty);
    void getAllMoves(ReversiGameData & data, ReversiMoveVector & moves);
    ReversiMove minimax(ReversiGameData & data, int depth, int alpha, int beta);
    int minimizer(ReversiGameData & data, int depth, int alpha, int beta);
    int maximizer(ReversiGameData & data, int depth, int alpha, int beta);
    int evaluate(ReversiGameData & data);
};

#endif
