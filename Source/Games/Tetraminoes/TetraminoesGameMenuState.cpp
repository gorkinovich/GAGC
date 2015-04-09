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

#include "TetraminoesGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesGameState.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW_OFFSET  2

#define COL0  244
#define COL1  380

#define ROW0  120
#define ROW1  156
#define ROW2  180

#define CONTINUE_OPTION  0
#define EXIT_OPTION      1
#define MAX_OPTIONS      2

#define BLACK_COLOR   sf::Color::Black
#define WHITE_COLOR   sf::Color::White
#define YELLOW_COLOR  sf::Color::Yellow

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();
    auto & tileset = manager->Tileset();

    selectedOption_ = -1;

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->Menu1Background());

    leftSelectorPosition_ = sf::Vector2i(COL0, 0);
    leftSelectorTexture_.reset(new Texture2D());
    leftSelectorTexture_->Load(tileset, sf::IntRect(64, 32, 16, 16));

    rightSelectorPosition_ = sf::Vector2i(COL1, 0);
    rightSelectorTexture_.reset(new Texture2D());
    rightSelectorTexture_->Load(tileset, sf::IntRect(80, 32, 16, 16));

    titleLabel_.reset(new SimpleLabel("Game Menu",
        0, ROW0, WHITE_COLOR, core->Retro70Font()));

    continueLabel_.reset(new SimpleLabel("Continue",
        0, ROW1, WHITE_COLOR, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit Game",
        0, ROW2, WHITE_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Menú Partida");
        continueLabel_->Text("Continuar");
        exitLabel_->Text("Salir Partida");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(continueLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void TetraminoesGameMenuState::Release() {
    backgroundTexture_.reset(nullptr);
    leftSelectorTexture_.reset(nullptr);
    rightSelectorTexture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    continueLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesGameMenuState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);

    auto color = Keyboard::isKeyPressed(Keyboard::Return) ||
        Keyboard::isKeyPressed(Keyboard::Space) ?
        YELLOW_COLOR : WHITE_COLOR;

    if (selectedOption_ < MAX_OPTIONS) {
        leftSelectorTexture_->Draw(leftSelectorPosition_, color);
        rightSelectorTexture_->Draw(rightSelectorPosition_, color);
    }

    switch (selectedOption_) {
    case CONTINUE_OPTION: continueLabel_->Color(color); break;
    case EXIT_OPTION:     exitLabel_->Color(color);     break;
    }

    titleLabel_->Draw();
    continueLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = TetraminoesManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->KeyboardSound().Play();
        switch (selectedOption_) {
        case CONTINUE_OPTION: core->SetNextState(MakeSharedState<TetraminoesGameState>()); break;
        case EXIT_OPTION:     core->SetNextState(MakeSharedState<TetraminoesMenuState>()); break;
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesGameState>());

    } else {
        unsigned int option = selectedOption_;
        if (Keyboard::IsKeyUp(Keyboard::Down)) {
            manager->KeyboardSound().Play();
            option = (option + 1) % MAX_OPTIONS;
        }
        if (Keyboard::IsKeyUp(Keyboard::Up)) {
            manager->KeyboardSound().Play();
            option = (option - 1);
            if (option >= MAX_OPTIONS) {
                option = MAX_OPTIONS - 1;
            }
        }
        changeOption(option);
    }
}

//--------------------------------------------------------------------------------

void TetraminoesGameMenuState::changeOption(unsigned int option) {
    selectedOption_ = option;
    continueLabel_->Color(WHITE_COLOR);
    exitLabel_->Color(WHITE_COLOR);

    int y = -32;
    switch (selectedOption_) {
    case CONTINUE_OPTION: y = ROW1 - ROW_OFFSET; break;
    case EXIT_OPTION:     y = ROW2 - ROW_OFFSET; break;
    }
    leftSelectorPosition_.y = y;
    rightSelectorPosition_.y = y;
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesGameMenuState::TetraminoesGameMenuState() : selectedOption_(0) {}

//--------------------------------------------------------------------------------

TetraminoesGameMenuState::~TetraminoesGameMenuState() {}
