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

#include "TicTacToeGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/TicTacToe/TicTacToeManager.h>
#include <Games/TicTacToe/TicTacToeGameState.h>
#include <Games/TicTacToe/TicTacToeMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void TicTacToeGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = TicTacToeManager::Instance();
    auto & tileset = manager->Tileset();

    titlePosition_ = sf::Vector2i(101, 29);
    titleTexture_.reset(new Texture2D());
    titleTexture_->Load(tileset, sf::IntRect(0, 0, 438, 122));

    continueLabel_.reset(new SimpleLabel("Continue", 0, 186,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit Game", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        continueLabel_->Text("Continuar");
        exitLabel_->Text("Salir Partida");
    }

    GUIUtil::CenterLabel(continueLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    continueButton_.reset(new TexturedButton());
    continueButton_->Initialize(240, 180, tileset,
        sf::IntRect(320, 310, 160, 24), sf::IntRect(320, 334, 160, 24),
        sf::IntRect(320, 358, 160, 24));
    continueButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeGameState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(240, 228, tileset,
        sf::IntRect(320, 310, 160, 24), sf::IntRect(320, 382, 160, 24),
        sf::IntRect(320, 406, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeMenuState>());
    });
}

//--------------------------------------------------------------------------------

void TicTacToeGameMenuState::Release() {
    titleTexture_.reset(nullptr);
    continueLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    continueButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TicTacToeGameMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);

    continueButton_->Draw();
    exitButton_->Draw();

    continueLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TicTacToeGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = TicTacToeManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::F1) ||
        Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeMenuState>());

    } else {
        continueButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TicTacToeGameMenuState::TicTacToeGameMenuState() {}

//--------------------------------------------------------------------------------

TicTacToeGameMenuState::~TicTacToeGameMenuState() {}
