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

#include "BattleshipPutShipsState.h"
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
#include <System/MathUtil.h>
#include <Games/Battleship/BattleshipBoard.h>
#include <Games/Battleship/BattleshipManager.h>

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipPutShipsState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = BattleshipManager::Instance();
    auto & tileset = manager->Tileset();
    auto & name = manager->GetPlayerName();

    board_.reset(new BattleshipBoard());
    board_->Initialize();
    board_->SetModePutFleet(
        [this] () {
            updateHud();
        },
        [this, manager] () {
            manager->AddPlayerBoard(*board_);
        }
    );

    ship1Area_ = sf::IntRect(504, 108, 24, 48);
    ship1Texture_.reset(new Texture2D());
    ship1Texture_->Load(tileset, sf::IntRect(121, 224, 24, 24));

    ship2Area_ = sf::IntRect(456, 84, 24, 72);
    ship2Texture_.reset(new Texture2D());
    ship2Texture_->Load(tileset, sf::IntRect(145, 224, 24, 48));

    ship3Area_ = sf::IntRect(408, 60, 24, 96);
    ship3Texture_.reset(new Texture2D());
    ship3Texture_->Load(tileset, sf::IntRect(169, 224, 24, 72));

    ship4Area_ = sf::IntRect(360, 36, 24, 120);
    ship4Texture_.reset(new Texture2D());
    ship4Texture_->Load(tileset, sf::IntRect(193, 224, 24, 96));

    titleLabel_.reset(new SimpleLabel(name + "'s fleet", 0, 4,
        AtariPalette::Hue01Lum14, core->Retro70Font()));

    ship1CountLabel_.reset(new SimpleLabel("4", 512, 138,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    ship2CountLabel_.reset(new SimpleLabel("3", 464, 138,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    ship3CountLabel_.reset(new SimpleLabel("2", 416, 138,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    ship4CountLabel_.reset(new SimpleLabel("1", 368, 138,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    automaticLabel_.reset(new SimpleLabel("Automatic", 0, 324,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    resetLabel_.reset(new SimpleLabel("Reset", 0, 324,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "Click on the ships to select the\n"
        "type. Click on the board to place\n"
        "the selected type. To rotate the\n"
        "ship press the right button.\n\n"
        "Use the automatic button to place\n"
        "ramdonly all the ships. Use the\n"
        "reset button to clear the board.",
        360, 168, AtariPalette::Hue00Lum14, core->Retro70Font()
    ));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("La flota de " + name);
        automaticLabel_->Text("Automático");
        resetLabel_->Text("Resetear");
        helpText_->Text(
            "Haz click sobre el tipo de barco\n"
            "y colócalo sobre el tablero con\n"
            "otro click. Para rotar el barco\n"
            "usa el botón derecho del ratón\n\n"
            "Con el botón automático se ponen\n"
            "de forma aleatoria la flota. Con\n"
            "resetear se limpia el tablero."
        );
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(automaticLabel_, 186);
    GUIUtil::CenterLabel(resetLabel_, 454);

    automaticButton_.reset(new TexturedButton());
    automaticButton_->Initialize(106, 318, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(160, 392, 160, 24),
        sf::IntRect(320, 392, 160, 24));
    automaticButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        board_->SetRandomFleet();
    });

    resetButton_.reset(new TexturedButton());
    resetButton_->Initialize(374, 318, tileset,
        sf::IntRect(0, 440, 160, 24), sf::IntRect(160, 416, 160, 24),
        sf::IntRect(320, 416, 160, 24));
    resetButton_->OnClick([this, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        board_->Reset();
    });

    updateHud();
    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BattleshipPutShipsState::Release() {
    board_.reset(nullptr);
    ship1Texture_.reset(nullptr);
    ship2Texture_.reset(nullptr);
    ship3Texture_.reset(nullptr);
    ship4Texture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    ship1CountLabel_.reset(nullptr);
    ship2CountLabel_.reset(nullptr);
    ship3CountLabel_.reset(nullptr);
    ship4CountLabel_.reset(nullptr);
    automaticLabel_.reset(nullptr);
    resetLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    automaticButton_.reset(nullptr);
    resetButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipPutShipsState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();

    ship4Texture_->Draw(ship4Area_.left, ship4Area_.top);
    ship4CountLabel_->Draw();

    ship3Texture_->Draw(ship3Area_.left, ship3Area_.top);
    ship3CountLabel_->Draw();

    ship2Texture_->Draw(ship2Area_.left, ship2Area_.top);
    ship2CountLabel_->Draw();

    ship1Texture_->Draw(ship1Area_.left, ship1Area_.top);
    ship1CountLabel_->Draw();

    helpText_->Draw();

    automaticButton_->Draw();
    automaticLabel_->Draw();

    resetButton_->Draw();
    resetLabel_->Draw();

    board_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipPutShipsState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    auto * manager = BattleshipManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::A)) {
        manager->KeyboardSound().Play();
        board_->SetRandomFleet();

    } else if (Keyboard::IsKeyUp(Keyboard::R)) {
        manager->KeyboardSound().Play();
        board_->Reset();

    } else if (Keyboard::IsKeyUp(Keyboard::Num1)) {
        manager->KeyboardSound().Play();
        board_->ChangeShipType(BattleshipBoard::SHIP_TYPE1);

    } else if (Keyboard::IsKeyUp(Keyboard::Num2)) {
        manager->KeyboardSound().Play();
        board_->ChangeShipType(BattleshipBoard::SHIP_TYPE2);

    } else if (Keyboard::IsKeyUp(Keyboard::Num3)) {
        manager->KeyboardSound().Play();
        board_->ChangeShipType(BattleshipBoard::SHIP_TYPE3);

    } else if (Keyboard::IsKeyUp(Keyboard::Num4)) {
        manager->KeyboardSound().Play();
        board_->ChangeShipType(BattleshipBoard::SHIP_TYPE4);

    } else {
        auto leftUp = Mouse::IsButtonUp(Mouse::Left);
        auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
        board_->Update();

        if (board_->MouseOverBoard() && leftUp) {
            board_->Click();

        } else if (MathUtil::PointInside(mouseCoords, ship1Area_) && leftUp) {
            manager->ClickSound().Play();
            board_->ChangeShipType(BattleshipBoard::SHIP_TYPE1);

        } else if (MathUtil::PointInside(mouseCoords, ship2Area_) && leftUp) {
            manager->ClickSound().Play();
            board_->ChangeShipType(BattleshipBoard::SHIP_TYPE2);

        } else if (MathUtil::PointInside(mouseCoords, ship3Area_) && leftUp) {
            manager->ClickSound().Play();
            board_->ChangeShipType(BattleshipBoard::SHIP_TYPE3);

        } else if (MathUtil::PointInside(mouseCoords, ship4Area_) && leftUp) {
            manager->ClickSound().Play();
            board_->ChangeShipType(BattleshipBoard::SHIP_TYPE4);

        } else if (Mouse::IsButtonUp(Mouse::Right)) {
            board_->NextShipDirection();

        } else {
            automaticButton_->Update();
            resetButton_->Update();
        }
    }
}

//--------------------------------------------------------------------------------

void BattleshipPutShipsState::updateHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };
    ship1CountLabel_->Text(ToString(board_->ShipTypeCount(BattleshipBoard::SHIP_TYPE1)));
    ship2CountLabel_->Text(ToString(board_->ShipTypeCount(BattleshipBoard::SHIP_TYPE2)));
    ship3CountLabel_->Text(ToString(board_->ShipTypeCount(BattleshipBoard::SHIP_TYPE3)));
    ship4CountLabel_->Text(ToString(board_->ShipTypeCount(BattleshipBoard::SHIP_TYPE4)));
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipPutShipsState::BattleshipPutShipsState() {}

//--------------------------------------------------------------------------------

BattleshipPutShipsState::~BattleshipPutShipsState() {}
