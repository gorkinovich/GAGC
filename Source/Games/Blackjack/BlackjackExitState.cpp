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

#include "BlackjackExitState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Blackjack/BlackjackManager.h>
#include <Games/Blackjack/BlackjackMenuState.h>

//********************************************************************************
// Methods
//********************************************************************************

void BlackjackExitState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue12Lum00);

    auto * manager = BlackjackManager::Instance();
    auto & tileset = manager->Tileset();

    titlePosition_ = sf::Vector2i(107, 69);
    titleTexture_.reset(new Texture2D());
    titleTexture_->Load(tileset, sf::IntRect(0, 470, 426, 42));

    spadePosition_ = sf::Vector2i(19, 77);
    spadeTexture_.reset(new Texture2D());
    spadeTexture_->Load(tileset, sf::IntRect(0, 330, 25, 26));

    heartPosition_ = sf::Vector2i(63, 80);
    heartTexture_.reset(new Texture2D());
    heartTexture_->Load(tileset, sf::IntRect(0, 356, 25, 23));

    clubPosition_ = sf::Vector2i(551, 79);
    clubTexture_.reset(new Texture2D());
    clubTexture_->Load(tileset, sf::IntRect(0, 379, 27, 24));

    diamondPosition_ = sf::Vector2i(601, 77);
    diamondTexture_.reset(new Texture2D());
    diamondTexture_->Load(tileset, sf::IntRect(0, 403, 15, 26));

    questionLabel_.reset(new SimpleLabel("Do you really want to exit?",
        0, 186, AtariPalette::Hue00Lum00, core->Retro70Font()));

    yesLabel_.reset(new SimpleLabel("Yes", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    noLabel_.reset(new SimpleLabel("No", 0, 234,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        questionLabel_->Text("¿Deseas realmente salir?");
        yesLabel_->Text("Sí");
        noLabel_->Text("No");
    }

    GUIUtil::CenterLabel(questionLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(yesLabel_, 200);
    GUIUtil::CenterLabel(noLabel_, 440);

    yesButton_.reset(new TexturedButton());
    yesButton_->Initialize(160, 228, tileset,
        sf::IntRect(32, 336, 80, 24), sf::IntRect(112, 360, 80, 24),
        sf::IntRect(112, 384, 80, 24));
    yesButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        manager->Release();
    });

    noButton_.reset(new TexturedButton());
    noButton_->Initialize(400, 228, tileset,
        sf::IntRect(32, 336, 80, 24), sf::IntRect(32, 360, 80, 24),
        sf::IntRect(32, 384, 80, 24));
    noButton_->OnClick([core, manager] (TexturedButton &) {
        manager->ClickSound().Play();
        core->SetNextState(MakeSharedState<BlackjackMenuState>());
    });
}

//--------------------------------------------------------------------------------

void BlackjackExitState::Release() {
    titleTexture_.reset(nullptr);
    spadeTexture_.reset(nullptr);
    heartTexture_.reset(nullptr);
    clubTexture_.reset(nullptr);
    diamondTexture_.reset(nullptr);
    questionLabel_.reset(nullptr);
    yesLabel_.reset(nullptr);
    noLabel_.reset(nullptr);
    yesButton_.reset(nullptr);
    noButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void BlackjackExitState::Draw(const sf::Time & timeDelta) {
    titleTexture_->Draw(titlePosition_);
    spadeTexture_->Draw(spadePosition_);
    heartTexture_->Draw(heartPosition_);
    clubTexture_->Draw(clubPosition_);
    diamondTexture_->Draw(diamondPosition_);

    yesButton_->Draw();
    noButton_->Draw();

    questionLabel_->Draw();
    yesLabel_->Draw();
    noLabel_->Draw();
}

//--------------------------------------------------------------------------------

void BlackjackExitState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = BlackjackManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape) || Keyboard::IsKeyUp(Keyboard::N)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<BlackjackMenuState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Y) || Keyboard::IsKeyUp(Keyboard::S)) {
        manager->KeyboardSound().Play();
        manager->Release();

    } else {
        yesButton_->Update();
        noButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BlackjackExitState::BlackjackExitState() {}

//--------------------------------------------------------------------------------

BlackjackExitState::~BlackjackExitState() {}
