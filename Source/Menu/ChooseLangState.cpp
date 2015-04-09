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

#include "ChooseLangState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <Menu/MenuManager.h>
#include <Menu/LogonState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ChooseLangState::Initialize() {
    auto * core = CoreManager::Instance();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum02);

    auto * menuCore = MenuManager::Instance();
    menuCore->Initialize();

    spanishLabel_.reset(new SimpleLabel("Español", 186, 225,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    englishLabel_.reset(new SimpleLabel("English", 398, 225,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    auto selectedOption = [core] (TextLanguage value) {
        core->Language(value);
        core->SetNextState(MakeSharedState<LogonState>());
    };

    spanishButton_.reset(new TexturedButton());
    spanishButton_->Initialize(156, 141, menuCore->Tileset(),
        sf::IntRect(0, 0, 121, 82), sf::IntRect(121, 0, 121, 82),
        sf::IntRect(242, 0, 121, 82));
    spanishButton_->LinkedLabel(spanishLabel_.get());
    spanishButton_->OnClick([selectedOption] (TexturedButton &) {
        selectedOption(TEXT_LANGUAGE_SPANISH);
    });

    englishButton_.reset(new TexturedButton());
    englishButton_->Initialize(348, 141, menuCore->Tileset(),
        sf::IntRect(363, 0, 160, 82), sf::IntRect(523, 0, 160, 82),
        sf::IntRect(683, 0, 160, 82));
    englishButton_->LinkedLabel(englishLabel_.get());
    englishButton_->OnClick([selectedOption] (TexturedButton &) {
        selectedOption(TEXT_LANGUAGE_ENGLISH);
    });
}

//--------------------------------------------------------------------------------

void ChooseLangState::Release() {
    spanishButton_.reset(nullptr);
    spanishLabel_.reset(nullptr);
    englishButton_.reset(nullptr);
    englishLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ChooseLangState::Draw(const sf::Time & timeDelta) {
    spanishButton_->Draw();
    spanishLabel_->Draw();
    englishButton_->Draw();
    englishLabel_->Draw();
}

//--------------------------------------------------------------------------------

void ChooseLangState::Update(const sf::Time & timeDelta) {
    spanishButton_->Update();
    englishButton_->Update();
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ChooseLangState::ChooseLangState() : spanishButton_(nullptr),
    spanishLabel_(nullptr), englishButton_(nullptr), englishLabel_(nullptr) {}

//--------------------------------------------------------------------------------

ChooseLangState::~ChooseLangState() {}
