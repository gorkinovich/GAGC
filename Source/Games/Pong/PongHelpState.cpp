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

#include "PongHelpState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Pong/PongManager.h>
#include <Games/Pong/PongMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  24
#define ROW1  60
#define ROW2  336

//********************************************************************************
// Methods
//********************************************************************************

void PongHelpState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    titleLabel_.reset(new SimpleLabel("Help", 0, ROW0,
        AtariPalette::Hue12Lum02, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "The rules of this game are simple. There\n"
        "are two players and one ball, used to play\n"
        "some points. Those points are scored if the\n"
        "opponent can't return the ball. It's similar\n"
        "to the ping-pong game.\n\n"
        "The controls of each player are:\n\n"
        "             Player 1   Player 2\n"
        "   Up:          W         Up\n"
        "   Down:        S        Down",
        0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Ayuda");
        helpText_->Text(
            "Las reglas del juego son sencillas. Hay\n"
            "dos jugadores y una bola, con ella se\n"
            "juegan una serie de puntos. Estos se\n"
            "ganan cuando el contrincante no logra\n"
            "devolver la bola. Parecido a lo que\n"
            "vendría a ser el ping-pong.\n\n"
            "Las teclas para cada jugador son:\n\n"
            "             Jugador 1   Jugador 2\n"
            "   Arriba:       W        Arriba\n"
            "   Abajo:        S        Abajo"
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void PongHelpState::Release() {
    titleLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PongHelpState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    helpText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PongHelpState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        PongManager::Instance()->MenuNextSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<PongMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PongHelpState::PongHelpState() : titleLabel_(nullptr),
    helpText_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

PongHelpState::~PongHelpState() {}
