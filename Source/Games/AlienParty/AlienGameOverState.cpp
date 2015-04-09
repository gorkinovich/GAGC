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

#include "AlienGameOverState.h"
#include <sstream>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0   114
#define ROW1A  150
#define ROW1B  162
#define ROW1C  174
#define ROW1D  186
#define ROW2   336

//********************************************************************************
// Methods
//********************************************************************************

void AlienGameOverState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };

    auto * manager = AlienManager::Instance();
    auto deathCount = ToString(manager->DeathCount());
    std::string line1, line2, line3; int x2, x3;
    if (manager->DeathCount() < 1) {
        if (language == TEXT_LANGUAGE_ENGLISH) {
            line1 = "No victims to regret, but you...";
            line2 = "Quite an experience to live in fear, isn't it?";
            line3 = "That's what it is to be a slave.";
        } else {
            line1 = "No ha habido víctimas que lamentar, excepto tú...";
            line2 = "Es toda una experiencia vivir con miedo, ¿verdad?";
            line3 = "Eso es lo que significa ser esclavo.";
        }
        x2 = ROW1C, x3 = ROW1D;
    } else if (manager->DeathCount() == 1) {
        if (language == TEXT_LANGUAGE_ENGLISH) {
            line1 = "A family won't see again his son, husband or father...";
            line2 = "Proud of yourself, little man?";
            line3 = "";
        } else {
            line1 = "Una familia no vera de nuevo a su hijo, esposo or padre...";
            line2 = "¿Estás orgulloso de ti, hombrecito?";
            line3 = "";
        }
        x2 = ROW1C, x3 = ROW1D;
    } else {
        if (language == TEXT_LANGUAGE_ENGLISH) {
            line1 = deathCount + " families won't see again their sons,";
            line2 = "husbands or fathers...";
            line3 = "Proud of yourself, little man?";
        } else {
            line1 = deathCount + " familias no veran de nuevo a sus hijos,";
            line2 = "esposos or padres...";
            line3 = "¿Estás orgulloso de ti, hombrecito?";
        }
        x2 = ROW1B, x3 = ROW1D;
    }

    titleLabel_.reset(new SimpleLabel("War is no game...", 0, ROW0,
        AtariPalette::Hue04Lum02, core->Retro70Font()));

    message1Label_.reset(new SimpleLabel(line1, 0, ROW1A,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    message2Label_.reset(new SimpleLabel(line2, 0, x2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    message3Label_.reset(new SimpleLabel(line3, 0, x3,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press any key to continue...",
        0, ROW2, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("La guerra no es un juego...");
        exitLabel_->Text("Presione cualquier tecla para continuar...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(message1Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(message2Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(message3Label_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void AlienGameOverState::Release() {
    titleLabel_.reset(nullptr);
    message1Label_.reset(nullptr);
    message2Label_.reset(nullptr);
    message3Label_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void AlienGameOverState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    message1Label_->Draw();
    message2Label_->Draw();
    message3Label_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void AlienGameOverState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        AlienManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<AlienMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

AlienGameOverState::AlienGameOverState() {}

//--------------------------------------------------------------------------------

AlienGameOverState::~AlienGameOverState() {}
