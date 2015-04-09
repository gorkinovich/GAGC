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

#ifndef __REVERSI_GAME_DATA_HEADER__
#define __REVERSI_GAME_DATA_HEADER__

#include <vector>
#include <functional>
#include <SFML/Graphics/Rect.hpp>

/**
 * This class represents the reversi board game data.
 */
class ReversiGameData {
public:
    friend class SaveManager;

    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int BOARD_SIZE = 8;

    static const int EASY_LEVEL   = 0;
    static const int NORMAL_LEVEL = 1;
    static const int HARD_LEVEL   = 2;

    static const int NO_WINNER = 0;
    static const int P1_WINNER = 1;
    static const int P2_WINNER = 2;
    static const int AI_WINNER = 3;
    static const int GAME_DRAW = 4;

    static const int NO_SIDE    = 0;
    static const int BOTH_SIDES = 1;
    static const int WHITE_SIDE = 2;
    static const int BLACK_SIDE = 3;

    static const int EMPTY_CELL = 0;

    static const int CELL_WIDTH  = 40;
    static const int CELL_HEIGHT = 40;

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
    bool BeginningState() const { return beginningState_; }
    bool WhiteSideBlocked() const { return whiteSideBlocked_; }
    bool BlackSideBlocked() const { return blackSideBlocked_; }
    const CoordsVector & Candidates() const { return candidates_; }

    bool GameOver() const { return winner_ != NO_WINNER; }

    //--------------------------------------------------------------------------------
    // General Methods
    //--------------------------------------------------------------------------------

    void Start(bool singlePlayer, int difficulty, int playerSide);
    void Reset();

    void ForEachInBoard(std::function<void (int, int, int)> operation);
    void SetWinner(int side);

    void NextTurn();
    void CheckBeginningEnded();
    void CheckGameOver();

    //--------------------------------------------------------------------------------
    // Move Methods
    //--------------------------------------------------------------------------------

    void CalculateCandidates();

    bool ValidateMove(int r, int c) const;
    bool ValidateMove(const sf::Vector2i & coords) const;

    void MakeMove(int r, int c);
    void MakeMove(const sf::Vector2i & coords);

    //--------------------------------------------------------------------------------
    // Query Methods
    //--------------------------------------------------------------------------------

    bool IsInside(int r, int c) const;
    bool IsInside(const sf::Vector2i & coords) const;
    int GetOppositeSide(int side) const;

    bool AnyCandidate() const;
    bool IsCandidate(int r, int c) const;
    bool IsCandidate(const sf::Vector2i & coords) const;

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    ReversiGameData();
    ReversiGameData(const ReversiGameData & source);
    ~ReversiGameData();
    ReversiGameData & operator =(const ReversiGameData & source);

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    bool singlePlayer_;
    int difficulty_;
    int playerSide_;
    int winner_;

    int turn_;
    bool beginningState_;
    bool whiteSideBlocked_;
    bool blackSideBlocked_;

    BoardTable board_;
    CoordsVector candidates_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    bool canTakePieces(int r, int c, const sf::Vector2i & inc) const;
    void takePieces(int r, int c, const sf::Vector2i & inc);
};

#endif
