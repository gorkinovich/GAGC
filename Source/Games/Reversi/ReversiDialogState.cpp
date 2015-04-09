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

#include "ReversiDialogState.h"
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
#include <Games/Reversi/ReversiSaveGames.h>
#include <Games/Reversi/ReversiGameState.h>
#include <Games/Reversi/ReversiMenuState.h>
#include <Games/Reversi/ReversiLoadState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ReversiDialogState::Initialize() {
    core_ = CoreManager::Instance();
    auto language = core_->Language();
    core_->HideMouseCursor();
    core_->SetClearColor(AtariPalette::Hue00Lum00);

    manager_ = ReversiManager::Instance();
    auto & tileset = manager_->Tileset();

    if (dialogType_ == EXIT_GAME_DLG) {
        titleTexture_.reset(nullptr);
        questionLabel_.reset(new SimpleLabel("Do you want to exit the current game?",
            0, 150, AtariPalette::Hue00Lum14, core_->Retro70Font()));

    } else if (dialogType_ == LOAD_FROM_MENU_DLG || dialogType_ == LOAD_FROM_GAME_DLG) {
        auto & savegames = manager_->Saves().GetData();
        auto savename = savegames[ReversiLoadState::LastSelecteIndex()].name;
        titleTexture_.reset(nullptr);
        questionLabel_.reset(new SimpleLabel("Do you want to load the savegame \"" +
            savename + "\"?", 0, 150, AtariPalette::Hue00Lum14, core_->Retro70Font()));

    } else if (dialogType_ == RESET_DLG) {
        titleTexture_.reset(nullptr);
        questionLabel_.reset(new SimpleLabel("Do you want to reset the current game?",
            0, 150, AtariPalette::Hue00Lum14, core_->Retro70Font()));

    } else {
        titleTexture_.reset(new Texture2D());
        titleTexture_->Load(tileset, sf::IntRect(0, 461, 336, 51));
        titlePosition_ = sf::Vector2i(152, 33);

        questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
            0, 186, AtariPalette::Hue00Lum14, core_->Retro70Font()));
    }

    yesLabel_.reset(new SimpleLabel("Yes", 0, 234,
        AtariPalette::Hue00Lum14, core_->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", 0, 234,
        AtariPalette::Hue00Lum14, core_->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        if (dialogType_ == EXIT_GAME_DLG) {
            questionLabel_->Text("¿Deseas salir de la partida actual?");
        } else if (dialogType_ == LOAD_FROM_MENU_DLG || dialogType_ == LOAD_FROM_GAME_DLG) {
            auto & savegames = manager_->Saves().GetData();
            auto savename = savegames[ReversiLoadState::LastSelecteIndex()].name;
            questionLabel_->Text("¿Deseas cargar la partida \"" + savename + "\"?");
        } else if (dialogType_ == RESET_DLG) {
            questionLabel_->Text("¿Deseas reiniciar la partida actual?");
        } else {
            questionLabel_->Text("¿Deseas realmente salir?");
        }
        yesLabel_->Text("Sí");
        noLabel_->Text("No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, 200);
    GUIUtil::CenterLabel(noLabel_, 440);

    yesButton_.reset(new TexturedButton());
    noButton_.reset(new TexturedButton());

    if (dialogType_ == LOAD_FROM_MENU_DLG || dialogType_ == LOAD_FROM_GAME_DLG) {
        yesButton_->Initialize(160, 228, tileset,
            sf::IntRect(80, 384, 80, 24), sf::IntRect(320, 384, 80, 24),
            sf::IntRect(400, 384, 80, 24));
        noButton_->Initialize(400, 228, tileset,
            sf::IntRect(80, 384, 80, 24), sf::IntRect(320, 408, 80, 24),
            sf::IntRect(400, 408, 80, 24));
    } else {
        yesButton_->Initialize(160, 228, tileset,
            sf::IntRect(80, 384, 80, 24), sf::IntRect(320, 408, 80, 24),
            sf::IntRect(400, 408, 80, 24));
        noButton_->Initialize(400, 228, tileset,
            sf::IntRect(80, 384, 80, 24), sf::IntRect(320, 384, 80, 24),
            sf::IntRect(400, 384, 80, 24));
    }

    yesButton_->OnClick([this] (TexturedButton &) {
        manager_->ClickSound().Play();
        executeYes();
    });
    noButton_->OnClick([this] (TexturedButton &) {
        manager_->ClickSound().Play();
        executeNo();
    });
}

//--------------------------------------------------------------------------------

void ReversiDialogState::Release() {
    titleTexture_.reset(nullptr);
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
    yesButton_.reset(nullptr);
    noButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ReversiDialogState::Draw(const sf::Time & timeDelta) {
    if (dialogType_ == EXIT_DLG) {
        titleTexture_->Draw(titlePosition_);
    }

    yesButton_->Draw();
    noButton_->Draw();

    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();

    manager_->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ReversiDialogState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager_->KeyboardSound().Play();
        executeNo();

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager_->KeyboardSound().Play();
        executeYes();

    } else {
        yesButton_->Update();
        noButton_->Update();
    }
}

//--------------------------------------------------------------------------------

void ReversiDialogState::executeYes() {
    if (dialogType_ == EXIT_GAME_DLG) {
        core_->SetNextState(MakeSharedState<ReversiMenuState>());
    } else if (dialogType_ == LOAD_FROM_MENU_DLG || dialogType_ == LOAD_FROM_GAME_DLG) {
        manager_->LoadPreviousGame(ReversiLoadState::LastSelecteIndex());
    } else if (dialogType_ == RESET_DLG) {
        manager_->ResetGame();
    } else {
        manager_->Release();
    }
}

//--------------------------------------------------------------------------------

void ReversiDialogState::executeNo() {
    if (dialogType_ == EXIT_GAME_DLG) {
        core_->SetNextState(MakeSharedState<ReversiGameState>());
    } else if (dialogType_ == LOAD_FROM_MENU_DLG) {
        core_->SetNextState(MakeSharedState<ReversiLoadState>(ReversiLoadState::FROM_MENU));
    } else if (dialogType_ == LOAD_FROM_GAME_DLG) {
        core_->SetNextState(MakeSharedState<ReversiLoadState>(ReversiLoadState::FROM_GAME));
    } else if (dialogType_ == RESET_DLG) {
        core_->SetNextState(MakeSharedState<ReversiGameState>());
    } else {
        core_->SetNextState(MakeSharedState<ReversiMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ReversiDialogState::ReversiDialogState(int dialogType) : dialogType_(dialogType) {}

//--------------------------------------------------------------------------------

ReversiDialogState::~ReversiDialogState() {}
