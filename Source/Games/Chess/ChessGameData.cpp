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

#include "ChessGameData.h"
#include <System/MathUtil.h>
#include <System/ForEach.h>
#include <Games/Chess/ChessManager.h>

//********************************************************************************
// Constants
//********************************************************************************

const int LAST_WHITE_ROW = 7;
const int LAST_BLACK_ROW = 0;

const int WHITE_PIECES_ROW = 0, WHITE_PAWNS_ROW = 1;
const int BLACK_PIECES_ROW = 7, BLACK_PAWNS_ROW = 6;

const int OPPOSITE_SIDE[ChessGameData::MAX_SIDES] = {
    ChessGameData::BLACK_SIDE, ChessGameData::WHITE_SIDE
};

//--------------------------------------------------------------------------------

const sf::Vector2i ChessGameData::NO_CELL = sf::Vector2i(-1, -1);

const int ChessGameData::MAX_OFFSETS[PIECE_TYPES] = {
    MAX_PAWN_OFFSETS, MAX_ROOK_OFFSETS, MAX_KNIGHT_OFFSETS,
    MAX_BISHOP_OFFSETS, MAX_QUEEN_OFFSETS, MAX_KING_OFFSETS
};

const sf::Vector2i ChessGameData::WHITE_PAWN_OFFSETS[MAX_PAWN_OFFSETS] = {
    sf::Vector2i(-1, 1),    // Kill Left
    sf::Vector2i(0, 1),     // Move 1
    sf::Vector2i(1, 1),     // Kill Right
    sf::Vector2i(0, 2)      // Move 2
};

const sf::Vector2i ChessGameData::BLACK_PAWN_OFFSETS[MAX_PAWN_OFFSETS] = {
    sf::Vector2i(-1, -1),   // Kill Left
    sf::Vector2i(0, -1),    // Move 1
    sf::Vector2i(1, -1),    // Kill Right
    sf::Vector2i(0, -2)     // Move 2
};

const sf::Vector2i ChessGameData::ROOK_OFFSETS[MAX_ROOK_OFFSETS] = {
    sf::Vector2i(0, 1), sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, -1),
    sf::Vector2i(0, 2), sf::Vector2i(-2, 0), sf::Vector2i(2, 0), sf::Vector2i(0, -2),
    sf::Vector2i(0, 3), sf::Vector2i(-3, 0), sf::Vector2i(3, 0), sf::Vector2i(0, -3),
    sf::Vector2i(0, 4), sf::Vector2i(-4, 0), sf::Vector2i(4, 0), sf::Vector2i(0, -4),
    sf::Vector2i(0, 5), sf::Vector2i(-5, 0), sf::Vector2i(5, 0), sf::Vector2i(0, -5),
    sf::Vector2i(0, 6), sf::Vector2i(-6, 0), sf::Vector2i(6, 0), sf::Vector2i(0, -6),
    sf::Vector2i(0, 7), sf::Vector2i(-7, 0), sf::Vector2i(7, 0), sf::Vector2i(0, -7)
};

const sf::Vector2i ChessGameData::KNIGHT_OFFSETS[MAX_KNIGHT_OFFSETS] = {
    sf::Vector2i(-1,  2), sf::Vector2i( 1,  2),
    sf::Vector2i(-2,  1), sf::Vector2i( 2,  1),
    sf::Vector2i(-2, -1), sf::Vector2i( 2, -1),
    sf::Vector2i(-1, -2), sf::Vector2i( 1, -2)
};

const sf::Vector2i ChessGameData::BISHOP_OFFSETS[MAX_BISHOP_OFFSETS] = {
    sf::Vector2i(-1, 1), sf::Vector2i(1, 1), sf::Vector2i(-1, -1), sf::Vector2i(1, -1),
    sf::Vector2i(-2, 2), sf::Vector2i(2, 2), sf::Vector2i(-2, -2), sf::Vector2i(2, -2),
    sf::Vector2i(-3, 3), sf::Vector2i(3, 3), sf::Vector2i(-3, -3), sf::Vector2i(3, -3),
    sf::Vector2i(-4, 4), sf::Vector2i(4, 4), sf::Vector2i(-4, -4), sf::Vector2i(4, -4),
    sf::Vector2i(-5, 5), sf::Vector2i(5, 5), sf::Vector2i(-5, -5), sf::Vector2i(5, -5),
    sf::Vector2i(-6, 6), sf::Vector2i(6, 6), sf::Vector2i(-6, -6), sf::Vector2i(6, -6),
    sf::Vector2i(-7, 7), sf::Vector2i(7, 7), sf::Vector2i(-7, -7), sf::Vector2i(7, -7)
};

const sf::Vector2i ChessGameData::QUEEN_OFFSETS[MAX_QUEEN_OFFSETS] = {
    sf::Vector2i(-1,  1), sf::Vector2i(0,  1), sf::Vector2i(1,  1),
    sf::Vector2i(-1,  0),                      sf::Vector2i(1,  0),
    sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(1, -1),

    sf::Vector2i(-2,  2), sf::Vector2i(0,  2), sf::Vector2i(2,  2),
    sf::Vector2i(-2,  0),                      sf::Vector2i(2,  0),
    sf::Vector2i(-2, -2), sf::Vector2i(0, -2), sf::Vector2i(2, -2),

    sf::Vector2i(-3,  3), sf::Vector2i(0,  3), sf::Vector2i(3,  3),
    sf::Vector2i(-3,  0),                      sf::Vector2i(3,  0),
    sf::Vector2i(-3, -3), sf::Vector2i(0, -3), sf::Vector2i(3, -3),

    sf::Vector2i(-4,  4), sf::Vector2i(0,  4), sf::Vector2i(4,  4),
    sf::Vector2i(-4,  0),                      sf::Vector2i(4,  0),
    sf::Vector2i(-4, -4), sf::Vector2i(0, -4), sf::Vector2i(4, -4),

    sf::Vector2i(-5,  5), sf::Vector2i(0,  5), sf::Vector2i(5,  5),
    sf::Vector2i(-5,  0),                      sf::Vector2i(5,  0),
    sf::Vector2i(-5, -5), sf::Vector2i(0, -5), sf::Vector2i(5, -5),

    sf::Vector2i(-6,  6), sf::Vector2i(0,  6), sf::Vector2i(6,  6),
    sf::Vector2i(-6,  0),                      sf::Vector2i(6,  0),
    sf::Vector2i(-6, -6), sf::Vector2i(0, -6), sf::Vector2i(6, -6),

    sf::Vector2i(-7,  7), sf::Vector2i(0,  7), sf::Vector2i(7,  7),
    sf::Vector2i(-7,  0),                      sf::Vector2i(7,  0),
    sf::Vector2i(-7, -7), sf::Vector2i(0, -7), sf::Vector2i(7, -7)
};

const sf::Vector2i ChessGameData::KING_OFFSETS[MAX_KING_OFFSETS] = {
    sf::Vector2i(-1,  1), sf::Vector2i(0,  1), sf::Vector2i(1,  1),
    sf::Vector2i(-1,  0),                      sf::Vector2i(1,  0),
    sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(1, -1),

    sf::Vector2i(-2,  0),                      sf::Vector2i(2,  0)
};

//********************************************************************************
// Static
//********************************************************************************

bool ChessGameData::showErrorMessages_ = true;

//********************************************************************************
// General Methods (Public)
//********************************************************************************

void ChessGameData::Start(bool singlePlayer, int difficulty, int playerSide) {
    // Config the one or two players game.
    singlePlayer_ = singlePlayer;
    difficulty_ = difficulty;
    playerSide_ = playerSide;
    // Reset the other logic fields.
    Reset();
}

//--------------------------------------------------------------------------------

void ChessGameData::Reset() {
    // Initialize the game board.
    const int HALF_PIECES = MAX_PIECES_SIDE / 2;
    const ChessGameData::Piece WHITE_PIECES[] = {
        ChessGameData::Piece(WHITE_SIDE, ROOK_PIECE,   sf::Vector2i(0, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, KNIGHT_PIECE, sf::Vector2i(1, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, BISHOP_PIECE, sf::Vector2i(2, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, QUEEN_PIECE,  sf::Vector2i(3, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, KING_PIECE,   sf::Vector2i(4, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, BISHOP_PIECE, sf::Vector2i(5, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, KNIGHT_PIECE, sf::Vector2i(6, WHITE_PIECES_ROW)),
        ChessGameData::Piece(WHITE_SIDE, ROOK_PIECE,   sf::Vector2i(7, WHITE_PIECES_ROW))
    };
    const ChessGameData::Piece BLACK_PIECES[] = {
        ChessGameData::Piece(BLACK_SIDE, ROOK_PIECE,   sf::Vector2i(0, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, KNIGHT_PIECE, sf::Vector2i(1, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, BISHOP_PIECE, sf::Vector2i(2, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, QUEEN_PIECE,  sf::Vector2i(3, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, KING_PIECE,   sf::Vector2i(4, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, BISHOP_PIECE, sf::Vector2i(5, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, KNIGHT_PIECE, sf::Vector2i(6, BLACK_PIECES_ROW)),
        ChessGameData::Piece(BLACK_SIDE, ROOK_PIECE,   sf::Vector2i(7, BLACK_PIECES_ROW))
    };
    for (int i = 0; i < HALF_PIECES; ++i) {
        pieces_[WHITE_PIECES_START + i].side = PAWN_PIECE;
        pieces_[WHITE_PIECES_START + i].type = WHITE_SIDE;
        pieces_[WHITE_PIECES_START + i].position = sf::Vector2i(i, WHITE_PAWNS_ROW);

        pieces_[WHITE_PIECES_START + HALF_PIECES + i] = WHITE_PIECES[i];

        pieces_[BLACK_PIECES_START + i].side = BLACK_SIDE;
        pieces_[BLACK_PIECES_START + i].type = PAWN_PIECE;
        pieces_[BLACK_PIECES_START + i].position = sf::Vector2i(i, BLACK_PAWNS_ROW);

        pieces_[BLACK_PIECES_START + HALF_PIECES + i] = BLACK_PIECES[i];
    }

    // Set some control information.
    winner_ = NO_WINNER;
    turn_ = WHITE_SIDE;
    whiteCheck_ = false;
    blackCheck_ = false;
}

//--------------------------------------------------------------------------------

void ChessGameData::SetWinner(int side) {
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

void ChessGameData::NextTurn() {
    if (winner_ == NO_WINNER) {
        // First, check if any of the kings are dead.
        if (getPieceByType(WHITE_SIDE, KING_PIECE) == PIECE_NOT_FOUND) {
            SetWinner(BLACK_SIDE);
        } else if (getPieceByType(BLACK_SIDE, KING_PIECE) == PIECE_NOT_FOUND) {
            SetWinner(WHITE_SIDE);
        } else {
            // No king dead, it's party time bro.
            whiteCheck_ = false;
            blackCheck_ = false;

            clearAllErrorMessages();
            showErrorMessages_ = false;

            if (turn_ == WHITE_SIDE) {
                // So the last turn was the white one. Let's change it and take
                // a look to the board to find a check situation. If there is a
                // check, we'll test the checkmate situation to warn the user.
                turn_ = BLACK_SIDE;
                if (isCheck()) {
                    if (isCheckmate()) {
                        showErrorMessages_ = true;
                        setErrorMessage(ChessManager::ERROR_BLACK_CHECKMATE);
                    } else {
                        showErrorMessages_ = true;
                        setErrorMessage(ChessManager::ERROR_BLACK_CHECK);
                    }
                    blackCheck_ = true;
                } else if (isCheckmate()) {
                    // If no check, but stalemate, the game is drawn.
                    SetWinner(BOTH_SIDES);
                }
            } else {
                // So the last turn was the black one. Let's change it and take
                // a look to the board to find a check situation. If there is a
                // check, we'll test the checkmate situation to warn the user.
                turn_ = WHITE_SIDE;
                if (isCheck()) {
                    if (isCheckmate()) {
                        showErrorMessages_ = true;
                        setErrorMessage(ChessManager::ERROR_WHITE_CHECKMATE);
                    } else {
                        showErrorMessages_ = true;
                        setErrorMessage(ChessManager::ERROR_WHITE_CHECK);
                    }
                    whiteCheck_ = true;
                } else if (isCheckmate()) {
                    // If no check, but stalemate, the game is drawn.
                    SetWinner(BOTH_SIDES);
                }
            }

            showErrorMessages_ = true;
        }
    }
}

//--------------------------------------------------------------------------------

void ChessGameData::ConvertPawnToRook()   { convertPawnTo(ROOK_PIECE);   }
void ChessGameData::ConvertPawnToKnight() { convertPawnTo(KNIGHT_PIECE); }
void ChessGameData::ConvertPawnToBishop() { convertPawnTo(BISHOP_PIECE); }
void ChessGameData::ConvertPawnToQueen()  { convertPawnTo(QUEEN_PIECE);  }

//--------------------------------------------------------------------------------

bool ChessGameData::MakeMove(const sf::Vector2i & orig, const sf::Vector2i & dest) {
    if (IsInside(dest) && orig != dest) {
        int index = getPiece(orig);
        if (index != PIECE_NOT_FOUND) {
            auto & piece = pieces_[index];
            if (piece.side == turn_) {
                switch (piece.type) {
                case PAWN_PIECE:   return makePawnMove(piece, dest);
                case ROOK_PIECE:   return makeRookMove(piece, dest);
                case KNIGHT_PIECE: return makeKnightMove(piece, dest);
                case BISHOP_PIECE: return makeBishopMove(piece, dest);
                case QUEEN_PIECE:  return makeQueenMove(piece, dest);
                case KING_PIECE:   return makeKingMove(piece, dest);
                }
            }
        }
    }
    return false;
}

//********************************************************************************
// General Methods (Private)
//********************************************************************************

const sf::Vector2i * ChessGameData::getMoveOffsets(int side, int type) const {
    switch (type) {
    case PAWN_PIECE:
        if (side == WHITE_SIDE)
            return WHITE_PAWN_OFFSETS;
        else if (side == BLACK_SIDE)
            return BLACK_PAWN_OFFSETS;
        break;

    case ROOK_PIECE:   return ROOK_OFFSETS;
    case KNIGHT_PIECE: return KNIGHT_OFFSETS;
    case BISHOP_PIECE: return BISHOP_OFFSETS;
    case QUEEN_PIECE:  return QUEEN_OFFSETS;
    case KING_PIECE:   return KING_OFFSETS;
    }
    return nullptr;
}

//--------------------------------------------------------------------------------

int ChessGameData::getPiece(int r, int c) const {
    for (int i = 0; i < MAX_PIECES; ++i) {
        // Find a not dead piece at a position.
        if (pieces_[i].NotDead() && pieces_[i].position.y == r &&
            pieces_[i].position.x == c) {
            return i;
        }
    }
    return PIECE_NOT_FOUND;
}

int ChessGameData::getPiece(const sf::Vector2i & coords) const {
    for (int i = 0; i < MAX_PIECES; ++i) {
        // Find a not dead piece at a position.
        if (pieces_[i].NotDead() && pieces_[i].position == coords) {
            return i;
        }
    }
    return PIECE_NOT_FOUND;
}

int ChessGameData::getPieceByType(int side, int type) const {
    for (int i = 0; i < MAX_PIECES; ++i) {
        // Find the first piece of a tpe and a side.
        if (pieces_[i].type == type && pieces_[i].side == side) {
            return i;
        }
    }
    return PIECE_NOT_FOUND;
}

//--------------------------------------------------------------------------------

void ChessGameData::convertPawnTo(int type) {
    for (int i = 0; i < MAX_PIECES; ++i) {
        if (pieces_[i].type == PAWN_PIECE) {
            // Convert all the found pawns at their end row.
            if ((pieces_[i].side == WHITE_SIDE && pieces_[i].position.y == LAST_WHITE_ROW) ||
                (pieces_[i].side == BLACK_SIDE && pieces_[i].position.y == LAST_BLACK_ROW)) {
                pieces_[i].type = type;
            }
        }
    }
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeFutureMove(int side, const sf::Vector2i & orig,
    const sf::Vector2i & dest) const {
    ChessGameData future(*this);
    future.turn_ = side;
    bool result;
    if (showErrorMessages_) {
        showErrorMessages_ = false;
        result = future.MakeMove(orig, dest);
        showErrorMessages_ = true;
    } else {
        result = future.MakeMove(orig, dest);
    }
    return result;
}

//--------------------------------------------------------------------------------

bool ChessGameData::isCheck() const {
    if ((turn_ == WHITE_SIDE && !whiteCheck_) || (turn_ == BLACK_SIDE && !blackCheck_)) {
        // If no previous check state, find the king of the current turn.
        auto enemySide = OPPOSITE_SIDE[turn_];
        auto kingIndex = getPieceByType(turn_, KING_PIECE);
        if (kingIndex != PIECE_NOT_FOUND) {
            auto & king = pieces_[kingIndex];
            // Try to move all the enemy pieces to the king's position.
            for (int i = 0; i < MAX_PIECES; ++i) {
                auto & enemy = pieces_[i];
                if (enemy.side == enemySide && enemy.NotDead() &&
                    makeFutureMove(enemySide, enemy.position, king.position)) {
                    setErrorMessage(ChessManager::ERROR_CHECK_IF_MOVE);
                    return true;
                }
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::isCheckmate() const {
    for (int i = 0; i < MAX_PIECES; ++i) {
        // Find all the alive pieces of this turn.
        auto & victim = pieces_[i];
        if (victim.side == turn_ && victim.NotDead()) {
            auto * offsets = getMoveOffsets(victim);
            if (offsets != nullptr) {
                // For each piece check all the possible moves to do.
                for (int j = 0; j < MAX_OFFSETS[victim.type]; ++j) {
                    auto destination = victim.position + offsets[j];
                    if (makeFutureMove(turn_, victim.position, destination)) {
                        return false;
                    }
                }
            }
        }
    }
    // No move done, then checkmate.
    return true;
}

//--------------------------------------------------------------------------------

void ChessGameData::checkAndMarkPawn(Piece & current, int enemySide, int r, int c) {
    // Get the piece inside the cell.
    int enemyIndex = getPiece(r, c);
    if (enemyIndex != PIECE_NOT_FOUND) {
        // Check the side and the type of the piece and mark the pawns.
        auto & enemy = pieces_[enemyIndex];
        if (enemy.side == enemySide && enemy.type == PAWN_PIECE) {
            enemy.mark = MARK_PAWN_ATTACK;
            current.mark = MARK_PAWN_VICTIM;
        }
    }
}

//--------------------------------------------------------------------------------

bool ChessGameData::collision(const sf::Vector2i & orig, const sf::Vector2i & dest) {
    auto delta = dest - orig;
    if (delta.x == 0 || delta.y == 0 || std::abs(delta.x) == std::abs(delta.y)) {
        // Only check horizontal, vertical and diagonal moves.
        auto transform = [] (int x) -> int {
            if (x < 0) return -1;
            else if (x > 0) return 1;
            else return 0;
        };
        delta.x = transform(delta.x);
        delta.y = transform(delta.y);
        // Try to find any piece in the middle of the trajectory-
        for (auto pos = orig + delta; pos != dest; pos += delta) {
            if (getPiece(pos) != PIECE_NOT_FOUND) {
                return true;
            }
        }
        // Nothing found, then no collision.
        return false;
    } else {
        // Invalid type of movement, collision by nonsense.
        return true;
    }
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeWhitePawnMove(Piece & current, const sf::Vector2i & dest) {
    if (current.position.x == dest.x) {
        // We're going to try to move forward.
        if (current.position.y == WHITE_PAWNS_ROW && current.position.y == dest.y - 2) {
            // Move forward 2 cells if nobody is in the way.
            if (getPiece(dest.y - 1, dest.x) == PIECE_NOT_FOUND &&
                getPiece(dest) == PIECE_NOT_FOUND) {
                auto oldPosition = current.position;
                current.position = dest;
                if (isCheck()) {
                    // You cant move the piece or the king dies.
                    current.position = oldPosition;
                } else {
                    // Check if the pawn must be marked and validate the move.
                    checkAndMarkPawn(current, BLACK_SIDE, dest.y, dest.x - 1);
                    checkAndMarkPawn(current, BLACK_SIDE, dest.y, dest.x + 1);
                    return true;
                }
            }
        } else if (current.position.y == dest.y - 1) {
            // Move forward 1 cell if nobody is in the way.
            if (getPiece(dest) == PIECE_NOT_FOUND) {
                auto oldPosition = current.position;
                current.position = dest;
                if (isCheck()) {
                    // You cant move the piece or the king dies.
                    current.position = oldPosition;
                } else {
                    // Validate the move.
                    return true;
                }
            }
        }
    } else if ((current.position.y == dest.y - 1) &&
        ((current.position.x == dest.x - 1) || (current.position.x == dest.x + 1))) {
        // We're going to try to kill somebody.
        auto index = getPiece(dest);
        if (index != PIECE_NOT_FOUND && pieces_[index].side == BLACK_SIDE) {
            // Lets try to make a normal kill.
            auto oldPosition = current.position;
            auto oldType = pieces_[index].type;
            current.position = dest;
            pieces_[index].type = DEAD_PIECE;
            if (oldType != KING_PIECE && isCheck()) {
                // You cant move the piece or the king dies.
                current.position = oldPosition;
                pieces_[index].type = oldType;
            } else {
                // Validate the move.
                return true;
            }
        } else if (index == PIECE_NOT_FOUND && current.position.y == 4) {
            index = getPiece(dest.y - 1, dest.x);
            if (index != PIECE_NOT_FOUND && pieces_[index].side == BLACK_SIDE &&
                pieces_[index].type == PAWN_PIECE && pieces_[index].mark == MARK_PAWN_VICTIM &&
                current.mark == MARK_PAWN_ATTACK) {
                // Lets try to make the special kill.
                auto oldPosition = current.position;
                auto oldType = pieces_[index].type;
                current.position = dest;
                pieces_[index].type = DEAD_PIECE;
                if (oldType != KING_PIECE && isCheck()) {
                    // You cant move the piece or the king dies.
                    current.position = oldPosition;
                    pieces_[index].type = oldType;
                } else {
                    // Validate the move.
                    return true;
                }
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeBlackPawnMove(Piece & current, const sf::Vector2i & dest) {
    if (current.position.x == dest.x) {
        // We're going to try to move forward.
        if (current.position.y == BLACK_PAWNS_ROW && current.position.y == dest.y + 2) {
            // Move forward 2 cells if nobody is in the way.
            if (getPiece(dest.y + 1, dest.x) == PIECE_NOT_FOUND &&
                getPiece(dest) == PIECE_NOT_FOUND) {
                auto oldPosition = current.position;
                current.position = dest;
                if (isCheck()) {
                    // You cant move the piece or the king dies.
                    current.position = oldPosition;
                } else {
                    // Check if the pawn must be marked and validate the move.
                    checkAndMarkPawn(current, WHITE_SIDE, dest.y, dest.x - 1);
                    checkAndMarkPawn(current, WHITE_SIDE, dest.y, dest.x + 1);
                    return true;
                }
            }
        } else if (current.position.y == dest.y + 1) {
            // Move forward 1 cell if nobody is in the way.
            if (getPiece(dest) == PIECE_NOT_FOUND) {
                auto oldPosition = current.position;
                current.position = dest;
                if (isCheck()) {
                    // You cant move the piece or the king dies.
                    current.position = oldPosition;
                } else {
                    // Validate the move.
                    return true;
                }
            }
        }
    } else if ((current.position.y == dest.y + 1) &&
        ((current.position.x == dest.x - 1) || (current.position.x == dest.x + 1))) {
        // We're going to try to kill somebody.
        auto index = getPiece(dest);
        if (index != PIECE_NOT_FOUND && pieces_[index].side == WHITE_SIDE) {
            // Lets try to make a normal kill.
            auto oldPosition = current.position;
            auto oldType = pieces_[index].type;
            current.position = dest;
            pieces_[index].type = DEAD_PIECE;
            if (oldType != KING_PIECE && isCheck()) {
                // You cant move the piece or the king dies.
                current.position = oldPosition;
                pieces_[index].type = oldType;
            } else {
                // Validate the move.
                return true;
            }
        } else if (index == PIECE_NOT_FOUND && current.position.y == 3) {
            index = getPiece(dest.y + 1, dest.x);
            if (index != PIECE_NOT_FOUND && pieces_[index].side == WHITE_SIDE &&
                pieces_[index].type == PAWN_PIECE && pieces_[index].mark == MARK_PAWN_VICTIM &&
                current.mark == MARK_PAWN_ATTACK) {
                // Lets try to make the special kill.
                auto oldPosition = current.position;
                auto oldType = pieces_[index].type;
                current.position = dest;
                pieces_[index].type = DEAD_PIECE;
                if (oldType != KING_PIECE && isCheck()) {
                    // You cant move the piece or the king dies.
                    current.position = oldPosition;
                    pieces_[index].type = oldType;
                } else {
                    // Validate the move.
                    return true;
                }
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makePawnMove(Piece & current, const sf::Vector2i & dest) {
    if (current.side == WHITE_SIDE)
        return makeWhitePawnMove(current, dest);
    else if (current.side == BLACK_SIDE)
        return makeBlackPawnMove(current, dest);
    else
        return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeGenericMove(Piece & current, const sf::Vector2i & dest) {
    auto index = getPiece(dest);
    if (getPiece(dest) == PIECE_NOT_FOUND) {
        // Just move the piece, because nobody is at the end of my way.
        auto oldPosition = current.position;
        current.position = dest;
        if (isCheck()) {
            // You cant move the piece or the king dies.
            current.position = oldPosition;
        } else {
            // Validate the move.
            return true;
        }
    } else if (pieces_[index].side != current.side) {
        // An enemy is going to die in my path of destruction.
        auto oldPosition = current.position;
        auto oldType = pieces_[index].type;
        current.position = dest;
        pieces_[index].type = DEAD_PIECE;
        if (oldType != KING_PIECE && isCheck()) {
            // You cant move the piece or the king dies.
            current.position = oldPosition;
            pieces_[index].type = oldType;
        } else {
            // Validate the move.
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeRookMove(Piece & current, const sf::Vector2i & dest) {
    if ((current.position.x == dest.x || current.position.y == dest.y) &&
        !collision(current.position, dest) && makeGenericMove(current, dest)) {
        current.mark = MARK_CANT_CASTLING;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeKnightMove(Piece & current, const sf::Vector2i & dest) {
    auto ox = current.position.x, oy = current.position.y;
    if (((ox == dest.x - 2 && (oy == dest.y - 1 || oy == dest.y + 1)) ||
         (ox == dest.x + 2 && (oy == dest.y - 1 || oy == dest.y + 1)) ||
         (ox == dest.x - 1 && (oy == dest.y - 2 || oy == dest.y + 2)) ||
         (ox == dest.x + 1 && (oy == dest.y - 2 || oy == dest.y + 2))) &&
        makeGenericMove(current, dest)) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeBishopMove(Piece & current, const sf::Vector2i & dest) {
    if (std::abs(dest.x - current.position.x) == std::abs(dest.y - current.position.y) &&
        !collision(current.position, dest) && makeGenericMove(current, dest)) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeQueenMove(Piece & current, const sf::Vector2i & dest) {
    return makeRookMove(current, dest) || makeBishopMove(current, dest);
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeKingCastling(Piece & current, const sf::Vector2i & dest) {
    if (((turn_ == WHITE_SIDE && !whiteCheck_) || (turn_ == BLACK_SIDE && !blackCheck_)) &&
        current.mark != MARK_CANT_CASTLING && !collision(current.position, dest)) {
        auto isCheckIfMove = [&] (int inc) -> bool {
            auto oldPosition = current.position;
            current.position.x += inc;
            auto result = isCheck();
            current.position = oldPosition;
            return result;
        };
        if (current.position.x < dest.x && !isCheckIfMove(1)) {
            // Castling to the right if we have a rook at the right side.
            auto index = getPiece(turn_ == WHITE_SIDE ? 0 : 7, 7);
            if (index != PIECE_NOT_FOUND) {
                auto & victim = pieces_[index];
                if (victim.type == ROOK_PIECE && victim.side == turn_ &&
                    victim.mark != MARK_CANT_CASTLING) {
                    // Test if the rook can move to the left of the king.
                    auto oldRookPosition = victim.position;
                    if (makeRookMove(victim, sf::Vector2i(dest.x - 1, dest.y))) {
                        auto oldPosition = current.position;
                        current.position = dest;
                        if (isCheck()) {
                            // You cant move the piece or the king dies.
                            victim.mark = MARK_NONE;
                            victim.position = oldRookPosition;
                            current.position = oldPosition;
                        } else {
                            // Validate the move.
                            current.mark = MARK_CANT_CASTLING;
                            return true;
                        }
                    }
                }
            }
        } else if (dest.x < current.position.x && !isCheckIfMove(-1)) {
            // Castling to the left if we have a rook at the left side.
            auto index = getPiece(turn_ == WHITE_SIDE ? 0 : 7, 0);
            if (index != PIECE_NOT_FOUND) {
                auto & victim = pieces_[index];
                if (victim.type == ROOK_PIECE && victim.side == turn_ &&
                    victim.mark != MARK_CANT_CASTLING) {
                    // Test if the rook can move to the left of the king.
                    auto oldRookPosition = victim.position;
                    if (makeRookMove(victim, sf::Vector2i(dest.x + 1, dest.y))) {
                        auto oldPosition = current.position;
                        current.position = dest;
                        if (isCheck()) {
                            // You cant move the piece or the king dies.
                            victim.mark = MARK_NONE;
                            victim.position = oldRookPosition;
                            current.position = oldPosition;
                        } else {
                            // Validate the move.
                            current.mark = MARK_CANT_CASTLING;
                            return true;
                        }
                    }
                }
            }
        }
    }
    setErrorMessage(ChessManager::ERROR_CANT_CASTLING);
    return false;
}

//--------------------------------------------------------------------------------

bool ChessGameData::makeKingMove(Piece & current, const sf::Vector2i & dest) {
    auto ox = current.position.x, oy = current.position.y;
    if (((ox == dest.x - 1 && dest.y - 1 <= oy && oy <= dest.y + 1) ||
        (ox == dest.x && (oy == dest.y - 1 || oy == dest.y + 1)) ||
        (ox == dest.x + 1 && dest.y - 1 <= oy && oy <= dest.y + 1)) &&
        makeGenericMove(current, dest)) {
        current.mark = MARK_CANT_CASTLING;
        return true;
    } else if (oy == dest.y && (ox == dest.x - 2 || ox == dest.x + 2)) {
        return makeKingCastling(current, dest);
    }
    return false;
}

//--------------------------------------------------------------------------------

void ChessGameData::clearAllErrorMessages() const {
    if (showErrorMessages_) {
        ChessManager::Instance()->ClearAllErrorMessages();
    }
}

//--------------------------------------------------------------------------------

void ChessGameData::clearErrorMessage(int error) const {
    if (showErrorMessages_) {
        ChessManager::Instance()->ClearErrorMessage(error);
    }
}

//--------------------------------------------------------------------------------

void ChessGameData::setErrorMessage(int error) const {
    if (showErrorMessages_) {
        ChessManager::Instance()->SetErrorMessage(error);
    }
}

//********************************************************************************
// Query Methods
//********************************************************************************

bool ChessGameData::IsInside(int r, int c) const {
    return 0 <= r && r < BOARD_SIZE && 0 <= c && c < BOARD_SIZE;
}

//--------------------------------------------------------------------------------

bool ChessGameData::AnyPawnToConvert() const {
    for (int i = 0; i < MAX_PIECES; ++i) {
        if (pieces_[i].type == PAWN_PIECE) {
            if ((pieces_[i].side == WHITE_SIDE && pieces_[i].position.y == LAST_WHITE_ROW) ||
                (pieces_[i].side == BLACK_SIDE && pieces_[i].position.y == LAST_BLACK_ROW)) {
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

void ChessGameData::GetCandidates(CoordsVector & victims) const {
    victims.clear();
    if (winner_ == NO_WINNER) {
        for (int i = 0; i < MAX_PIECES; ++i) {
            if (pieces_[i].NotDead() && pieces_[i].side == turn_) {
                victims.push_back(pieces_[i].position);
            }
        }
    }
}

//--------------------------------------------------------------------------------

void ChessGameData::GetPossibleMoves(CoordsVector & victims, const sf::Vector2i & coords) const {
    victims.clear();
    auto index = getPiece(coords);
    if (index != PIECE_NOT_FOUND && pieces_[index].side == turn_) {
        auto * offsets = getMoveOffsets(pieces_[index]);
        if (offsets != nullptr) {
            for (int j = 0; j < MAX_OFFSETS[pieces_[index].type]; ++j) {
                auto destination = pieces_[index].position + offsets[j];
                if (makeFutureMove(turn_, pieces_[index].position, destination)) {
                    victims.push_back(destination);
                }
            }
        }
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

ChessGameData::ChessGameData() : singlePlayer_(false), difficulty_(NORMAL_LEVEL),
    playerSide_(WHITE_SIDE), winner_(NO_WINNER), turn_(WHITE_SIDE), whiteCheck_(false),
    blackCheck_(false) {}

//--------------------------------------------------------------------------------

ChessGameData::ChessGameData(const ChessGameData & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

ChessGameData::~ChessGameData() {}

//--------------------------------------------------------------------------------

ChessGameData & ChessGameData::operator =(const ChessGameData & source) {
    singlePlayer_ = source.singlePlayer_;
    difficulty_ = source.difficulty_;
    playerSide_ = source.playerSide_;
    winner_ = source.winner_;
    turn_ = source.turn_;
    whiteCheck_ = source.whiteCheck_;
    blackCheck_ = source.blackCheck_;
    ForEachInPieces([&] (Piece &, int i) {
        pieces_[i] = source.pieces_[i];
    });
    return *this;
}
