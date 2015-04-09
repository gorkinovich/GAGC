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

#include "PongExitState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Pong/PongManager.h>
#include <Games/Pong/PongMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW1        150
#define ROW2        198
#define COL1        160
#define COL2        480

#define YES_OPTION  1
#define NO_OPTION   0

//********************************************************************************
// Methods
//********************************************************************************

void PongExitState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    selectedOption_ = -1;

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("Yes", COL1, ROW2,
        AtariPalette::Hue00Lum02, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", COL2, ROW2,
        AtariPalette::Hue00Lum02, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        questionLabel_->Text("¿Deseas realmente salir?");
        yesLabel_->Text("Sí");
        noLabel_->Text("No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, COL1);
    GUIUtil::CenterLabel(noLabel_, COL2);
}

//--------------------------------------------------------------------------------

void PongExitState::Release() {
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PongExitState::Draw(const sf::Time & timeDelta) {
    bool pressed = Keyboard::isKeyPressed(Keyboard::Return) ||
                   Keyboard::isKeyPressed(Keyboard::Space);
    if (selectedOption_ == YES_OPTION) {
        yesLabel_->Color(pressed ? AtariPalette::Hue04Lum02 : AtariPalette::Hue04Lum00);
    } else if (selectedOption_ == NO_OPTION) {
        noLabel_->Color(pressed ? AtariPalette::Hue11Lum02 : AtariPalette::Hue11Lum00);
    }

    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PongExitState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = PongManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->MenuKeySound().Play();
        if (selectedOption_ == YES_OPTION) {
            PongManager::Instance()->Release();

        } else if (selectedOption_ == NO_OPTION) {
            core->SetNextState(MakeSharedState<PongMenuState>());
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<PongMenuState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->MenuKeySound().Play();
        PongManager::Instance()->Release();

    } else if (Keyboard::IsKeyUp(Keyboard::Left)) {
        manager->MenuSelectSound().Play();
        changeOption(YES_OPTION);

    } else if (Keyboard::IsKeyUp(Keyboard::Right)) {
        manager->MenuSelectSound().Play();
        changeOption(NO_OPTION);
    }
}

//--------------------------------------------------------------------------------

void PongExitState::changeOption(unsigned int option) {
    selectedOption_ = option;
    switch (selectedOption_) {
    case YES_OPTION:
        yesLabel_->Color(AtariPalette::Hue04Lum00);
        noLabel_->Color(AtariPalette::Hue00Lum02);
        break;

    case NO_OPTION:
        yesLabel_->Color(AtariPalette::Hue00Lum02);
        noLabel_->Color(AtariPalette::Hue11Lum00);
        break;
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PongExitState::PongExitState() : selectedOption_(0), questionLabel_(nullptr),
    yesLabel_(nullptr), noLabel_(nullptr) {}

//--------------------------------------------------------------------------------

PongExitState::~PongExitState() {}
