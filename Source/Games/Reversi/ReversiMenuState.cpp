/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#include "ReversiMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Reversi/ReversiManager.h>
#include <Games/Reversi/ReversiConfigGameState.h>
#include <Games/Reversi/ReversiLoadState.h>
#include <Games/Reversi/ReversiHelpState.h>
#include <Games/Reversi/ReversiCreditsState.h>
#include <Games/Reversi/ReversiDialogState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ReversiMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = ReversiManager::Instance();
    manager->Initialize();

    auto & tileset = manager->Tileset();

    titleTexture_.reset(new Texture2D());
    titleTexture_->Load(tileset, sf::IntRect(0, 461, 336, 51));
    titlePosition_ = sf::Vector2i(152, 33);

    whiteTexture_.reset(new Texture2D());
    whiteTexture_->Load(tileset, sf::IntRect(364, 44, 32, 32));

    blackTexture_.reset(new Texture2D());
    blackTexture_->Load(tileset, sf::IntRect(404, 44, 32, 32));

    const int ROW0 = 108, ROW1 = 144, ROW2 = 180;
    const int ROW3 = 216, ROW4 = 252, ROW5 = 300;
    const int COL0 = 240, INCY = 6;

    onePlayerNewGameLabel_.reset(new SimpleLabel("1 Player",
        0, ROW0 + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()));

    twoPlayerNewGameLabel_.reset(new SimpleLabel("2 Players",
        0, ROW1 + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()));

    loadLabel_.reset(new SimpleLabel("Load",
        0, ROW2 + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help",
        0, ROW3 + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits",
        0, ROW4 + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit",
        0, ROW5 + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        onePlayerNewGameLabel_->Text("1 Jugador");
        twoPlayerNewGameLabel_->Text("2 Jugadores");
        loadLabel_->Text("Cargar");
        helpLabel_->Text("Ayuda");
        creditsLabel_->Text("Cr�ditos");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(onePlayerNewGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(twoPlayerNewGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(loadLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(creditsLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    onePlayerNewGameButton_.reset(new TexturedButton());
    onePlayerNewGameButton_->Initialize(COL0, ROW0, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    onePlayerNewGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<ReversiConfigGameState>());
    });

    twoPlayerNewGameButton_.reset(new TexturedButton());
    twoPlayerNewGameButton_->Initialize(COL0, ROW1, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    twoPlayerNewGameButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->StartTwoPlayersGame();
    });

    loadButton_.reset(new TexturedButton());
    loadButton_->Initialize(COL0, ROW2, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    loadButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<ReversiLoadState>(ReversiLoadState::FROM_MENU));
    });

    helpButton_.reset(new TexturedButton());
    helpButton_->Initialize(COL0, ROW3, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    helpButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<ReversiHelpState>());
    });

    creditsButton_.reset(new TexturedButton());
    creditsButton_->Initialize(COL0, ROW4, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    creditsButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<ReversiCreditsState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(COL0, ROW5, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(ReversiDialogState::MakeExitDialog());
    });
}

//--------------------------------------------------------------------------------

void ReversiMenuState::Release() {
    titleTexture_.reset(nullptr);
    whiteTexture_.reset(nullptr);
    blackTexture_.reset(nullptr);
    onePlayerNewGameLabel_.reset(nullptr);
    twoPlayerNewGameLabel_.reset(nullptr);
    loadLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    onePlayerNewGameButton_.reset(nullptr);
    twoPlayerNewGameButton_.reset(nullptr);
    loadButton_.reset(nullptr);
    helpButton_.reset(nullptr);
    creditsButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ReversiMenuState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);

    whiteTexture_->Draw(sf::Vector2i(64, 123));
    whiteTexture_->Draw(sf::Vector2i(112, 178));
    whiteTexture_->Draw(sf::Vector2i(68, 233));
    whiteTexture_->Draw(sf::Vector2i(116, 288));

    blackTexture_->Draw(sf::Vector2i(576, 123));
    blackTexture_->Draw(sf::Vector2i(528, 178));
    blackTexture_->Draw(sf::Vector2i(572, 233));
    blackTexture_->Draw(sf::Vector2i(524, 288));

    onePlayerNewGameButton_->Draw();
    twoPlayerNewGameButton_->Draw();
    loadButton_->Draw();
    helpButton_->Draw();
    creditsButton_->Draw();
    exitButton_->Draw();

    onePlayerNewGameLabel_->Draw();
    twoPlayerNewGameLabel_->Draw();
    loadLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();

    ReversiManager::Instance()->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ReversiMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = ReversiManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::F1) || Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<ReversiConfigGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2) || Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        manager->StartTwoPlayersGame();

    } else if (Keyboard::IsKeyUp(Keyboard::F3) || Keyboard::IsKeyUp(Keyboard::Num3)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<ReversiLoadState>(ReversiLoadState::FROM_MENU));

    } else if (Keyboard::IsKeyUp(Keyboard::F4) || Keyboard::IsKeyUp(Keyboard::Num4)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<ReversiHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F5) || Keyboard::IsKeyUp(Keyboard::Num5)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<ReversiCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::Num6)) {
        manager->KeyboardSound().Play();
        core->SetNextState(ReversiDialogState::MakeExitDialog());

    } else {
        onePlayerNewGameButton_->Update();
        twoPlayerNewGameButton_->Update();
        loadButton_->Update();
        helpButton_->Update();
        creditsButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ReversiMenuState::ReversiMenuState() {}

//--------------------------------------------------------------------------------

ReversiMenuState::~ReversiMenuState() {}
