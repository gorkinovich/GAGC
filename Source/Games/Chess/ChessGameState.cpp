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

#include "ChessGameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Chess/ChessManager.h>
#include <Games/Chess/ChessDialogState.h>
#include <Games/Chess/ChessSaveState.h>
#include <Games/Chess/ChessLoadState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ChessGameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    manager_ = ChessManager::Instance();
    auto & tileset = manager_->Tileset();

    boardPosition_ = sf::Vector2i(44, 0);
    boardTexture_.reset(new Texture2D());
    boardTexture_->Load(tileset, sf::IntRect(0, 0, 360, 360));

    const int ROW0 =  24, ROW1 =  60, ROW2 =  96, ROW3 = 144;
    const int COL0 = 464, COL1 = 544, INCY =   6;

    resetLabel_.reset(new SimpleLabel("Reset", 0, ROW0 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    saveLabel_.reset(new SimpleLabel("Save", 0, ROW1 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    loadLabel_.reset(new SimpleLabel("Load", 0, ROW2 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 0, ROW3 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        resetLabel_->Text("Reiniciar");
        saveLabel_->Text("Salvar");
        loadLabel_->Text("Cargar");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(resetLabel_, COL1);
    GUIUtil::CenterLabel(saveLabel_, COL1);
    GUIUtil::CenterLabel(loadLabel_, COL1);
    GUIUtil::CenterLabel(exitLabel_, COL1);

    resetButton_.reset(new TexturedButton());
    resetButton_->Initialize(COL0, ROW0, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    resetButton_->OnClick([core, this] (TexturedButton &) {
        manager_->ClickSound().Play();
        core->SetNextState(ChessDialogState::MakeResetDialog());
    });

    saveButton_.reset(new TexturedButton());
    saveButton_->Initialize(COL0, ROW1, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    saveButton_->OnClick([core, this] (TexturedButton &) {
        manager_->ClickSound().Play();
        core->SetNextState(MakeSharedState<ChessSaveState>());
    });

    loadButton_.reset(new TexturedButton());
    loadButton_->Initialize(COL0, ROW2, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    loadButton_->OnClick([core, this] (TexturedButton &) {
        manager_->ClickSound().Play();
        core->SetNextState(MakeSharedState<ChessLoadState>(ChessLoadState::FROM_GAME));
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(COL0, ROW3, tileset,
        sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
        sf::IntRect(320, 360, 160, 24));
    exitButton_->OnClick([core, this] (TexturedButton &) {
        manager_->ClickSound().Play();
        core->SetNextState(ChessDialogState::MakeExitGameDialog());
    });

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void ChessGameState::Release() {
    manager_ = nullptr;
    boardTexture_.reset(nullptr);
    resetLabel_.reset(nullptr);
    saveLabel_.reset(nullptr);
    loadLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    resetButton_.reset(nullptr);
    saveButton_.reset(nullptr);
    loadButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ChessGameState::Draw(const sf::Time & timeDelta) {
    boardTexture_->Draw(boardPosition_);

    if (!(manager_->PawnToChange() || manager_->MachineTurn())) {
        resetButton_->Draw();
        saveButton_->Draw();
        loadButton_->Draw();
        exitButton_->Draw();

        resetLabel_->Draw();
        saveLabel_->Draw();
        loadLabel_->Draw();
        exitLabel_->Draw();
    }

    manager_->DrawGame();
    manager_->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ChessGameState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (manager_->GameNotFinished()) {
        if (manager_->PawnToChange()) {
            manager_->SelectPiece();

        } else if (manager_->MachineTurn()) {
            manager_->UpdateMachine(timeDelta);

        } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager_->KeyboardSound().Play();
            CoreManager::Instance()->SetNextState(ChessDialogState::MakeExitGameDialog());

        } else if (manager_->MouseOverBoard()) {
            manager_->UpdateBoard();

        } else {
            resetButton_->Update();
            saveButton_->Update();
            loadButton_->Update();
            exitButton_->Update();
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ChessGameState::ChessGameState() : manager_(nullptr) {}

//--------------------------------------------------------------------------------

ChessGameState::~ChessGameState() {}
