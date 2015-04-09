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

#include "ReversiGameData.h"
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const int MAX_DIRS = 8;
const sf::Vector2i DIRS[MAX_DIRS] = {
    sf::Vector2i(-1,  1), sf::Vector2i(0,  1), sf::Vector2i( 1,  1), sf::Vector2i( 1, 0),
    sf::Vector2i( 1, -1), sf::Vector2i(0, -1), sf::Vector2i(-1, -1), sf::Vector2i(-1, 0)
};

const int START_CENTER = 3;
const int END_CENTER   = 4;

//********************************************************************************
// General Methods
//********************************************************************************

void ReversiGameData::Start(bool singlePlayer, int difficulty, int playerSide) {
    // Config the one or two players game.
    singlePlayer_ = singlePlayer;
    difficulty_ = difficulty;
    playerSide_ = playerSide;
    // Reset the other logic fields.
    Reset();
}

//--------------------------------------------------------------------------------

void ReversiGameData::Reset() {
    // Initialize the game board.
    for (int i = 0, k = 1; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j, ++k) {
            board_[i][j] = EMPTY_CELL;
        }
    }
    // Set some control information.
    winner_ = NO_WINNER;
    turn_ = WHITE_SIDE;
    beginningState_ = true;
    whiteSideBlocked_ = false;
    blackSideBlocked_ = false;
    // And set the next candidates to move.
    CalculateCandidates();
}

//--------------------------------------------------------------------------------

void ReversiGameData::ForEachInBoard(std::function<void (int, int, int)> operation) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            operation(board_[i][j], i, j);
        }
    }
}

//--------------------------------------------------------------------------------

void ReversiGameData::SetWinner(int side) {
    if (side == BOTH_SIDES) {
        // If both sides win there is a draw.
        winner_ = GAME_DRAW;

    } else if (singlePlayer_ && side != playerSide_) {
        // The machine will win on a single player game when the
        // side of the winner is not the player's side.
        winner_ = AI_WINNER;

    } else {
        // Otherwise will win a human player.
        winner_ = side == WHITE_SIDE ? P1_WINNER : P2_WINNER;
    }
}

//--------------------------------------------------------------------------------

void ReversiGameData::NextTurn() {
    if (winner_ == NO_WINNER) {
        // Only when there is no winner we'll change the current turn and
        // then recalculate the current candidates to be moved.
        turn_ = GetOppositeSide(turn_);
        CalculateCandidates();
        // Here we check the block flag of the current side.
        if (turn_ == BLACK_SIDE) {
            blackSideBlocked_ = candidates_.empty();
        } else {
            whiteSideBlocked_ = candidates_.empty();
        }
        // And finally, check the game over.
        CheckGameOver();
    } else {
        candidates_.clear();
    }
}

//--------------------------------------------------------------------------------

void ReversiGameData::CheckBeginningEnded() {
    bool anyEmpty = false;
    for (int i = START_CENTER; i <= END_CENTER; ++i) {
        for (int j = START_CENTER; j <= END_CENTER; ++j)  {
            if (board_[i][j] == EMPTY_CELL) {
                anyEmpty = true;
                break;
            }
        }
    }
    if (!anyEmpty) {
        beginningState_ = false;
    }
}

//--------------------------------------------------------------------------------

void ReversiGameData::CheckGameOver() {
    // Count all the types of entities inside the board.
    int emptyCount = 0, whiteCount = 0, blackCount = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            auto victim = board_[i][j];
            if (victim == BLACK_SIDE) {
                ++blackCount;
            } else if (victim == WHITE_SIDE) {
                ++whiteCount;
            } else {
                ++emptyCount;
            }
        }
    }
    // Check if the game over is reached and then set the winner.
    if (emptyCount <= 0 || (blackSideBlocked_ && whiteSideBlocked_)) {
        if (whiteCount < blackCount) {
            SetWinner(BLACK_SIDE);
        } else if (whiteCount > blackCount) {
            SetWinner(WHITE_SIDE);
        } else {
            SetWinner(BOTH_SIDES);
        }
    }
}

//********************************************************************************
// Move Methods
//********************************************************************************

void ReversiGameData::CalculateCandidates() {
    candidates_.clear();
    if (beginningState_) {
        // Select the middle empty cells.
        for (int i = START_CENTER; i <= END_CENTER; ++i) {
            for (int j = START_CENTER; j <= END_CENTER; ++j)  {
                if (board_[i][j] == EMPTY_CELL) {
                    candidates_.push_back(sf::Vector2i(j, i));
                }
            }
        }
    } else {
        // Select the empty validated cells.
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (ValidateMove(i, j)) {
                    candidates_.push_back(sf::Vector2i(j, i));
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------

bool ReversiGameData::ValidateMove(int r, int c) const {
    if (IsInside(r, c) && board_[r][c] == EMPTY_CELL) {
        for (int i = 0; i < MAX_DIRS; ++i) {
            if (canTakePieces(r, c, DIRS[i])) {
                return true;
            }
        }
    }
    return false;
}

bool ReversiGameData::ValidateMove(const sf::Vector2i & coords) const {
    return ValidateMove(coords.y, coords.x);
}

//--------------------------------------------------------------------------------

void ReversiGameData::MakeMove(int r, int c) {
    if (IsInside(r, c) && board_[r][c] == EMPTY_CELL) {
        if (beginningState_) {
            if (3 <= r && r <= 4 && 3 <= c && c <= 4) {
                board_[r][c] = turn_;
            }
            CheckBeginningEnded();
        } else {
            for (int i = 0; i < MAX_DIRS; ++i) {
                takePieces(r, c, DIRS[i]);
            }
        }
    }
}

void ReversiGameData::MakeMove(const sf::Vector2i & coords) {
    MakeMove(coords.y, coords.x);
}

//--------------------------------------------------------------------------------

bool ReversiGameData::canTakePieces(int r, int c, const sf::Vector2i & inc) const {
    int enemySide = GetOppositeSide(turn_);
    sf::Vector2i dest(c + inc.x, r + inc.y);
    if (IsInside(dest) && board_[dest.y][dest.x] == enemySide) {
        do {
            if (board_[dest.y][dest.x] == turn_) {
                return true;
            } else if (board_[dest.y][dest.x] == EMPTY_CELL) {
                return false;
            }
            dest += inc;
        } while (IsInside(dest));
    }
    return false;
}

//--------------------------------------------------------------------------------

void ReversiGameData::takePieces(int r, int c, const sf::Vector2i & inc) {
    int enemySide = GetOppositeSide(turn_);
    sf::Vector2i dest(c + inc.x, r + inc.y);
    if (IsInside(dest) && board_[dest.y][dest.x] == enemySide) {
        do {
            if (board_[dest.y][dest.x] == turn_) {
                sf::Vector2i orig(c, r);
                do {
                    dest -= inc;
                    board_[dest.y][dest.x] = turn_;
                } while (orig != dest);
                return;
            } else if (board_[dest.y][dest.x] == EMPTY_CELL) {
                return;
            }
            dest += inc;
        } while (IsInside(dest));
    }
}

//********************************************************************************
// Query Methods
//********************************************************************************

bool ReversiGameData::IsInside(int r, int c) const {
    return 0 <= r && r < BOARD_SIZE && 0 <= c && c < BOARD_SIZE;
}

bool ReversiGameData::IsInside(const sf::Vector2i & coords) const {
    return IsInside(coords.y, coords.x);
}

int ReversiGameData::GetOppositeSide(int side) const {
    if (side == WHITE_SIDE)
        return BLACK_SIDE;
    else if (side == BLACK_SIDE)
        return WHITE_SIDE;
    else
        return NO_SIDE;
}

//--------------------------------------------------------------------------------

bool ReversiGameData::AnyCandidate() const {
    return !candidates_.empty();
}

bool ReversiGameData::IsCandidate(int r, int c) const {
    if (AnyCandidate()) {
        auto end = candidates_.end();
        auto it = std::find(candidates_.begin(), end, sf::Vector2i(c, r));
        if (it != end) return true;
    }
    return false;
}

bool ReversiGameData::IsCandidate(const sf::Vector2i & coords) const {
    return IsCandidate(coords.y, coords.x);
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

ReversiGameData::ReversiGameData() : singlePlayer_(false), difficulty_(NORMAL_LEVEL),
    playerSide_(WHITE_SIDE), winner_(NO_WINNER), turn_(WHITE_SIDE), beginningState_(true),
    whiteSideBlocked_(false), blackSideBlocked_(false), candidates_() {}

//--------------------------------------------------------------------------------

ReversiGameData::ReversiGameData(const ReversiGameData & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

ReversiGameData::~ReversiGameData() {}

//--------------------------------------------------------------------------------

ReversiGameData & ReversiGameData::operator =(const ReversiGameData & source) {
    singlePlayer_ = source.singlePlayer_;
    difficulty_ = source.difficulty_;
    playerSide_ = source.playerSide_;
    winner_ = source.winner_;
    turn_ = source.turn_;
    beginningState_ = source.beginningState_;
    whiteSideBlocked_ = source.whiteSideBlocked_;
    blackSideBlocked_ = source.blackSideBlocked_;
    ForEachInBoard([&] (int, int i, int j) {
        board_[i][j] = source.board_[i][j];
    });
    candidates_ = source.candidates_;
    return *this;
}
