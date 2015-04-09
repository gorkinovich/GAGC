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

#include "SnakeMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeNewGameState.h>
#include <Games/Snake/SnakeRankingState.h>
#include <Games/Snake/SnakeHelpState.h>
#include <Games/Snake/SnakeCreditsState.h>
#include <Games/Snake/SnakeExitState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0            80
#define ROW1            116
#define ROW2            140
#define ROW3            164
#define ROW4            188
#define ROW5            212

#define REC_WIDTH       160
#define REC_HEIGHT      20
#define REC_ROW1        112
#define REC_ROW2        136
#define REC_ROW3        160
#define REC_ROW4        184
#define REC_ROW5        208
#define REC_COL         240

#define NEWGAME_OPTION  0
#define RANKING_OPTION  1
#define HELP_OPTION     2
#define CREDITS_OPTION  3
#define EXIT_OPTION     4

#define MAX_OPTIONS     5

//********************************************************************************
// Methods
//********************************************************************************

void SnakeMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = SnakeManager::Instance();
    manager->Initialize();

    selectedOption_ = -1;

    rectangleTexture_.reset(new Texture2D());
    rectangleTexture_->Load(REC_WIDTH, REC_HEIGHT);

    titleLabel_.reset(new SimpleLabel("Snake", 0, ROW0,
        AtariPalette::Hue11Lum02, core->Retro70Font()));

    newGameLabel_.reset(new SimpleLabel("New Game (F1)", 0, ROW1,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    rankingLabel_.reset(new SimpleLabel("Ranking (F2)", 0, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help (F3)", 0, ROW3,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits (F4)", 0, ROW4,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit (ESC)", 0, ROW5,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        newGameLabel_->Text("Nueva Partida (F1)");
        rankingLabel_->Text("Puntuaciones (F2)");
        helpLabel_->Text("Ayuda (F3)");
        creditsLabel_->Text("Créditos (F4)");
        exitLabel_->Text("Salir (ESC)");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(newGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(rankingLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(creditsLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void SnakeMenuState::Release() {
    rectangleTexture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    newGameLabel_.reset(nullptr);
    rankingLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeMenuState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the selector of the menu.
    sf::Color rectangleColor =
        Keyboard::isKeyPressed(Keyboard::Return) ||
        Keyboard::isKeyPressed(Keyboard::Space) ?
        AtariPalette::Hue00Lum08 : AtariPalette::Hue00Lum04;
    switch (selectedOption_) {
    case NEWGAME_OPTION: rectangleTexture_->Draw(REC_COL, REC_ROW1, rectangleColor); break;
    case RANKING_OPTION: rectangleTexture_->Draw(REC_COL, REC_ROW2, rectangleColor); break;
    case HELP_OPTION:    rectangleTexture_->Draw(REC_COL, REC_ROW3, rectangleColor); break;
    case CREDITS_OPTION: rectangleTexture_->Draw(REC_COL, REC_ROW4, rectangleColor); break;
    case EXIT_OPTION:    rectangleTexture_->Draw(REC_COL, REC_ROW5, rectangleColor); break;
    }
    // Draw the labels of the menu.
    titleLabel_->Draw();
    newGameLabel_->Draw();
    rankingLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = SnakeManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->MenuKeySound().Play();
        switch (selectedOption_) {
        case NEWGAME_OPTION: core->SetNextState(MakeSharedState<SnakeNewGameState>()); break;
        case RANKING_OPTION: core->SetNextState(MakeSharedState<SnakeRankingState>()); break;
        case HELP_OPTION:    core->SetNextState(MakeSharedState<SnakeHelpState>());    break;
        case CREDITS_OPTION: core->SetNextState(MakeSharedState<SnakeCreditsState>()); break;
        case EXIT_OPTION:    core->SetNextState(MakeSharedState<SnakeExitState>());    break;
        }

    } else if (Keyboard::IsKeyUp(Keyboard::F1)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeNewGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeRankingState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F4)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->MenuKeySound().Play();
        core->SetNextState(MakeSharedState<SnakeExitState>());

    } else {
        if (Keyboard::IsKeyUp(Keyboard::Down)) {
            manager->MenuDownSound().Play();
            selectedOption_ = (selectedOption_ + 1) % MAX_OPTIONS;
        }
        if (Keyboard::IsKeyUp(Keyboard::Up)) {
            manager->MenuUpSound().Play();
            selectedOption_ = (selectedOption_ - 1);
            if (selectedOption_ >= MAX_OPTIONS)
                selectedOption_ = MAX_OPTIONS - 1;
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeMenuState::SnakeMenuState() : selectedOption_(0), rectangleTexture_(nullptr),
    titleLabel_(nullptr), newGameLabel_(nullptr), rankingLabel_(nullptr),
    helpLabel_(nullptr), creditsLabel_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeMenuState::~SnakeMenuState() {}
