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

#include "PongGameOverState.h"
#include <sstream>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Pong/PongManager.h>
#include <Games/Pong/PongMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  150
#define ROW1  198
#define ROW2  336

//********************************************************************************
// Methods
//********************************************************************************

void PongGameOverState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = PongManager::Instance();
    auto score1 = manager->ScorePlayer1();
    auto score2 = manager->ScorePlayer2();

    titleLabel_.reset(new SimpleLabel("Game Over", 0, ROW0,
        AtariPalette::Hue06Lum02, core->Retro70Font()));

    playerLabel_.reset(new SimpleLabel("", 0, ROW1,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press any key to continue...",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Partida Finalizada");
        if (score1 > score2) {
            playerLabel_->Text("¡Jugador 1 gana!");
        } else if (score1 < score2) {
            playerLabel_->Text("¡Jugador 2 gana!");
        } else {
            playerLabel_->Text("¡Empate!");
        }
        exitLabel_->Text("Presione cualquier tecla para continuar...");
    } else {
        if (score1 > score2) {
            playerLabel_->Text("Player 1 wins!");
        } else if (score1 < score2) {
            playerLabel_->Text("Player 2 wins!");
        } else {
            playerLabel_->Text("Draw!");
        }
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(playerLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void PongGameOverState::Release() {
    titleLabel_.reset(nullptr);
    playerLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PongGameOverState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    playerLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PongGameOverState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        PongManager::Instance()->MenuNextSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<PongMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PongGameOverState::PongGameOverState() : titleLabel_(nullptr),
    playerLabel_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

PongGameOverState::~PongGameOverState() {}
