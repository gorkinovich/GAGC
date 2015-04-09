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

#include "SnakeExitState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW1        146
#define ROW2        182
#define COL1        160
#define COL2        480

#define REC_WIDTH   60
#define REC_HEIGHT  20
#define REC_ROW     178
#define REC_COL1    130
#define REC_COL2    450

#define YES_OPTION  1
#define NO_OPTION   0

//********************************************************************************
// Methods
//********************************************************************************

void SnakeExitState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    selectedOption_ = -1;

    rectangleTexture_.reset(new Texture2D());
    rectangleTexture_->Load(REC_WIDTH, REC_HEIGHT);

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("Yes", COL1, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", COL2, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

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

void SnakeExitState::Release() {
    rectangleTexture_.reset(nullptr);
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeExitState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the selector of the menu.
    bool pressed = Keyboard::isKeyPressed(Keyboard::Return) ||
                   Keyboard::isKeyPressed(Keyboard::Space);
    if (selectedOption_ == YES_OPTION) {
        rectangleTexture_->Draw(
            REC_COL1, REC_ROW,
            pressed ? AtariPalette::Hue04Lum02 : AtariPalette::Hue04Lum00
        );
    } else if (selectedOption_ == NO_OPTION) {
        rectangleTexture_->Draw(
            REC_COL2, REC_ROW,
            pressed ? AtariPalette::Hue11Lum02 : AtariPalette::Hue11Lum00
        );
    }
    // Draw the labels of the menu.
    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeExitState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = SnakeManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->MenuKeySound().Play();
        if (selectedOption_ == YES_OPTION) {
            SnakeManager::Instance()->Release();

        } else if (selectedOption_ == NO_OPTION) {
            core->SetNextState(MakeSharedState<SnakeMenuState>());
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeMenuState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->MenuKeySound().Play();
        SnakeManager::Instance()->Release();

    } else if (Keyboard::IsKeyUp(Keyboard::Left)) {
        manager->MenuUpSound().Play();
        selectedOption_ = YES_OPTION;

    } else if (Keyboard::IsKeyUp(Keyboard::Right)) {
        manager->MenuDownSound().Play();
        selectedOption_ = NO_OPTION;
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeExitState::SnakeExitState() : selectedOption_(0), rectangleTexture_(nullptr),
 questionLabel_(nullptr), yesLabel_(nullptr), noLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeExitState::~SnakeExitState() {}
