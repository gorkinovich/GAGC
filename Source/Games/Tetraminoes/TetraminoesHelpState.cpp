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

#include "TetraminoesHelpState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  48
#define ROW1  84
#define ROW2  300

#define BLACK_COLOR   sf::Color::Black
#define WHITE_COLOR   sf::Color::White
#define YELLOW_COLOR  sf::Color::Yellow
#define GREEN_COLOR   sf::Color::Green

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesHelpState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->Menu2Background());

    titleLabel_.reset(new SimpleLabel("Help",
        0, ROW0, GREEN_COLOR, core->Retro70Font()));

    helpText_.reset(new TextLabel(
        "The controls of the game are:\n\n"
        "   Move Left:    Left\n"
        "   Move Right:   Right\n"
        "   Turn Left:    A & Space\n"
        "   Turn Right:   D & Up\n"
        "   Down:         Down\n"
        "   Fall:         X\n"
        "   Pause:        P",
        0, ROW1, WHITE_COLOR, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, YELLOW_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Ayuda");
        helpText_->Text(
            "Las teclas para poder jugar son:\n\n"
            " Mover Izquierda:    Izquierda\n"
            " Move Derecha:       Derecha\n"
            " Rotar Izquierda:    A y Espacio\n"
            " Rotar Derecha:      D y Arriba\n"
            " Bajar:              Abajo\n"
            " Soltar:             X\n"
            " Pausar:             P"
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void TetraminoesHelpState::Release() {
    backgroundTexture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    helpText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesHelpState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);
    titleLabel_->Draw();
    helpText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesHelpState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        TetraminoesManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<TetraminoesMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesHelpState::TetraminoesHelpState() {}

//--------------------------------------------------------------------------------

TetraminoesHelpState::~TetraminoesHelpState() {}
