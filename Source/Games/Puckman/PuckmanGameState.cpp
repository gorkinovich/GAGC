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

#include "PuckmanGameState.h"
#include <sstream>
#include <iomanip>
#include <SFML/System/Time.hpp>
#include <System/Sound.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Puckman/PuckmanGameTimer.h>
#include <Games/Puckman/PuckmanSprites.h>
#include <Games/Puckman/PuckmanInsertCoinState.h>
#include <Games/Puckman/PuckmanSelectPlayerState.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Gets the current fruit for the level.
 */
FruitTypeEnum GameState::getCurrentFruit() {
    if(data_->Level() <= 1) {
        return FruitType::Cherry;
    } else if(data_->Level() <= 2) {
        return FruitType::Strawberry;
    } else if(data_->Level() <= 4) {
        return FruitType::Peach;
    } else if(data_->Level() <= 6) {
        return FruitType::Apple;
    } else if(data_->Level() <= 8) {
        return FruitType::Grape;
    } else if(data_->Level() <= 10) {
        return FruitType::Flower;
    } else if(data_->Level() <= 12) {
        return FruitType::Bell;
    } else {
        return FruitType::Key;
    }
}

//--------------------------------------------------------------------------------

/**
 * Starts a new generic game.
 */
void GameState::startGenericGame() {
    scoreBlinkInterval_ = 0;
    showScore_ = true;
    data_->PlayerTurn(1);
    FruitTypeEnum currentFruit = getCurrentFruit();
    data_->LastFruits(std::vector<FruitTypeEnum>());
    auxLastFruits_ = std::vector<FruitTypeEnum>();
    auxLastFruits_.push_back(currentFruit);
    data_->AddFruit(currentFruit);
    data_->PlayerData().ExtraLives(data_->PlayerData().ExtraLives() - 1);
    logic_.Initialize(currentFruit, true, false);
}

//--------------------------------------------------------------------------------

/**
 * Starts a new one player game.
 */
void GameState::StartOnePlayerGame() {
    data_->Coins(data_->Coins() - 1);
    GenericState::showScorePlayer2_ = false;
    data_->NumberOfPlayers(1);
    data_->Player1().NewGame();
    startGenericGame();
}

//--------------------------------------------------------------------------------

/**
 * Starts a new two players game.
 */
void GameState::StartTwoPlayersGame() {
    data_->Coins(data_->Coins() - 2);
    GenericState::showScorePlayer2_ = true;
    data_->NumberOfPlayers(2);
    data_->Player1().NewGame();
    data_->Player2().NewGame();
    startGenericGame();
}

//--------------------------------------------------------------------------------

/**
 * Initializes the state.
 */
void GameState::Initialize() {
}

//--------------------------------------------------------------------------------

/**
 * Releases the state.
 */
void GameState::Release() {
}

//--------------------------------------------------------------------------------

/**
 * Draws the game HUD.
 */
void GameState::drawGameHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << std::setfill('0') << std::setw(2) << victim;
        return strconv.str();
    };

    if(showScore_ || data_->PlayerTurn() == 2) {
        manager_->DrawText(0, 3, "1UP");
    }
    manager_->DrawTextLeft(1, 6, ToString(data_->Player1().Score()));

    manager_->DrawText(0, 9, "HIGH SCORE");
    if(data_->HighScore() > 0) {
        manager_->DrawTextLeft(1, 16, ToString(data_->HighScore()));
    }

    if(showScorePlayer2_) {
        if(showScore_ || data_->PlayerTurn() == 1) {
            manager_->DrawText(0, 22, "2UP");
        }
        manager_->DrawTextLeft(1, 25, ToString(data_->Player2().Score()));
    }

    drawLivesHud();
    drawFruitsHud();
}

//--------------------------------------------------------------------------------

/**
 * Draws the content of the state.
 */
void GameState::Draw(const sf::Time & timeDelta) {
    logic_.Draw();

    manager_->ClearSprite(Manager::WORMHOLE_LX - Sprites::Width, Manager::WORMHOLE_Y);
    manager_->ClearSprite(Manager::WORMHOLE_LX, Manager::WORMHOLE_Y);
    manager_->ClearSprite(Manager::WORMHOLE_RX, Manager::WORMHOLE_Y);
    manager_->ClearSprite(Manager::WORMHOLE_RX + Sprites::Width, Manager::WORMHOLE_Y);

    drawGameHud();
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner logic of the state.
 */
void GameState::Update(const sf::Time & timeDelta) {
    auto time = timeDelta.asMilliseconds();

    // Ends the current game.
    if(Keyboard::IsKeyUp(Keyboard::F5)) {
        getBackToTheMenu();
        return;
    }

    // Updates the game logic of this round.
    GameTimer::Update(time);
    logic_.Update(time);

    // Updates the score blinking.
    scoreBlinkInterval_ += time;
    while(scoreBlinkInterval_ >= SCORE_BLINK_DELAY_TIME) {
        scoreBlinkInterval_ -= SCORE_BLINK_DELAY_TIME;
        showScore_ = !showScore_;
    }

    // Checks if the round have end and if puckman is victorious or not.
    if(logic_.Ended()) {
        if(logic_.Victory()) {
            nextLevel();
        } else {
            if(data_->NumberOfPlayers() == 1) {
                nextLife();
            } else {
                nextTurn();
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Goes to the next life of the one player game.
 */
void GameState::nextLevel() {
    data_->PlayerData().NextLevel();
    FruitTypeEnum currentFruit = getCurrentFruit();
    data_->AddFruit(currentFruit);
    logic_.Initialize(currentFruit, false, true);
}

//--------------------------------------------------------------------------------

/**
 * Goes to the next life of the one player game.
 */
void GameState::nextLife() {
    // First, we'll check if puckman can play one more round.
    if(data_->ExtraLives() > 0) {
        // Second, we'll quit one extra life.
        data_->ExtraLives(data_->ExtraLives() - 1);
        // And we'll initialize the current round logic.
        logic_.Initialize(getCurrentFruit(), false, false);
    } else {
        // But if the player can't play we'll get back to the menu.
        getBackToTheMenu();
    }
}

//--------------------------------------------------------------------------------

/**
 * Goes to the next life of the two players game.
 */
void GameState::nextTurn() {
    // First, we'll change the current player turn.
    data_->ChangeTurn();
    // Then, we'll check if the next player can play or not another round.
    if(data_->ExtraLives() > 0) {
        // If the player can play, we'll get the user last fruits array.
        swapLastFruits();
        // After that we'll get an extra life.
        data_->ExtraLives(data_->ExtraLives() - 1);
        // And we'll initialize the current round logic.
        logic_.Initialize(getCurrentFruit(), false, false);
    } else {
        // If the next player can't play, we'll get back to the previous player.
        data_->ChangeTurn();
        // And we'll check if the previous player can play or not another round.
        if(data_->ExtraLives() > 0) {
            // If the player can play, we'll get an extra life.
            data_->ExtraLives(data_->ExtraLives() - 1);
            // And we'll initialize the current round logic.
            logic_.Initialize(getCurrentFruit(), false, false);
        } else {
            // If nobody can't play, we'll get back to the menu.
            getBackToTheMenu();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Swaps the last fruits array.
 */
void GameState::swapLastFruits() {
    auto aux = data_->LastFruits();
    data_->LastFruits(auxLastFruits_);
    auxLastFruits_ = aux;
}

//--------------------------------------------------------------------------------

/**
 * Gets back to the menu of the game.
 */
void GameState::getBackToTheMenu() {
    // Stop all the loop sounds first.
    manager_->StopAll();
    // And then change the inner state of the program.
    if(data_->Coins() > GameData::MIN_COINS) {
        manager_->SetNextState(Manager::Instance()->SelectPlayerStateInstance());
    } else {
        manager_->SetNextState(Manager::Instance()->InsertCoinStateInstance());
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
GameState::GameState() : GenericState(), logic_(), scoreBlinkInterval_(0),
    showScore_(false), auxLastFruits_() {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
GameState::~GameState() {}
