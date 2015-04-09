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

#include "BattleshipManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <Menu/DesktopState.h>
#include <Games/Battleship/BattleshipBoard.h>
#include <Games/Battleship/BattleshipEnterNameState.h>
#include <Games/Battleship/BattleshipPutShipsState.h>
#include <Games/Battleship/BattleshipGameState.h>
#include <Games/Battleship/BattleshipWaitState.h>
#include <Games/Battleship/BattleshipGameMenuState.h>
#include <Games/Battleship/BattleshipGameOverState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int P1_NAME_STATE  = 0;
const int P1_SHIPS_STATE = 1;
const int P2_NAME_STATE  = 2;
const int P2_SHIPS_STATE = 3;
const int P1_PLAY_STATE  = 4;
const int P1_WAIT_STATE  = 5;
const int P2_PLAY_STATE  = 6;
const int P2_WAIT_STATE  = 7;

//********************************************************************************
// InnerData
//********************************************************************************

struct BattleshipManager::Player {
    int turn;
    std::string name;
    BattleshipBoard board;
};

struct BattleshipManager::InnerData {
    SharedTexture tileset;

    int winner;
    int substate;
    int numberOfPlayers;

    BattleshipManager::Player player1;
    BattleshipManager::Player player2;

    Sound keyboardSound;
    Sound clickSound;

    Sound waterSound;
    Sound hitSound;
    Sound sunkSound;
};

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & BattleshipManager::Tileset() {
    return data_->tileset;
}

//--------------------------------------------------------------------------------

int BattleshipManager::Winner() {
    return data_->winner;
}

//--------------------------------------------------------------------------------

const std::string & BattleshipManager::Player1Name() {
    return data_->player1.name;
}

//--------------------------------------------------------------------------------

const std::string & BattleshipManager::Player2Name() {
    return data_->player2.name;
}

//--------------------------------------------------------------------------------

Sound & BattleshipManager::KeyboardSound() { return data_->keyboardSound; }
Sound & BattleshipManager::ClickSound()    { return data_->clickSound;    }

Sound & BattleshipManager::WaterSound()    { return data_->waterSound;    }
Sound & BattleshipManager::HitSound()      { return data_->hitSound;      }
Sound & BattleshipManager::SunkSound()     { return data_->sunkSound;     }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void BattleshipManager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();

        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->tileset = core->LoadTexture("Content/Textures/Battleship.png");

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");
        data_->waterSound.Load("Content/Sounds/BattleshipWater.wav");
        data_->hitSound.Load("Content/Sounds/BattleshipHit.wav");
        data_->sunkSound.Load("Content/Sounds/BattleshipSunk.wav");

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void BattleshipManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();
        data_->clickSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->clickSound.Unload();
        data_->waterSound.Unload();
        data_->hitSound.Unload();
        data_->sunkSound.Unload();

        // Unload the textures of the game.
        data_->tileset = nullptr;

        // Remove the inner data of the game.
        data_.reset(nullptr);
    }

    // Change the initialized flag and set the next state.
    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//********************************************************************************
// Game methods
//********************************************************************************

/**
 * Starts a new game.
 */
void BattleshipManager::StartGame(int numberOfPlayers) {
    auto * core = CoreManager::Instance();
    auto language = core->Language();

    // Set the general fields.
    data_->winner = NO_WINNER;
    data_->substate = P1_NAME_STATE;
    data_->numberOfPlayers = numberOfPlayers;

    // Initialize the players.
    auto initPlayer = [language] (Player & victim) {
        victim.turn = 0;
        victim.name = language == TEXT_LANGUAGE_SPANISH ?
            "Jugador" : "Player";
    };

    initPlayer(data_->player1);
    initPlayer(data_->player2);

    data_->player1.name += " 1";
    data_->player2.name += " 2";

    // Change the next state.
    core->SetNextState(MakeSharedState<BattleshipEnterNameState>());
}

//--------------------------------------------------------------------------------

/**
 * Returns the name of the current player.
 */
const std::string & BattleshipManager::GetPlayerName() {
    if (data_->substate == P2_NAME_STATE || data_->substate == P2_SHIPS_STATE ||
        data_->substate == P2_PLAY_STATE || data_->substate == P2_WAIT_STATE) {
        return data_->player2.name;
    } else {
        return data_->player1.name;
    }
}

//--------------------------------------------------------------------------------

/**
 * Adds the player's name to the current player.
 */
void BattleshipManager::AddPlayerName(const std::string & name) {
    // After add a name we'll have to update the player
    // name and set the next substate of the game.
    if (data_->substate == P1_NAME_STATE) {
        data_->player1.name = name;
        nextSubstate();

    } else if (data_->substate == P2_NAME_STATE) {
        data_->player2.name = name;
        nextSubstate();
    }
}

//--------------------------------------------------------------------------------

/**
 * Adds the player's fleet to the current player.
 */
void BattleshipManager::AddPlayerBoard(const BattleshipBoard & board) {
    // After finish to put the fleet on the board, we'll add to the current player
    // the board. We'll need to reset the count, because the ships count is used
    // to check the victory condition. Finally we'll set the next substate.
    if (data_->substate == P1_SHIPS_STATE) {
        data_->player1.board = board;
        data_->player1.board.ResetCount();
        nextSubstate();

    } else if (data_->substate == P2_SHIPS_STATE) {
        data_->player2.board = board;
        data_->player2.board.ResetCount();
        nextSubstate();
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets the current data.
 */
void BattleshipManager::GetCurrentData(CurrentData & data) {
    switch (data_->substate) {
    case P1_PLAY_STATE:
    case P2_WAIT_STATE:
        data.name = data_->player1.name;
        data.turn = data_->player1.turn;
        data.playerBoard = &(data_->player1.board);
        data.enemyBoard = &(data_->player2.board);
        break;

    case P2_PLAY_STATE:
    case P1_WAIT_STATE:
        data.name = data_->player2.name;
        data.turn = data_->player2.turn;
        data.playerBoard = &(data_->player2.board);
        data.enemyBoard = &(data_->player1.board);
        break;

    default:
        data.name = "";
        data.turn = 0;
        data.playerBoard = nullptr;
        data.enemyBoard = nullptr;
    }
}

//--------------------------------------------------------------------------------

/**
 * Is called after fire over an enemy board.
 */
void BattleshipManager::OnFireUpdate() {
    // After any of the players make a fire movement, we'll update the turn field.
    // Then we'll check the victory condition and change the next substate.
    switch (data_->substate) {
    case P1_PLAY_STATE:
        data_->player1.turn++;
        if (!data_->player2.board.AnyShipAvailable()) {
            data_->winner = P1_WINNER;
        }
        nextSubstate();
        break;

    case P2_PLAY_STATE:
        data_->player2.turn++;
        if (!data_->player1.board.AnyShipAvailable()) {
            data_->winner = P2_WINNER;
        }
        nextSubstate();
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Is called after the wait is over.
 */
void BattleshipManager::WaitDone() {
    if (data_->substate == P1_WAIT_STATE || data_->substate == P2_WAIT_STATE) {
        nextSubstate();
    }
}

//--------------------------------------------------------------------------------

/**
 * Executes the machine turn.
 */
void BattleshipManager::machineTurn() {
    // The machine fires over the player board and then checks the victory condition.
    data_->player1.board.MachineFires();
    if (!data_->player1.board.AnyShipAvailable()) {
        data_->winner = AI_WINNER;
        CoreManager::Instance()->SetNextState(MakeSharedState<BattleshipGameOverState>());
    }
}

//--------------------------------------------------------------------------------

/**
 * Changes the game to the next substate.
 */
void BattleshipManager::nextSubstate() {
    auto * core = CoreManager::Instance();

    switch (data_->substate) {
    case P1_NAME_STATE:
        // After add the player's name, we'll send her/him to the put ships state.
        data_->substate = P1_SHIPS_STATE;
        core->SetNextState(MakeSharedState<BattleshipPutShipsState>());
        break;

    case P1_SHIPS_STATE:
        // After add the player's fleet, we'll check the number of players.
        if (data_->numberOfPlayers > 1) {
            // If there are 2 players, we'll need to recieve the next player data.
            data_->substate = P2_NAME_STATE;
            core->SetNextState(MakeSharedState<BattleshipEnterNameState>());
        } else {
            // If we play against the machine, we'll have to initialize this
            // "virtual" player that will play as "player 2".
            data_->substate = P1_PLAY_STATE;
            data_->player2.board.Initialize();
            data_->player2.board.SetModeAIPutFleet();
            data_->player2.board.SetRandomFleet();
            data_->player2.board.ResetCount();
            core->SetNextState(MakeSharedState<BattleshipGameState>());
        }
        break;

    case P2_NAME_STATE:
        // After add the player's name, we'll send her/him to the put ships state.
        data_->substate = P2_SHIPS_STATE;
        core->SetNextState(MakeSharedState<BattleshipPutShipsState>());
        break;

    case P2_SHIPS_STATE:
        // After add the player's fleet, we'll send her/him to the game state.
        data_->substate = P1_PLAY_STATE;
        core->SetNextState(MakeSharedState<BattleshipGameState>());
        break;

    case P1_WAIT_STATE:
        // The wait ends and the player 1 can now play.
        data_->substate = P1_PLAY_STATE;
        core->SetNextState(MakeSharedState<BattleshipGameState>());
        break;

    case P1_PLAY_STATE:
        // First, we'll check that there is no winner.
        if (data_->winner == NO_WINNER) {
            // When there is no winner, we'll continue with the next turn.
            if (data_->numberOfPlayers > 1) {
                data_->substate = P2_WAIT_STATE;
                core->SetNextState(MakeSharedState<BattleshipWaitState>());
            } else {
                machineTurn();
            }
        } else {
            // If there is a winner, we'll call the game over state.
            core->SetNextState(MakeSharedState<BattleshipGameOverState>());
        }
        break;

    case P2_WAIT_STATE:
        // The wait ends and the player 2 can now play.
        data_->substate = P2_PLAY_STATE;
        core->SetNextState(MakeSharedState<BattleshipGameState>());
        break;

    case P2_PLAY_STATE:
        // First, we'll check that there is no winner.
        if (data_->winner == NO_WINNER) {
            // When there is no winner, we'll continue with the next turn.
            data_->substate = P1_WAIT_STATE;
            core->SetNextState(MakeSharedState<BattleshipWaitState>());
        } else {
            // If there is a winner, we'll call the game over state.
            core->SetNextState(MakeSharedState<BattleshipGameOverState>());
        }
        break;
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
BattleshipManager * BattleshipManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
BattleshipManager::BattleshipManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
BattleshipManager::~BattleshipManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
BattleshipManager * BattleshipManager::Instance() {
    if (!instance_) {
        instance_ = new BattleshipManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
BattleshipManager & BattleshipManager::Reference() {
    return *(Instance());
}
