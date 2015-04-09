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

#ifndef __CHECKERS_GAME_DATA_HEADER__
#define __CHECKERS_GAME_DATA_HEADER__

#include <vector>
#include <functional>
#include <SFML/Graphics/Rect.hpp>

/**
 * This class represents the checkers board game data.
 */
class CheckersGameData {
public:
    friend class SaveManager;

    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int BOARD_SIZE = 10;

    static const int EASY_LEVEL   = 0;
    static const int NORMAL_LEVEL = 1;
    static const int HARD_LEVEL   = 2;

    static const int NO_WINNER = 0;
    static const int P1_WINNER = 1;
    static const int P2_WINNER = 2;
    static const int AI_WINNER = 3;

    static const int NO_SIDE    = 0;
    static const int WHITE_SIDE = 1;
    static const int BLACK_SIDE = 2;

    static const int EMPTY_CELL = 0;

    static const int CELL_WIDTH  = 32;
    static const int CELL_HEIGHT = 32;

    static const int WHITE_PIECE       = 4;
    static const int WHITE_QUEEN_PIECE = 5;
    static const int BLACK_PIECE       = 6;
    static const int BLACK_QUEEN_PIECE = 7;

    static const int MOVE_INVALID         = 0;
    static const int MOVE_VALID           = 1;
    static const int MOVE_VALID_WITH_KILL = 2;

    static const int CAN_DO_NOTHING = 0;
    static const int CAN_ONLY_MOVE  = 1;
    static const int CAN_KILL       = 2;

    static const sf::Vector2i NO_CELL;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    typedef int BoardTable[BOARD_SIZE][BOARD_SIZE];
    typedef std::vector<sf::Vector2i> CoordsVector;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool SinglePlayer() const { return singlePlayer_; }
    int Difficulty() const { return difficulty_; }
    int PlayerSide() const { return playerSide_; }
    int Winner() const { return winner_; }
    int Turn() const { return turn_; }
    const CoordsVector & Candidates() const { return candidates_; }

    bool LastTurnFinished() const { return nextPieceToMove_ == NO_CELL; }
    bool GameOver() const { return winner_ != NO_WINNER; }

    //--------------------------------------------------------------------------------
    // General Methods
    //--------------------------------------------------------------------------------

    void Start(bool singlePlayer, int difficulty, int playerSide);
    void Reset();

    void ForEachInBoard(std::function<void (int, int, int)> operation);
    void SetWinner(int side);

    void NextTurn();
    int CountPieces(int side);

    //--------------------------------------------------------------------------------
    // Move Methods
    //--------------------------------------------------------------------------------

    void GetPieceMoveOffsets(int piece, CoordsVector & offsets) const;
    void GetPossibleMoves(CoordsVector & victims, int r, int c) const;
    void GetPossibleMoves(CoordsVector & victims, const sf::Vector2i & coords) const;

    void CalculateCandidates();

    int CanPieceMoveOrKill(int r, int c) const;
    int CanPieceMoveOrKill(const sf::Vector2i & coords) const;

    int ValidateMove(int r1, int c1, int r2, int c2) const;
    int ValidateMove(const sf::Vector2i & c1, const sf::Vector2i & c2) const;

    void MakeMove(int r1, int c1, int r2, int c2);
    void MakeMove(const sf::Vector2i & c1, const sf::Vector2i & c2);

    //--------------------------------------------------------------------------------
    // Query Methods
    //--------------------------------------------------------------------------------

    bool IsInside(int r, int c) const;
    bool IsInside(const sf::Vector2i & coords) const;

    bool IsWorldCell(int r, int c) const;
    bool IsWorldCell(const sf::Vector2i & coords) const;

    bool IsNormalPiece(int piece) const;
    bool IsQueenPiece(int piece) const;
    bool IsBlackPiece(int piece) const;
    bool IsWhitePiece(int piece) const;
    bool IsPieceSide(int piece, int side) const;
    bool IsTurnPiece(int piece) const;

    bool IsPlayerPiece(int r, int c, int side) const;
    bool IsPlayerPiece(int r, int c) const;

    bool IsPlayerPiece(const sf::Vector2i & coords, int side) const;
    bool IsPlayerPiece(const sf::Vector2i & coords) const;

    int GetOppositeSide(int side) const;
    int GetPieceSide(int piece) const;

    bool AnyCandidate() const;
    bool IsCandidate(int r, int c) const;
    bool IsCandidate(const sf::Vector2i & coords) const;

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    CheckersGameData();
    CheckersGameData(const CheckersGameData & source);
    ~CheckersGameData();
    CheckersGameData & operator =(const CheckersGameData & source);

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    bool singlePlayer_;
    int difficulty_;
    int playerSide_;
    int winner_;

    int turn_;
    sf::Vector2i nextPieceToMove_;

    BoardTable board_;
    CoordsVector candidates_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    int countEnemiesBetween(int r1, int c1, int r2, int c2) const;
    int countEnemiesBetween(int r1, int c1, int r2, int c2,
        std::function<void (const sf::Vector2i &)> onEnemyFind) const;

    void checkAndTransform(int r, int c);
};

#endif
