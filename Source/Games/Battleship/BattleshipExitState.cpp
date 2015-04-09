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

#include "BattleshipExitState.h"
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
#include <Games/Battleship/BattleshipMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipExitState::Initialize() {
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

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
        0, 186, AtariPalette::Hue00Lum14, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("Yes", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        questionLabel_->Text("¿Deseas realmente salir?");
        yesLabel_->Text("Sí");
        noLabel_->Text("No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, 200);
    GUIUtil::CenterLabel(noLabel_, 440);

    yesButton_.reset(new TexturedButton());
    yesButton_->Initialize(160, 228, tileset,
        sf::IntRect(0, 368, 80, 24), sf::IntRect(80, 392, 80, 24),
        sf::IntRect(80, 416, 80, 24));
    yesButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->Release();
    });

    noButton_.reset(new TexturedButton());
    noButton_->Initialize(400, 228, tileset,
        sf::IntRect(0, 368, 80, 24), sf::IntRect(0, 392, 80, 24),
        sf::IntRect(0, 416, 80, 24));
    noButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipMenuState>());
    });
}

//--------------------------------------------------------------------------------

void BattleshipExitState::Release() {
    titleTexture_.reset(nullptr);
    ship1Texture_.reset(nullptr);
    ship2Texture_.reset(nullptr);
    ship3Texture_.reset(nullptr);
    ship4Texture_.reset(nullptr);
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
    yesButton_.reset(nullptr);
    noButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipExitState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);
    ship1Texture_->Draw(ship1Position_);
    ship2Texture_->Draw(ship2Position_);
    ship3Texture_->Draw(ship3Position_);
    ship4Texture_->Draw(ship4Position_);

    yesButton_->Draw();
    noButton_->Draw();

    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipExitState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = BattleshipManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BattleshipMenuState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->KeyboardSound().Play();
        manager->Release();

    } else {
        yesButton_->Update();
        noButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipExitState::BattleshipExitState() {}

//--------------------------------------------------------------------------------

BattleshipExitState::~BattleshipExitState() {}
