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

#include "SnakeHelpState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  32
#define ROW1  56
#define ROW2  339

//********************************************************************************
// Methods
//********************************************************************************

void SnakeHelpState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    titleLabel_.reset(new SimpleLabel("Help", 0, ROW0,
        AtariPalette::Hue11Lum02, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "This game can be played with 2 types of controls:\n\n"
        "A) Control type standard:\n"
        "                  Player 1      Player 2\n"
        "   North:          Up              W\n"
        "   East:           Right           D\n"
        "   South:          Down            S\n"
        "   West:           Left            A\n\n"
        "B) Control type simple:\n"
        "                  Player 1      Player 2\n"
        "   Turn left:      Left            A\n"
        "   Turn right:     Right           D\n\n"
        "The objective of the game is to make grow your snake eating\n"
        "the white dots and avoiding to collide with your body and the\n"
        "walls. Sometimes a yellow dot appears a few seconds and if\n"
        "you get it you'll score double points than the white ones.",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue01Lum12, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Ayuda");
        helpText_->Text(
            "Este juego se puede jugar con 2 tipos de control:\n\n"
            "A) Control de tipo estándar:\n"
            "                      Jugador 1      Jugador 2\n"
            "   Norte:              Arriba           W\n"
            "   Este:               Derecha          D\n"
            "   Sur:                Abajo            S\n"
            "   Oeste:              Izquierda        A\n\n"
            "B) Control de tipo simple:\n"
            "                      Jugador 1      Jugador 2\n"
            "   Giro derecha:       Derecha          A\n"
            "   Giro izquierda:     Izquierda        D\n\n"
            "El objetivo del juego es hacer crecer a tu serpiente comiendo\n"
            "los puntos blancos y evitando chocar con tu propio cuerpo y\n"
            "las paredes del muro. A veces aparecerá un punto amarillo por\n"
            "unos pocos segundos y en caso de lograr cogerlo se obtendrá\n"
            "el doble de puntuación que por uno de los puntos blancos."
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void SnakeHelpState::Release() {
    titleLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeHelpState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the labels of the menu.
    titleLabel_->Draw();
    helpText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeHelpState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        SnakeManager::Instance()->MenuNextSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<SnakeMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeHelpState::SnakeHelpState() : titleLabel_(nullptr),
    helpText_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeHelpState::~SnakeHelpState() {}
