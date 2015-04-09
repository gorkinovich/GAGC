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

#include "ChessMiniMax.h"
#include <limits>
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const int INITIAL_ALPHA = std::numeric_limits<int>::min();
const int INITIAL_BETA  = std::numeric_limits<int>::max();

const int AI_DEFEAT  = std::numeric_limits<int>::min();
const int AI_VICTORY = std::numeric_limits<int>::max();

const int PAWN_VALUE   =     1;
const int ROOK_VALUE   =     5;
const int KNIGHT_VALUE =     3;
const int BISHOP_VALUE =     3;
const int QUEEN_VALUE  =     9;
const int KING1_VALUE  =   100;
const int KING2_VALUE  =  1000;

const int PIECES1_VALUES[ChessGameData::PIECE_TYPES] = {
    PAWN_VALUE, ROOK_VALUE, KNIGHT_VALUE,
    BISHOP_VALUE, QUEEN_VALUE, KING1_VALUE
};

const int PIECES2_VALUES[ChessGameData::PIECE_TYPES] = {
    PAWN_VALUE, ROOK_VALUE, KNIGHT_VALUE,
    BISHOP_VALUE, QUEEN_VALUE, KING2_VALUE
};

const int OPPOSITE_SIDE[ChessGameData::MAX_SIDES] = {
    ChessGameData::BLACK_SIDE, ChessGameData::WHITE_SIDE
};

const int POSITION_VALUE =  1;
const int THREAT_VALUE   = 16;
const int ALIVE_VALUE    = 64;

const int NORMAL_STATE     = 0;
const int QUEEN_END_STATE  = 1;
const int ROOK_END_STATE   = 2;
const int BISHOP_END_STATE = 3;

//********************************************************************************
// ChessMove
//********************************************************************************

ChessMove::ChessMove() : origin(-1, -1), destination(-1, -1) {}

ChessMove::ChessMove(const sf::Vector2i & orig, const sf::Vector2i & dest)
    : origin(orig), destination(dest) {}

//--------------------------------------------------------------------------------

bool ChessMove::MakeMove(ChessGameData & victim) {
    if (victim.MakeMove(origin, destination)) {
        if (victim.AnyPawnToConvert()) {
            victim.ConvertPawnToQueen();
        }
        victim.NextTurn();
        return true;
    } else {
        return false;
    }
}

//********************************************************************************
// Methods (Public)
//********************************************************************************

void ChessMiniMax::Initialize(int difficulty) {
    state_ = NORMAL_STATE;
    if (difficulty == ChessGameData::HARD_LEVEL) {
        maxDepth_ = 3;
    } else if (difficulty == ChessGameData::NORMAL_LEVEL) {
        maxDepth_ = 2;
    } else {
        maxDepth_ = 1;
    }
}

//--------------------------------------------------------------------------------

ChessMove ChessMiniMax::Execute(ChessGameData & data) {
    if (state_ == NORMAL_STATE) checkState(data);
    return minimax(data, 0, INITIAL_ALPHA, INITIAL_BETA);
}

//********************************************************************************
// Methods (Private)
//********************************************************************************

void ChessMiniMax::checkState(ChessGameData & data) {
    // Count all the types of pieces by side.
    int countPieces[ChessGameData::MAX_SIDES][ChessGameData::PIECE_TYPES];
    std::memset(countPieces, 0, ChessGameData::MAX_SIDES * ChessGameData::PIECE_TYPES);
    for (int i = 0; i < ChessGameData::MAX_PIECES; ++i) {
        auto & victim = data.pieces_[i];
        if (victim.NotDead()) {
            countPieces[victim.side][victim.type]++;
        }
    }

    // Check that the enemy only has the king.
    bool onlyPlayerKing = true;
    int playerSide = data.PlayerSide();
    for (int i = 0; i < ChessGameData::KING_PIECE; ++i) {
        if (countPieces[playerSide][i] > 0) {
            onlyPlayerKing = false;
            break;
        }
    }

    // Check the machine pieces.
    int machineSide = OPPOSITE_SIDE[playerSide];
    if (onlyPlayerKing && countPieces[machineSide][ChessGameData::PAWN_PIECE] == 0) {
        if (countPieces[machineSide][ChessGameData::QUEEN_PIECE] > 0) {
            state_ = QUEEN_END_STATE;
        } else if (countPieces[machineSide][ChessGameData::ROOK_PIECE] > 0) {
            state_ = ROOK_END_STATE;
        } else if (countPieces[machineSide][ChessGameData::BISHOP_PIECE] > 1) {
            state_ = BISHOP_END_STATE;
        } else {
            state_ = NORMAL_STATE;
        }
    }
}

//--------------------------------------------------------------------------------

void ChessMiniMax::getPossibleAllMoves(ChessGameData & data, ChessMoveVector & moves) {
    // Check all the pieces of the current turn that are alive.
    for (int i = 0; i < ChessGameData::MAX_PIECES; ++i) {
        auto & victim = data.pieces_[i];
        if (victim.side == data.turn_ && victim.NotDead()) {
            // And add all the possible destinations for each of them.
            auto * offsets = data.getMoveOffsets(victim);
            if (offsets != nullptr) {
                for (int j = 0; j < ChessGameData::MAX_OFFSETS[victim.type]; ++j) {
                    auto destination = victim.position + offsets[j];
                    moves.push_back(ChessMove(victim.position, destination));
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------

ChessMove ChessMiniMax::minimax(ChessGameData & data, int depth, int alpha, int beta) {
    ChessMove result;
    ChessGameData::ShowErrorMessages(false);
    if (!data.GameOver()) {
        // If the game is not over, get all the moves inside the board.
        ChessMoveVector moves;
        getPossibleAllMoves(data, moves);

        // If we have some moves to check, we'll test if there is a checkmate.
        if (!moves.empty()) {
            if (data.isCheckmate()) {
                data.whiteCheck_ = data.blackCheck_ = true;
                // Find the first move to do and return it.
                auto it = moves.begin(), end = moves.end();
                for (; it != end; ++it) {
                    ChessGameData future(data);
                    if (it->MakeMove(future)) {
                        return ChessMove(*it);
                    }
                }
            } else {
                data.whiteCheck_ = data.blackCheck_ = false;
                // For each move we'll try to get the maximum result.
                int actval, maxval = INITIAL_ALPHA;
                auto it = moves.begin(), end = moves.end();
                for (; it != end; ++it) {
                    ChessGameData future(data);
                    if (it->MakeMove(future)) {
                        actval = minimizer(future, depth + 1, alpha, beta);
                        if (maxval < actval || result.origin.x == -1) {
                            maxval = actval;
                            alpha = actval;
                            result = ChessMove(*it);
                        }
                    }
                }
            }
        }
    }
    ChessGameData::ShowErrorMessages(true);
    return result;
}

//--------------------------------------------------------------------------------

int ChessMiniMax::minimizer(ChessGameData & data, int depth, int alpha, int beta) {
    // If we reach the maximum depth o the game is over, we'll evaluate the current
    // state of the game. If the current side does not have any moves the game ends,
    // so is not necesary to check if we have or not moves when there is no game over.
    if (checkStop(data, depth)) {
        return evaluate(data, depth);
    } else {
        // If the game is not over, get all the moves inside the board.
        ChessMoveVector moves;
        getPossibleAllMoves(data, moves);

        // We'll initialize some variables.
        int actval, minval = INITIAL_BETA;
        auto it = moves.begin(), end = moves.end();
        // And then for each move we'll try to get the minimum result.
        for (; it != end; ++it) {
            ChessGameData future(data);
            if (it->MakeMove(future)) {
                actval = maximizer(future, depth + 1, alpha, beta);

                if(minval > actval) {
                    minval = actval;
                    beta = actval;
                }

                if(alpha > beta) {
                    return minval;
                }
            }
        }
        return minval;
    }
}

//--------------------------------------------------------------------------------

int ChessMiniMax::maximizer(ChessGameData & data, int depth, int alpha, int beta) {
    // If we reach the maximum depth o the game is over, we'll evaluate the current
    // state of the game. If the current side does not have any moves the game ends,
    // so is not necesary to check if we have or not moves when there is no game over.
    if (checkStop(data, depth)) {
        return evaluate(data, depth);
    } else {
        // If the game is not over, get all the moves inside the board.
        ChessMoveVector moves;
        getPossibleAllMoves(data, moves);

        // We'll initialize some variables.
        int actval, maxval = INITIAL_ALPHA;
        auto it = moves.begin(), end = moves.end();
        // And then for each move we'll try to get the maximum result.
        for (; it != end; ++it) {
            ChessGameData future(data);
            if (it->MakeMove(future)) {
                actval = minimizer(future, depth + 1, alpha, beta);

                if(maxval < actval) {
                    maxval = actval;
                    alpha = actval;
                }

                if(alpha > beta) {
                    return maxval;
                }
            }
        }
        return maxval;
    }
}

//--------------------------------------------------------------------------------

bool ChessMiniMax::checkStop(ChessGameData & data, int depth) {
    return maxDepth_ < depth || data.GameOver() ||
           data.WhiteCheck() || data.BlackCheck();
}

//--------------------------------------------------------------------------------

int ChessMiniMax::evaluate(ChessGameData & data, int depth) {
    if (data.PlayerSide() == ChessGameData::WHITE_SIDE) {
        if (data.BlackCheck()) {
            return AI_DEFEAT + depth;
        } else if (data.WhiteCheck()) {
            return AI_VICTORY + depth;
        } else {
            return evaluate(data);
        }
    } else {
        if (data.WhiteCheck()) {
            return AI_DEFEAT + depth;
        } else if (data.BlackCheck()) {
            return AI_VICTORY + depth;
        } else {
            return evaluate(data);
        }
    }
}

//--------------------------------------------------------------------------------

int ChessMiniMax::evaluate(ChessGameData & data) {
    if (data.GameOver()) {
        // When there is a winner, there is no other option in life.
        return data.Winner() == ChessGameData::AI_WINNER ? AI_VICTORY : AI_DEFEAT;

    } else {
        // Let's prepare some counters, yeah... Happiness everywhere...
        int alivePoints[ChessGameData::MAX_SIDES] = { 0, 0 };
        int threatPoints[ChessGameData::MAX_SIDES] = { 0, 0 };
        int positionPoints[ChessGameData::MAX_SIDES] = { 0, 0 };

        // Check each alive piece in the game to evaluate the alive, position and threat factors.
        for (int i = 0; i < ChessGameData::MAX_PIECES; ++i) {
            auto & victim = data.pieces_[i];
            if (ChessGameData::PAWN_PIECE <= victim.type && victim.type <= ChessGameData::KING_PIECE) {
                int pieceValue = PIECES2_VALUES[victim.type];
                alivePoints[victim.side] += pieceValue;
                positionPoints[victim.side] += pieceValue * evaluatePosition(data, victim);
                if (victim.type != ChessGameData::KING_PIECE && isInDanger(data, victim)) {
                    threatPoints[OPPOSITE_SIDE[victim.side]] += pieceValue;
                }
            }
        }

        // Recount all the factors by side.
        int blackPoints = alivePoints[ChessGameData::BLACK_SIDE] * ALIVE_VALUE +
                          positionPoints[ChessGameData::BLACK_SIDE] * POSITION_VALUE +
                          threatPoints[ChessGameData::BLACK_SIDE] * THREAT_VALUE;
        int whitePoints = alivePoints[ChessGameData::WHITE_SIDE] * ALIVE_VALUE +
                          positionPoints[ChessGameData::WHITE_SIDE] * POSITION_VALUE +
                          threatPoints[ChessGameData::WHITE_SIDE] * THREAT_VALUE;

        // And return the final evaluation value.
        return data.PlayerSide() == ChessGameData::WHITE_SIDE ?
            blackPoints - whitePoints : whitePoints - blackPoints;
    }
}

//--------------------------------------------------------------------------------

int ChessMiniMax::evaluatePosition(ChessGameData & data, ChessGameData::Piece & victim) {
    switch (state_) {
    case NORMAL_STATE:     return 0; //evaluatePositionNormal(victim);
    case QUEEN_END_STATE:  return evaluatePositionQueenOrRook(data, victim);
    case ROOK_END_STATE:   return evaluatePositionQueenOrRook(data, victim);
    case BISHOP_END_STATE: return 0; //evaluatePositionNormal(victim);
    }
    return 0;
}

//--------------------------------------------------------------------------------

int ChessMiniMax::evaluatePositionNormal(ChessGameData::Piece & victim) {
    // The more you go forward, the more you like it.
    if (victim.type != ChessGameData::KING_PIECE) {
        return victim.side == ChessGameData::WHITE_SIDE ?
            victim.position.y + 1 : 8 - victim.position.y;
    }
    return 0;
}

//--------------------------------------------------------------------------------

int ChessMiniMax::evaluatePositionQueenOrRook(ChessGameData & data, ChessGameData::Piece & victim) {
    int playerSide = data.PlayerSide();
    if (victim.side == playerSide) {
        // When you only have a king, the more centered you are, the more safe that will be.
        if (victim.position.y == 0 || victim.position.y == 7 ||
            victim.position.x == 0 || victim.position.x == 7) {
            return 0;
        } else if (victim.position.y == 1 || victim.position.y == 6 ||
                   victim.position.x == 1 || victim.position.x == 6) {
            return 10;
        } else if (victim.position.y == 2 || victim.position.y == 5 ||
                   victim.position.x == 2 || victim.position.x == 5) {
            return 20;
        } else if (victim.position.y == 3 || victim.position.y == 4 ||
                   victim.position.x == 3 || victim.position.x == 4) {
            return 40;
        } else {
            return 80;
        }
    } else {
        // In the other side, our army should get near and near to the other king.
        auto index = data.getPieceByType(playerSide, ChessGameData::KING_PIECE);
        if (index != ChessGameData::PIECE_NOT_FOUND) {
            auto & player = data.pieces_[index];
            int dx = std::abs(player.position.x - victim.position.x);
            int dy = std::abs(player.position.y - victim.position.y);

            if (victim.type == ChessGameData::KING_PIECE) {
                return -(dx > dy ? dx : dy);

            } else if (victim.type == ChessGameData::QUEEN_PIECE) {
                return (dx <= 1 && dy <= 1 ? -QUEEN_VALUE : QUEEN_VALUE) * 10;

            } else if (victim.type == ChessGameData::ROOK_PIECE) {
                return (dx <= 1 && dy <= 1 ? -ROOK_VALUE : ROOK_VALUE) * 10;
            }
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------

bool ChessMiniMax::isInDanger(ChessGameData & data, ChessGameData::Piece & victim) {
    // Get all the threats to the current pieces.
    std::vector<int> enemies;
    findThreats(data, victim, enemies);
    if (enemies.empty()) return false;

    // Get all the allies to the current pieces.
    std::vector<int> allies;
    findDefenses(data, victim, allies);
    if (allies.empty()) return true;

    // Calculate the value of the enemies and sort it.
    for (unsigned int k = 0; k < enemies.size(); ++k) {
        enemies[k] = PIECES1_VALUES[data.pieces_[enemies[k]].type];
    }
    std::sort(enemies.begin(), enemies.end());

    // Calculate the value of the allies and sort it. But before the
    // sort operatio we'll add a negative value to be able to set the
    // current piece as the head of the list of values in the attack.
    for (unsigned int k = 0; k < allies.size(); ++k) {
        allies[k] = PIECES1_VALUES[data.pieces_[allies[k]].type];
    }
    allies.push_back(-1);
    std::sort(allies.begin(), allies.end());
    allies[0] = PIECES1_VALUES[victim.type];

    bool oneMoreTime;
    int lost = 0, gain = 0;
    unsigned int i = 0, j = 0;
    do {
        // We start thinking that we're going to exit the loop.
        oneMoreTime = false;
        // The enemy moves first and test the value of the lowest
        // piece with the current in danger in this moment.
        if (enemies[i] <= allies[j]) {
            lost += allies[j]; ++j;
            // Check that we still have allies to attack the enemy.
            if (j < allies.size()) {
                // We'll move if our lowest piece in value is worse
                // than the current piece of the enemy.
                if (enemies[i] >= allies[j]) {
                    gain += enemies[i]; ++i;
                    // Check that the enemy has pieces to attack.
                    if (i < enemies.size()) {
                        oneMoreTime = true;
                    }
                }
            }
        }
    } while (oneMoreTime);

    // The victim will be in danger if the lost is bigger than the gain.
    return lost > gain;
}

//--------------------------------------------------------------------------------

void ChessMiniMax::findDefenses(ChessGameData & data, ChessGameData::Piece & victim,
    std::vector<int> & allies) {
    // Our victim is going to the enemy side, madness!!!
    auto backUp = victim;
    victim.side = OPPOSITE_SIDE[victim.side];
    // Test if any ally piece can get into your position and destroy you.
    for (int i = 0; i < ChessGameData::MAX_PIECES; ++i) {
        auto & ally = data.pieces_[i];
        if (ally.side == backUp.side && ally.NotDead()) {
            ChessGameData future(data);
            if (future.MakeMove(ally.position, victim.position)) {
                allies.push_back(i);
            }
        }
    }
    victim = backUp;
}

//--------------------------------------------------------------------------------

void ChessMiniMax::findThreats(ChessGameData & data, ChessGameData::Piece & victim,
    std::vector<int> & enemies) {
    // Test if any enemy can get into your position and finish your life.
    int enemySide = OPPOSITE_SIDE[victim.side];
    for (int i = 0; i < ChessGameData::MAX_PIECES; ++i) {
        auto & enemy = data.pieces_[i];
        if (enemy.side == enemySide && enemy.NotDead()) {
            ChessGameData future(data);
            if (future.MakeMove(enemy.position, victim.position)) {
                enemies.push_back(i);
            }
        }
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

ChessMiniMax::ChessMiniMax() {}

//--------------------------------------------------------------------------------

ChessMiniMax::~ChessMiniMax() {}
