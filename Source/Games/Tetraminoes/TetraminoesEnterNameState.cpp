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

#include "TetraminoesEnterNameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Games/SaveManager.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  138
#define ROW1  174
#define ROW2  210

#define BLACK_COLOR   sf::Color::Black
#define WHITE_COLOR   sf::Color::White
#define YELLOW_COLOR  sf::Color::Yellow

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesEnterNameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->Menu2Background());

    line1Label_.reset(new SimpleLabel("Congratulations, you get a top score!",
        0, ROW0, YELLOW_COLOR, core->Retro70Font()
    ));

    line2Label_.reset(new SimpleLabel("Enter your name (max. 16 characters):",
        0, ROW1, WHITE_COLOR, core->Retro70Font()));

    nameLabel_.reset(new SimpleLabel("_", 0, ROW2,
        WHITE_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        line1Label_->Text("¡Felicidades, has logrado una puntuación máxima!");
        line2Label_->Text("Introduce tu nombre (max. 16 caracteres):");
    }

    GUIUtil::CenterLabel(line1Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(line2Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(nameLabel_, CoreManager::HALF_LOW_WIDTH);

    core->ClearKeyboardTextInput();
    core->EnableKeyboardTextInput();
}

//--------------------------------------------------------------------------------

void TetraminoesEnterNameState::Release() {
    SaveManager::Instance()->TetraminoesSave();
    auto * core = CoreManager::Instance();
    core->DisableKeyboardTextInput();
    backgroundTexture_.reset(nullptr);
    line1Label_.reset(nullptr);
    line2Label_.reset(nullptr);
    nameLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesEnterNameState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);
    line1Label_->Draw();
    line2Label_->Draw();
    nameLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesEnterNameState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return)) {
        auto * manager = TetraminoesManager::Instance();
        manager->KeyboardSound().Play();
        std::string name = nameLabel_->Text().substr(0, nameLabel_->Text().length() - 1);
        manager->AddScore(name, manager->Score());
        core->SetNextState(MakeSharedState<TetraminoesMenuState>());
    } else {
        const int MAX_CHARS = 16;
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

TetraminoesEnterNameState::TetraminoesEnterNameState() {}

//--------------------------------------------------------------------------------

TetraminoesEnterNameState::~TetraminoesEnterNameState() {}
