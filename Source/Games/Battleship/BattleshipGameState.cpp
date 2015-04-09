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

#include "BattleshipGameState.h"
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
#include <Games/Battleship/BattleshipGameMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipGameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    manager_ = BattleshipManager::Instance();
    auto & tileset = manager_->Tileset();

    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };

    BattleshipManager::CurrentData currentData;
    manager_->GetCurrentData(currentData);
    playerBoard_ = currentData.playerBoard;
    enemyBoard_ = currentData.enemyBoard;

    playerBoard_->SetModePlayer();
    enemyBoard_->SetModeEnemy([this] () {
        manager_->OnFireUpdate();
        updateHud();
    });

    playerPanelPosition_ = sf::Vector2i(28, 308);
    playerPanelTexture_.reset(new Texture2D());
    playerPanelTexture_->Load(tileset, sf::IntRect(1, 59, 216, 44));

    infoPanelPosition_ = sf::Vector2i(348, 303);
    infoPanelTexture_.reset(new Texture2D());
    infoPanelTexture_->Load(tileset, sf::IntRect(25, 103, 96, 49));

    playerBoardLabel_.reset(new SimpleLabel("Player's board",
        0, 4, AtariPalette::Hue01Lum14, core->Retro70Font()));

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

    menuLabel_.reset(new SimpleLabel("Menu", 0, 324,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        playerBoardLabel_->Text("Tablero del jugador");
        enemyBoardLabel_->Text("Tablero del enemigo");
        playerLabel_->Text("Jugador:");
        turnLabel_->Text("  Turno:");
        waterLabel_->Text("Agua");
        shipLabel_->Text("Barco");
        hitLabel_->Text("Tocado");
        menuLabel_->Text("Menú");
    }

    GUIUtil::CenterLabel(playerBoardLabel_, 160);
    GUIUtil::CenterLabel(enemyBoardLabel_, 480);
    GUIUtil::CenterLabel(menuLabel_, 576);

    menuButton_.reset(new TexturedButton());
    menuButton_->Initialize(536, 318, tileset,
        sf::IntRect(0, 368, 80, 24), sf::IntRect(217, 53, 80, 24),
        sf::IntRect(297, 53, 80, 24));
    menuButton_->OnClick([core, this] (TexturedButton &) {
        manager_->ClickSound().Play();
        core->SetNextState(MakeSharedState<BattleshipGameMenuState>());
    });

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BattleshipGameState::Release() {
    manager_ = nullptr;
    playerBoard_ = nullptr;
    enemyBoard_ = nullptr;
    playerPanelTexture_.reset(nullptr);
    infoPanelTexture_.reset(nullptr);
    playerBoardLabel_.reset(nullptr);
    enemyBoardLabel_.reset(nullptr);
    playerLabel_.reset(nullptr);
    turnLabel_.reset(nullptr);
    playerNameLabel_.reset(nullptr);
    playerTurnLabel_.reset(nullptr);
    waterLabel_.reset(nullptr);
    shipLabel_.reset(nullptr);
    hitLabel_.reset(nullptr);
    menuLabel_.reset(nullptr);
    menuButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipGameState::Draw(const sf::Time & timeDelta) {
    playerBoardLabel_->Draw();
    enemyBoardLabel_->Draw();

    playerBoard_->Draw();
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

    menuButton_->Draw();
    menuLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipGameState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager_->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BattleshipGameMenuState>());
    } else {
        enemyBoard_->Update();
        if (enemyBoard_->MouseOverBoard() && Mouse::IsButtonUp(Mouse::Left)) {
            enemyBoard_->Click();
        } else {
            menuButton_->Update();
        }
    }
}

//--------------------------------------------------------------------------------

void BattleshipGameState::updateHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };

    BattleshipManager::CurrentData currentData;
    manager_->GetCurrentData(currentData);

    playerNameLabel_->Text(currentData.name);
    playerTurnLabel_->Text(ToString(currentData.turn));
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipGameState::BattleshipGameState() : manager_(nullptr) {}

//--------------------------------------------------------------------------------

BattleshipGameState::~BattleshipGameState() {}
