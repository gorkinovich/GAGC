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

#include "LogonState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/MathUtil.h>
#include <System/GUIUtil.h>
#include <Menu/MenuManager.h>
#include <Menu/DesktopState.h>
#include <Menu/ExitDialogState.h>

//********************************************************************************
// Constants
//********************************************************************************

const float LogonState::MAX_RADIUS = 3.0f;

//********************************************************************************
// Methods
//********************************************************************************

void LogonState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum02);

    auto * menuCore = MenuManager::Instance();
    auto & tileset = menuCore->Tileset();

    menuCore->LoadGeneralPlaylist();

    eyesBackground_.reset(new Texture2D());
    eyesBackground_->Load(tileset, sf::IntRect(498, 250, 46, 15));
    eye_.reset(new Texture2D());
    eye_->Load(tileset, sf::IntRect(498, 265, 5, 5));

    eyesBackgroundDestination_ = sf::Vector2i(305, 173);
    leftEyeBase_ = sf::Vector2i(311, 182);
    rightEyeBase_ = sf::Vector2i(343, 174);
    leftEyeDestination_ = leftEyeBase_;
    rightEyeDestination_ = rightEyeBase_;

    welcomeLabel_.reset(new SimpleLabel("Welcome to GAGC OS", 240, 36,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    userLabel_.reset(new SimpleLabel("User", 304, 276,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    startSessionLabel_.reset(new SimpleLabel("Start Session", 272, 312,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 584, 336,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        welcomeLabel_->Text("Bienvenido a GAGC OS");
        userLabel_->Text("Usuario");
        startSessionLabel_->Text("Iniciar Sesión");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(welcomeLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(userLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(startSessionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, 600);

    auto onStartSessionClick = [core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<DesktopState>());
    };
    auto onExitClick = [] (TexturedButton &) {
        ExitDialogState::ChangeStateFrom(ExitDialogState::LOGON_STATE);
    };

    avatarButton_.reset(new TexturedButton());
    avatarButton_->Initialize(237, 97, menuCore->Tileset(),
        sf::IntRect(0, 82, 166, 166), sf::IntRect(166, 82, 166, 166),
        sf::IntRect(332, 82, 166, 166));
    avatarButton_->OnClick(onStartSessionClick);

    startSessionButton_.reset(new TexturedButton());
    startSessionButton_->Initialize(237, 306, menuCore->Tileset(),
        sf::IntRect(0, 248, 166, 24), sf::IntRect(166, 248, 166, 24),
        sf::IntRect(332, 248, 166, 24));
    startSessionButton_->OnClick(onStartSessionClick);

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(576, 288, menuCore->Tileset(),
        sf::IntRect(0, 272, 48, 48), sf::IntRect(48, 272, 48, 48),
        sf::IntRect(96, 272, 48, 48));
    exitButton_->LinkedLabel(exitLabel_.get());
    exitButton_->OnClick(onExitClick);
}

//--------------------------------------------------------------------------------

void LogonState::Release() {
    eyesBackground_.reset(nullptr);
    eye_.reset(nullptr);
    eyesBackgroundDestination_ = sf::Vector2i();
    leftEyeDestination_ = leftEyeBase_ = sf::Vector2i();
    rightEyeDestination_ = rightEyeBase_ = sf::Vector2i();
    welcomeLabel_.reset(nullptr);
    userLabel_.reset(nullptr);
    startSessionLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    avatarButton_.reset(nullptr);
    startSessionButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void LogonState::Draw(const sf::Time & timeDelta) {
    eyesBackground_->Draw(eyesBackgroundDestination_);
    eye_->Draw(leftEyeDestination_);
    eye_->Draw(rightEyeDestination_);

    welcomeLabel_->Draw();
    avatarButton_->Draw();
    userLabel_->Draw();
    startSessionButton_->Draw();
    startSessionLabel_->Draw();

    exitLabel_->Draw();
    exitButton_->Draw();
}

//--------------------------------------------------------------------------------

void LogonState::Update(const sf::Time & timeDelta) {
    avatarButton_->Update();
    startSessionButton_->Update();
    exitButton_->Update();

    auto & mousePosition = CoreManager::Instance()->GetMousePosition();
    sf::Vector2f leftDirection(mousePosition - leftEyeBase_);
    if (leftDirection == MathUtil::V2F_ZERO) {
        leftEyeDestination_ = leftEyeBase_;
    } else if (MathUtil::Length(leftDirection) < MAX_RADIUS) {
        leftEyeDestination_ = sf::Vector2i(
            static_cast<int>(leftEyeBase_.x + leftDirection.x),
            static_cast<int>(leftEyeBase_.y + leftDirection.y)
        );
    } else {
        MathUtil::Normalize(leftDirection);
        leftEyeDestination_ = sf::Vector2i(
            static_cast<int>(leftEyeBase_.x + MAX_RADIUS * leftDirection.x),
            static_cast<int>(leftEyeBase_.y + MAX_RADIUS * leftDirection.y)
        );
    }

    sf::Vector2f rightDirection(mousePosition - rightEyeBase_);
    if (rightDirection == MathUtil::V2F_ZERO) {
        rightEyeDestination_ = rightEyeBase_;
    } else if (MathUtil::Length(rightDirection) < MAX_RADIUS) {
        rightEyeDestination_ = sf::Vector2i(
            static_cast<int>(rightEyeBase_.x + rightDirection.x),
            static_cast<int>(rightEyeBase_.y + rightDirection.y)
        );
    } else {
        MathUtil::Normalize(rightDirection);
        rightEyeDestination_ = sf::Vector2i(
            static_cast<int>(rightEyeBase_.x + MAX_RADIUS * rightDirection.x),
            static_cast<int>(rightEyeBase_.y + MAX_RADIUS * rightDirection.y)
        );
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

LogonState::LogonState() : eyesBackground_(nullptr), eye_(nullptr),
    eyesBackgroundDestination_(), leftEyeBase_(), rightEyeBase_(),
    leftEyeDestination_(), rightEyeDestination_(), welcomeLabel_(nullptr),
    userLabel_(nullptr), startSessionLabel_(nullptr), exitLabel_(nullptr),
    avatarButton_(nullptr), startSessionButton_(nullptr), exitButton_(nullptr) {}

//--------------------------------------------------------------------------------

LogonState::~LogonState() {}
