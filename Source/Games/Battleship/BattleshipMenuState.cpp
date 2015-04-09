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

#include "BattleshipMenuState.h"
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
#include <Games/Battleship/BattleshipHelpState.h>
#include <Games/Battleship/BattleshipCreditsState.h>
#include <Games/Battleship/BattleshipExitState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = BattleshipManager::Instance();
    manager->Initialize();

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

    onePlayerNewGameLabel_.reset(new SimpleLabel("1P New Game", 0, 186,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    twoPlayerNewGameLabel_.reset(new SimpleLabel("2P New Game", 0, 186,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 0, 282,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        onePlayerNewGameLabel_->Text("Nueva Partida 1J");
        twoPlayerNewGameLabel_->Text("Nueva Partida 2J");
        helpLabel_->Text("Ayuda");
        creditsLabel_->Text("Créditos");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(onePlayerNewGameLabel_, 200);
    GUIUtil::CenterLabel(twoPlayerNewGameLabel_, 440);
    GUIUtil::CenterLabel(helpLabel_, 200);
    GUIUtil::CenterLabel(creditsLabel_, 440);
    GUIUtil::CenterLabel(exitLabel_, 320);

    onePlayerNewGameButton_.reset(new TexturedButton());
    onePlayerNewGameButton_->Initialize(120, 180, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(0, 464, 160, 24),
        sf::IntRect(0, 488, 160, 24));
    onePlayerNewGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->StartGame(1);
    });

    twoPlayerNewGameButton_.reset(new TexturedButton());
    twoPlayerNewGameButton_->Initialize(360, 180, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(0, 464, 160, 24),
        sf::IntRect(0, 488, 160, 24));
    twoPlayerNewGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->StartGame(2);
    });

    helpButton_.reset(new TexturedButton());
    helpButton_->Initialize(120, 228, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(160, 464, 160, 24),
        sf::IntRect(160, 488, 160, 24));
    helpButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipHelpState>());
    });

    creditsButton_.reset(new TexturedButton());
    creditsButton_->Initialize(360, 228, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(320, 464, 160, 24),
        sf::IntRect(320, 488, 160, 24));
    creditsButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipCreditsState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(240, 276, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(160, 440, 160, 24),
        sf::IntRect(320, 440, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipExitState>());
    });
}

//--------------------------------------------------------------------------------

void BattleshipMenuState::Release() {
    titleTexture_.reset(nullptr);
    ship1Texture_.reset(nullptr);
    ship2Texture_.reset(nullptr);
    ship3Texture_.reset(nullptr);
    ship4Texture_.reset(nullptr);
    onePlayerNewGameLabel_.reset(nullptr);
    twoPlayerNewGameLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    onePlayerNewGameButton_.reset(nullptr);
    twoPlayerNewGameButton_.reset(nullptr);
    helpButton_.reset(nullptr);
    creditsButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);
    ship1Texture_->Draw(ship1Position_);
    ship2Texture_->Draw(ship2Position_);
    ship3Texture_->Draw(ship3Position_);
    ship4Texture_->Draw(ship4Position_);

    onePlayerNewGameButton_->Draw();
    twoPlayerNewGameButton_->Draw();
    helpButton_->Draw();
    creditsButton_->Draw();
    exitButton_->Draw();

    onePlayerNewGameLabel_->Draw();
    twoPlayerNewGameLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = BattleshipManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::F1) || Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        manager->StartGame(1);

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        manager->StartGame(2);

    } else if (Keyboard::IsKeyUp(Keyboard::F3) || Keyboard::IsKeyUp(Keyboard::Num3)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BattleshipHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F4) || Keyboard::IsKeyUp(Keyboard::Num4)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BattleshipCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::Num5)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BattleshipExitState>());

    } else {
        onePlayerNewGameButton_->Update();
        twoPlayerNewGameButton_->Update();
        helpButton_->Update();
        creditsButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipMenuState::BattleshipMenuState() {}

//--------------------------------------------------------------------------------

BattleshipMenuState::~BattleshipMenuState() {}
