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

#include "ChessConfigGameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Chess/ChessManager.h>
#include <Games/Chess/ChessGameData.h>
#include <Games/Chess/ChessMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ChessConfigGameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = ChessManager::Instance();
    auto & tileset = manager->Tileset();

    side_ = ChessGameData::WHITE_SIDE;
    difficulty_ = ChessGameData::NORMAL_LEVEL;

    selectedTexture_.reset(new Texture2D());
    selectedTexture_->Load(tileset, sf::IntRect(160, 384, 80, 24));

    const int ROW0 =  38, ROW1 =  76, ROW2 = 163;
    const int ROW3 = 201, ROW4 = 308, INCY =   6;
    const int COL0 = 200, COL1 = 360, COL2 = 120, COL3 = 280;
    const int COL4 = 440, COL5 = 200, COL6 = 360, INCX =  40;

    chooseSideLabel_.reset(new SimpleLabel("Choose a side",
        0, ROW0, AtariPalette::Hue01Lum14, core->Retro70Font()));

    whiteSideLabel_.reset(new SimpleLabel("White", 0, ROW1 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    blackSideLabel_.reset(new SimpleLabel("Black", 0, ROW1 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    chooseDifficultyLabel_.reset(new SimpleLabel("Choose a difficult level",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    easyLabel_.reset(new SimpleLabel("Easy", 0, ROW3 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    normalLabel_.reset(new SimpleLabel("Normal", 0, ROW3 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    hardLabel_.reset(new SimpleLabel("Hard", 0, ROW3 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    cancelLabel_.reset(new SimpleLabel("Cancel", 0, ROW4 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    startLabel_.reset(new SimpleLabel("Start", 0, ROW4 + INCY,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        chooseSideLabel_->Text("Elije un bando");
        whiteSideLabel_->Text("Blancas");
        blackSideLabel_->Text("Negras");
        chooseDifficultyLabel_->Text("Elije un nivel de dificultad");
        easyLabel_->Text("Fácil");
        normalLabel_->Text("Normal");
        hardLabel_->Text("Duro");
        startLabel_->Text("Empezar");
        cancelLabel_->Text("Cancelar");
    }

    GUIUtil::CenterLabel(chooseSideLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(whiteSideLabel_, COL0 + INCX);
    GUIUtil::CenterLabel(blackSideLabel_, COL1 + INCX);
    GUIUtil::CenterLabel(chooseDifficultyLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(easyLabel_, COL2 + INCX);
    GUIUtil::CenterLabel(normalLabel_, COL3 + INCX);
    GUIUtil::CenterLabel(hardLabel_, COL4 + INCX);
    GUIUtil::CenterLabel(cancelLabel_, COL5 + INCX);
    GUIUtil::CenterLabel(startLabel_, COL6 + INCX);

    whiteSideButton_.reset(new TexturedButton());
    whiteSideButton_->Initialize(COL0, ROW1, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(80, 384, 80, 24),
        sf::IntRect(80, 384, 80, 24));
    whiteSideButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        side_ = ChessGameData::WHITE_SIDE;
    });

    blackSideButton_.reset(new TexturedButton());
    blackSideButton_->Initialize(COL1, ROW1, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(80, 384, 80, 24),
        sf::IntRect(80, 384, 80, 24));
    blackSideButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        side_ = ChessGameData::BLACK_SIDE;
    });

    easyButton_.reset(new TexturedButton());
    easyButton_->Initialize(COL2, ROW3, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(80, 384, 80, 24),
        sf::IntRect(80, 384, 80, 24));
    easyButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        difficulty_ = ChessGameData::EASY_LEVEL;
    });

    normalButton_.reset(new TexturedButton());
    normalButton_->Initialize(COL3, ROW3, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(80, 384, 80, 24),
        sf::IntRect(80, 384, 80, 24));
    normalButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        difficulty_ = ChessGameData::NORMAL_LEVEL;
    });

    hardButton_.reset(new TexturedButton());
    hardButton_->Initialize(COL4, ROW3, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(80, 384, 80, 24),
        sf::IntRect(80, 384, 80, 24));
    hardButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        difficulty_ = ChessGameData::HARD_LEVEL;
    });

    cancelButton_.reset(new TexturedButton());
    cancelButton_->Initialize(COL5, ROW4, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(320, 408, 80, 24),
        sf::IntRect(400, 408, 80, 24));
    cancelButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<ChessMenuState>());
    });

    startButton_.reset(new TexturedButton());
    startButton_->Initialize(COL6, ROW4, tileset,
        sf::IntRect(80, 384, 80, 24), sf::IntRect(320, 384, 80, 24),
        sf::IntRect(400, 384, 80, 24));
    startButton_->OnClick([this, manager] (TexturedButton &) {
        if (side_ != ChessGameData::WHITE_SIDE &&
            side_ != ChessGameData::BLACK_SIDE &&
            difficulty_ != ChessGameData::EASY_LEVEL &&
            difficulty_ != ChessGameData::NORMAL_LEVEL &&
            difficulty_ != ChessGameData::HARD_LEVEL) {
            manager->WrongSound().Play();
        } else {
            manager->ClickSound().Play();
            manager->StartOnePlayerGame(side_, difficulty_);
        }
    });
}

//--------------------------------------------------------------------------------

void ChessConfigGameState::Release() {
    selectedTexture_.reset(nullptr);
    chooseSideLabel_.reset(nullptr);
    whiteSideLabel_.reset(nullptr);
    blackSideLabel_.reset(nullptr);
    chooseDifficultyLabel_.reset(nullptr);
    easyLabel_.reset(nullptr);
    normalLabel_.reset(nullptr);
    hardLabel_.reset(nullptr);
    startLabel_.reset(nullptr);
    cancelLabel_.reset(nullptr);
    whiteSideButton_.reset(nullptr);
    blackSideButton_.reset(nullptr);
    easyButton_.reset(nullptr);
    normalButton_.reset(nullptr);
    hardButton_.reset(nullptr);
    cancelButton_.reset(nullptr);
    startButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ChessConfigGameState::Draw(const sf::Time & timeDelta) {
    chooseSideLabel_->Draw();

    if (side_ == ChessGameData::WHITE_SIDE) {
        selectedTexture_->Draw(whiteSideButton_->X(), whiteSideButton_->Y());
        blackSideButton_->Draw();
    } else if (side_ == ChessGameData::BLACK_SIDE) {
        whiteSideButton_->Draw();
        selectedTexture_->Draw(blackSideButton_->X(), blackSideButton_->Y());
    } else {
        whiteSideButton_->Draw();
        blackSideButton_->Draw();
    }

    whiteSideLabel_->Draw();
    blackSideLabel_->Draw();

    chooseDifficultyLabel_->Draw();

    if (difficulty_ == ChessGameData::EASY_LEVEL) {
        selectedTexture_->Draw(easyButton_->X(), easyButton_->Y());
        normalButton_->Draw();
        hardButton_->Draw();
    } else if (difficulty_ == ChessGameData::NORMAL_LEVEL) {
        easyButton_->Draw();
        selectedTexture_->Draw(normalButton_->X(), normalButton_->Y());
        hardButton_->Draw();
    } else if (difficulty_ == ChessGameData::HARD_LEVEL) {
        easyButton_->Draw();
        normalButton_->Draw();
        selectedTexture_->Draw(hardButton_->X(), hardButton_->Y());
    } else {
        easyButton_->Draw();
        normalButton_->Draw();
        hardButton_->Draw();
    }

    easyLabel_->Draw();
    normalLabel_->Draw();
    hardLabel_->Draw();

    cancelButton_->Draw();
    cancelLabel_->Draw();

    startButton_->Draw();
    startLabel_->Draw();

    ChessManager::Instance()->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ChessConfigGameState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        ChessManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<ChessMenuState>());

    } else {
        whiteSideButton_->Update();
        blackSideButton_->Update();
        easyButton_->Update();
        normalButton_->Update();
        hardButton_->Update();
        cancelButton_->Update();
        startButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ChessConfigGameState::ChessConfigGameState() {}

//--------------------------------------------------------------------------------

ChessConfigGameState::~ChessConfigGameState() {}
