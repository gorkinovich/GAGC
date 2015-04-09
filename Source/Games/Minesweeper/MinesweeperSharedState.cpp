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

#include "MinesweeperSharedState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <Games/Minesweeper/MinesweeperManager.h>
#include <Games/Minesweeper/MinesweeperOptionsState.h>
#include <Games/Minesweeper/MinesweeperCreditsState.h>
#include <Games/Minesweeper/MinesweeperExitState.h>

//********************************************************************************
// Methods
//********************************************************************************

void MinesweeperSharedState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum02);

    auto * manager = MinesweeperManager::Instance();
    auto & tileset = manager->Tileset();

    barPosition_ = sf::Vector2i(480, 0);
    barTexture_.reset(new Texture2D());
    barTexture_->Load(tileset, sf::IntRect(0, 137, 160, 24));

    optionsLabel_.reset(new SimpleLabel("Options", 8, 6,
        AtariPalette::Hue00Lum00, core->Retro70Font()));

    aboutLabel_.reset(new SimpleLabel("About", 168, 6,
        AtariPalette::Hue00Lum00, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 328, 6,
        AtariPalette::Hue00Lum00, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        optionsLabel_->Text("Opciones");
        aboutLabel_->Text("Sobre...");
        exitLabel_->Text("Salir");
    }

    optionsButton_.reset(new TexturedButton());
    optionsButton_->Initialize(0, 0, tileset,
        sf::IntRect(0, 65, 160, 24), sf::IntRect(0, 89, 160, 24),
        sf::IntRect(0, 113, 160, 24));
    optionsButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<MinesweeperOptionsState>());
    });

    aboutButton_.reset(new TexturedButton());
    aboutButton_->Initialize(160, 0, tileset,
        sf::IntRect(0, 65, 160, 24), sf::IntRect(0, 89, 160, 24),
        sf::IntRect(0, 113, 160, 24));
    aboutButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<MinesweeperCreditsState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(320, 0, tileset,
        sf::IntRect(0, 65, 160, 24), sf::IntRect(0, 89, 160, 24),
        sf::IntRect(0, 113, 160, 24));
    exitButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<MinesweeperExitState>());
    });
}

//--------------------------------------------------------------------------------

void MinesweeperSharedState::Release() {
    barTexture_.reset(nullptr);
    optionsLabel_.reset(nullptr);
    aboutLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    optionsButton_.reset(nullptr);
    aboutButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void MinesweeperSharedState::Draw(const sf::Time & timeDelta) {
    optionsButton_->Draw();
    aboutButton_->Draw();
    exitButton_->Draw();

    optionsLabel_->Draw();
    aboutLabel_->Draw();
    exitLabel_->Draw();

    barTexture_->Draw(barPosition_);
}

//--------------------------------------------------------------------------------

void MinesweeperSharedState::Update(const sf::Time & timeDelta) {
    optionsButton_->Update();
    aboutButton_->Update();
    exitButton_->Update();
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

MinesweeperSharedState::MinesweeperSharedState() {}

//--------------------------------------------------------------------------------

MinesweeperSharedState::~MinesweeperSharedState() {}
