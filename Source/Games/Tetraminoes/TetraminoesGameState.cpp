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

#include "TetraminoesGameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <Games/Tetraminoes/TetraminoesManager.h>

//********************************************************************************
// Defines
//********************************************************************************

#define BLACK_COLOR  sf::Color::Black
#define WHITE_COLOR  sf::Color::White

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesGameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    manager = TetraminoesManager::Instance();

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->GameBackground());

    nextLabel_.reset(new SimpleLabel("Next",
        384, 29, WHITE_COLOR, core->Retro70Font()));

    levelLabel_.reset(new SimpleLabel("Level",
        384, 176, WHITE_COLOR, core->Retro70Font()));

    linesLabel_.reset(new SimpleLabel("Lines",
        384, 226, WHITE_COLOR, core->Retro70Font()));

    scoreLabel_.reset(new SimpleLabel("Score",
        384, 276, WHITE_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        nextLabel_->Text("Siguiente");
        levelLabel_->Text("Nivel");
        linesLabel_->Text("Líneas");
        scoreLabel_->Text("Puntos");
    }
}

//--------------------------------------------------------------------------------

void TetraminoesGameState::Release() {
    manager = nullptr;
    backgroundTexture_.reset(nullptr);
    nextLabel_.reset(nullptr);
    levelLabel_.reset(nullptr);
    linesLabel_.reset(nullptr);
    scoreLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesGameState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);
    nextLabel_->Draw();
    levelLabel_->Draw();
    linesLabel_->Draw();
    scoreLabel_->Draw();
    manager->DrawGame();
}

//--------------------------------------------------------------------------------

void TetraminoesGameState::Update(const sf::Time & timeDelta) {
    manager->UpdateGame(timeDelta);
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesGameState::TetraminoesGameState() : manager(nullptr) {}

//--------------------------------------------------------------------------------

TetraminoesGameState::~TetraminoesGameState() {}
