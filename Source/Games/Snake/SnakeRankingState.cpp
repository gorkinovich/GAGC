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

#include "SnakeRankingState.h"
#include <sstream>
#include <iomanip>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  32
#define ROW1  56
#define ROW2  339
#define COL1  160
#define COL2  480

//********************************************************************************
// Methods
//********************************************************************************

void SnakeRankingState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = SnakeManager::Instance();
    auto & ranking = manager->Ranking();

    std::string names = "";
    std::string scores = "";

    ForEach(ranking, [&] (SnakeManager::RankingEntry & victim) {
        names += victim.Name + "\n\n";
        std::stringstream strconv;
        strconv << std::setfill('0') << std::setw(8) << victim.Score;
        scores += strconv.str() + "\n\n";
    });

    titleLabel_.reset(new SimpleLabel("Ranking", 0, ROW0,
        AtariPalette::Hue11Lum02, core->Retro70Font()));

    namesText_.reset(new TextLabel(names, COL1, ROW1,
        AtariPalette::Hue04Lum02, core->Retro70Font()));

    scoresText_.reset(new TextLabel(scores, COL2, ROW1,
        AtariPalette::Hue09Lum02, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press any key to return to the menu...",
        0, ROW2, AtariPalette::Hue01Lum12, core->Retro70Font()));

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

void SnakeRankingState::Release() {
    titleLabel_.reset(nullptr);
    namesText_.reset(nullptr);
    scoresText_.reset(nullptr);
    exitLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeRankingState::Draw(const sf::Time & timeDelta) {
    // Draw the border of the screen.
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);
    // Draw the labels of the menu.
    titleLabel_->Draw();
    namesText_->Draw();
    scoresText_->Draw();
    exitLabel_->Draw();
}

//--------------------------------------------------------------------------------

void SnakeRankingState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsAnyKeyUp()) {
        SnakeManager::Instance()->MenuNextSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<SnakeMenuState>());
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeRankingState::SnakeRankingState() : titleLabel_(nullptr),
    namesText_(nullptr), scoresText_(nullptr), exitLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeRankingState::~SnakeRankingState() {}
