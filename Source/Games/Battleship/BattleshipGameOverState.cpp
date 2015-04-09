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

#include "BattleshipGameOverState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Battleship/BattleshipManager.h>
#include <Games/Battleship/BattleshipMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  150
#define ROW1  198
#define ROW2  336

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipGameOverState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = BattleshipManager::Instance();
    auto winner = manager->Winner();

    titleLabel_.reset(new SimpleLabel("Game Over", 0, ROW0,
        AtariPalette::Hue06Lum02, core->Retro70Font()));

    playerLabel_.reset(new SimpleLabel("", 0, ROW1,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press any key to continue...",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Partida Finalizada");
        exitLabel_->Text("Presione cualquier tecla para continuar...");
        switch (winner) {
        case BattleshipManager::P1_WINNER:
            playerLabel_->Text("¡" + manager->Player1Name() + " gana!");
            break;
        case BattleshipManager::P2_WINNER:
            playerLabel_->Text("¡" + manager->Player2Name() + " gana!");
            break;
        case BattleshipManager::AI_WINNER:
            playerLabel_->Text("¡La máquina gana!");
            break;
        case BattleshipManager::NO_WINNER:
            playerLabel_->Text("¡Empate!");
            break;
        }
    } else {
        switch (winner) {
        case BattleshipManager::P1_WINNER:
            playerLabel_->Text(manager->Player1Name() + " wins!");
            break;
        case BattleshipManager::P2_WINNER:
            playerLabel_->Text(manager->Player2Name() + " wins!");
            break;
        case BattleshipManager::AI_WINNER:
            playerLabel_->Text("Machine wins!");
            break;
        case BattleshipManager::NO_WINNER:
            playerLabel_->Text("Draw!");
            break;
        }
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(playerLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BattleshipGameOverState::Release() {
    titleLabel_.reset(nullptr);
    playerLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipGameOverState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    playerLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipGameOverState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Mouse::IsAnyButtonUp()) {
        BattleshipManager::Instance()->ClickSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BattleshipMenuState>());
    } else if (Keyboard::IsAnyKeyUp()) {
        BattleshipManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BattleshipMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipGameOverState::BattleshipGameOverState() {}

//--------------------------------------------------------------------------------

BattleshipGameOverState::~BattleshipGameOverState() {}
