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

#include "TetraminoesMenuState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesNewGameState.h>
#include <Games/Tetraminoes/TetraminoesRankingState.h>
#include <Games/Tetraminoes/TetraminoesHelpState.h>
#include <Games/Tetraminoes/TetraminoesCreditsState.h>
#include <Games/Tetraminoes/TetraminoesExitState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW_OFFSET  2

#define COL0  244
#define COL1  380

#define ROW0  144
#define ROW1  168
#define ROW2  192
#define ROW3  216
#define ROW4  240

#define NEWGAME_OPTION  0
#define RANKING_OPTION  1
#define HELP_OPTION     2
#define CREDITS_OPTION  3
#define EXIT_OPTION     4
#define MAX_OPTIONS     5

#define BLACK_COLOR   sf::Color::Black
#define WHITE_COLOR   sf::Color::White
#define YELLOW_COLOR  sf::Color::Yellow

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesMenuState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->InitRndSeed();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();
    auto & tileset = manager->Tileset();

    selectedOption_ = -1;

    background1Texture_.reset(new Texture2D());
    background1Texture_->Load(manager->Menu1Background());

    background2Position_ = sf::Vector2i(215, 39);
    background2Texture_.reset(new Texture2D());
    background2Texture_->Load(tileset, sf::IntRect(209, 166, 209, 273));

    leftSelectorPosition_ = sf::Vector2i(COL0, 0);
    leftSelectorTexture_.reset(new Texture2D());
    leftSelectorTexture_->Load(tileset, sf::IntRect(64, 32, 16, 16));

    rightSelectorPosition_ = sf::Vector2i(COL1, 0);
    rightSelectorTexture_.reset(new Texture2D());
    rightSelectorTexture_->Load(tileset, sf::IntRect(80, 32, 16, 16));

    newGameLabel_.reset(new SimpleLabel("New Game", 0, ROW0,
        WHITE_COLOR, core->Retro70Font()));

    rankingLabel_.reset(new SimpleLabel("Ranking", 0, ROW1,
        WHITE_COLOR, core->Retro70Font()));

    helpLabel_.reset(new SimpleLabel("Help", 0, ROW2,
        WHITE_COLOR, core->Retro70Font()));

    creditsLabel_.reset(new SimpleLabel("Credits", 0, ROW3,
        WHITE_COLOR, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 0, ROW4,
        WHITE_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        newGameLabel_->Text("Nueva Partida");
        rankingLabel_->Text("Puntuaciones");
        helpLabel_->Text("Ayuda");
        creditsLabel_->Text("Créditos");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(newGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(rankingLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(helpLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(creditsLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void TetraminoesMenuState::Release() {
    background1Texture_.reset(nullptr);
    background2Texture_.reset(nullptr);
    leftSelectorTexture_.reset(nullptr);
    rightSelectorTexture_.reset(nullptr);
    newGameLabel_.reset(nullptr);
    rankingLabel_.reset(nullptr);
    helpLabel_.reset(nullptr);
    creditsLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesMenuState::Draw(const sf::Time & timeDelta) {
    background1Texture_->Draw(0, 0);
    background2Texture_->Draw(background2Position_);

    auto color = Keyboard::isKeyPressed(Keyboard::Return) ||
        Keyboard::isKeyPressed(Keyboard::Space) ?
        YELLOW_COLOR : WHITE_COLOR;

    if (selectedOption_ < MAX_OPTIONS) {
        leftSelectorTexture_->Draw(leftSelectorPosition_, color);
        rightSelectorTexture_->Draw(rightSelectorPosition_, color);
    }

    switch (selectedOption_) {
    case NEWGAME_OPTION: newGameLabel_->Color(color); break;
    case RANKING_OPTION: rankingLabel_->Color(color); break;
    case HELP_OPTION:    helpLabel_->Color(color);    break;
    case CREDITS_OPTION: creditsLabel_->Color(color); break;
    case EXIT_OPTION:    exitLabel_->Color(color);    break;
    }

    newGameLabel_->Draw();
    rankingLabel_->Draw();
    helpLabel_->Draw();
    creditsLabel_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesMenuState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = TetraminoesManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Return) || Keyboard::IsKeyUp(Keyboard::Space)) {
        manager->KeyboardSound().Play();
        switch (selectedOption_) {
        case NEWGAME_OPTION: core->SetNextState(MakeSharedState<TetraminoesNewGameState>()); break;
        case RANKING_OPTION: core->SetNextState(MakeSharedState<TetraminoesRankingState>()); break;
        case HELP_OPTION:    core->SetNextState(MakeSharedState<TetraminoesHelpState>());    break;
        case CREDITS_OPTION: core->SetNextState(MakeSharedState<TetraminoesCreditsState>()); break;
        case EXIT_OPTION:    core->SetNextState(MakeSharedState<TetraminoesExitState>());    break;
        }

    } else if (Keyboard::IsKeyUp(Keyboard::F1)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesNewGameState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesRankingState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesHelpState>());

    } else if (Keyboard::IsKeyUp(Keyboard::F4)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesCreditsState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<TetraminoesExitState>());

    } else {
        unsigned int option = selectedOption_;
        if (Keyboard::IsKeyUp(Keyboard::Down)) {
            manager->KeyboardSound().Play();
            option = (option + 1) % MAX_OPTIONS;
        }
        if (Keyboard::IsKeyUp(Keyboard::Up)) {
            manager->KeyboardSound().Play();
            option = (option - 1);
            if (option >= MAX_OPTIONS) {
                option = MAX_OPTIONS - 1;
            }
        }
        changeOption(option);
    }
}

//--------------------------------------------------------------------------------

void TetraminoesMenuState::changeOption(unsigned int option) {
    selectedOption_ = option;
    newGameLabel_->Color(WHITE_COLOR);
    rankingLabel_->Color(WHITE_COLOR);
    helpLabel_->Color(WHITE_COLOR);
    creditsLabel_->Color(WHITE_COLOR);
    exitLabel_->Color(WHITE_COLOR);

    int y = -32;
    switch (selectedOption_) {
    case NEWGAME_OPTION: y = ROW0 - ROW_OFFSET; break;
    case RANKING_OPTION: y = ROW1 - ROW_OFFSET; break;
    case HELP_OPTION:    y = ROW2 - ROW_OFFSET; break;
    case CREDITS_OPTION: y = ROW3 - ROW_OFFSET; break;
    case EXIT_OPTION:    y = ROW4 - ROW_OFFSET; break;
    }
    leftSelectorPosition_.y = y;
    rightSelectorPosition_.y = y;
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesMenuState::TetraminoesMenuState() : selectedOption_(0) {}

//--------------------------------------------------------------------------------

TetraminoesMenuState::~TetraminoesMenuState() {}
