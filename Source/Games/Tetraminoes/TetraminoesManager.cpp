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

#include "TetraminoesManager.h"
#include "TetraminoesInnerData.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Keyboard.h>
#include <Menu/DesktopState.h>
#include <Games/SaveManager.h>
#include <Games/Tetraminoes/TetraminoesGameState.h>
#include <Games/Tetraminoes/TetraminoesGameMenuState.h>
#include <Games/Tetraminoes/TetraminoesEnterNameState.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & TetraminoesManager::Tileset()         { return data_->tileset;         }
SharedTexture & TetraminoesManager::Menu1Background() { return data_->menu1Background; }
SharedTexture & TetraminoesManager::Menu2Background() { return data_->menu2Background; }
SharedTexture & TetraminoesManager::GameBackground()  { return data_->gameBackground;  }

//--------------------------------------------------------------------------------

TetraminoesManager::RankingArray & TetraminoesManager::Ranking() {
    return data_->ranking_;
}

//--------------------------------------------------------------------------------

int TetraminoesManager::Score() {
    return data_->score;
}

//--------------------------------------------------------------------------------

Sound & TetraminoesManager::KeyboardSound() { return data_->keyboardSound; }
Sound & TetraminoesManager::HitSound()      { return data_->hitSound;      }
Sound & TetraminoesManager::FallSound()     { return data_->fallSound;     }
Sound & TetraminoesManager::LineSound()     { return data_->lineSound;     }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void TetraminoesManager::Initialize() {
    if (!initialized_) {
        data_.reset(new InnerData());
        data_->Initialize();
        SaveManager::Instance()->TetraminoesLoad();
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void TetraminoesManager::Release() {
    if (initialized_ && data_) {
        data_->Release();
        data_.reset(nullptr);
    }
    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//--------------------------------------------------------------------------------

/**
 * Checks if a score is in the top.
 */
bool TetraminoesManager::IsTopScore(int score) {
    return data_->ranking_[MAX_RANKING_ENTRIES - 1].Score <= score;
}

//--------------------------------------------------------------------------------

/**
 * Adds a score into the top.
 */
void TetraminoesManager::AddScore(const std::string & name, int score) {
    for (int i = 0; i < MAX_RANKING_ENTRIES; ++i) {
        if (data_->ranking_[i].Score <= score) {
            // After find the position of the score in the ranking, we'll
            // move down all the other scores in the ranking.
            for (int j = MAX_RANKING_ENTRIES - 1; j > i; --j) {
                data_->ranking_[j].Name = data_->ranking_[j - 1].Name;
                data_->ranking_[j].Score = data_->ranking_[j - 1].Score;
            }
            // Set the last top score and exit.
            data_->ranking_[i].Score = score;
            data_->ranking_[i].Name = name;
            break;
        }
    }
}

//********************************************************************************
// Game methods
//********************************************************************************

/**
 * Starts a new game.
 */
void TetraminoesManager::StartGame(int difficulty) {
    data_->StartGame(difficulty);
    CoreManager::Instance()->SetNextState(MakeSharedState<TetraminoesGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void TetraminoesManager::DrawGame() {
    data_->levelLabel->Draw();
    data_->linesLabel->Draw();
    data_->scoreLabel->Draw();

    if (data_->gameOver) {
        data_->gameOverLabel->Draw();
    } else {
        data_->RenderWorld();
        data_->RenderPiece();
        data_->RenderNextPiece();
        if (data_->gamePaused) {
            data_->gamePaused1Label->Draw();
            data_->gamePaused2Label->Draw();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void TetraminoesManager::UpdateGame(const sf::Time & timeDelta) {
    if (data_->gameOver) {
        // The game is over, check if the score is a top one and change the state.
        if (Keyboard::IsAnyKeyUp()) {
            KeyboardSound().Play();
            if (IsTopScore(data_->score)) {
                data_->core->SetNextState(MakeSharedState<TetraminoesEnterNameState>());
            } else {
                data_->core->SetNextState(MakeSharedState<TetraminoesMenuState>());
            }
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        // Lets invoke the game menu of the game.
        KeyboardSound().Play();
        data_->core->SetNextState(MakeSharedState<TetraminoesGameMenuState>());

    } else if (!data_->gamePaused) {
        // We are now playing the game, so lets update the time controller.
        data_->time += timeDelta.asMilliseconds();
        data_->keyTimeMark += timeDelta.asMilliseconds();

        // Here we'll check the repeatable keys input.
        if (!data_->keyPressed) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                data_->MoveLeft();
                data_->SetKeyPressed();
            } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                data_->MoveRight();
                data_->SetKeyPressed();
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                data_->MoveDown();
                data_->SetKeyPressed();
            }
        } else if (!Keyboard::IsAnyKeyPressed() || data_->keyTimeMark > MAX_KEY_INTERVAL) {
            data_->keyPressed = false;
        }

        // Here we'll check the other keys input.
        if (Keyboard::IsKeyUp(Keyboard::A) || Keyboard::IsKeyUp(Keyboard::Space)) {
            data_->TurnRight();

        } else if (Keyboard::IsKeyUp(Keyboard::D) || Keyboard::IsKeyUp(Keyboard::Up)) {
            data_->TurnLeft();
        }
        if (Keyboard::IsKeyUp(Keyboard::X)) {
            data_->FallDown();
        }
        if (Keyboard::IsKeyUp(Keyboard::P)) {
            data_->gamePaused = true;
        }

        // After some interval of time we'll move the piece down.
        if (data_->time >= data_->maxInterval) {
            data_->time -= data_->maxInterval;
            data_->MoveDown();
        }

    } else if (Keyboard::IsKeyUp(Keyboard::P)) {
        data_->gamePaused = false;
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
TetraminoesManager * TetraminoesManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
TetraminoesManager::TetraminoesManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
TetraminoesManager::~TetraminoesManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
TetraminoesManager * TetraminoesManager::Instance() {
    if (!instance_) {
        instance_ = new TetraminoesManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
TetraminoesManager & TetraminoesManager::Reference() {
    return *(Instance());
}
