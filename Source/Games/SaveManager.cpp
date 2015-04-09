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

#include "SaveManager.h"
#include <System/File.h>
#include <System/ForEach.h>
#include <Menu/MenuManager.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/Checkers/CheckersManager.h>
#include <Games/Checkers/CheckersSaveGames.h>
#include <Games/Chess/ChessManager.h>
#include <Games/Chess/ChessSaveGames.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Reversi/ReversiManager.h>
#include <Games/Reversi/ReversiSaveGames.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Tetraminoes/TetraminoesManager.h>

//********************************************************************************
// Defines
//********************************************************************************

#define SAVE_DATA_PATH "savedata.bin"

//********************************************************************************
// Static
//********************************************************************************

bool SaveManager::SaveAfterChanges = true;

//********************************************************************************
// InnerData
//********************************************************************************

struct CheckersSaveData {
    CheckersSaveGames saves;
};

struct ChessSaveData {
    ChessSaveGames saves;
};

struct PuckmanSaveData {
    int highScore;
};

struct ReversiSaveData {
    ReversiSaveGames saves;
};

struct SnakeSaveData {
    SnakeManager::RankingArray ranking;
};

struct TetraminoesSaveData {
    TetraminoesManager::RankingArray ranking;
};

struct SaveManager::InnerData {
    // Fields
    CheckersSaveData checkers;
    ChessSaveData chess;
    PuckmanSaveData puckman;
    ReversiSaveData reversi;
    SnakeSaveData snake;
    TetraminoesSaveData tetraminoes;
    // Methods
    void Reset();
    void Load();
    void Save();
};

//--------------------------------------------------------------------------------

void SaveManager::InnerData::Reset() {
    //SnakeSaveData
    for (int i = 0; i < SnakeManager::MAX_RANKING_ENTRIES; ++i) {
        snake.ranking[i] = SnakeManager::RankingEntry();
    }

    //TetraminoesSaveData
    for (int i = 0; i < TetraminoesManager::MAX_RANKING_ENTRIES; ++i) {
        tetraminoes.ranking[i] = TetraminoesManager::RankingEntry();
    }

    //CheckersSaveData
    for (int i = 0; i < CheckersSaveGames::MAX_ENTRIES; ++i) {
        checkers.saves.data_[i] = CheckersSaveGames::Entry();
    }

    //ReversiSaveData
    for (int i = 0; i < ReversiSaveGames::MAX_ENTRIES; ++i) {
        reversi.saves.data_[i] = ReversiSaveGames::Entry();
    }

    //ChessSaveData
    for (int i = 0; i < ChessSaveGames::MAX_ENTRIES; ++i) {
        chess.saves.data_[i] = ChessSaveGames::Entry();
    }

    //PuckmanSaveData
    puckman.highScore = 0;
}

//--------------------------------------------------------------------------------

void SaveManager::InnerData::Load() {
    try {
        File file;
        if (file.OpenForRead(SAVE_DATA_PATH)) {
            //SnakeSaveData
            for (int i = 0; i < SnakeManager::MAX_RANKING_ENTRIES; ++i) {
                file.Read(snake.ranking[i].Name);
                file.Read(snake.ranking[i].Score);
            }

            //TetraminoesSaveData
            for (int i = 0; i < TetraminoesManager::MAX_RANKING_ENTRIES; ++i) {
                file.Read(tetraminoes.ranking[i].Name);
                file.Read(tetraminoes.ranking[i].Score);
            }

            //CheckersSaveData
            for (int i = 0; i < CheckersSaveGames::MAX_ENTRIES; ++i) {
                file.Read(checkers.saves.data_[i].used);
                file.Read(checkers.saves.data_[i].name);
                file.Read(checkers.saves.data_[i].data.singlePlayer_);
                file.Read(checkers.saves.data_[i].data.difficulty_);
                file.Read(checkers.saves.data_[i].data.playerSide_);
                file.Read(checkers.saves.data_[i].data.winner_);
                file.Read(checkers.saves.data_[i].data.turn_);
                file.Read(checkers.saves.data_[i].data.nextPieceToMove_.x);
                file.Read(checkers.saves.data_[i].data.nextPieceToMove_.y);
                checkers.saves.data_[i].data.ForEachInBoard([&] (int, int r, int c) {
                    file.Read(checkers.saves.data_[i].data.board_[r][c]);
                });
                int candidatesSize = 0;
                file.Read(candidatesSize);
                checkers.saves.data_[i].data.candidates_.clear();
                for (int j = 0; j < candidatesSize; ++j) {
                    sf::Vector2i candidate;
                    file.Read(candidate.x);
                    file.Read(candidate.y);
                    checkers.saves.data_[i].data.candidates_.push_back(candidate);
                }
            }

            //ReversiSaveData
            for (int i = 0; i < ReversiSaveGames::MAX_ENTRIES; ++i) {
                file.Read(reversi.saves.data_[i].used);
                file.Read(reversi.saves.data_[i].name);
                file.Read(reversi.saves.data_[i].data.singlePlayer_);
                file.Read(reversi.saves.data_[i].data.difficulty_);
                file.Read(reversi.saves.data_[i].data.playerSide_);
                file.Read(reversi.saves.data_[i].data.winner_);
                file.Read(reversi.saves.data_[i].data.turn_);
                file.Read(reversi.saves.data_[i].data.beginningState_);
                file.Read(reversi.saves.data_[i].data.whiteSideBlocked_);
                file.Read(reversi.saves.data_[i].data.blackSideBlocked_);
                reversi.saves.data_[i].data.ForEachInBoard([&] (int, int r, int c) {
                    file.Read(reversi.saves.data_[i].data.board_[r][c]);
                });
                int candidatesSize = 0;
                file.Read(candidatesSize);
                reversi.saves.data_[i].data.candidates_.clear();
                for (int j = 0; j < candidatesSize; ++j) {
                    sf::Vector2i candidate;
                    file.Read(candidate.x);
                    file.Read(candidate.y);
                    reversi.saves.data_[i].data.candidates_.push_back(candidate);
                }
            }

            //ChessSaveData
            for (int i = 0; i < ChessSaveGames::MAX_ENTRIES; ++i) {
                file.Read(chess.saves.data_[i].used);
                file.Read(chess.saves.data_[i].name);
                file.Read(chess.saves.data_[i].data.singlePlayer_);
                file.Read(chess.saves.data_[i].data.difficulty_);
                file.Read(chess.saves.data_[i].data.playerSide_);
                file.Read(chess.saves.data_[i].data.winner_);
                file.Read(chess.saves.data_[i].data.turn_);
                file.Read(chess.saves.data_[i].data.whiteCheck_);
                file.Read(chess.saves.data_[i].data.blackCheck_);
                chess.saves.data_[i].data.ForEachInPieces(
                    [&] (ChessGameData::Piece &, int idx) {
                        file.Read(chess.saves.data_[i].data.pieces_[idx]);
                    }
                );
            }

            //PuckmanSaveData
            file.Read(puckman.highScore);

            file.Close();
        } else {
            Reset();
        }
    } catch (...) {
        Reset();
    }
}

//--------------------------------------------------------------------------------

void SaveManager::InnerData::Save() {
    try {
        File file;
        if (file.OpenForWrite(SAVE_DATA_PATH)) {
            //SnakeSaveData
            for (int i = 0; i < SnakeManager::MAX_RANKING_ENTRIES; ++i) {
                file.Write(snake.ranking[i].Name);
                file.Write(snake.ranking[i].Score);
            }

            //TetraminoesSaveData
            for (int i = 0; i < TetraminoesManager::MAX_RANKING_ENTRIES; ++i) {
                file.Write(tetraminoes.ranking[i].Name);
                file.Write(tetraminoes.ranking[i].Score);
            }

            //CheckersSaveData
            for (int i = 0; i < CheckersSaveGames::MAX_ENTRIES; ++i) {
                file.Write(checkers.saves.data_[i].used);
                file.Write(checkers.saves.data_[i].name);
                file.Write(checkers.saves.data_[i].data.singlePlayer_);
                file.Write(checkers.saves.data_[i].data.difficulty_);
                file.Write(checkers.saves.data_[i].data.playerSide_);
                file.Write(checkers.saves.data_[i].data.winner_);
                file.Write(checkers.saves.data_[i].data.turn_);
                file.Write(checkers.saves.data_[i].data.nextPieceToMove_.x);
                file.Write(checkers.saves.data_[i].data.nextPieceToMove_.y);
                checkers.saves.data_[i].data.ForEachInBoard([&] (int item, int, int) {
                    file.Write(item);
                });
                int candidatesSize = checkers.saves.data_[i].data.candidates_.size();
                file.Write(candidatesSize);
                for (int j = 0; j < candidatesSize; ++j) {
                    file.Write(checkers.saves.data_[i].data.candidates_[j].x);
                    file.Write(checkers.saves.data_[i].data.candidates_[j].y);
                }
            }

            //ReversiSaveData
            for (int i = 0; i < ReversiSaveGames::MAX_ENTRIES; ++i) {
                file.Write(reversi.saves.data_[i].used);
                file.Write(reversi.saves.data_[i].name);
                file.Write(reversi.saves.data_[i].data.singlePlayer_);
                file.Write(reversi.saves.data_[i].data.difficulty_);
                file.Write(reversi.saves.data_[i].data.playerSide_);
                file.Write(reversi.saves.data_[i].data.winner_);
                file.Write(reversi.saves.data_[i].data.turn_);
                file.Write(reversi.saves.data_[i].data.beginningState_);
                file.Write(reversi.saves.data_[i].data.whiteSideBlocked_);
                file.Write(reversi.saves.data_[i].data.blackSideBlocked_);
                reversi.saves.data_[i].data.ForEachInBoard([&] (int item, int, int) {
                    file.Write(item);
                });
                int candidatesSize = reversi.saves.data_[i].data.candidates_.size();
                file.Write(candidatesSize);
                for (int j = 0; j < candidatesSize; ++j) {
                    file.Write(reversi.saves.data_[i].data.candidates_[j].x);
                    file.Write(reversi.saves.data_[i].data.candidates_[j].y);
                }
            }

            //ChessSaveData
            for (int i = 0; i < ChessSaveGames::MAX_ENTRIES; ++i) {
                file.Write(chess.saves.data_[i].used);
                file.Write(chess.saves.data_[i].name);
                file.Write(chess.saves.data_[i].data.singlePlayer_);
                file.Write(chess.saves.data_[i].data.difficulty_);
                file.Write(chess.saves.data_[i].data.playerSide_);
                file.Write(chess.saves.data_[i].data.winner_);
                file.Write(chess.saves.data_[i].data.turn_);
                file.Write(chess.saves.data_[i].data.whiteCheck_);
                file.Write(chess.saves.data_[i].data.blackCheck_);
                chess.saves.data_[i].data.ForEachInPieces(
                    [&] (ChessGameData::Piece &, int idx) {
                        file.Write(chess.saves.data_[i].data.pieces_[idx]);
                    }
                );
            }

            //PuckmanSaveData
            file.Write(puckman.highScore);

            file.Close();
        }
    } catch (...) {
    }
}

//********************************************************************************
// Methods
//********************************************************************************

void SaveManager::Initialize() {
    if (!initialized_) {
        data_.reset(new InnerData());
        data_->Load();
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

void SaveManager::Release() {
    if (initialized_) {
        if (data_) {
            data_->Save();
            data_.reset(nullptr);
        }
        initialized_ = false;
    }
}

//********************************************************************************
// Checkers Methods
//********************************************************************************

void SaveManager::CheckersLoad() {
    auto * manager = CheckersManager::Instance();
    auto & saves = manager->Saves();
    saves = data_->checkers.saves;
}

//--------------------------------------------------------------------------------

void SaveManager::CheckersSave() {
    auto * manager = CheckersManager::Instance();
    data_->checkers.saves = manager->Saves();

    if (SaveAfterChanges) {
        data_->Save();
    }
}

//********************************************************************************
// Chess Methods
//********************************************************************************

void SaveManager::ChessLoad() {
    auto * manager = ChessManager::Instance();
    auto & saves = manager->Saves();
    saves = data_->chess.saves;
}

//--------------------------------------------------------------------------------

void SaveManager::ChessSave() {
    auto * manager = ChessManager::Instance();
    data_->chess.saves = manager->Saves();

    if (SaveAfterChanges) {
        data_->Save();
    }
}

//********************************************************************************
// Puckman Methods
//********************************************************************************

void SaveManager::PuckmanLoad() {
    auto * manager = Puckman::Manager::Instance();
    manager->DataInstance()->HighScore(data_->puckman.highScore);
}

//--------------------------------------------------------------------------------

void SaveManager::PuckmanSave() {
    auto * manager = Puckman::Manager::Instance();
    data_->puckman.highScore = manager->DataInstance()->HighScore();

    if (SaveAfterChanges) {
        data_->Save();
    }
}

//********************************************************************************
// Reversi Methods
//********************************************************************************

void SaveManager::ReversiLoad() {
    auto * manager = ReversiManager::Instance();
    auto & saves = manager->Saves();
    saves = data_->reversi.saves;
}

//--------------------------------------------------------------------------------

void SaveManager::ReversiSave() {
    auto * manager = ReversiManager::Instance();
    data_->reversi.saves = manager->Saves();

    if (SaveAfterChanges) {
        data_->Save();
    }
}

//********************************************************************************
// Snake Methods
//********************************************************************************

void SaveManager::SnakeLoad() {
    auto * manager = SnakeManager::Instance();
    auto & ranking = manager->Ranking();
    ranking = data_->snake.ranking;
}

//--------------------------------------------------------------------------------

void SaveManager::SnakeSave() {
    auto * manager = SnakeManager::Instance();
    data_->snake.ranking = manager->Ranking();

    if (SaveAfterChanges) {
        data_->Save();
    }
}

//********************************************************************************
// Tetraminoes Methods
//********************************************************************************

void SaveManager::TetraminoesLoad() {
    auto * manager = TetraminoesManager::Instance();
    auto & ranking = manager->Ranking();
    ranking = data_->tetraminoes.ranking;
}

//--------------------------------------------------------------------------------

void SaveManager::TetraminoesSave() {
    auto * manager = TetraminoesManager::Instance();
    data_->tetraminoes.ranking = manager->Ranking();

    if (SaveAfterChanges) {
        data_->Save();
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
SaveManager * SaveManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
SaveManager::SaveManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
SaveManager::~SaveManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
SaveManager * SaveManager::Instance() {
    if (!instance_) {
        instance_ = new SaveManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
SaveManager & SaveManager::Reference() {
    return *(Instance());
}
