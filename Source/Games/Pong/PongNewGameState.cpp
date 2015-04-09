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

#include "PongNewGameState.h"
#include <sstream>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Pong/PongManager.h>
#include <Games/Pong/PongMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define STATE_SELPOI  0
#define STATE_SELPLA  1
#define STATE_SELDIF  2
#define STATE_FINAL   3

#define NOP_1P  1
#define NOP_2P  2

#define COL0  120
#define COL1  480
#define ROW0  120
#define ROW1  156
#define ROW2  180
#define ROW3  204
#define ROW4  252
#define ROW5  336

#define NORMAL_COLOR  AtariPalette::Hue00Lum14
#define SELECT_COLOR  AtariPalette::Hue01Lum14

//********************************************************************************
// Methods
//********************************************************************************

void PongNewGameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    substate_ = STATE_SELPOI;
    numberOfPoints_ = 3;
    numberOfPlayers_ = NOP_1P;
    difficulty_ = PongManager::DIFFICULTY_NORMAL;

    titleLabel_.reset(new SimpleLabel("New game", 0, ROW0,
        AtariPalette::Hue04Lum02, core->Retro70Font()));

    numberOfPointsLabel_.reset(new SimpleLabel("Number of points:",
        COL0, ROW1, NORMAL_COLOR, core->Retro70Font()));

    pointsLabel_.reset(new SimpleLabel(numberOfPointsToString(),
        COL1, ROW1, SELECT_COLOR, core->Retro70Font()));

    numberOfPlayersLabel_.reset(new SimpleLabel("Number of players:",
        COL0, ROW2, NORMAL_COLOR, core->Retro70Font()));

    onePlayerLabel_.reset(new SimpleLabel("1", COL1, ROW2,
        NORMAL_COLOR, core->Retro70Font()));
    twoPlayersLabel_.reset(new SimpleLabel("2", COL1, ROW2,
        NORMAL_COLOR, core->Retro70Font()));

    difficultyLabel_.reset(new SimpleLabel("Difficulty level:",
        COL0, ROW3, NORMAL_COLOR, core->Retro70Font()));

    easyLabel_.reset(new SimpleLabel("easy", COL1, ROW3,
        NORMAL_COLOR, core->Retro70Font()));
    normalLabel_.reset(new SimpleLabel("normal", COL1, ROW3,
        NORMAL_COLOR, core->Retro70Font()));
    hardLabel_.reset(new SimpleLabel("hard", COL1, ROW3,
        NORMAL_COLOR, core->Retro70Font()));

    startGameLabel_.reset(new SimpleLabel("Press Enter to start the game",
        0, ROW4, NORMAL_COLOR, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press escape (ESC) to return to the menu...",
        0, ROW5, AtariPalette::Hue01Lum14, core->Retro70Font()));
    cancelLabel_.reset(new SimpleLabel("Press escape (ESC) to get back...",
        0, ROW5, AtariPalette::Hue01Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Nueva partida");
        numberOfPointsLabel_->Text("Número de puntos:");
        numberOfPlayersLabel_->Text("Número de jugadores:");
        difficultyLabel_->Text("Nivel de dificultad:");
        easyLabel_->Text("fácil");
        normalLabel_->Text("normal");
        hardLabel_->Text("duro");
        startGameLabel_->Text("Pulsa Enter para empezar la partida");
        exitLabel_->Text("Presione escape (ESC) para volver al menú...");
        cancelLabel_->Text("Presione escape (ESC) para retroceder...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(pointsLabel_, COL1);
    GUIUtil::CenterLabel(onePlayerLabel_, COL1);
    GUIUtil::CenterLabel(twoPlayersLabel_, COL1);
    GUIUtil::CenterLabel(easyLabel_, COL1);
    GUIUtil::CenterLabel(normalLabel_, COL1);
    GUIUtil::CenterLabel(hardLabel_, COL1);
    GUIUtil::CenterLabel(startGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(cancelLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void PongNewGameState::Release() {
    titleLabel_.reset(nullptr);
    difficultyLabel_.reset(nullptr);
    easyLabel_.reset(nullptr);
    normalLabel_.reset(nullptr);
    hardLabel_.reset(nullptr);
    numberOfPlayersLabel_.reset(nullptr);
    onePlayerLabel_.reset(nullptr);
    twoPlayersLabel_.reset(nullptr);
    numberOfPointsLabel_.reset(nullptr);
    pointsLabel_.reset(nullptr);
    startGameLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    cancelLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void PongNewGameState::Draw(const sf::Time & timeDelta) {
    titleLabel_->Draw();

    numberOfPointsLabel_->Draw();
    pointsLabel_->Draw();

    if (substate_ >= STATE_SELPLA) {
        numberOfPlayersLabel_->Draw();
        if (numberOfPlayers_ == NOP_1P) {
            onePlayerLabel_->Draw();
        } else if (numberOfPlayers_ == NOP_2P) {
            twoPlayersLabel_->Draw();
        }
    }

    if (substate_ >= STATE_SELDIF && numberOfPlayers_ == NOP_1P) {
        difficultyLabel_->Draw();
        if (difficulty_ == PongManager::DIFFICULTY_EASY) {
            easyLabel_->Draw();
        } else if (difficulty_ == PongManager::DIFFICULTY_NORMAL) {
            normalLabel_->Draw();
        } else if (difficulty_ == PongManager::DIFFICULTY_HARD) {
            hardLabel_->Draw();
        }
    }

    if (substate_ >= STATE_FINAL) {
        startGameLabel_->Draw();
    }

    if (substate_ == STATE_SELDIF) {
        exitLabel_->Draw();
    } else {
        cancelLabel_->Draw();
    }
}

//--------------------------------------------------------------------------------

void PongNewGameState::Update(const sf::Time & timeDelta) {
    auto * manager = PongManager::Instance();
    switch (substate_) {
    case STATE_SELPOI:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuKeySound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<PongMenuState>());
        } else if (Keyboard::IsKeyUp(Keyboard::Left)) {
            if (numberOfPoints_ > 1) {
                manager->MenuSelectSound().Play();
                --numberOfPoints_;
                pointsLabel_->Text(numberOfPointsToString());
                GUIUtil::CenterLabel(pointsLabel_, COL1);
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Right)) {
            if (numberOfPoints_ < 16) {
                manager->MenuSelectSound().Play();
                ++numberOfPoints_;
                pointsLabel_->Text(numberOfPointsToString());
                GUIUtil::CenterLabel(pointsLabel_, COL1);
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuKeySound().Play();
            substate_ = STATE_SELPLA;
            pointsLabel_->Color(NORMAL_COLOR);
            onePlayerLabel_->Color(SELECT_COLOR);
            twoPlayersLabel_->Color(SELECT_COLOR);
        }
        break;

    case STATE_SELPLA:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuKeySound().Play();
            substate_ = STATE_SELPOI;
            pointsLabel_->Color(SELECT_COLOR);
        } else if (Keyboard::IsKeyUp(Keyboard::Left) || Keyboard::IsKeyUp(Keyboard::Right)) {
            manager->MenuSelectSound().Play();
            numberOfPlayers_ = (numberOfPlayers_ == NOP_1P) ? NOP_2P : NOP_1P;
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuKeySound().Play();
            if (numberOfPlayers_ == NOP_1P) {
                substate_ = STATE_SELDIF;
                easyLabel_->Color(SELECT_COLOR);
                normalLabel_->Color(SELECT_COLOR);
                hardLabel_->Color(SELECT_COLOR);
            } else {
                substate_ = STATE_FINAL;
            }
            onePlayerLabel_->Color(NORMAL_COLOR);
            twoPlayersLabel_->Color(NORMAL_COLOR);
        }
        break;

    case STATE_SELDIF:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuKeySound().Play();
            substate_ = STATE_SELPLA;
            onePlayerLabel_->Color(SELECT_COLOR);
            twoPlayersLabel_->Color(SELECT_COLOR);
        } else if (Keyboard::IsKeyUp(Keyboard::Left)) {
            manager->MenuSelectSound().Play();
            switch (difficulty_) {
            case PongManager::DIFFICULTY_EASY: difficulty_ = PongManager::DIFFICULTY_HARD; break;
            case PongManager::DIFFICULTY_NORMAL: difficulty_ = PongManager::DIFFICULTY_EASY; break;
            case PongManager::DIFFICULTY_HARD: difficulty_ = PongManager::DIFFICULTY_NORMAL; break;
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Right)) {
            manager->MenuSelectSound().Play();
            switch (difficulty_) {
            case PongManager::DIFFICULTY_EASY: difficulty_ = PongManager::DIFFICULTY_NORMAL; break;
            case PongManager::DIFFICULTY_NORMAL: difficulty_ = PongManager::DIFFICULTY_HARD; break;
            case PongManager::DIFFICULTY_HARD: difficulty_ = PongManager::DIFFICULTY_EASY; break;
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuKeySound().Play();
            substate_ = STATE_FINAL;
            easyLabel_->Color(NORMAL_COLOR);
            normalLabel_->Color(NORMAL_COLOR);
            hardLabel_->Color(NORMAL_COLOR);
        }
        break;

    case STATE_FINAL:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuKeySound().Play();
            if (numberOfPlayers_ == NOP_1P) {
                substate_ = STATE_SELDIF;
                easyLabel_->Color(SELECT_COLOR);
                normalLabel_->Color(SELECT_COLOR);
                hardLabel_->Color(SELECT_COLOR);
            } else {
                substate_ = STATE_SELPLA;
                onePlayerLabel_->Color(SELECT_COLOR);
                twoPlayersLabel_->Color(SELECT_COLOR);
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuNextSound().Play();
            PongManager::Instance()->StartGame(difficulty_, numberOfPlayers_, numberOfPoints_);
        }
        break;
    }
}

//--------------------------------------------------------------------------------

std::string PongNewGameState::numberOfPointsToString() {
    std::stringstream strconv;
    strconv << numberOfPoints_;
    return strconv.str();
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PongNewGameState::PongNewGameState() : substate_(0), difficulty_(0),
    numberOfPlayers_(0), numberOfPoints_(0), titleLabel_(nullptr),
    difficultyLabel_(nullptr), easyLabel_(nullptr), normalLabel_(nullptr),
    hardLabel_(nullptr), numberOfPlayersLabel_(nullptr), onePlayerLabel_(nullptr),
    twoPlayersLabel_(nullptr), startGameLabel_(nullptr), numberOfPointsLabel_(nullptr),
    pointsLabel_(nullptr), exitLabel_(nullptr), cancelLabel_(nullptr) {}

//--------------------------------------------------------------------------------

PongNewGameState::~PongNewGameState() {}
