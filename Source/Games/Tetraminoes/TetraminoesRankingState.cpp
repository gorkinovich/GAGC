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

#include "TetraminoesRankingState.h"
#include <sstream>
#include <iomanip>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Games/Tetraminoes/TetraminoesManager.h>
#include <Games/Tetraminoes/TetraminoesMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  48
#define ROW1  84
#define ROW2  300
#define COL1  180
#define COL2  460

#define BLACK_COLOR   sf::Color::Black
#define WHITE_COLOR   sf::Color::White
#define YELLOW_COLOR  sf::Color::Yellow
#define RED_COLOR     sf::Color::Red

//********************************************************************************
// Methods
//********************************************************************************

void TetraminoesRankingState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(BLACK_COLOR);

    auto * manager = TetraminoesManager::Instance();
    auto & ranking = manager->Ranking();

    std::string names = "";
    std::string scores = "";

    ForEach(ranking, [&] (TetraminoesManager::RankingEntry & victim) {
        names += victim.Name + "\n\n";
        std::stringstream strconv;
        strconv << std::setfill('0') << std::setw(8) << victim.Score;
        scores += strconv.str() + "\n\n";
    });

    backgroundTexture_.reset(new Texture2D());
    backgroundTexture_->Load(manager->Menu2Background());

    titleLabel_.reset(new SimpleLabel("Ranking",
        0, ROW0, RED_COLOR, core->Retro70Font()));

    namesText_.reset(new TextLabel(names, COL1, ROW1,
        WHITE_COLOR, core->Retro70Font()));

    scoresText_.reset(new TextLabel(scores, COL2, ROW1,
        WHITE_COLOR, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, YELLOW_COLOR, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Puntuaciones");
        exitLabel_->Text("Presione cualquier tecla para volver al menú...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(namesText_, COL1);
    GUIUtil::CenterLabel(scoresText_, COL2);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void TetraminoesRankingState::Release() {
    backgroundTexture_.reset(nullptr);
    titleLabel_.reset(nullptr);
    namesText_.reset(nullptr);
    scoresText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void TetraminoesRankingState::Draw(const sf::Time & timeDelta) {
    backgroundTexture_->Draw(0, 0);
    titleLabel_->Draw();
    namesText_->Draw();
    scoresText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void TetraminoesRankingState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        TetraminoesManager::Instance()->KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<TetraminoesMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TetraminoesRankingState::TetraminoesRankingState() {}

//--------------------------------------------------------------------------------

TetraminoesRankingState::~TetraminoesRankingState() {}
