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

#include "TetraminoesExitState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW1        144
#define ROW2        204
#define COL1        160
#define COL2        480

#define YES_OPTION  1
#define NO_OPTION   0

#define BLACK_COLOR       sf::Color::Black
#define WHITE_COLOR       sf::Color::White
#define RED_COLOR         sf::Color::Red
#define GREEN_COLOR       sf::Color::Green
#define DARK_RED_COLOR    sf::Color(127, 0, 0)
#define DARK_GREEN_COLOR  sf::Color(0, 127, 0)

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesExitState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->Menu2Background());

    selectedOption_ = -1;

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?", 0, ROW1,
        WHITE_COLOR, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("Yes", COL1, ROW2,
        WHITE_COLOR, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", COL2, ROW2,
        WHITE_COLOR, core->Retro70Font()));

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

void TetraminoesExitState::Release() {
    backgroundTexture_.reset(nullptr);
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesExitState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);

    bool pressed = Keyboard::isKeyPressed(Keyboard::Return) ||
                   Keyboard::isKeyPressed(Keyboard::Space);
    if (selectedOption_ == YES_OPTION) {
        yesLabel_->Color(pressed ? RED_COLOR : DARK_RED_COLOR);
    } else if (selectedOption_ == NO_OPTION) {
        noLabel_->Color(pressed ? GREEN_COLOR : DARK_GREEN_COLOR);
    }

    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesExitState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = TetraminoesManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->KeyboardSound().Play();
        if (selectedOption_ == YES_OPTION) {
            TetraminoesManager::Instance()->Release();

        } else if (selectedOption_ == NO_OPTION) {
            core->SetNextState(MakeSharedState<TetraminoesMenuState>());
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesMenuState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->KeyboardSound().Play();
        TetraminoesManager::Instance()->Release();

    } else if (Keyboard::IsKeyUp(Keyboard::Left)) {
        manager->KeyboardSound().Play();
        changeOption(YES_OPTION);

    } else if (Keyboard::IsKeyUp(Keyboard::Right)) {
        manager->KeyboardSound().Play();
        changeOption(NO_OPTION);
    }
}

//--------------------------------------------------------------------------------

void TetraminoesExitState::changeOption(unsigned int option) {
    selectedOption_ = option;
    switch (selectedOption_) {
    case YES_OPTION:
        yesLabel_->Color(DARK_RED_COLOR);
        noLabel_->Color(WHITE_COLOR);
        break;

    case NO_OPTION:
        yesLabel_->Color(WHITE_COLOR);
        noLabel_->Color(DARK_GREEN_COLOR);
        break;
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesExitState::TetraminoesExitState() : selectedOption_(0) {}

//--------------------------------------------------------------------------------

TetraminoesExitState::~TetraminoesExitState() {}
