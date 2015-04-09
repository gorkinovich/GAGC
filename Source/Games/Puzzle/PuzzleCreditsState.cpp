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

#include "PuzzleCreditsState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Puzzle/PuzzleManager.h>
#include <Games/Puzzle/PuzzleGameState.h>

//********************************************************************************
// Methods
//********************************************************************************

void PuzzleCreditsState::Initialize() {
    PuzzleSharedState::Initialize();

    auto * core = CoreManager::Instance();
    auto language = core->Language();

    auto * manager = PuzzleManager::Instance();
    auto & tileset = manager->Tileset();

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
        0, 60, AtariPalette::Hue00Lum12, core->Retro70Font()
    ));

    returnLabel_.reset(new SimpleLabel("Return", 0, 324,
        AtariPalette::Hue00Lum00, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
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
        returnLabel_->Text("Volver");
    }

    GUIUtil::CenterLabel(creditsText_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(returnLabel_, CoreManager::HALF_LOW_WIDTH);

    returnButton_.reset(new TexturedButton());
    returnButton_->Initialize(280, 318, tileset,
        sf::IntRect(176, 42, 80, 24), sf::IntRect(176, 66, 80, 24),
        sf::IntRect(176, 90, 80, 24));
    returnButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<PuzzleGameState>());
    });
}

//--------------------------------------------------------------------------------

void PuzzleCreditsState::Release() {
    PuzzleSharedState::Release();
    creditsText_.reset(nullptr);
    returnLabel_.reset(nullptr);
    returnButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PuzzleCreditsState::Draw(const sf::Time & timeDelta) {
    PuzzleSharedState::Draw(timeDelta);
    creditsText_->Draw();
    returnButton_->Draw();
    returnLabel_->Draw();
}

//--------------------------------------------------------------------------------

void PuzzleCreditsState::Update(const sf::Time & timeDelta) {
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
        } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            PuzzleManager::Instance()->KeyboardSound().Play();
            core->SetNextState(MakeSharedState<PuzzleGameState>());
        } else {
            PuzzleSharedState::Update(timeDelta);
            returnButton_->Update();
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
        } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            PuzzleManager::Instance()->KeyboardSound().Play();
            core->SetNextState(MakeSharedState<PuzzleGameState>());
        } else {
            PuzzleSharedState::Update(timeDelta);
            returnButton_->Update();
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PuzzleCreditsState::PuzzleCreditsState() {}

//--------------------------------------------------------------------------------

PuzzleCreditsState::~PuzzleCreditsState() {}
