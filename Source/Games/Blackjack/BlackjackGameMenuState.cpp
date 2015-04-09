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

#include "BlackjackGameMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Blackjack/BlackjackManager.h>
#include <Games/Blackjack/BlackjackGameState.h>
#include <Games/Blackjack/BlackjackMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BlackjackGameMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue12Lum00);

    auto * manager = BlackjackManager::Instance();
    auto & tileset = manager->Tileset();

    titlePosition_ = sf::Vector2i(107, 69);
    titleTexture_.reset(new Texture2D());
    titleTexture_->Load(tileset, sf::IntRect(0, 470, 426, 42));

    spadePosition_ = sf::Vector2i(19, 77);
    spadeTexture_.reset(new Texture2D());
    spadeTexture_->Load(tileset, sf::IntRect(0, 330, 25, 26));

    heartPosition_ = sf::Vector2i(63, 80);
    heartTexture_.reset(new Texture2D());
    heartTexture_->Load(tileset, sf::IntRect(0, 356, 25, 23));

    clubPosition_ = sf::Vector2i(551, 79);
    clubTexture_.reset(new Texture2D());
    clubTexture_->Load(tileset, sf::IntRect(0, 379, 27, 24));

    diamondPosition_ = sf::Vector2i(601, 77);
    diamondTexture_.reset(new Texture2D());
    diamondTexture_->Load(tileset, sf::IntRect(0, 403, 15, 26));

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
        sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 336, 160, 24),
        sf::IntRect(352, 336, 160, 24));
    continueButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BlackjackGameState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(240, 228, tileset,
        sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 360, 160, 24),
        sf::IntRect(352, 360, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BlackjackMenuState>());
    });
}

//--------------------------------------------------------------------------------

void BlackjackGameMenuState::Release() {
    titleTexture_.reset(nullptr);
    spadeTexture_.reset(nullptr);
    heartTexture_.reset(nullptr);
    clubTexture_.reset(nullptr);
    diamondTexture_.reset(nullptr);
    continueLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    continueButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BlackjackGameMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);
    spadeTexture_->Draw(spadePosition_);
    heartTexture_->Draw(heartPosition_);
    clubTexture_->Draw(clubPosition_);
    diamondTexture_->Draw(diamondPosition_);

    continueButton_->Draw();
    exitButton_->Draw();

    continueLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BlackjackGameMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = BlackjackManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::F1) ||
        Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BlackjackGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BlackjackMenuState>());

    } else {
        continueButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BlackjackGameMenuState::BlackjackGameMenuState() {}

//--------------------------------------------------------------------------------

BlackjackGameMenuState::~BlackjackGameMenuState() {}
