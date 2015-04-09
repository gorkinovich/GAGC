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

#include "ExitDialogState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Menu/MenuManager.h>
#include <Menu/DesktopState.h>
#include <Menu/LogonState.h>

//********************************************************************************
// Static
//********************************************************************************

ExitDialogState::PreviousState ExitDialogState::previousState_ = ExitDialogState::LOGON_STATE;

//********************************************************************************
// Methods
//********************************************************************************

void ExitDialogState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum02);

    auto * menuCore = MenuManager::Instance();
    auto & tileset = menuCore->Tileset();

    panelPosition_ = sf::Vector2i(192, 96);
    panelBackground_.reset(new Texture2D());
    panelBackground_->Load(tileset, sf::IntRect(498, 82, 256, 168));

    messageLabel_.reset(new TextLabel(
        " Do you really want to exit\n"
        "this wonderful and marvelous\n"
        "     and magical and...\n\n"
        " Ok, I know... This app is\n"
        " awful and horrible, do you\n"
        "   want to exit now this\n"
        "         nightmare?",
        208, 108,
        AtariPalette::Hue00Lum14,
        core->Retro70Font()
    ));

    yesLabel_.reset(new SimpleLabel("Yes", 256, 240,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", 368, 240,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        messageLabel_->Text(
            " ¿Estás seguro realmente de\n"
            "   querer salir de este\n"
            "  fantástico y maravilloso\n"
            "      y mágico y...?\n\n"
            "Vale, lo sé... Este programa\n"
            "es atroz y horrible, ¿deseas\n"
            "salir ya de esta pesadilla?"
        );
        yesLabel_->Text("Sí");
        noLabel_->Text("No");
    }

    GUIUtil::CenterLabel(messageLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, 264);
    GUIUtil::CenterLabel(noLabel_, 376);

    yesButton_.reset(new TexturedButton());
    yesButton_->Initialize(216, 234, menuCore->Tileset(),
        sf::IntRect(754, 82, 96, 24), sf::IntRect(754, 154, 96, 24),
        sf::IntRect(754, 178, 96, 24));
    yesButton_->OnClick([core] (TexturedButton &) {
        core->Exit();
    });

    noButton_.reset(new TexturedButton());
    noButton_->Initialize(328, 234, menuCore->Tileset(),
        sf::IntRect(754, 82, 96, 24), sf::IntRect(754, 106, 96, 24),
        sf::IntRect(754, 130, 96, 24));
    noButton_->OnClick([core] (TexturedButton &) {
        if (ExitDialogState::previousState_ == ExitDialogState::LOGON_STATE) {
            core->SetNextState(MakeSharedState<LogonState>());
        } else if (ExitDialogState::previousState_ == ExitDialogState::DESKTOP_STATE) {
            core->SetNextState(MakeSharedState<DesktopState>());
        }
    });
}

//--------------------------------------------------------------------------------

void ExitDialogState::Release() {
    panelPosition_ = sf::Vector2i();
    panelBackground_.reset(nullptr);
    messageLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    yesButton_.reset(nullptr);
    noLabel_.reset(nullptr);
    noButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ExitDialogState::Draw(const sf::Time & timeDelta) {
    panelBackground_->Draw(panelPosition_);
    messageLabel_->Draw();
    yesButton_->Draw();
    yesLabel_->Draw();
    noButton_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void ExitDialogState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        if (ExitDialogState::previousState_ == ExitDialogState::LOGON_STATE) {
            core->SetNextState(MakeSharedState<LogonState>());
        } else if (ExitDialogState::previousState_ == ExitDialogState::DESKTOP_STATE) {
            core->SetNextState(MakeSharedState<DesktopState>());
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        core->Exit();

    } else {
        yesButton_->Update();
        noButton_->Update();
    }
}

//--------------------------------------------------------------------------------

void ExitDialogState::ChangeStateFrom(PreviousState state) {
    previousState_ = state;
    CoreManager::Instance()->SetNextState(MakeSharedState<ExitDialogState>());
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ExitDialogState::ExitDialogState() : panelPosition_(),
    panelBackground_(nullptr), messageLabel_(nullptr), yesLabel_(nullptr),
    yesButton_(nullptr), noLabel_(nullptr), noButton_(nullptr) {}

//--------------------------------------------------------------------------------

ExitDialogState::~ExitDialogState() {}
