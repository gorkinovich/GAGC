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

#include "SnakeEnterNameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Games/SaveManager.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  146
#define ROW1  170
#define ROW2  194

//********************************************************************************
// Methods
//********************************************************************************

void SnakeEnterNameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = SnakeManager::Instance();

    if (manager->IsTopScore(manager->Players()[0].Score())) {
        substate_ = 0;
    } else {
        substate_ = 1;
    }

    line1P1Label_.reset(new SimpleLabel(
        manager->NumberOfPlayers() < 2 ?
        "Congratulations, you get a top score!" :
        "Congratulations player 1, you get a top score!",
        0, ROW0, AtariPalette::Hue00Lum14, core->Retro70Font()
    ));

    line1P2Label_.reset(new SimpleLabel("Congratulations player 2, you get a top score!",
        0, ROW0, AtariPalette::Hue00Lum14, core->Retro70Font()));

    line2Label_.reset(new SimpleLabel("Enter your name (max. 8 characters):",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()));

    nameLabel_.reset(new SimpleLabel("_", 0, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        line1P1Label_->Text(manager->NumberOfPlayers() < 2 ?
            "¡Felicidades, has logrado una puntuación máxima!" :
            "¡Felicidades jugador 1, has logrado una puntuación máxima!"
        );
        line1P2Label_->Text("¡Felicidades jugador 2, has logrado una puntuación máxima!");
        line2Label_->Text("Introduce tu nombre (max. 8 caracteres):");
    }

    GUIUtil::CenterLabel(line1P1Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(line1P2Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(line2Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(nameLabel_, CoreManager::HALF_LOW_WIDTH);

    core->ClearKeyboardTextInput();
    core->EnableKeyboardTextInput();
}

//--------------------------------------------------------------------------------

void SnakeEnterNameState::Release() {
    SaveManager::Instance()->SnakeSave();
    auto * core = CoreManager::Instance();
    core->DisableKeyboardTextInput();
    line1P1Label_.reset(nullptr);
    line1P2Label_.reset(nullptr);
    line2Label_.reset(nullptr);
    nameLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeEnterNameState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the labels of the menu.
    if (substate_ == 0) {
        line1P1Label_->Draw();
    } else {
        line1P2Label_->Draw();
    }
    line2Label_->Draw();
    nameLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeEnterNameState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return)) {
        auto * manager = SnakeManager::Instance();
        manager->MenuNextSound().Play();
        std::string name = nameLabel_->Text().substr(0, nameLabel_->Text().length() - 1);
        if (substate_ == 0) {
            manager->AddScore(name, manager->Players()[0].Score());
            if (manager->NumberOfPlayers() > 1) {
                core->ClearKeyboardTextInput();
                nameLabel_->Text("_");
                substate_ = 1;
            } else {
                core->SetNextState(MakeSharedState<SnakeMenuState>());
            }
        } else {
            manager->AddScore(name, manager->Players()[1].Score());
            core->SetNextState(MakeSharedState<SnakeMenuState>());
        }
    } else {
        const int MAX_CHARS = 8;
        std::string name = "";
        std::string text = core->KeyboardText().substr(0, MAX_CHARS);
        ForEach(text, [&] (char item) {
            if (item != '\n') {
                name += item;
            }
        });
        core->KeyboardText(name);
        nameLabel_->Text(name + "_");
        GUIUtil::CenterLabel(nameLabel_, CoreManager::HALF_LOW_WIDTH);
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeEnterNameState::SnakeEnterNameState() : substate_(0), line1P1Label_(nullptr),
    line1P2Label_(nullptr), line2Label_(nullptr), nameLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeEnterNameState::~SnakeEnterNameState() {}
