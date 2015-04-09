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

#include "SnakeGameOverState.h"
#include <sstream>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Snake/SnakeManager.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  146
#define ROW1  182
#define ROW2  194
#define ROW3  339
#define COL1  160
#define COL2  480

//********************************************************************************
// Methods
//********************************************************************************

void SnakeGameOverState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = SnakeManager::Instance();
    numberOfPlayers_ = manager->NumberOfPlayers();
    auto & players = manager->Players();

    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };

    titleLabel_.reset(new SimpleLabel("Game Over", 0, ROW0,
        AtariPalette::Hue11Lum02, core->Retro70Font()));

    if (numberOfPlayers_ < 2) {
        player1Label_.reset(new SimpleLabel("Your final score",
            0, ROW1, AtariPalette::Hue04Lum02, core->Retro70Font()));
    } else {
        player1Label_.reset(new SimpleLabel("Player 1 score",
            0, ROW1, AtariPalette::Hue04Lum02, core->Retro70Font()));
    }

    score1Label_.reset(new SimpleLabel(ToString(players[0].Score()),
        0, ROW2, AtariPalette::Hue00Lum14, core->Retro70Font()));

    player2Label_.reset(new SimpleLabel("Player 2 score",
        0, ROW1, AtariPalette::Hue09Lum02, core->Retro70Font()));

    score2Label_.reset(new SimpleLabel(ToString(players[1].Score()),
        0, ROW2, AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press any key to continue...",
        0, ROW3, AtariPalette::Hue01Lum12, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Partida Finalizada");
        if (numberOfPlayers_ < 2) {
            player1Label_->Text("Tu puntuación final");
        } else {
            player1Label_->Text("Puntuación jugador 1");
        }
        player2Label_->Text("Puntuación jugador 2");
        exitLabel_->Text("Presione cualquier tecla para continuar...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    if (numberOfPlayers_ < 2) {
        GUIUtil::CenterLabel(player1Label_, CoreManager::HALF_LOW_WIDTH);
        GUIUtil::CenterLabel(score1Label_, CoreManager::HALF_LOW_WIDTH);
    } else {
        GUIUtil::CenterLabel(player1Label_, COL1);
        GUIUtil::CenterLabel(score1Label_, COL1);
    }
    GUIUtil::CenterLabel(player2Label_, COL2);
    GUIUtil::CenterLabel(score2Label_, COL2);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void SnakeGameOverState::Release() {
    titleLabel_.reset(nullptr);
    player1Label_.reset(nullptr);
    score1Label_.reset(nullptr);
    player2Label_.reset(nullptr);
    score2Label_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeGameOverState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the labels of the menu.
    titleLabel_->Draw();
    player1Label_->Draw();
    score1Label_->Draw();
    if (numberOfPlayers_ > 1) {
        player2Label_->Draw();
        score2Label_->Draw();
    }
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeGameOverState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        auto * manager = SnakeManager::Instance();
        manager->MenuNextSound().Play();
        manager->ExitGameOver();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeGameOverState::SnakeGameOverState() : numberOfPlayers_(0),
    titleLabel_(nullptr), player1Label_(nullptr), score1Label_(nullptr),
    player2Label_(nullptr), score2Label_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeGameOverState::~SnakeGameOverState() {}
