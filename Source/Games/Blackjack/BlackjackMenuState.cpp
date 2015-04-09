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

#include "BlackjackMenuState.h"
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
#include <Games/Blackjack/BlackjackHelpState.h>
#include <Games/Blackjack/BlackjackCreditsState.h>
#include <Games/Blackjack/BlackjackExitState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BlackjackMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue12Lum00);

    auto * manager = BlackjackManager::Instance();
    manager->Initialize();

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

    newGameLabel_.reset(new SimpleLabel("New Game", 0, 186,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help", 0, 186,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        newGameLabel_->Text("Nueva Partida");
        helpLabel_->Text("Ayuda");
        creditsLabel_->Text("Créditos");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(newGameLabel_, 200);
    GUIUtil::CenterLabel(helpLabel_, 440);
    GUIUtil::CenterLabel(creditsLabel_, 200);
    GUIUtil::CenterLabel(exitLabel_, 440);

    newGameButton_.reset(new TexturedButton());
    newGameButton_->Initialize(120, 180, tileset,
        sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 264, 160, 24),
        sf::IntRect(352, 264, 160, 24));
    newGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->StartGame();
    });

    helpButton_.reset(new TexturedButton());
    helpButton_->Initialize(360, 180, tileset,
        sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 288, 160, 24),
        sf::IntRect(352, 288, 160, 24));
    helpButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BlackjackHelpState>());
    });

    creditsButton_.reset(new TexturedButton());
    creditsButton_->Initialize(120, 228, tileset,
        sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 312, 160, 24),
        sf::IntRect(352, 312, 160, 24));
    creditsButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BlackjackCreditsState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(360, 228, tileset,
        sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 384, 160, 24),
        sf::IntRect(352, 384, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BlackjackExitState>());
    });
}

//--------------------------------------------------------------------------------

void BlackjackMenuState::Release() {
    titleTexture_.reset(nullptr);
    spadeTexture_.reset(nullptr);
    heartTexture_.reset(nullptr);
    clubTexture_.reset(nullptr);
    diamondTexture_.reset(nullptr);
    newGameLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    newGameButton_.reset(nullptr);
    helpButton_.reset(nullptr);
    creditsButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BlackjackMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);
    spadeTexture_->Draw(spadePosition_);
    heartTexture_->Draw(heartPosition_);
    clubTexture_->Draw(clubPosition_);
    diamondTexture_->Draw(diamondPosition_);

    newGameButton_->Draw();
    helpButton_->Draw();
    creditsButton_->Draw();
    exitButton_->Draw();

    newGameLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BlackjackMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = BlackjackManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::F1) || Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        manager->StartGame();

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BlackjackHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F3) || Keyboard::IsKeyUp(Keyboard::Num3)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BlackjackCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::Num5)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BlackjackExitState>());

    } else {
        newGameButton_->Update();
        helpButton_->Update();
        creditsButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BlackjackMenuState::BlackjackMenuState() {}

//--------------------------------------------------------------------------------

BlackjackMenuState::~BlackjackMenuState() {}
