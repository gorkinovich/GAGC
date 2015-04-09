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

#include "PuckmanSelectPlayerState.h"
#include <SFML/System/Time.hpp>
#include <System/Sound.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Puckman/PuckmanGameState.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the state.
 */
void SelectPlayerState::Initialize() {
}

//--------------------------------------------------------------------------------

/**
 * Releases the state.
 */
void SelectPlayerState::Release() {
}

//--------------------------------------------------------------------------------

/**
 * Draws the content of the state.
 */
void SelectPlayerState::Draw(const sf::Time & timeDelta) {
    drawMenuHud();

    manager_->DrawText(16, 6, "PUSH START BUTTON", Palette::Brown);

    if(data_->Coins() > 1) {
        manager_->DrawText(20, 8, "1 OR 2 PLAYERS", Palette::Cyan);
    } else {
        manager_->DrawText(20, 8, "1 PLAYER ONLY", Palette::Cyan);
    }

    manager_->DrawText(24, 1, "BONUS PUCKMAN FOR 10000", Palette::DarkPink);
    manager_->DrawSymbol(24, 25, FontSymbol::Pts, Palette::DarkPink);

    manager_->DrawSymbol(28, 8, FontSymbol::Copyright, Palette::Pink);
    manager_->DrawSymbol(28, 10, FontSymbol::Namco, Palette::Pink);
    manager_->DrawText(28, 18, "1980", Palette::Pink);
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner logic of the state.
 */
void SelectPlayerState::Update(const sf::Time & timeDelta) {
    // Here we check if the user wants to insert a coin.
    if(Keyboard::IsKeyUp(Keyboard::C)) {
        data_->AddCoin();
        manager_->FruitEat().Play();
        return;
    }

    // Here we check if the user wants to start a one player game.
    if(Keyboard::IsKeyUp(Keyboard::Num1) && data_->Coins() > 0) {
        manager_->GameStateInstance()->StartOnePlayerGame();
        manager_->SetNextState(Manager::Instance()->GameStateInstance());
        return;
    }

    // Here we check if the user wants to start a two players game.
    if(Keyboard::IsKeyUp(Keyboard::Num2) && data_->Coins() > 1) {
        manager_->GameStateInstance()->StartTwoPlayersGame();
        manager_->SetNextState(Manager::Instance()->GameStateInstance());
        return;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
SelectPlayerState::SelectPlayerState() : GenericState() {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
SelectPlayerState::~SelectPlayerState() {}
