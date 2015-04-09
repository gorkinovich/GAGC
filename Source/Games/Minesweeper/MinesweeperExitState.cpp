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

#include "MinesweeperExitState.h"
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

void MinesweeperExitState::Initialize() {
    MinesweeperSharedState::Initialize();

    auto * core = CoreManager::Instance();
    auto language = core->Language();

    auto * manager = MinesweeperManager::Instance();
    auto & tileset = manager->Tileset();

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
        0, 132, AtariPalette::Hue00Lum12, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("Yes", 0, 222,
        AtariPalette::Hue00Lum00, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", 0, 222,
        AtariPalette::Hue00Lum00, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        questionLabel_->Text("¿Deseas realmente salir?");
        yesLabel_->Text("Sí");
        noLabel_->Text("No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, 200);
    GUIUtil::CenterLabel(noLabel_, 440);

    yesButton_.reset(new TexturedButton());
    yesButton_->Initialize(160, 216, tileset,
        sf::IntRect(176, 42, 80, 24), sf::IntRect(176, 162, 80, 24),
        sf::IntRect(176, 186, 80, 24));
    yesButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->Release();
    });

    noButton_.reset(new TexturedButton());
    noButton_->Initialize(400, 216, tileset,
        sf::IntRect(176, 42, 80, 24), sf::IntRect(176, 114, 80, 24),
        sf::IntRect(176, 138, 80, 24));
    noButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<MinesweeperGameState>());
    });
}

//--------------------------------------------------------------------------------

void MinesweeperExitState::Release() {
    MinesweeperSharedState::Release();
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
    yesButton_.reset(nullptr);
    noButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void MinesweeperExitState::Draw(const sf::Time & timeDelta) {
    MinesweeperSharedState::Draw(timeDelta);

    yesButton_->Draw();
    noButton_->Draw();

    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void MinesweeperExitState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = MinesweeperManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<MinesweeperGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->KeyboardSound().Play();
        manager->Release();

    } else {
        MinesweeperSharedState::Update(timeDelta);
        yesButton_->Update();
        noButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

MinesweeperExitState::MinesweeperExitState() {}

//--------------------------------------------------------------------------------

MinesweeperExitState::~MinesweeperExitState() {}
