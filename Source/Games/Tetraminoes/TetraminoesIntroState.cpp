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

#include "TetraminoesIntroState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/Keyboard.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesIntroState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(sf::Color::Black);

    auto * manager = TetraminoesManager::Instance();
    manager->Initialize();

    auto & tileset = manager->Tileset();

    background1Texture_.reset(new Texture2D());
    background1Texture_->Load(manager->Menu1Background());

    background2Position_ = sf::Vector2i(215, 39);
    background2Texture_.reset(new Texture2D());
    background2Texture_->Load(tileset, sf::IntRect(0, 166, 209, 273));

    textPosition_ = sf::Vector2i(215, 239);
    textTexture_.reset(new Texture2D());
    if (language == TEXT_LANGUAGE_ENGLISH) {
        textTexture_->Load(tileset, sf::IntRect(0, 439, 209, 73));
    } else {
        textTexture_->Load(tileset, sf::IntRect(209, 439, 209, 73));
    }
}

//--------------------------------------------------------------------------------

void TetraminoesIntroState::Release() {
    background1Texture_.reset(nullptr);
    background2Texture_.reset(nullptr);
    textTexture_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesIntroState::Draw(const sf::Time & timeDelta) {
    background1Texture_->Draw(0, 0);
    background2Texture_->Draw(background2Position_);
    textTexture_->Draw(textPosition_);
}

//--------------------------------------------------------------------------------

void TetraminoesIntroState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = TetraminoesManager::Instance();
    if (Keyboard::IsAnyKeyUp()) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesIntroState::TetraminoesIntroState() {}

//--------------------------------------------------------------------------------

TetraminoesIntroState::~TetraminoesIntroState() {}
