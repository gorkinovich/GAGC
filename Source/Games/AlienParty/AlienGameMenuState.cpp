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

#include "AlienGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienGameState.h>
#include <Games/AlienParty/AlienMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW1  150
#define ROW2  198
#define COL1  160
#define COL2  480

//********************************************************************************
// Methods
//********************************************************************************

void AlienGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    questionLabel_.reset(new SimpleLabel("Stop the slaughtering?",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("(Y) Yes", COL1, ROW2,
        AtariPalette::Hue12Lum02, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("(N) No", COL2, ROW2,
        AtariPalette::Hue04Lum02, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        questionLabel_->Text("¿Parar la matanza?");
        yesLabel_->Text("(S) Sí");
        noLabel_->Text("(N) No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, COL1);
    GUIUtil::CenterLabel(noLabel_, COL2);
}

//--------------------------------------------------------------------------------

void AlienGameMenuState::Release() {
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void AlienGameMenuState::Draw(const sf::Time & timeDelta) {
    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void AlienGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = AlienManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<AlienGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<AlienMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

AlienGameMenuState::AlienGameMenuState() {}

//--------------------------------------------------------------------------------

AlienGameMenuState::~AlienGameMenuState() {}
