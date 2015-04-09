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

#include "CheckersGameData.h"
#include <System/MathUtil.h>
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const int LAST_WHITE_ROW = 9;
const int LAST_BLACK_ROW = 0;

const sf::Vector2i CheckersGameData::NO_CELL = sf::Vector2i(-1, -1);

//********************************************************************************
// General Methods
//********************************************************************************

void CheckersGameData::Start(bool singlePlayer, int difficulty, int playerSide) {
    // Config the one or two players game.
    singlePlayer_ = singlePlayer;
    difficulty_ = difficulty;
    playerSide_ = playerSide;
    // Reset the other logic fields.
    Reset();
}

//--------------------------------------------------------------------------------

void CheckersGameData::Reset() {
    // Initialize the game board.
    for (int i = 0, k = 1; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j, ++k) {
            if (i <= 3) {
                board_[i][j] = IsWorldCell(i, j) ? WHITE_PIECE : EMPTY_CELL;
            } else if (6 <= i) {
                board_[i][j] = IsWorldCell(i, j) ? BLACK_PIECE : EMPTY_CELL;
            } else {
                board_[i][j] = EMPTY_CELL;
            }
        }
    }
    // Set some control information.
    winner_ = NO_WINNER;
    turn_ = WHITE_SIDE;
    nextPieceToMove_ = NO_CELL;
    // And set the next candidates to move.
    CalculateCandidates();
}

//--------------------------------------------------------------------------------

void CheckersGameData::ForEachInBoard(std::function<void (int, int, int)> operation) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            operation(board_[i][j], i, j);
        }
    }
}

//--------------------------------------------------------------------------------

void CheckersGameData::SetWinner(int side) {
    if (singlePlayer_ && side != playerSide_) {
        // The machine will win on a single player game when the
        // side of the winner is not the player's side.
        winner_ = AI_WINNER;
    } else {
        // Otherwise will win a human player.
        winner_ = side == WHITE_SIDE ? P1_WINNER : P2_WINNER;
    }
}

//--------------------------------------------------------------------------------

void CheckersGameData::NextTurn() {
    if (winner_ == NO_WINNER) {
        // Only when there is no winner and there is no piece with
        // a next kill to achieve, we'll change the current turn.
        if (nextPieceToMove_ == NO_CELL) {
            turn_ = GetOppositeSide(turn_);
        }
        // Recalculate the current candidates to be moved.
        CalculateCandidates();
    } else {
        candidates_.clear();
    }
}

//--------------------------------------------------------------------------------

int CheckersGameData::CountPieces(int side) {
    int count = 0;
    ForEachInBoard([&] (int victim, int i, int j) {
        if (IsPieceSide(victim, side)) {
            ++count;
        }
    });
    return count;
}

//********************************************************************************
// Move Methods
//********************************************************************************

void CheckersGameData::GetPieceMoveOffsets(int piece, CoordsVector & offsets) const {
    offsets.clear();
    switch (piece) {
    case WHITE_PIECE:
        offsets.push_back(sf::Vector2i(-1,  1));
        offsets.push_back(sf::Vector2i( 1,  1));
        offsets.push_back(sf::Vector2i(-2,  2));
        offsets.push_back(sf::Vector2i( 2,  2));
        offsets.push_back(sf::Vector2i( 2, -2));
        offsets.push_back(sf::Vector2i(-2, -2));
        break;
    case BLACK_PIECE:
        offsets.push_back(sf::Vector2i( 1, -1));
        offsets.push_back(sf::Vector2i(-1, -1));
        offsets.push_back(sf::Vector2i(-2,  2));
        offsets.push_back(sf::Vector2i( 2,  2));
        offsets.push_back(sf::Vector2i( 2, -2));
        offsets.push_back(sf::Vector2i(-2, -2));
        break;
    case WHITE_QUEEN_PIECE:
    case BLACK_QUEEN_PIECE:
        offsets.push_back(sf::Vector2i(-1,  1));
        offsets.push_back(sf::Vector2i( 1,  1));
        offsets.push_back(sf::Vector2i( 1, -1));
        offsets.push_back(sf::Vector2i(-1, -1));
        offsets.push_back(sf::Vector2i(-2,  2));
        offsets.push_back(sf::Vector2i( 2,  2));
        offsets.push_back(sf::Vector2i( 2, -2));
        offsets.push_back(sf::Vector2i(-2, -2));
        offsets.push_back(sf::Vector2i(-3,  3));
        offsets.push_back(sf::Vector2i( 3,  3));
        offsets.push_back(sf::Vector2i( 3, -3));
        offsets.push_back(sf::Vector2i(-3, -3));
        offsets.push_back(sf::Vector2i(-4,  4));
        offsets.push_back(sf::Vector2i( 4,  4));
        offsets.push_back(sf::Vector2i( 4, -4));
        offsets.push_back(sf::Vector2i(-4, -4));
        offsets.push_back(sf::Vector2i(-5,  5));
        offsets.push_back(sf::Vector2i( 5,  5));
        offsets.push_back(sf::Vector2i( 5, -5));
        offsets.push_back(sf::Vector2i(-5, -5));
        offsets.push_back(sf::Vector2i(-6,  6));
        offsets.push_back(sf::Vector2i( 6,  6));
        offsets.push_back(sf::Vector2i( 6, -6));
        offsets.push_back(sf::Vector2i(-6, -6));
        offsets.push_back(sf::Vector2i(-7,  7));
        offsets.push_back(sf::Vector2i( 7,  7));
        offsets.push_back(sf::Vector2i( 7, -7));
        offsets.push_back(sf::Vector2i(-7, -7));
        break;
    }
}

//--------------------------------------------------------------------------------

void CheckersGameData::GetPossibleMoves(CoordsVector & victims, int r, int c) const {
    victims.clear();
    if (IsInside(r, c)) {
        // We'll only get the possible moves of a piece inside the board.
        auto piece = board_[r][c];
        auto orig = sf::Vector2i(c, r);

        // Here we'll get the piece move offsets to calculate the next moves.
        CoordsVector offsets;
        GetPieceMoveOffsets(piece, offsets);
        CoordsVector movesWithKill, movesNormal;

        // For each offset we'll validate the possible destination move.
        // We also will differentiate between a normal move and a kill move.
        ForEach(offsets, [&] (const sf::Vector2i & item) {
            auto dest = orig + item;
            auto validation = ValidateMove(orig, dest);
            if (validation == MOVE_VALID_WITH_KILL) {
                movesWithKill.push_back(dest);
            } else if (validation == MOVE_VALID) {
                movesNormal.push_back(dest);
            }
        });

        // If we doesn't have any kill move, we'll get the normal ones.
        // Otherwise we have to take the kill moves because is mandatory to
        // kill if there is an option in the checkers game.
        if (movesWithKill.empty()) {
            victims = movesNormal;
        } else if (IsNormalPiece(piece)) {
            victims = movesWithKill;
        } else {
            // There is also a differentiation factor when the piece that
            // kills is a queen. In that case we'll get only the moves
            // that have a second kill if exists. Otherwise we'll get all
            // the other kill moves.
            CoordsVector movesWithSecondKill;
            ForEach(movesWithKill, [&] (const sf::Vector2i & item) {
                CheckersGameData future(*this);
                future.MakeMove(orig, item);
                if (!future.LastTurnFinished()) {
                    movesWithSecondKill.push_back(item);
                }
            });
            if (movesWithSecondKill.empty()) {
                victims = movesWithKill;
            } else {
                victims = movesWithSecondKill;
            }
        }
    }
}

void CheckersGameData::GetPossibleMoves(CoordsVector & victims, const sf::Vector2i & coords) const {
    GetPossibleMoves(victims, coords.y, coords.x);
}

//--------------------------------------------------------------------------------

void CheckersGameData::CalculateCandidates() {
    candidates_.clear();
    if (nextPieceToMove_ == NO_CELL) {
        // First, get all the pieces coordinates of the correct side.
        CoordsVector pieces;
        if (turn_ == WHITE_SIDE) {
            ForEachInBoard([&] (int victim, int i, int j) {
                if (IsWhitePiece(victim)) {
                    pieces.push_back(sf::Vector2i(j, i));
                }
            });
        } else if (turn_ == BLACK_SIDE) {
            ForEachInBoard([&] (int victim, int i, int j) {
                if (IsBlackPiece(victim)) {
                    pieces.push_back(sf::Vector2i(j, i));
                }
            });
        }

        // Second, check if any of those pieces can move or kill.
        CoordsVector piecesWithKill, piecesWithMove;
        ForEach(pieces, [&] (const sf::Vector2i & item) {
            auto result = CanPieceMoveOrKill(item);
            if (result == CAN_KILL) {
                piecesWithKill.push_back(item);
            } else if (result == CAN_ONLY_MOVE) {
                piecesWithMove.push_back(item);
            }

        });

        // If we doesn't have any piece with a kill move, we'll get all the
        // others with only move. Otherwise we have to take the kill moves
        // because is mandatory to kill if there is an option in the game.
        if (piecesWithKill.empty()) {
            candidates_ = piecesWithMove;
        } else {
            candidates_ = piecesWithKill;
        }

        // If at the end of the process we don't have any candidate
        // the opposite side will win the game.
        if (candidates_.empty()) {
            SetWinner(GetOppositeSide(turn_));
        }
    } else {
        // You still have moves to do with the previous killer piece.
        candidates_.push_back(nextPieceToMove_);
    }
}

//--------------------------------------------------------------------------------

int CheckersGameData::CanPieceMoveOrKill(int r, int c) const {
    if (IsInside(r, c)) {
        // Only check pieces inside the board of the current playing side.
        auto piece = board_[r][c];
        if (IsTurnPiece(piece)) {
            // Get the piece move offsets to calculate the next moves.
            CoordsVector offsets;
            GetPieceMoveOffsets(piece, offsets);

            // For each offset we'll validate the possible destination move.
            // We also will differentiate between a normal move and a kill
            // move. If a normal move is found, we'll update a flag to return
            // at the end the "can only move" value. We have to continue
            // seeking for a kill move, but when one of this is found we'll
            // return the result of the method; because kill is mandatory.
            bool canMove = false;
            auto orig = sf::Vector2i(c, r);
            auto i = offsets.begin(), end = offsets.end();
            for (; i != end; ++i) {
                auto dest = orig + *i;
                auto validation = ValidateMove(orig, dest);
                if (validation == MOVE_VALID_WITH_KILL) {
                    return CAN_KILL;
                } else if (validation == MOVE_VALID) {
                    canMove = true;
                }
            }
            if (canMove) {
                return CAN_ONLY_MOVE;
            }
        }
    }
    return CAN_DO_NOTHING;
}

int CheckersGameData::CanPieceMoveOrKill(const sf::Vector2i & coords) const {
    return CanPieceMoveOrKill(coords.y, coords.x);
}

//--------------------------------------------------------------------------------

int CheckersGameData::ValidateMove(int r1, int c1, int r2, int c2) const {
    if (IsInside(r1, c1) && IsInside(r2, c2)) {
        // We can only validate a move when the origin and the
        // destination are inside the board.
        auto piece = board_[r1][c1];
        auto destination = board_[r2][c2];
        if (IsTurnPiece(piece) && destination == EMPTY_CELL) {
            // The origin piece must be a piece of the current turn's
            // side and the destination must be empty. Then we'll check
            // the delta of the current move.
            auto deltar = std::abs(r1 - r2), deltac = std::abs(c1 - c2);
            if (deltar == deltac) {
                if (deltar > 1) {
                    // With a delta greater than 1 we have to check if
                    // there are enemies in between of the "long" move.
                    int enemyCount = countEnemiesBetween(r1, c1, r2, c2);
                    if (0 <= enemyCount && enemyCount <= 1) {
                        // We can only kill one enemy by one. But if the piece
                        // is a queen can make a long move without kill or simply
                        // kill an enemy. The normal pieces must kill somebody and
                        // the delta cannot be more than 2 cells.
                        if (IsQueenPiece(piece)) {
                            return enemyCount > 0 ? MOVE_VALID_WITH_KILL : MOVE_VALID;
                        } else if (enemyCount == 1 && deltar == 2) {
                            return MOVE_VALID_WITH_KILL;
                        }
                    }
                } else if (deltar == 1) {
                    // If the delta is 1 we'll validate a normal move.
                    return MOVE_VALID;
                }
            }
        }
    }
    return MOVE_INVALID;
}

int CheckersGameData::ValidateMove(const sf::Vector2i & c1, const sf::Vector2i & c2) const {
    return ValidateMove(c1.y, c1.x, c2.y, c2.x);
}

//--------------------------------------------------------------------------------

void CheckersGameData::MakeMove(int r1, int c1, int r2, int c2) {
    bool killedByDeath = false;
    if (IsInside(r1, c1) && IsInside(r2, c2)) {
        // We can only move when the origin and the destination are inside the board.
        auto piece = board_[r1][c1];
        auto destination = board_[r2][c2];
        if (IsTurnPiece(piece) && destination == EMPTY_CELL) {
            // The origin must be a piece of the current turn's side and the destination
            // must be empty. Then we'll check the delta of the current move.
            auto deltar = std::abs(r1 - r2), deltac = std::abs(c1 - c2);
            if (deltar == deltac) {
                auto movePiece = [&] () {
                    board_[r2][c2] = piece;
                    board_[r1][c1] = EMPTY_CELL;
                    nextPieceToMove_ = NO_CELL;
                };
                auto setNextPieceToMove = [&] () {
                    // Set the next piece to move when it can kill after being moved.
                    if (CanPieceMoveOrKill(r2, c2) == CAN_KILL) {
                        nextPieceToMove_ = sf::Vector2i(c2, r2);
                    }
                };
                if (deltar > 1) {
                    // With a delta greater than 1 we have to check if
                    // there are enemies in between of the "long" move.
                    CoordsVector enemies;
                    int enemyCount = countEnemiesBetween(r1, c1, r2, c2,
                        [&] (const sf::Vector2i & item) { enemies.push_back(item); });

                    if (IsQueenPiece(piece)) {
                        // If the piece is a queen we'll check if we're making
                        // a long move without kill or simply killing an enemy.
                        if (0 == enemyCount) {
                            movePiece();
                        } else if (enemyCount == 1) {
                            movePiece();
                            killedByDeath = true;
                        }
                    } else if (enemyCount == 1 && deltar == 2) {
                        // The normal pieces must kill somebody and the delta
                        // cannot be more than 2 cells. It also have to check
                        // if the moved piece must be transform into queen.
                        movePiece();
                        checkAndTransform(r2, c2);
                        killedByDeath = true;
                    }

                    if (killedByDeath) {
                        // After set a kill, we'll remove the victim.
                        auto & enemy = enemies[0];
                        board_[enemy.y][enemy.x] = EMPTY_CELL;
                        // Here we check if we need to set the next piece to move.
                        setNextPieceToMove();
                    }
                } else if (deltar == 1) {
                    // If the delta is 1 we'll validate a normal move.
                    movePiece();
                    checkAndTransform(r2, c2);
                }
            }
        }
    }

    if (killedByDeath) {
        // After any kind of kill, we'll check if there is any winner.
        int whiteCount = CountPieces(WHITE_SIDE);
        int blackCount = CountPieces(BLACK_SIDE);
        if (whiteCount < 1) {
            SetWinner(BLACK_SIDE);
        } else if (blackCount < 1) {
            SetWinner(WHITE_SIDE);
        }
    }
}

void CheckersGameData::MakeMove(const sf::Vector2i & c1, const sf::Vector2i & c2) {
    MakeMove(c1.y, c1.x, c2.y, c2.x);
}

//--------------------------------------------------------------------------------

int CheckersGameData::countEnemiesBetween(int r1, int c1, int r2, int c2) const {
    return countEnemiesBetween(r1, c1, r2, c2, nullptr);
}

int CheckersGameData::countEnemiesBetween(int r1, int c1, int r2, int c2,
    std::function<void (const sf::Vector2i &)> onEnemyFind) const {
    // Set some initial data to execute the algorithm.
    auto offset = sf::Vector2i(c2 - c1 < 0 ? -1 : 1, r2 - r1 < 0 ? -1 : 1);
    auto orig = sf::Vector2i(c1, r1), dest = sf::Vector2i(c2, r2);
    int pieceSide = GetPieceSide(board_[r1][c1]);
    int enemySide = GetOppositeSide(pieceSide);
    int enemyCount = 0;
    // We'll go from the next cell from the origin to the destination.
    for (auto i = orig + offset; i != dest; i = i + offset) {
        // For each piece inside the trajectory, if we found a piece of our
        // own side, we won't be able to move. Otherwise we'll count the
        // enemies found in the path.
        auto item = board_[i.y][i.x];
        auto itemSide = GetPieceSide(item);
        if (itemSide == pieceSide) {
            return -1;
        } else if (itemSide == enemySide) {
            if (onEnemyFind) onEnemyFind(i);
            ++enemyCount;
        }
    }
    return enemyCount;
}

//--------------------------------------------------------------------------------

void CheckersGameData::checkAndTransform(int r, int c) {
    // We take the piece from the board and check the type and the side.
    // If the piece is in the last row, it will be transformed.
    auto piece = board_[r][c];
    if (IsNormalPiece(piece)) {
        if (IsWhitePiece(piece) && r == LAST_WHITE_ROW) {
            board_[r][c] = WHITE_QUEEN_PIECE;
        } else if (IsBlackPiece(piece) && r == LAST_BLACK_ROW) {
            board_[r][c] = BLACK_QUEEN_PIECE;
        }
    }
}

//********************************************************************************
// Query Methods
//********************************************************************************

bool CheckersGameData::IsInside(int r, int c) const {
    return 0 <= r && r < BOARD_SIZE && 0 <= c && c < BOARD_SIZE;
}

bool CheckersGameData::IsInside(const sf::Vector2i & coords) const {
    return IsInside(coords.y, coords.x);
}

//--------------------------------------------------------------------------------

bool CheckersGameData::IsWorldCell(int r, int c) const {
    return MathUtil::IsEven(r) ? MathUtil::IsEven(c) : MathUtil::IsOdd(c);
}

bool CheckersGameData::IsWorldCell(const sf::Vector2i & coords) const {
    return IsWorldCell(coords.y, coords.x);
}

//--------------------------------------------------------------------------------

bool CheckersGameData::IsNormalPiece(int piece) const {
    return piece == BLACK_PIECE || piece == WHITE_PIECE;
}

bool CheckersGameData::IsQueenPiece(int piece) const {
    return piece == BLACK_QUEEN_PIECE || piece == WHITE_QUEEN_PIECE;
}

bool CheckersGameData::IsBlackPiece(int piece) const {
    return piece == BLACK_PIECE || piece == BLACK_QUEEN_PIECE;
}

bool CheckersGameData::IsWhitePiece(int piece) const {
    return piece == WHITE_PIECE || piece == WHITE_QUEEN_PIECE;
}

bool CheckersGameData::IsPieceSide(int piece, int side) const {
    if (side == WHITE_SIDE) {
        return IsWhitePiece(piece);
    } else if (side == BLACK_SIDE) {
        return IsBlackPiece(piece);
    } else {
        return false;
    }
}

bool CheckersGameData::IsTurnPiece(int piece) const {
    return IsPieceSide(piece, turn_);
}

//--------------------------------------------------------------------------------

bool CheckersGameData::IsPlayerPiece(int r, int c, int side) const {
    if (IsInside(r, c) && board_[r][c] != EMPTY_CELL) {
        if (side == WHITE_SIDE) {
            return IsWhitePiece(board_[r][c]);
        } else if (side == BLACK_SIDE) {
            return IsBlackPiece(board_[r][c]);
        } else {
            return false;
        }
    }
    return false;
}

bool CheckersGameData::IsPlayerPiece(int r, int c) const {
    return IsPlayerPiece(r, c, turn_);
}

bool CheckersGameData::IsPlayerPiece(const sf::Vector2i & coords, int side) const {
    return IsPlayerPiece(coords.y, coords.x, side);
}

bool CheckersGameData::IsPlayerPiece(const sf::Vector2i & coords) const {
    return IsPlayerPiece(coords.y, coords.x);
}

//--------------------------------------------------------------------------------

int CheckersGameData::GetOppositeSide(int side) const {
    if (side == WHITE_SIDE)
        return BLACK_SIDE;
    else if (side == BLACK_SIDE)
        return WHITE_SIDE;
    else
        return NO_SIDE;
}

//--------------------------------------------------------------------------------

int CheckersGameData::GetPieceSide(int piece) const {
    if (IsWhitePiece(piece))
        return WHITE_SIDE;
    else if (IsBlackPiece(piece))
        return BLACK_SIDE;
    else
        return NO_SIDE;
}

//--------------------------------------------------------------------------------

bool CheckersGameData::AnyCandidate() const {
    return !candidates_.empty();
}

bool CheckersGameData::IsCandidate(int r, int c) const {
    if (AnyCandidate()) {
        auto end = candidates_.end();
        auto it = std::find(candidates_.begin(), end, sf::Vector2i(c, r));
        if (it != end) return true;
    }
    return false;
}

bool CheckersGameData::IsCandidate(const sf::Vector2i & coords) const {
    return IsCandidate(coords.y, coords.x);
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

CheckersGameData::CheckersGameData() : singlePlayer_(false), difficulty_(NORMAL_LEVEL),
    playerSide_(WHITE_SIDE), winner_(NO_WINNER), turn_(WHITE_SIDE), nextPieceToMove_(-1, -1),
    candidates_() {}

//--------------------------------------------------------------------------------

CheckersGameData::CheckersGameData(const CheckersGameData & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

CheckersGameData::~CheckersGameData() {}

//--------------------------------------------------------------------------------

CheckersGameData & CheckersGameData::operator =(const CheckersGameData & source) {
    singlePlayer_ = source.singlePlayer_;
    difficulty_ = source.difficulty_;
    playerSide_ = source.playerSide_;
    winner_ = source.winner_;
    turn_ = source.turn_;
    nextPieceToMove_ = source.nextPieceToMove_;
    ForEachInBoard([&] (int, int i, int j) {
        board_[i][j] = source.board_[i][j];
    });
    candidates_ = source.candidates_;
    return *this;
}
