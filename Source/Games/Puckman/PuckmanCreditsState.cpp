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

#include "PuckmanCreditsState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanMainState.h>

using namespace Puckman;

//********************************************************************************
// Defines
//********************************************************************************

#define ROW1  60
#define ROW2  336
#define COL1  40

//********************************************************************************
// Methods
//********************************************************************************

void PuckmanCreditsState::Initialize() {
    PuckmanSharedState::Initialize();

    auto * core = CoreManager::Instance();
    auto language = core->Language();

    creditsText_.reset(new TextLabel(
    //---1234567890123456789012345678901234567890123456789012345678901234567890
        "Credits:\n\n"
        "   Programming by Gorka Suárez García.\n"
        "   Original game made by the awesome company Namco.\n\n"
        "   Press F5 to visit the wikipedia to learn more about this game."
        "\n\n\nHelp:\n\n"
        "   C     -> Insert coin\n"
        "   1     -> 1 player game\n"
        "   2     -> 2 players game\n"
        "   Up    -> Move up\n"
        "   Down  -> Move down\n"
        "   Left  -> Move left\n"
        "   Right -> Move right",
        COL1, ROW1, Puckman::Palette::White, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, Puckman::Palette::White, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        creditsText_->Text(
        //---1234567890123456789012345678901234567890123456789012345678901234567890
            "Créditos:\n\n"
            "   Programación por Gorka Suárez García.\n"
            "   Juego original creado por la increíble compañía Namco.\n\n"
            "   Pulsa F5 para saber más sobre este juego en la wikipedia."
            "\n\n\nAyuda:\n\n"
            "   C         -> Meter moneda\n"
            "   1         -> Juego de 1 jugador\n"
            "   2         -> Juego de 2 jugadores\n"
            "   Arriba    -> Mover hacia arriba\n"
            "   Abajo     -> Mover hacia abajo\n"
            "   Izquierda -> Mover hacia la izquierda\n"
            "   Derecha   -> Mover hacia la derecha"
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    upBarLabel_->Text(language == TEXT_LANGUAGE_ENGLISH ?
        "(ESC) Return   (F1) About the game    (F3) Reset     " :
        "(ESC) Volver   (F1) Sobre el juego    (F3) Reiniciar ");
}

//--------------------------------------------------------------------------------

void PuckmanCreditsState::Release() {
    PuckmanSharedState::Release();
    creditsText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PuckmanCreditsState::Draw(const sf::Time & timeDelta) {
    PuckmanSharedState::Draw(timeDelta);
    creditsText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PuckmanCreditsState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::F5)) {
        auto language = core->Language();
        if (language == TEXT_LANGUAGE_ENGLISH) {
            core->OpenURL("http://en.wikipedia.org/wiki/Pacman");
        } else {
            core->OpenURL("http://es.wikipedia.org/wiki/Pac-Man");
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        Manager::Instance()->FruitEat().Play();
        core->SetNextState(MakeSharedState<PuckmanMainState>());

    } else {
        PuckmanSharedState::Update(timeDelta);
        if (!updateOptionSelected_ && Keyboard::IsAnyKeyUp()) {
            Manager::Instance()->FruitEat().Play();
            core->SetNextState(MakeSharedState<PuckmanMainState>());
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PuckmanCreditsState::PuckmanCreditsState() {}

//--------------------------------------------------------------------------------

PuckmanCreditsState::~PuckmanCreditsState() {}
