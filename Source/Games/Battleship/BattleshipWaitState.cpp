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

#include "BattleshipWaitState.h"
#include <sstream>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Battleship/BattleshipBoard.h>
#include <Games/Battleship/BattleshipManager.h>

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipWaitState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    manager_ = BattleshipManager::Instance();
    auto & tileset = manager_->Tileset();
    auto & name = manager_->GetPlayerName();

    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };

    BattleshipManager::CurrentData currentData;
    manager_->GetCurrentData(currentData);
    enemyBoard_ = currentData.enemyBoard;

    playerPanelPosition_ = sf::Vector2i(28, 308);
    playerPanelTexture_.reset(new Texture2D());
    playerPanelTexture_->Load(tileset, sf::IntRect(1, 59, 216, 44));

    infoPanelPosition_ = sf::Vector2i(348, 303);
    infoPanelTexture_.reset(new Texture2D());
    infoPanelTexture_->Load(tileset, sf::IntRect(25, 103, 96, 49));

    enemyBoardLabel_.reset(new SimpleLabel("Enemy's board",
        0, 4, AtariPalette::Hue01Lum14, core->Retro70Font()));

    playerLabel_.reset(new SimpleLabel(" Player:", 40, 312,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    turnLabel_.reset(new SimpleLabel("   Turn:", 40, 324,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    playerNameLabel_.reset(new SimpleLabel(currentData.name,
        112, 312, AtariPalette::Hue00Lum14, core->Retro70Font()));

    playerTurnLabel_.reset(new SimpleLabel(ToString(currentData.turn),
        112, 324, AtariPalette::Hue00Lum14, core->Retro70Font()));

    waterLabel_.reset(new SimpleLabel("Water", 376, 312,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    shipLabel_.reset(new SimpleLabel("Ship", 376, 324,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    hitLabel_.reset(new SimpleLabel("Hit", 376, 336,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    line1Label_.reset(new SimpleLabel("Please player " + name + ",",
        0, 168, AtariPalette::Hue01Lum14, core->Retro70Font()));

    line2Label_.reset(new SimpleLabel("press any key to continue...",
        0, 180, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        enemyBoardLabel_->Text("Tablero del enemigo");
        playerLabel_->Text("Jugador:");
        turnLabel_->Text("  Turno:");
        waterLabel_->Text("Agua");
        shipLabel_->Text("Barco");
        hitLabel_->Text("Tocado");
        line1Label_->Text("Por favor jugador " + name + ",");
        line2Label_->Text("presione cualquier tecla para continuar...");
    }

    GUIUtil::CenterLabel(enemyBoardLabel_, 480);
    GUIUtil::CenterLabel(line1Label_, CoreManager::LOW_WIDTH / 4);
    GUIUtil::CenterLabel(line2Label_, CoreManager::LOW_WIDTH / 4);

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BattleshipWaitState::Release() {
    manager_ = nullptr;
    enemyBoard_ = nullptr;
    playerPanelTexture_.reset(nullptr);
    infoPanelTexture_.reset(nullptr);
    enemyBoardLabel_.reset(nullptr);
    playerLabel_.reset(nullptr);
    turnLabel_.reset(nullptr);
    playerNameLabel_.reset(nullptr);
    playerTurnLabel_.reset(nullptr);
    waterLabel_.reset(nullptr);
    shipLabel_.reset(nullptr);
    hitLabel_.reset(nullptr);
    line1Label_.reset(nullptr);
    line2Label_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipWaitState::Draw(const sf::Time & timeDelta) {
    enemyBoardLabel_->Draw();
    enemyBoard_->Draw();

    playerPanelTexture_->Draw(playerPanelPosition_);
    playerLabel_->Draw();
    turnLabel_->Draw();
    playerNameLabel_->Draw();
    playerTurnLabel_->Draw();

    infoPanelTexture_->Draw(infoPanelPosition_);
    waterLabel_->Draw();
    shipLabel_->Draw();
    hitLabel_->Draw();

    line1Label_->Draw();
    line2Label_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipWaitState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Mouse::IsAnyButtonUp()) {
        manager_->ClickSound().Play();
        manager_->WaitDone();
    } else if (Keyboard::IsAnyKeyUp()) {
        manager_->KeyboardSound().Play();
        manager_->WaitDone();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipWaitState::BattleshipWaitState() {}

//--------------------------------------------------------------------------------

BattleshipWaitState::~BattleshipWaitState() {}
