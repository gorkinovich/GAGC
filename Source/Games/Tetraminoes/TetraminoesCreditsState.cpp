/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#include "TetraminoesCreditsState.h"
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
#define ROW1  72
#define ROW2  300
#define COL1  80
#define COL2  64

#define BLACK_COLOR   sf::Color::Black
#define WHITE_COLOR   sf::Color::White
#define YELLOW_COLOR  sf::Color::Yellow
#define BLUE_COLOR    sf::Color::Blue

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesCreditsState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->Menu2Background());

    titleLabel_.reset(new SimpleLabel("Credits",
        0, ROW0, BLUE_COLOR, core->Retro70Font()));

    creditsText_.reset(new TextLabel(
    //---1234567890123456789012345678901234567890123456789012345678901234567890
        "Programming and graphic design by Gorka Su�rez Garc�a.\n"
        "St Basils cathedral photo by David Crawshaw.\n\n"
        "Sound effects made with the great tool sfxr created by Tomas\n"
        "Pettersson. URL: http://www.drpetter.se/project_sfxr.html \n\n"
        "Dance of the Sugar Plum Fairies, Kevin MacLeod (incompetech.com)\n"
        "Licensed under Creative Commons: By Attribution 3.0\n\n"
        "If you want to contact with me you do it by:\n"
        "e-mail:   gorkasg@yahoo.es\n"
        "LinkedIn: http://www.linkedin.com/in/gorkasg/en \n"
        "Twitter:  https://twitter.com/gorkinovich/ \n\n"
        "Thanks for playing! ^_^\n\n"
        "Press to visit: [F1] LinkedIn, [F2] Twitter\n"
        "                [F5] sfxr web, [F6] Kevin MacLeod web",
        COL1, ROW1, WHITE_COLOR, core->Retro70Font()
    ));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, YELLOW_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Cr�ditos");
        creditsText_->Text(
        //---1234567890123456789012345678901234567890123456789012345678901234567890
            "Programaci�n y dise�o gr�fico por Gorka Su�rez Garc�a.\n"
            "Foto de la catedral de San Basilio por David Crawshaw.\n\n"
            "Efectos de sonido creados con la gran herramienta sfxr creada por\n"
            "Tomas Pettersson. URL: http://www.drpetter.se/project_sfxr.html \n\n"
            "Dance of the Sugar Plum Fairies, Kevin MacLeod (incompetech.com)\n"
            "Licenciada bajo Creative Commons: By Attribution 3.0\n\n"
            "Puedes contactar conmigo por e-mail: gorkasg@yahoo.es, o por:\n"
            "LinkedIn: http://www.linkedin.com/in/gorkasg/es \n"
            "Facebook: https://www.facebook.com/Gorkinovich \n"
            "Twitter:  https://twitter.com/gorkinovich/ \n\n"
            "�Gracias por jugar! ^_^\n\n"
            "Pulsa para visitar: [F1] LinkedIn, [F2] Facebook, [F3] Twitter\n"
            "                    [F5] sfxr, [F6] Kevin MacLeod"
        );
        exitLabel_->Text("Presione cualquier tecla para volver al men�...");
        creditsText_->Position(sf::Vector2i(COL2, ROW1));
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void TetraminoesCreditsState::Release() {
    backgroundTexture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    creditsText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesCreditsState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);
    titleLabel_->Draw();
    creditsText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesCreditsState::Update(const sf::Time & timeDelta) {
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
        } else if (Keyboard::IsAnyKeyUp()) {
            TetraminoesManager::Instance()->KeyboardSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<TetraminoesMenuState>());
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
        } else if (Keyboard::IsAnyKeyUp()) {
            TetraminoesManager::Instance()->KeyboardSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<TetraminoesMenuState>());
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesCreditsState::TetraminoesCreditsState() {}

//--------------------------------------------------------------------------------

TetraminoesCreditsState::~TetraminoesCreditsState() {}
