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

#include "BlackjackCreditsState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <Games/Blackjack/BlackjackManager.h>
#include <Games/Blackjack/BlackjackMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  24
#define ROW1  60
#define ROW2  336
#define COL1  40

//********************************************************************************
// Methods
//********************************************************************************

void BlackjackCreditsState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue12Lum00);

    titleLabel_.reset(new SimpleLabel("Credits", 0, ROW0,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    creditsText_.reset(new TextLabel(
    //---1234567890123456789012345678901234567890123456789012345678901234567890
        "Programming and graphic design by Gorka Suárez García.\n\n"
        "Sound effects made with the great tool sfxr created by Tomas\n"
        "Pettersson. URL: http://www.drpetter.se/project_sfxr.html \n\n"
        "Music made by Kevin MacLeod (incompetech.com).\n"
        "(List of songs inside the \"About\" section of the main menu.)\n\n"
        "If you want to contact with me you do it by:\n"
        "e-mail:   gorkasg@yahoo.es\n"
        "LinkedIn: http://www.linkedin.com/in/gorkasg/en \n"
        "Twitter:  https://twitter.com/gorkinovich/ \n\n"
        "Thanks for playing! ^_^\n\n\n"
        "Press to visit: [F1] LinkedIn, [F2] Twitter\n"
        "                [F5] sfxr web, [F6] Kevin MacLeod web",
        COL1, ROW1, AtariPalette::Hue00Lum00, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue00Lum06, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Créditos");
        creditsText_->Text(
        //---1234567890123456789012345678901234567890123456789012345678901234567890
            "Programación y diseño gráfico por Gorka Suárez García.\n\n"
            "Efectos de sonido creados con la gran herramienta sfxr creada por\n"
            "Tomas Pettersson. URL: http://www.drpetter.se/project_sfxr.html \n\n"
            "Música creada por Kevin MacLeod (incompetech.com).\n"
            "(Lista de canciones en la sección \"Sobre\" del menú principal.)\n\n"
            "Si deseas contactar conmigo puedes hacerlo a través de:\n"
            "e-mail:   gorkasg@yahoo.es\n"
            "LinkedIn: http://www.linkedin.com/in/gorkasg/es \n"
            "Facebook: https://www.facebook.com/Gorkinovich \n"
            "Twitter:  https://twitter.com/gorkinovich/ \n\n"
            "¡Gracias por jugar! ^_^\n\n\n"
            "Pulsa para visitar: [F1] LinkedIn, [F2] Facebook, [F3] Twitter\n"
            "                    [F5] sfxr, [F6] Kevin MacLeod"
        );
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);

    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BlackjackCreditsState::Release() {
    titleLabel_.reset(nullptr);
    creditsText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BlackjackCreditsState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();
    creditsText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BlackjackCreditsState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    if (language == TEXT_LANGUAGE_ENGLISH) {
        if (Keyboard::IsKeyUp(Keyboard::F1)) {
            core->OpenURL("http://www.linkedin.com/in/gorkasg/en");
        } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
            core->OpenURL("https://twitter.com/gorkinovich/");
        } else if (Keyboard::IsKeyUp(Keyboard::F5)) {
            core->OpenURL("http://www.drpetter.se/project_sfxr.html");
        } else if (Keyboard::IsKeyUp(Keyboard::F6)) {
            core->OpenURL("http://incompetech.com");
        } else if (Mouse::IsAnyButtonUp()) {
            BlackjackManager::Instance()->ClickSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackMenuState>());
        } else if (Keyboard::IsAnyKeyUp()) {
            BlackjackManager::Instance()->KeyboardSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackMenuState>());
        }
    } else {
        if (Keyboard::IsKeyUp(Keyboard::F1)) {
            core->OpenURL("http://www.linkedin.com/in/gorkasg/es");
        } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
            core->OpenURL("https://www.facebook.com/Gorkinovich");
        } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
            core->OpenURL("https://twitter.com/gorkinovich/");
        } else if (Keyboard::IsKeyUp(Keyboard::F5)) {
            core->OpenURL("http://www.drpetter.se/project_sfxr.html");
        } else if (Keyboard::IsKeyUp(Keyboard::F6)) {
            core->OpenURL("http://incompetech.com");
        } else if (Mouse::IsAnyButtonUp()) {
            BlackjackManager::Instance()->ClickSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackMenuState>());
        } else if (Keyboard::IsAnyKeyUp()) {
            BlackjackManager::Instance()->KeyboardSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackMenuState>());
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BlackjackCreditsState::BlackjackCreditsState() {}

//--------------------------------------------------------------------------------

BlackjackCreditsState::~BlackjackCreditsState() {}
