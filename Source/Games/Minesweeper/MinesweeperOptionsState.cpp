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

#include "MinesweeperOptionsState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Minesweeper/MinesweeperManager.h>
#include <Games/Minesweeper/MinesweeperGameState.h>

//********************************************************************************
// Methods
//********************************************************************************

void MinesweeperOptionsState::Initialize() {
    MinesweeperSharedState::Initialize();

    auto * core = CoreManager::Instance();
    auto language = core->Language();

    auto * manager = MinesweeperManager::Instance();
    auto & tileset = manager->Tileset();

    beginnerInfoLabel_.reset(new SimpleLabel("9x9 cells 10 mines",
        0, 74, AtariPalette::Hue00Lum12, core->Retro70Font()));

    intermediateInfoLabel_.reset(new SimpleLabel("16x16 cells 40 mines",
        0, 186, AtariPalette::Hue00Lum12, core->Retro70Font()));

    advanceInfoLabel_.reset(new SimpleLabel("16x30 cells 99 mines",
        0, 298, AtariPalette::Hue00Lum12, core->Retro70Font()));

    beginnerLabel_.reset(new SimpleLabel("Beginner",
        0, 74, AtariPalette::Hue09Lum02, core->Retro70Font()));

    intermediateLabel_.reset(new SimpleLabel("Intermediate",
        0, 186, AtariPalette::Hue12Lum02, core->Retro70Font()));

    advanceLabel_.reset(new SimpleLabel("Advance",
        0, 298, AtariPalette::Hue04Lum02, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        beginnerInfoLabel_->Text("9x9 celdas 10 minas");
        intermediateInfoLabel_->Text("16x16 celdas 40 minas");
        advanceInfoLabel_->Text("16x30 celdas 99 minas");
        beginnerLabel_->Text("Principiante");
        intermediateLabel_->Text("Intermedio");
        advanceLabel_->Text("Avanzado");
    }

    GUIUtil::CenterLabel(beginnerInfoLabel_, 213);
    GUIUtil::CenterLabel(intermediateInfoLabel_, 213);
    GUIUtil::CenterLabel(advanceInfoLabel_, 213);
    GUIUtil::CenterLabel(beginnerLabel_, 427);
    GUIUtil::CenterLabel(intermediateLabel_, 427);
    GUIUtil::CenterLabel(advanceLabel_, 427);

    beginnerButton_.reset(new TexturedButton());
    beginnerButton_->Initialize(347, 68, tileset,
        sf::IntRect(0, 161, 160, 24), sf::IntRect(0, 185, 160, 24),
        sf::IntRect(0, 209, 160, 24));
    beginnerButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->ConfigBeginnerGame();
    });

    intermediateButton_.reset(new TexturedButton());
    intermediateButton_->Initialize(347, 180, tileset,
        sf::IntRect(0, 161, 160, 24), sf::IntRect(0, 185, 160, 24),
        sf::IntRect(0, 209, 160, 24));
    intermediateButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->ConfigIntermediateGame();
    });

    advanceButton_.reset(new TexturedButton());
    advanceButton_->Initialize(347, 292, tileset,
        sf::IntRect(0, 161, 160, 24), sf::IntRect(0, 185, 160, 24),
        sf::IntRect(0, 209, 160, 24));
    advanceButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->ConfigAdvanceGame();
    });
}

//--------------------------------------------------------------------------------

void MinesweeperOptionsState::Release() {
    MinesweeperSharedState::Release();
    beginnerInfoLabel_.reset(nullptr);
    intermediateInfoLabel_.reset(nullptr);
    advanceInfoLabel_.reset(nullptr);
    beginnerLabel_.reset(nullptr);
    intermediateLabel_.reset(nullptr);
    advanceLabel_.reset(nullptr);
    beginnerButton_.reset(nullptr);
    intermediateButton_.reset(nullptr);
    advanceButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void MinesweeperOptionsState::Draw(const sf::Time & timeDelta) {
    MinesweeperSharedState::Draw(timeDelta);

    beginnerButton_->Draw();
    intermediateButton_->Draw();
    advanceButton_->Draw();

    beginnerLabel_->Draw();
    intermediateLabel_->Draw();
    advanceLabel_->Draw();

    beginnerInfoLabel_->Draw();
    intermediateInfoLabel_->Draw();
    advanceInfoLabel_->Draw();
}

//--------------------------------------------------------------------------------

void MinesweeperOptionsState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = MinesweeperManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<MinesweeperGameState>());

    } else {
        MinesweeperSharedState::Update(timeDelta);
        beginnerButton_->Update();
        intermediateButton_->Update();
        advanceButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

MinesweeperOptionsState::MinesweeperOptionsState() {}

//--------------------------------------------------------------------------------

MinesweeperOptionsState::~MinesweeperOptionsState() {}
