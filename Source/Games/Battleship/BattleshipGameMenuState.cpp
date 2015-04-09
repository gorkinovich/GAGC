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

#include "BattleshipGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Battleship/BattleshipManager.h>
#include <Games/Battleship/BattleshipGameState.h>
#include <Games/Battleship/BattleshipMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = BattleshipManager::Instance();
    auto & tileset = manager->Tileset();

    titlePosition_ = sf::Vector2i(83, 31);
    titleTexture_.reset(new Texture2D());
    titleTexture_->Load(tileset, sf::IntRect(0, 0, 474, 42));

    ship1Position_ = sf::Vector2i(128, 116);
    ship1Texture_.reset(new Texture2D());
    ship1Texture_->Load(tileset, sf::IntRect(1, 320, 24, 24));

    ship2Position_ = sf::Vector2i(200, 116);
    ship2Texture_.reset(new Texture2D());
    ship2Texture_->Load(tileset, sf::IntRect(1, 296, 48, 24));

    ship3Position_ = sf::Vector2i(296, 116);
    ship3Texture_.reset(new Texture2D());
    ship3Texture_->Load(tileset, sf::IntRect(1, 272, 72, 24));

    ship4Position_ = sf::Vector2i(416, 116);
    ship4Texture_.reset(new Texture2D());
    ship4Texture_->Load(tileset, sf::IntRect(1, 248, 96, 24));

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
        sf::IntRect(0, 440, 160, 24), sf::IntRect(160, 392, 160, 24),
        sf::IntRect(320, 392, 160, 24));
    continueButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipGameState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(240, 228, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(160, 416, 160, 24),
        sf::IntRect(320, 416, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipMenuState>());
    });
}

//--------------------------------------------------------------------------------

void BattleshipGameMenuState::Release() {
    titleTexture_.reset(nullptr);
    ship1Texture_.reset(nullptr);
    ship2Texture_.reset(nullptr);
    ship3Texture_.reset(nullptr);
    ship4Texture_.reset(nullptr);
    continueLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    continueButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipGameMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);
    ship1Texture_->Draw(ship1Position_);
    ship2Texture_->Draw(ship2Position_);
    ship3Texture_->Draw(ship3Position_);
    ship4Texture_->Draw(ship4Position_);

    continueButton_->Draw();
    exitButton_->Draw();

    continueLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = BattleshipManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::F1) ||
        Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BattleshipGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BattleshipMenuState>());

    } else {
        continueButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipGameMenuState::BattleshipGameMenuState() {}

//--------------------------------------------------------------------------------

BattleshipGameMenuState::~BattleshipGameMenuState() {}
