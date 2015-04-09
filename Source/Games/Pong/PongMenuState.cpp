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

#include "PongMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Pong/PongManager.h>
#include <Games/Pong/PongNewGameState.h>
#include <Games/Pong/PongHelpState.h>
#include <Games/Pong/PongCreditsState.h>
#include <Games/Pong/PongExitState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0            102
#define ROW1            138
#define ROW2            162
#define ROW3            186
#define ROW4            210

#define NEWGAME_OPTION  0
#define HELP_OPTION     1
#define CREDITS_OPTION  2
#define EXIT_OPTION     3

#define MAX_OPTIONS     4

#define NORMAL_COLOR    AtariPalette::Hue00Lum02
#define SELECT_COLOR    AtariPalette::Hue00Lum12

//********************************************************************************
// Methods
//********************************************************************************

void PongMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = PongManager::Instance();
    manager->Initialize();

    selectedOption_ = -1;

    titleLabel_.reset(new SimpleLabel("Pong", 0, ROW0,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    newGameLabel_.reset(new SimpleLabel("New Game (F1)", 0, ROW1,
        NORMAL_COLOR, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help (F2)", 0, ROW2,
        NORMAL_COLOR, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits (F3)", 0, ROW3,
        NORMAL_COLOR, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit (ESC)", 0, ROW4,
        NORMAL_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        newGameLabel_->Text("Nueva Partida (F1)");
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

void PongMenuState::Release() {
    titleLabel_.reset(nullptr);
    newGameLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PongMenuState::Draw(const sf::Time & timeDelta) {
    switch (selectedOption_) {
    case NEWGAME_OPTION:
        newGameLabel_->Color(
            Keyboard::isKeyPressed(Keyboard::Return) ||
            Keyboard::isKeyPressed(Keyboard::Space) ?
            AtariPalette::Hue04Lum02 : SELECT_COLOR
        );
        break;
    case HELP_OPTION:
        helpLabel_->Color(
            Keyboard::isKeyPressed(Keyboard::Return) ||
            Keyboard::isKeyPressed(Keyboard::Space) ?
            AtariPalette::Hue12Lum02 : SELECT_COLOR
        );
        break;
    case CREDITS_OPTION:
        creditsLabel_->Color(
            Keyboard::isKeyPressed(Keyboard::Return) ||
            Keyboard::isKeyPressed(Keyboard::Space) ?
            AtariPalette::Hue09Lum02 : SELECT_COLOR
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
    newGameLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PongMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = PongManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->MenuKeySound().Play();
        switch (selectedOption_) {
        case NEWGAME_OPTION: core->SetNextState(MakeSharedState<PongNewGameState>()); break;
        case HELP_OPTION:    core->SetNextState(MakeSharedState<PongHelpState>());    break;
        case CREDITS_OPTION: core->SetNextState(MakeSharedState<PongCreditsState>()); break;
        case EXIT_OPTION:    core->SetNextState(MakeSharedState<PongExitState>());    break;
        }

    } else if (Keyboard::IsKeyUp(Keyboard::F1)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<PongNewGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<PongHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<PongCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<PongExitState>());

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

void PongMenuState::changeOption(unsigned int option) {
    selectedOption_ = option;
    switch (selectedOption_) {
    case NEWGAME_OPTION:
        newGameLabel_->Color(SELECT_COLOR);
        helpLabel_->Color(NORMAL_COLOR);
        creditsLabel_->Color(NORMAL_COLOR);
        exitLabel_->Color(NORMAL_COLOR);
        break;

    case HELP_OPTION:
        newGameLabel_->Color(NORMAL_COLOR);
        helpLabel_->Color(SELECT_COLOR);
        creditsLabel_->Color(NORMAL_COLOR);
        exitLabel_->Color(NORMAL_COLOR);
        break;

    case CREDITS_OPTION:
        newGameLabel_->Color(NORMAL_COLOR);
        helpLabel_->Color(NORMAL_COLOR);
        creditsLabel_->Color(SELECT_COLOR);
        exitLabel_->Color(NORMAL_COLOR);
        break;

    case EXIT_OPTION:
        newGameLabel_->Color(NORMAL_COLOR);
        helpLabel_->Color(NORMAL_COLOR);
        creditsLabel_->Color(NORMAL_COLOR);
        exitLabel_->Color(SELECT_COLOR);
        break;
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PongMenuState::PongMenuState() : selectedOption_(0), titleLabel_(nullptr),
    newGameLabel_(nullptr), helpLabel_(nullptr), creditsLabel_(nullptr),
    exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

PongMenuState::~PongMenuState() {}
