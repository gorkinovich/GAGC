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

#include "PuckmanExitState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanMainState.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

void PuckmanExitState::Initialize() {
    PuckmanSharedState::Initialize();
    auto * core = CoreManager::Instance();
    auto language = core->Language();

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
        0, 132, Puckman::Palette::White, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("(Y) Yes", 0, 222,
        Puckman::Palette::Red, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("(N) No", 0, 222,
        Puckman::Palette::Green, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        questionLabel_->Text("¿Deseas realmente salir?");
        yesLabel_->Text("(S) Sí");
        noLabel_->Text("(N) No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, 200);
    GUIUtil::CenterLabel(noLabel_, 440);

    upBarLabel_->Text(language == TEXT_LANGUAGE_ENGLISH ?
        "(ESC) Return   (F1) About the game    (F3) Reset     " :
        "(ESC) Volver   (F1) Sobre el juego    (F3) Reiniciar ");
}

//--------------------------------------------------------------------------------

void PuckmanExitState::Release() {
    PuckmanSharedState::Release();
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PuckmanExitState::Draw(const sf::Time & timeDelta) {
    PuckmanSharedState::Draw(timeDelta);
    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PuckmanExitState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        Manager::Instance()->GhostEat().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<PuckmanMainState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        Manager::Instance()->Death().Play();
        Manager::Instance()->Release();

    } else {
        PuckmanSharedState::Update(timeDelta);
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PuckmanExitState::PuckmanExitState() {}

//--------------------------------------------------------------------------------

PuckmanExitState::~PuckmanExitState() {}
