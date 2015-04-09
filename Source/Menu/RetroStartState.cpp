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

#include "RetroStartState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/TextConsole.h>
#include <Menu/ChooseLangState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int RetroStartState::BGR_POS_X = 159;
const int RetroStartState::BGR_POS_Y = 23;
const int RetroStartState::BGR_WIDTH = 322;
const int RetroStartState::BGR_HEIGHT = 302;

//********************************************************************************
// Methods
//********************************************************************************

void RetroStartState::Initialize() {
    auto * core = CoreManager::Instance();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue07Lum12);

    console_ = TextConsole::Instance();
    console_->Position(sf::Vector2i(160, 24));
    console_->Opaque(false);
    console_->ForegroundColor(AtariPalette::Hue07Lum12);
    console_->SetCurrentFont(core->Retro70Font());
    console_->ClearColor(AtariPalette::Hue08Lum04);
    console_->ChangeSize(40, 25);

    timer_.SetAndEnable(1000, [this, core](Timer &) {
        if (step_ < actionsData_.size()) {
            ActionStepData data;
            do {
                data = actionsData_[step_++];
                console_->DirectWrite(data.Row, data.Column, data.TextLine);
                if (data.NextInterval > 0) {
                    timer_.Interval(data.NextInterval);
                    timer_.ResetTime();
                }
            } while (data.NextInterval == 0);
        } else {
            timer_.Enable(false);
            core->SetNextState(MakeSharedState<ChooseLangState>());
        }
    });

    background_.Load(BGR_WIDTH, BGR_HEIGHT, AtariPalette::Hue08Lum04);
    step_ = 0;
    actionsData_.clear();

    actionsData_.push_back(ActionStepData(1, 4, 100, "**** GORKINOVICH 64 BASIC V2 ****"));
    actionsData_.push_back(ActionStepData(3, 1, 100, "64K RAM SYSTEM  38911 BASIC BYTES FREE"));
    actionsData_.push_back(ActionStepData(5, 0, 100, "READY."));

    actionsData_.push_back(ActionStepData(6, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 400, " "));
    actionsData_.push_back(ActionStepData(6, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 400, " "));
    actionsData_.push_back(ActionStepData(6, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 400, " "));

    actionsData_.push_back(ActionStepData(6, 0, 400, "L\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LO\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOA"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"G\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GA"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAG"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC \x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC O"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC OS"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC OS\"\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC OS\",\x99"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC OS\",8"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC OS\",8,"));
    actionsData_.push_back(ActionStepData(6, 0, 200, "LOAD\"GAGC OS\",8,1\x99"));
    actionsData_.push_back(ActionStepData(6, 0,   0, "LOAD\"GAGC OS\",8,1 "));
    actionsData_.push_back(ActionStepData(7, 0, 200, "\x99"));

    actionsData_.push_back(ActionStepData( 7, 0,  400, " "));
    actionsData_.push_back(ActionStepData( 8, 0, 1000, "SEARCHING FOR GAGC OS"));
    actionsData_.push_back(ActionStepData( 9, 0, 1000, "LOADING"));
    actionsData_.push_back(ActionStepData(10, 0,  400, "READY."));

    actionsData_.push_back(ActionStepData(11, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(11, 0, 400, " "));
    actionsData_.push_back(ActionStepData(11, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(11, 0, 400, " "));
    actionsData_.push_back(ActionStepData(11, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(11, 0, 400, " "));

    actionsData_.push_back(ActionStepData(11, 0, 400, "R\x99"));
    actionsData_.push_back(ActionStepData(11, 0, 200, "RU\x99"));
    actionsData_.push_back(ActionStepData(11, 0, 200, "RUN"));

    actionsData_.push_back(ActionStepData(12, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(12, 0, 400, " "));
    actionsData_.push_back(ActionStepData(12, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(12, 0, 400, " "));
    actionsData_.push_back(ActionStepData(12, 0, 400, "\x99"));
    actionsData_.push_back(ActionStepData(12, 0, 800, " "));
}

//--------------------------------------------------------------------------------

void RetroStartState::Release() {
}

//--------------------------------------------------------------------------------

void RetroStartState::Draw(const sf::Time & timeDelta) {
    background_.Draw(BGR_POS_X, BGR_POS_Y);
    console_->Draw();
}

//--------------------------------------------------------------------------------

void RetroStartState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    if (core->IsKeyPressed(sf::Keyboard::Escape)) {
        core->SetNextState(MakeSharedState<ChooseLangState>());
    } else {
        timer_.Update(timeDelta);
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

RetroStartState::RetroStartState() : console_(nullptr), timer_(),
    background_(), step_(0), actionsData_() {}

//--------------------------------------------------------------------------------

RetroStartState::~RetroStartState() {}
