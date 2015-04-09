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

#include "TicTacToeMenuState.h"
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
#include <Games/TicTacToe/TicTacToeHelpState.h>
#include <Games/TicTacToe/TicTacToeCreditsState.h>
#include <Games/TicTacToe/TicTacToeExitState.h>

//********************************************************************************
// Methods
//********************************************************************************

void TicTacToeMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = TicTacToeManager::Instance();
    manager->Initialize();

    auto & tileset = manager->Tileset();

    titlePosition_ = sf::Vector2i(101, 29);
    titleTexture_.reset(new Texture2D());
    titleTexture_->Load(tileset, sf::IntRect(0, 0, 438, 122));

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
        sf::IntRect(0, 430, 160, 24), sf::IntRect(0, 454, 160, 24),
        sf::IntRect(0, 478, 160, 24));
    onePlayerNewGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->StartGame(1);
    });

    twoPlayerNewGameButton_.reset(new TexturedButton());
    twoPlayerNewGameButton_->Initialize(360, 180, tileset,
        sf::IntRect(0, 430, 160, 24), sf::IntRect(0, 454, 160, 24),
        sf::IntRect(0, 478, 160, 24));
    twoPlayerNewGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->StartGame(2);
    });

    helpButton_.reset(new TexturedButton());
    helpButton_->Initialize(120, 228, tileset,
        sf::IntRect(0, 430, 160, 24), sf::IntRect(160, 454, 160, 24),
        sf::IntRect(160, 478, 160, 24));
    helpButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeHelpState>());
    });

    creditsButton_.reset(new TexturedButton());
    creditsButton_->Initialize(360, 228, tileset,
        sf::IntRect(0, 430, 160, 24), sf::IntRect(320, 454, 160, 24),
        sf::IntRect(320, 478, 160, 24));
    creditsButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeCreditsState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(240, 276, tileset,
        sf::IntRect(0, 430, 160, 24), sf::IntRect(160, 430, 160, 24),
        sf::IntRect(320, 430, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeExitState>());
    });
}

//--------------------------------------------------------------------------------

void TicTacToeMenuState::Release() {
    titleTexture_.reset(nullptr);
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

void TicTacToeMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);

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

void TicTacToeMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = TicTacToeManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::F1) || Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        manager->StartGame(1);

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        manager->StartGame(2);

    } else if (Keyboard::IsKeyUp(Keyboard::F3) || Keyboard::IsKeyUp(Keyboard::Num3)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F4) || Keyboard::IsKeyUp(Keyboard::Num4)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::Num5)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TicTacToeExitState>());

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

TicTacToeMenuState::TicTacToeMenuState() {}

//--------------------------------------------------------------------------------

TicTacToeMenuState::~TicTacToeMenuState() {}
