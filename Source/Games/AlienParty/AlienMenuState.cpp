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

#include "AlienMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienHelpState.h>
#include <Games/AlienParty/AlienCreditsState.h>
#include <Games/AlienParty/AlienExitState.h>

//********************************************************************************
// Methods
//********************************************************************************

void AlienMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = AlienManager::Instance();
    manager->Initialize();

    titleLabel_.reset(new SimpleLabel("Alien Party", 0, 102,
        AtariPalette::Hue12Lum02, core->Retro70Font()));

    newGameLabel_.reset(new SimpleLabel("Start (F1)", 0, 138,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help (F2)", 0, 162,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits (F3)", 0, 186,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit (ESC)", 0, 210,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        newGameLabel_->Text("Empezar (F1)");
        helpLabel_->Text("Ayuda (F2)");
        creditsLabel_->Text("Créditos (F3)");
        exitLabel_->Text("Salir (ESC)");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(newGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(creditsLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void AlienMenuState::Release() {
    titleLabel_.reset(nullptr);
    newGameLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void AlienMenuState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    newGameLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void AlienMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = AlienManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::F1)) {
        manager->KeyboardSound().Play();
        manager->StartGame();

    } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<AlienHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<AlienCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<AlienExitState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

AlienMenuState::AlienMenuState() {}

//--------------------------------------------------------------------------------

AlienMenuState::~AlienMenuState() {}
