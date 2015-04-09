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

#include "BattleshipHelpState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Battleship/BattleshipManager.h>
#include <Games/Battleship/BattleshipMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  24
#define ROW1  60
#define ROW2  336

//********************************************************************************
// Methods
//********************************************************************************

void BattleshipHelpState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    titleLabel_.reset(new SimpleLabel("Help", 0, ROW0,
        AtariPalette::Hue12Lum02, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "In this game each player has a fleet inside a 10x10 board. The goal is\n"
        "to destroy the enemy fleet before being destroyed. During the game at\n"
        "the left we'll have a board with our fleet and at the right the board\n"
        "where we'll be shooting to the enemy.\n\n"
        "Each fleet will have 1 ship of size 4, 2 of size 3, 3 of size 2 & 4 of\n"
        "size 1. They can be placed in horizontal or vertical, but cannot be\n"
        "placed in any cell immediate to another ship.\n\n"
        "In each player's turn, 1 shot can be made on a coordinates. The shoot\n"
        "can hit water, a part of a ship or sink a ship. The game ends when one\n"
        "of the players sinks all the enemy fleet.\n\n"
        "To learn more about this game press F1 to visit the wikipedia.",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Ayuda");
        helpText_->Text(
            "Durante el juego cada jugador tiene una flota que tendrá que colocar dentro\n"
            "de un tablero de 10x10. El objetivo es destruir a la flota enemiga antes de\n"
            "ser destruido. Durante el juego tendremos un tablero a la izquierda con\n"
            "nuestra flota y otro a la derecha donde efectuaremos los disparos contra el\n"
            "enemigo.\n\n"
            "Cada flota tendrá 1 barco de tamaño 4, 2 de tamaño 3, 3 de tamaño 2 y 4 de\n"
            "tamaño 1. Se podrán poner en horizontal o vertical cada uno, pero no se\n"
            "pueden poner en ninguna casilla contigua a otro barco.\n\n"
            "En cada turno el jugador podrá efectuar un disparo a unas coordenadas. Tras\n"
            "el disparo puede haber dado al agua, haber tocado un barco o haber hundido\n"
            "uno del todo. El juego finaliza cuando uno de los bandos logra hundir toda\n"
            "la flota enemiga.\n\n"
            "Para saber más sobre este juego pulsa F1 para visitar la wikipedia."
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BattleshipHelpState::Release() {
    titleLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BattleshipHelpState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    helpText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BattleshipHelpState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    if (Keyboard::IsKeyUp(Keyboard::F1)) {
        auto * core = CoreManager::Instance();
        auto language = core->Language();
        if (language == TEXT_LANGUAGE_ENGLISH) {
            core->OpenURL("http://en.wikipedia.org/wiki/Battleship_(game)");
        } else {
            core->OpenURL("http://es.wikipedia.org/wiki/Batalla_naval_(juego)");
        }
    } else if (Mouse::IsAnyButtonUp()) {
        BattleshipManager::Instance()->ClickSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BattleshipMenuState>());
    } else if (Keyboard::IsAnyKeyUp()) {
        BattleshipManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BattleshipMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BattleshipHelpState::BattleshipHelpState() {}

//--------------------------------------------------------------------------------

BattleshipHelpState::~BattleshipHelpState() {}
