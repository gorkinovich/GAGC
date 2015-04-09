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

#include "PongGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Pong/PongManager.h>
#include <Games/Pong/PongGameState.h>
#include <Games/Pong/PongMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0             126
#define ROW1             162
#define ROW2             186

#define CONTINUE_OPTION  0
#define EXIT_OPTION      1

#define MAX_OPTIONS      2

#define NORMAL_COLOR     AtariPalette::Hue00Lum02
#define SELECT_COLOR     AtariPalette::Hue00Lum12

//********************************************************************************
// Methods
//********************************************************************************

void PongGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    selectedOption_ = -1;

    titleLabel_.reset(new SimpleLabel("Game Menu", 0, ROW0,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    continueLabel_.reset(new SimpleLabel("Continue (ESC)", 0, ROW1,
        NORMAL_COLOR, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit Game", 0, ROW2,
        NORMAL_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Menú Partida");
        continueLabel_->Text("Continuar (ESC)");
        exitLabel_->Text("Salir Partida");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(continueLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void PongGameMenuState::Release() {
    titleLabel_.reset(nullptr);
    continueLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PongGameMenuState::Draw(const sf::Time & timeDelta) {
    switch (selectedOption_) {
    case CONTINUE_OPTION:
        continueLabel_->Color(
            Keyboard::isKeyPressed(Keyboard::Return) ||
            Keyboard::isKeyPressed(Keyboard::Space) ?
            AtariPalette::Hue01Lum14 : SELECT_COLOR
        );
        break;
    case EXIT_OPTION:
        exitLabel_->Color(
            Keyboard::isKeyPressed(Keyboard::Return) ||
            Keyboard::isKeyPressed(Keyboard::Space) ?
            AtariPalette::Hue01Lum14 : SELECT_COLOR
        );
        break;
    }

    titleLabel_->Draw();
    continueLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PongGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = PongManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->MenuKeySound().Play();
        switch (selectedOption_) {
        case CONTINUE_OPTION:
            core->SetNextState(MakeSharedState<PongGameState>());
            break;
        case EXIT_OPTION:
            core->SetNextState(MakeSharedState<PongMenuState>());
            break;
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<PongGameState>());

    } else {
        unsigned int option = selectedOption_;
        if (Keyboard::IsKeyUp(Keyboard::Down)) {
            manager->MenuSelectSound().Play();
            option = (option + 1) % MAX_OPTIONS;
        }
        if (Keyboard::IsKeyUp(Keyboard::Up)) {
            manager->MenuSelectSound().Play();
            option = (option - 1);
            if (option >= MAX_OPTIONS)
                option = MAX_OPTIONS - 1;
        }
        changeOption(option);
    }
}

//--------------------------------------------------------------------------------

void PongGameMenuState::changeOption(unsigned int option) {
    selectedOption_ = option;
    switch (selectedOption_) {
    case CONTINUE_OPTION:
        continueLabel_->Color(SELECT_COLOR);
        exitLabel_->Color(NORMAL_COLOR);
        break;

    case EXIT_OPTION:
        continueLabel_->Color(NORMAL_COLOR);
        exitLabel_->Color(SELECT_COLOR);
        break;
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PongGameMenuState::PongGameMenuState() : selectedOption_(0), titleLabel_(nullptr),
    continueLabel_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

PongGameMenuState::~PongGameMenuState() {}
