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

#include "ReversiGameOverState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/TextLabel.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Reversi/ReversiManager.h>
#include <Games/Reversi/ReversiGameData.h>
#include <Games/Reversi/ReversiMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ReversiGameOverState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = ReversiManager::Instance();
    auto & tileset = manager->Tileset();
    auto winner = manager->Winner();

    boardPosition_ = sf::Vector2i(44, 0);
    boardTexture_.reset(new Texture2D());
    boardTexture_->Load(tileset, sf::IntRect(0, 0, 360, 360));

    const int ROW0 = 168, ROW1 = 312, COL0 = 440;
    std::string playerText = "", exitText = "";
    if (language == TEXT_LANGUAGE_ENGLISH) {
        playerText = "Game Over:\n";
        if (winner == ReversiGameData::P1_WINNER) {
            playerText += "Player 1 wins!";
        } else if (winner == ReversiGameData::P2_WINNER) {
            playerText += "Player 2 wins!";
        } else if (winner == ReversiGameData::AI_WINNER) {
            playerText += "Machine wins!";
        } else {
            playerText += "Draw!";
        }
        exitText =
            "Press any key\n"
            "to continue...";
    } else {
        playerText = "Partida finalizada:\n";
        if (winner == ReversiGameData::P1_WINNER) {
            playerText += "¡Jugador 1 gana!";
        } else if (winner == ReversiGameData::P2_WINNER) {
            playerText += "¡Jugador 2 gana!";
        } else if (winner == ReversiGameData::AI_WINNER) {
            playerText += "¡La máquina gana!";
        } else {
            playerText += "¡Empate!";
        }
        exitText =
            "Presione cualquier\n"
            "tecla para continuar...";
    }

    playerLabel_.reset(new TextLabel(playerText, COL0, ROW0,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new TextLabel(exitText, COL0, ROW1,
        AtariPalette::Hue01Lum14, core->Retro70Font()));

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void ReversiGameOverState::Release() {
    boardTexture_.reset(nullptr);
    playerLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ReversiGameOverState::Draw(const sf::Time & timeDelta) {
    boardTexture_->Draw(boardPosition_);
    playerLabel_->Draw();
    exitLabel_->Draw();

    auto * manager = ReversiManager::Instance();
    manager->DrawGame();
    manager->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ReversiGameOverState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Mouse::IsAnyButtonUp()) {
        ReversiManager::Instance()->ClickSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<ReversiMenuState>());
    } else if (Keyboard::IsAnyKeyUp()) {
        ReversiManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<ReversiMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ReversiGameOverState::ReversiGameOverState() {}

//--------------------------------------------------------------------------------

ReversiGameOverState::~ReversiGameOverState() {}
