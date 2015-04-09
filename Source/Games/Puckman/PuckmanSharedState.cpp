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

#include "PuckmanSharedState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanCreditsState.h>
#include <Games/Puckman/PuckmanExitState.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

void PuckmanSharedState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(Puckman::Palette::Black);

    std::string barText = language == TEXT_LANGUAGE_ENGLISH ?
        "(ESC) Exit     (F1) About the game    (F3) Reset     " :
        "(ESC) Salir    (F1) Sobre el juego    (F3) Reiniciar ";

    upBarLabel_.reset(new SimpleLabel(barText, 8, 0,
        Puckman::Palette::DarkGreen, core->Retro70Font()));

    GUIUtil::CenterLabel(upBarLabel_, CoreManager::HALF_LOW_WIDTH);

    updateOptionSelected_ = false;
}

//--------------------------------------------------------------------------------

void PuckmanSharedState::Release() {
    upBarLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PuckmanSharedState::Draw(const sf::Time & timeDelta) {
    upBarLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PuckmanSharedState::Update(const sf::Time & timeDelta) {
    updateOptionSelected_ = false;
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        updateOptionSelected_ = true;
        Manager::Instance()->FruitEat().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<PuckmanExitState>());
    } else if (Keyboard::IsKeyUp(Keyboard::F1)) {
        updateOptionSelected_ = true;
        Manager::Instance()->FruitEat().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<PuckmanCreditsState>());
    } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
        updateOptionSelected_ = true;
        Manager::Instance()->FruitEat().Play();
        Manager::Instance()->ResetGame();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PuckmanSharedState::PuckmanSharedState() {}

//--------------------------------------------------------------------------------

PuckmanSharedState::~PuckmanSharedState() {}
