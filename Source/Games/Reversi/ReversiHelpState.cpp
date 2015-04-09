/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#include "ReversiHelpState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Reversi/ReversiManager.h>
#include <Games/Reversi/ReversiMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  24
#define ROW1  60
#define ROW2  336

//********************************************************************************
// Methods
//********************************************************************************

void ReversiHelpState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    titleLabel_.reset(new SimpleLabel("Help", 0, ROW0,
        AtariPalette::Hue10Lum04, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "Press F1 to visit the wikipedia to learn more about\n"
        "the rules of this game. This version follows the\n"
        "classical reversi set of rules.",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Ayuda");
        helpText_->Text(
            "Pulsa F1 para visitar la wikipedia para aprender m�s\n"
            "sobre las reglas de este juego. Esta versi�n sigue\n"
            "las reglas de la variante cl�sica del reversi."
        );
        exitLabel_->Text("Presione cualquier tecla para volver al men�...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void ReversiHelpState::Release() {
    titleLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ReversiHelpState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    helpText_->Draw();
    exitLabel_->Draw();

    ReversiManager::Instance()->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ReversiHelpState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Keyboard::IsKeyUp(Keyboard::F1)) {
        auto * core = CoreManager::Instance();
        auto language = core->Language();
        if (language == TEXT_LANGUAGE_ENGLISH) {
            core->OpenURL("http://en.wikipedia.org/wiki/Reversi");
        } else {
            core->OpenURL("http://es.wikipedia.org/wiki/Reversi");
        }
    } else if (Mouse::IsAnyButtonUp()) {
        ReversiManager::Instance()->ClickSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<ReversiMenuState>());
    } else if (Keyboard::IsAnyKeyUp()) {
        ReversiManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<ReversiMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ReversiHelpState::ReversiHelpState() {}

//--------------------------------------------------------------------------------

ReversiHelpState::~ReversiHelpState() {}
