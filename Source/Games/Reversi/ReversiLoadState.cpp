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

#include "ReversiLoadState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Games/Reversi/ReversiManager.h>
#include <Games/Reversi/ReversiSaveGames.h>
#include <Games/Reversi/ReversiMenuState.h>
#include <Games/Reversi/ReversiGameState.h>
#include <Games/Reversi/ReversiDialogState.h>

//********************************************************************************
// Static
//********************************************************************************

int ReversiLoadState::lastSelecteIndex_ = -1;

//********************************************************************************
// Methods
//********************************************************************************

void ReversiLoadState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    const int ROW0 = 24, ROW1 = 336;

    titleLabel_.reset(new SimpleLabel("Load a previous game",
        0, ROW0, AtariPalette::Hue10Lum04, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press escape to return to previous screen...",
        0, ROW1, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Cargar una partida previa");
        exitLabel_->Text("Presione escape para volver a la pantalla anterior...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    auto * manager = ReversiManager::Instance();
    auto & tileset = manager->Tileset();
    auto & savegames = manager->Saves().GetData();

    int dialogType = (calledFrom_ == FROM_GAME) ?
        ReversiDialogState::LOAD_FROM_GAME_DLG :
        ReversiDialogState::LOAD_FROM_MENU_DLG;

    const int TABLE_ROW = 60, TABLE_OFFSET = 36;
    const int TABLE_COL0 = 110, TABLE_COL1 = 370, INCY = 6;
    int x = TABLE_COL0, y = TABLE_ROW;
    for (int i = 0; i < ReversiSaveGames::MAX_ENTRIES; ++i) {
        savesLabels_.push_back(std::unique_ptr<SimpleLabel>(nullptr));
        savesButtons_.push_back(std::unique_ptr<TexturedButton>(nullptr));

        savesLabels_[i].reset(new SimpleLabel(
            savegames[i].used ? savegames[i].name : "----------------",
            0, y + INCY, AtariPalette::Hue00Lum14, core->Retro70Font()
        ));

        GUIUtil::CenterLabel(savesLabels_[i], x + 80);

        savesButtons_[i].reset(new TexturedButton());
        savesButtons_[i]->Initialize(x, y, tileset,
            sf::IntRect(0, 360, 160, 24), sf::IntRect(160, 360, 160, 24),
            sf::IntRect(320, 360, 160, 24));
        savesButtons_[i]->OnClick([=] (TexturedButton &) {
            if (savegames[i].used) {
                manager->ClickSound().Play();
                lastSelecteIndex_ = i;
                core->SetNextState(MakeSharedState<ReversiDialogState>(dialogType));
            } else {
                manager->WrongSound().Play();
            }
        });

        if (x == TABLE_COL0) {
            x = TABLE_COL1;
        } else {
            x = TABLE_COL0;
            y += TABLE_OFFSET;
        }
    }
}

//--------------------------------------------------------------------------------

void ReversiLoadState::Release() {
    titleLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    ForEach(savesLabels_, [] (std::unique_ptr<SimpleLabel> & victim) {
        victim.reset(nullptr);
    });
    ForEach(savesButtons_, [] (std::unique_ptr<TexturedButton> & victim) {
        victim.reset(nullptr);
    });
    savesLabels_.clear();
    savesButtons_.clear();
}

//--------------------------------------------------------------------------------

void ReversiLoadState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    exitLabel_->Draw();

    ForEach(savesButtons_, [] (std::unique_ptr<TexturedButton> & victim) {
        victim->Draw();
    });
    ForEach(savesLabels_, [] (std::unique_ptr<SimpleLabel> & victim) {
        victim->Draw();
    });

    ReversiManager::Instance()->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ReversiLoadState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    auto * core = CoreManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape))  {
        ReversiManager::Instance()->KeyboardSound().Play();
        if (calledFrom_ == FROM_GAME) {
            core->SetNextState(MakeSharedState<ReversiGameState>());
        } else {
            core->SetNextState(MakeSharedState<ReversiMenuState>());
        }
    } else {
        ForEach(savesButtons_, [] (std::unique_ptr<TexturedButton> & victim) {
            victim->Update();
        });
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ReversiLoadState::ReversiLoadState(int calledFrom) : calledFrom_(calledFrom) {}

//--------------------------------------------------------------------------------

ReversiLoadState::~ReversiLoadState() {}
