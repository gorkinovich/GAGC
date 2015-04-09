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

#include "SnakeGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeGameState.h>
#include <Games/Snake/SnakeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0             122
#define ROW1             158
#define ROW2             182

#define REC_WIDTH        160
#define REC_HEIGHT       20
#define REC_ROW1         154
#define REC_ROW2         178
#define REC_COL          240

#define CONTINUE_OPTION  0
#define EXIT_OPTION      1

#define MAX_OPTIONS      2

//********************************************************************************
// Methods
//********************************************************************************

void SnakeGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    selectedOption_ = -1;

    rectangleTexture_.reset(new Texture2D());
    rectangleTexture_->Load(REC_WIDTH, REC_HEIGHT);

    titleLabel_.reset(new SimpleLabel("Game Menu", 0, ROW0,
        AtariPalette::Hue11Lum02, core->Retro70Font()));

    continueLabel_.reset(new SimpleLabel("Continue (ESC)", 0, ROW1,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit Game", 0, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

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

void SnakeGameMenuState::Release() {
    rectangleTexture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    continueLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeGameMenuState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the selector of the menu.
    sf::Color rectangleColor =
        Keyboard::isKeyPressed(Keyboard::Return) ||
        Keyboard::isKeyPressed(Keyboard::Space) ?
        AtariPalette::Hue00Lum08 : AtariPalette::Hue00Lum04;
    switch (selectedOption_) {
    case CONTINUE_OPTION:
        rectangleTexture_->Draw(REC_COL, REC_ROW1, rectangleColor);
        break;
    case EXIT_OPTION:
        rectangleTexture_->Draw(REC_COL, REC_ROW2, rectangleColor);
        break;
    }
    // Draw the labels of the menu.
    titleLabel_->Draw();
    continueLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = SnakeManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->MenuKeySound().Play();
        switch (selectedOption_) {
        case CONTINUE_OPTION:
            core->SetNextState(MakeSharedState<SnakeGameState>());
            break;
        case EXIT_OPTION:
            core->SetNextState(MakeSharedState<SnakeMenuState>());
            break;
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeGameState>());

    } else {
        if (Keyboard::IsKeyUp(Keyboard::Down)) {
            manager->MenuDownSound().Play();
            selectedOption_ = (selectedOption_ + 1) % MAX_OPTIONS;
        }
        if (Keyboard::IsKeyUp(Keyboard::Up)) {
            manager->MenuUpSound().Play();
            selectedOption_ = (selectedOption_ - 1);
            if (selectedOption_ >= MAX_OPTIONS)
                selectedOption_ = MAX_OPTIONS - 1;
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeGameMenuState::SnakeGameMenuState() : selectedOption_(0), rectangleTexture_(nullptr),
    titleLabel_(nullptr), continueLabel_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeGameMenuState::~SnakeGameMenuState() {}
