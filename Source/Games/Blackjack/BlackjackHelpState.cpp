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

#include "BlackjackHelpState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Blackjack/BlackjackManager.h>
#include <Games/Blackjack/BlackjackMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  24
#define ROW1  60
#define ROW2  336

//********************************************************************************
// Methods
//********************************************************************************

void BlackjackHelpState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue12Lum00);

    titleLabel_.reset(new SimpleLabel("Help", 0, ROW0,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "Press F1 to visit the wikipedia to learn more\n"
        "about the rules of this game.",
        0, ROW1, AtariPalette::Hue00Lum00, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue00Lum06, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Ayuda");
        helpText_->Text(
            "Pulsa F1 para visitar la wikipedia para aprender más\n"
            "sobre las reglas de este juego."
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BlackjackHelpState::Release() {
    titleLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BlackjackHelpState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    helpText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BlackjackHelpState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Keyboard::IsKeyUp(Keyboard::F1)) {
        auto * core = CoreManager::Instance();
        auto language = core->Language();
        if (language == TEXT_LANGUAGE_ENGLISH) {
            core->OpenURL("http://en.wikipedia.org/wiki/Blackjack");
        } else {
            core->OpenURL("http://es.wikipedia.org/wiki/Blackjack");
        }
    } else if (Mouse::IsAnyButtonUp()) {
        BlackjackManager::Instance()->ClickSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackMenuState>());
    } else if (Keyboard::IsAnyKeyUp()) {
        BlackjackManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BlackjackHelpState::BlackjackHelpState() {}

//--------------------------------------------------------------------------------

BlackjackHelpState::~BlackjackHelpState() {}
