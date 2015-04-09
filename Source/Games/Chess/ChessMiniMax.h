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

#ifndef __CHESS_MINIMAX_HEADER__
#define __CHESS_MINIMAX_HEADER__

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <Games/Chess/ChessGameData.h>

/**
 * This structure represents a move in the game.
 */
struct ChessMove {
    sf::Vector2i origin;
    sf::Vector2i destination;
    ChessMove();
    ChessMove(const sf::Vector2i & orig, const sf::Vector2i & dest);
    bool MakeMove(ChessGameData & victim);
};

/**
 * This type represents a vector of moves.
 */
typedef std::vector<ChessMove> ChessMoveVector;

/**
 * This class represents the mini-max algorithm.
 */
class ChessMiniMax {
public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize(int difficulty);
    ChessMove Execute(ChessGameData & data);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    ChessMiniMax();
    virtual ~ChessMiniMax();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int maxDepth_;
    int state_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void checkState(ChessGameData & data);

    void getPossibleAllMoves(ChessGameData & data, ChessMoveVector & moves);

    ChessMove minimax(ChessGameData & data, int depth, int alpha, int beta);
    int minimizer(ChessGameData & data, int depth, int alpha, int beta);
    int maximizer(ChessGameData & data, int depth, int alpha, int beta);

    bool checkStop(ChessGameData & data, int depth);

    int evaluate(ChessGameData & data, int depth);
    int evaluate(ChessGameData & data);

    int evaluatePosition(ChessGameData & data, ChessGameData::Piece & victim);
    int evaluatePositionNormal(ChessGameData::Piece & victim);
    int evaluatePositionQueenOrRook(ChessGameData & data, ChessGameData::Piece & victim);

    bool isInDanger(ChessGameData & data, ChessGameData::Piece & victim);
    void findDefenses(ChessGameData & data, ChessGameData::Piece & victim,
        std::vector<int> & allies);
    void findThreats(ChessGameData & data, ChessGameData::Piece & victim,
        std::vector<int> & enemies);
};

#endif
