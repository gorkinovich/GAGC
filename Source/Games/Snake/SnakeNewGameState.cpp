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

#include "SnakeNewGameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Games/Snake/SnakeManager.h>
#include <Games/Snake/SnakeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define STATE_SELDIF  0
#define STATE_SELNOP  1
#define STATE_SELCT1  2
#define STATE_SELCT2  3
#define STATE_FINAL   4

#define NOP_1P  1
#define NOP_2P  2

#define COL0  120
#define COL1  480
#define ROW0  80
#define ROW1  116
#define ROW2  140
#define ROW3  164
#define ROW4  188
#define ROW5  224
#define ROW6  339

//********************************************************************************
// Methods
//********************************************************************************

void SnakeNewGameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    substate_ = STATE_SELDIF;
    difficulty_ = SnakeManager::DIFFICULTY_NORMAL;
    numberOfPlayers_ = NOP_1P;
    simpleControlPlayer1_ = true;
    simpleControlPlayer2_ = true;

    titleLabel_.reset(new SimpleLabel("New game", 0, ROW0,
        AtariPalette::Hue11Lum02, core->Retro70Font()));

    difficultyLabel_.reset(new SimpleLabel("Difficulty level:",
        COL0, ROW1, AtariPalette::Hue00Lum14, core->Retro70Font()));

    easyLabel_.reset(new SimpleLabel("easy", COL1, ROW1,
        AtariPalette::Hue01Lum14, core->Retro70Font()));
    normalLabel_.reset(new SimpleLabel("normal", COL1, ROW1,
        AtariPalette::Hue01Lum14, core->Retro70Font()));
    hardLabel_.reset(new SimpleLabel("hard", COL1, ROW1,
        AtariPalette::Hue01Lum14, core->Retro70Font()));

    numberOfPlayersLabel_.reset(new SimpleLabel("Number of players:",
        COL0, ROW2, AtariPalette::Hue00Lum14, core->Retro70Font()));

    onePlayerLabel_.reset(new SimpleLabel("1", COL1, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));
    twoPlayersLabel_.reset(new SimpleLabel("2", COL1, ROW2,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    controlTypePlayer1Label_.reset(new SimpleLabel("Control type player 1:",
        COL0, ROW3, AtariPalette::Hue04Lum02, core->Retro70Font()));

    standard1Label_.reset(new SimpleLabel("Standard", COL1, ROW3,
        AtariPalette::Hue00Lum14, core->Retro70Font()));
    simple1Label_.reset(new SimpleLabel("Simple", COL1, ROW3,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    controlTypePlayer2Label_.reset(new SimpleLabel("Control type player 2:",
        COL0, ROW4, AtariPalette::Hue09Lum02, core->Retro70Font()));

    standard2Label_.reset(new SimpleLabel("Standard", COL1, ROW4,
        AtariPalette::Hue00Lum14, core->Retro70Font()));
    simple2Label_.reset(new SimpleLabel("Simple", COL1, ROW4,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    startGameLabel_.reset(new SimpleLabel("Press Enter to start the game", 0, ROW5,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Press escape (ESC) to return to the menu...",
        0, ROW6, AtariPalette::Hue01Lum12, core->Retro70Font()));
    cancelLabel_.reset(new SimpleLabel("Press escape (ESC) to get back...",
        0, ROW6, AtariPalette::Hue01Lum12, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        titleLabel_->Text("Nueva partida");
        difficultyLabel_->Text("Nivel de dificultad:");
        easyLabel_->Text("fácil");
        normalLabel_->Text("normal");
        hardLabel_->Text("duro");
        numberOfPlayersLabel_->Text("Número de jugadores:");
        controlTypePlayer1Label_->Text("Tipo de control jugador 1:");
        standard1Label_->Text("Estándar");
        simple1Label_->Text("Simple");
        controlTypePlayer2Label_->Text("Tipo de control jugador 2:");
        standard2Label_->Text("Estándar");
        simple2Label_->Text("Simple");
        startGameLabel_->Text("Pulsa Enter para empezar la partida");
        exitLabel_->Text("Presione escape (ESC) para volver al menú...");
        cancelLabel_->Text("Presione escape (ESC) para retroceder...");
    }

    GUIUtil::CenterLabel(titleLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(easyLabel_, COL1);
    GUIUtil::CenterLabel(normalLabel_, COL1);
    GUIUtil::CenterLabel(hardLabel_, COL1);
    GUIUtil::CenterLabel(onePlayerLabel_, COL1);
    GUIUtil::CenterLabel(twoPlayersLabel_, COL1);
    GUIUtil::CenterLabel(standard1Label_, COL1);
    GUIUtil::CenterLabel(simple1Label_, COL1);
    GUIUtil::CenterLabel(standard2Label_, COL1);
    GUIUtil::CenterLabel(simple2Label_, COL1);
    GUIUtil::CenterLabel(startGameLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(exitLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(cancelLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

void SnakeNewGameState::Release() {
    titleLabel_.reset(nullptr);
    difficultyLabel_.reset(nullptr);
    easyLabel_.reset(nullptr);
    normalLabel_.reset(nullptr);
    hardLabel_.reset(nullptr);
    numberOfPlayersLabel_.reset(nullptr);
    onePlayerLabel_.reset(nullptr);
    twoPlayersLabel_.reset(nullptr);
    controlTypePlayer1Label_.reset(nullptr);
    standard1Label_.reset(nullptr);
    simple1Label_.reset(nullptr);
    controlTypePlayer2Label_.reset(nullptr);
    standard2Label_.reset(nullptr);
    simple2Label_.reset(nullptr);
    startGameLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    cancelLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void SnakeNewGameState::Draw(const sf::Time & timeDelta) {
    SnakeManager::Instance()->DrawBorder(AtariPalette::Hue10Lum02);

    titleLabel_->Draw();

    difficultyLabel_->Draw();
    if (difficulty_ == SnakeManager::DIFFICULTY_EASY) {
        easyLabel_->Draw();
    } else if (difficulty_ == SnakeManager::DIFFICULTY_NORMAL) {
        normalLabel_->Draw();
    } else if (difficulty_ == SnakeManager::DIFFICULTY_HARD) {
        hardLabel_->Draw();
    }

    if (substate_ >= STATE_SELNOP) {
        numberOfPlayersLabel_->Draw();
        if (numberOfPlayers_ == NOP_1P) {
            onePlayerLabel_->Draw();
        } else if (numberOfPlayers_ == NOP_2P) {
            twoPlayersLabel_->Draw();
        }
    }

    if (substate_ >= STATE_SELCT1) {
        controlTypePlayer1Label_->Draw();
        if (simpleControlPlayer1_) {
            simple1Label_->Draw();
        } else {
            standard1Label_->Draw();
        }
    }

    if (substate_ >= STATE_SELCT2 && numberOfPlayers_ == NOP_2P) {
        controlTypePlayer2Label_->Draw();
        if (simpleControlPlayer2_) {
            simple2Label_->Draw();
        } else {
            standard2Label_->Draw();
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

void SnakeNewGameState::Update(const sf::Time & timeDelta) {
    auto * manager = SnakeManager::Instance();
    switch (substate_) {
    case STATE_SELDIF:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuDownSound().Play();
            CoreManager::Instance()->SetNextState(MakeSharedState<SnakeMenuState>());
        } else if (Keyboard::IsKeyUp(Keyboard::Left)) {
            manager->MenuKeySound().Play();
            switch (difficulty_) {
            case SnakeManager::DIFFICULTY_EASY: difficulty_ = SnakeManager::DIFFICULTY_HARD; break;
            case SnakeManager::DIFFICULTY_NORMAL: difficulty_ = SnakeManager::DIFFICULTY_EASY; break;
            case SnakeManager::DIFFICULTY_HARD: difficulty_ = SnakeManager::DIFFICULTY_NORMAL; break;
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Right)) {
            manager->MenuKeySound().Play();
            switch (difficulty_) {
            case SnakeManager::DIFFICULTY_EASY: difficulty_ = SnakeManager::DIFFICULTY_NORMAL; break;
            case SnakeManager::DIFFICULTY_NORMAL: difficulty_ = SnakeManager::DIFFICULTY_HARD; break;
            case SnakeManager::DIFFICULTY_HARD: difficulty_ = SnakeManager::DIFFICULTY_EASY; break;
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuUpSound().Play();
            substate_ = STATE_SELNOP;
            easyLabel_->Color(AtariPalette::Hue00Lum14);
            normalLabel_->Color(AtariPalette::Hue00Lum14);
            hardLabel_->Color(AtariPalette::Hue00Lum14);
            onePlayerLabel_->Color(AtariPalette::Hue01Lum14);
            twoPlayersLabel_->Color(AtariPalette::Hue01Lum14);
        }
        break;

    case STATE_SELNOP:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuDownSound().Play();
            substate_ = STATE_SELDIF;
            easyLabel_->Color(AtariPalette::Hue01Lum14);
            normalLabel_->Color(AtariPalette::Hue01Lum14);
            hardLabel_->Color(AtariPalette::Hue01Lum14);
        } else if (Keyboard::IsKeyUp(Keyboard::Left) || Keyboard::IsKeyUp(Keyboard::Right)) {
            manager->MenuKeySound().Play();
            numberOfPlayers_ = (numberOfPlayers_ == NOP_1P) ? NOP_2P : NOP_1P;
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuUpSound().Play();
            substate_ = STATE_SELCT1;
            onePlayerLabel_->Color(AtariPalette::Hue00Lum14);
            twoPlayersLabel_->Color(AtariPalette::Hue00Lum14);
            standard1Label_->Color(AtariPalette::Hue01Lum14);
            simple1Label_->Color(AtariPalette::Hue01Lum14);
        }
        break;

    case STATE_SELCT1:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuDownSound().Play();
            substate_ = STATE_SELNOP;
            onePlayerLabel_->Color(AtariPalette::Hue01Lum14);
            twoPlayersLabel_->Color(AtariPalette::Hue01Lum14);
        } else if (Keyboard::IsKeyUp(Keyboard::Left) || Keyboard::IsKeyUp(Keyboard::Right)) {
            manager->MenuKeySound().Play();
            simpleControlPlayer1_ = !simpleControlPlayer1_;
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuUpSound().Play();
            if (numberOfPlayers_ == NOP_2P) {
                substate_ = STATE_SELCT2;
                standard1Label_->Color(AtariPalette::Hue00Lum14);
                simple1Label_->Color(AtariPalette::Hue00Lum14);
                standard2Label_->Color(AtariPalette::Hue01Lum14);
                simple2Label_->Color(AtariPalette::Hue01Lum14);
            } else {
                substate_ = STATE_FINAL;
                standard1Label_->Color(AtariPalette::Hue00Lum14);
                simple1Label_->Color(AtariPalette::Hue00Lum14);
            }
        }
        break;

    case STATE_SELCT2:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuDownSound().Play();
            substate_ = STATE_SELCT1;
            standard1Label_->Color(AtariPalette::Hue01Lum14);
            simple1Label_->Color(AtariPalette::Hue01Lum14);
        } else if (Keyboard::IsKeyUp(Keyboard::Left) || Keyboard::IsKeyUp(Keyboard::Right)) {
            manager->MenuKeySound().Play();
            simpleControlPlayer2_ = !simpleControlPlayer2_;
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuUpSound().Play();
            substate_ = STATE_FINAL;
            standard2Label_->Color(AtariPalette::Hue00Lum14);
            simple2Label_->Color(AtariPalette::Hue00Lum14);
        }
        break;

    case STATE_FINAL:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            manager->MenuDownSound().Play();
            if (numberOfPlayers_ == NOP_2P) {
                substate_ = STATE_SELCT2;
                standard2Label_->Color(AtariPalette::Hue01Lum14);
                simple2Label_->Color(AtariPalette::Hue01Lum14);
            } else {
                substate_ = STATE_SELCT1;
                standard1Label_->Color(AtariPalette::Hue01Lum14);
                simple1Label_->Color(AtariPalette::Hue01Lum14);
            }
        } else if (Keyboard::IsKeyUp(Keyboard::Space) || Keyboard::IsKeyUp(Keyboard::Return)) {
            manager->MenuNextSound().Play();
            if (numberOfPlayers_ == NOP_2P) {
                SnakeManager::Instance()->StartGame(difficulty_, simpleControlPlayer1_, simpleControlPlayer2_);
            } else {
                SnakeManager::Instance()->StartGame(difficulty_, simpleControlPlayer1_);
            }
        }
        break;
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

SnakeNewGameState::SnakeNewGameState() : substate_(0), difficulty_(0),
    numberOfPlayers_(0), simpleControlPlayer1_(false), simpleControlPlayer2_(false),
    titleLabel_(nullptr), difficultyLabel_(nullptr), easyLabel_(nullptr),
    normalLabel_(nullptr), hardLabel_(nullptr), numberOfPlayersLabel_(nullptr),
    onePlayerLabel_(nullptr), twoPlayersLabel_(nullptr), controlTypePlayer1Label_(nullptr),
    standard1Label_(nullptr), simple1Label_(nullptr), controlTypePlayer2Label_(nullptr),
    standard2Label_(nullptr), simple2Label_(nullptr), startGameLabel_(nullptr),
    exitLabel_(nullptr), cancelLabel_(nullptr) {}

//--------------------------------------------------------------------------------

SnakeNewGameState::~SnakeNewGameState() {}
