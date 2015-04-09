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

#include "DesktopState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Menu/MenuManager.h>
#include <Menu/AboutState.h>
#include <Menu/ExitDialogState.h>
#include <Games/Snake/SnakeMenuState.h>
#include <Games/Minesweeper/MinesweeperGameState.h>
#include <Games/Battleship/BattleshipMenuState.h>
#include <Games/Blackjack/BlackjackMenuState.h>
#include <Games/TicTacToe/TicTacToeMenuState.h>
#include <Games/Pong/PongMenuState.h>
#include <Games/Puzzle/PuzzleGameState.h>
#include <Games/AlienParty/AlienMenuState.h>
#include <Games/Reversi/ReversiMenuState.h>
#include <Games/Chess/ChessMenuState.h>
#include <Games/Checkers/CheckersMenuState.h>
#include <Games/Puckman/PuckmanMainState.h>
#include <Games/Tetraminoes/TetraminoesIntroState.h>

//********************************************************************************
// Methods
//********************************************************************************

void DesktopState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum02);

    auto * menuCore = MenuManager::Instance();
    auto & tileset = menuCore->Tileset();

    menuCore->LoadGeneralPlaylist();

    spartanPosition_ = sf::Vector2i(240, 56);
    spartanTexture_.reset(new Texture2D());
    spartanTexture_->Load(tileset, sf::IntRect(864, 0, 160, 248));

    snakeLabel_.reset(new SimpleLabel("Snake", 20, 60,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    minesweeperLabel_.reset(new SimpleLabel("Minesweeper", 84, 60,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    battleshipLabel_.reset(new SimpleLabel("Battleship", 192, 60,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    blackjackLabel_.reset(new SimpleLabel("Blackjack", 372, 60,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    tictactoeLabel_.reset(new SimpleLabel("Tic-Tac-Toe", 468, 60,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    pongLabel_.reset(new SimpleLabel("Pong", 584, 60,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    puzzleLabel_.reset(new SimpleLabel("Puzzle", 16, 144,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    alienPartyLabel_.reset(new SimpleLabel("Alien Party", 84, 144,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    reversiLabel_.reset(new SimpleLabel("Reversi", 484, 144,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    chessLabel_.reset(new SimpleLabel("Chess", 580, 144,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    checkersLabel_.reset(new SimpleLabel("Checkers", 568, 228,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    puckmanLabel_.reset(new SimpleLabel("Puckman", 12, 228,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    tetraminoesLabel_.reset(new SimpleLabel("Tetraminoes", 84, 228,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    aboutLabel_.reset(new SimpleLabel("About", 20, 336,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    exitLabel_.reset(new SimpleLabel("Exit", 584, 336,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        snakeLabel_->Text("Snake");
        minesweeperLabel_->Text("Buscaminas");
        battleshipLabel_->Text("Battleship");
        blackjackLabel_->Text("Blackjack");
        tictactoeLabel_->Text("Tic-Tac-Toe");
        pongLabel_->Text("Pong");
        puzzleLabel_->Text("Puzzle");
        alienPartyLabel_->Text("Alien Party");
        reversiLabel_->Text("Reversi");
        chessLabel_->Text("Ajedrez");
        checkersLabel_->Text("Damas");
        puckmanLabel_->Text("Puckman");
        tetraminoesLabel_->Text("Tetraminoes");
        aboutLabel_->Text("Sobre...");
        exitLabel_->Text("Salir");
    }

    GUIUtil::CenterLabel(snakeLabel_, 40);
    GUIUtil::CenterLabel(minesweeperLabel_, 128);
    GUIUtil::CenterLabel(battleshipLabel_, 232);
    GUIUtil::CenterLabel(blackjackLabel_, 408);
    GUIUtil::CenterLabel(tictactoeLabel_, 512);
    GUIUtil::CenterLabel(pongLabel_, 600);
    GUIUtil::CenterLabel(puzzleLabel_, 40);
    GUIUtil::CenterLabel(alienPartyLabel_, 128);
    GUIUtil::CenterLabel(reversiLabel_, 512);
    GUIUtil::CenterLabel(chessLabel_, 600);
    GUIUtil::CenterLabel(checkersLabel_, 600);
    GUIUtil::CenterLabel(puckmanLabel_, 40);
    GUIUtil::CenterLabel(tetraminoesLabel_, 128);
    GUIUtil::CenterLabel(aboutLabel_, 40);
    GUIUtil::CenterLabel(exitLabel_, 600);

    snakeButton_.reset(new TexturedButton());
    snakeButton_->Initialize(16, 12, menuCore->Tileset(),
        sf::IntRect(288, 272, 48, 48), sf::IntRect(336, 272, 48, 48),
        sf::IntRect(384, 272, 48, 48));
    snakeButton_->LinkedLabel(snakeLabel_.get());
    snakeButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<SnakeMenuState>());
    });

    minesweeperButton_.reset(new TexturedButton());
    minesweeperButton_->Initialize(104, 12, menuCore->Tileset(),
        sf::IntRect(432, 272, 48, 48), sf::IntRect(480, 272, 48, 48),
        sf::IntRect(528, 272, 48, 48));
    minesweeperButton_->LinkedLabel(minesweeperLabel_.get());
    minesweeperButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<MinesweeperGameState>());
    });

    battleshipButton_.reset(new TexturedButton());
    battleshipButton_->Initialize(208, 12, menuCore->Tileset(),
        sf::IntRect(576, 272, 48, 48), sf::IntRect(624, 272, 48, 48),
        sf::IntRect(672, 272, 48, 48));
    battleshipButton_->LinkedLabel(battleshipLabel_.get());
    battleshipButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<BattleshipMenuState>());
    });

    blackjackButton_.reset(new TexturedButton());
    blackjackButton_->Initialize(384, 12, menuCore->Tileset(),
        sf::IntRect(720, 272, 48, 48), sf::IntRect(768, 272, 48, 48),
        sf::IntRect(816, 272, 48, 48));
    blackjackButton_->LinkedLabel(blackjackLabel_.get());
    blackjackButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<BlackjackMenuState>());
    });

    tictactoeButton_.reset(new TexturedButton());
    tictactoeButton_->Initialize(488, 12, menuCore->Tileset(),
        sf::IntRect(864, 272, 48, 48), sf::IntRect(912, 272, 48, 48),
        sf::IntRect(960, 272, 48, 48));
    tictactoeButton_->LinkedLabel(tictactoeLabel_.get());
    tictactoeButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<TicTacToeMenuState>());
    });

    pongButton_.reset(new TexturedButton());
    pongButton_->Initialize(576, 12, menuCore->Tileset(),
        sf::IntRect(0, 320, 48, 48), sf::IntRect(48, 320, 48, 48),
        sf::IntRect(96, 320, 48, 48));
    pongButton_->LinkedLabel(pongLabel_.get());
    pongButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<PongMenuState>());
    });

    puzzleButton_.reset(new TexturedButton());
    puzzleButton_->Initialize(16, 96, menuCore->Tileset(),
        sf::IntRect(144, 320, 48, 48), sf::IntRect(192, 320, 48, 48),
        sf::IntRect(240, 320, 48, 48));
    puzzleButton_->LinkedLabel(puzzleLabel_.get());
    puzzleButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<PuzzleGameState>());
    });

    alienPartyButton_.reset(new TexturedButton());
    alienPartyButton_->Initialize(104, 96, menuCore->Tileset(),
        sf::IntRect(288, 320, 48, 48), sf::IntRect(336, 320, 48, 48),
        sf::IntRect(384, 320, 48, 48));
    alienPartyButton_->LinkedLabel(alienPartyLabel_.get());
    alienPartyButton_->OnClick([core, menuCore] (TexturedButton &) {
        menuCore->LoadAlienPartyPlaylist();
        core->SetNextState(MakeSharedState<AlienMenuState>());
    });

    reversiButton_.reset(new TexturedButton());
    reversiButton_->Initialize(488, 96, menuCore->Tileset(),
        sf::IntRect(432, 320, 48, 48), sf::IntRect(480, 320, 48, 48),
        sf::IntRect(528, 320, 48, 48));
    reversiButton_->LinkedLabel(reversiLabel_.get());
    reversiButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<ReversiMenuState>());
    });

    chessButton_.reset(new TexturedButton());
    chessButton_->Initialize(576, 96, menuCore->Tileset(),
        sf::IntRect(576, 320, 48, 48), sf::IntRect(624, 320, 48, 48),
        sf::IntRect(672, 320, 48, 48));
    chessButton_->LinkedLabel(chessLabel_.get());
    chessButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<ChessMenuState>());
    });

    checkersButton_.reset(new TexturedButton());
    checkersButton_->Initialize(576, 180, menuCore->Tileset(),
        sf::IntRect(0, 368, 48, 48), sf::IntRect(48, 368, 48, 48),
        sf::IntRect(96, 368, 48, 48));
    checkersButton_->LinkedLabel(checkersLabel_.get());
    checkersButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<CheckersMenuState>());
    });

    puckmanButton_.reset(new TexturedButton());
    puckmanButton_->Initialize(16, 180, menuCore->Tileset(),
        sf::IntRect(720, 320, 48, 48), sf::IntRect(768, 320, 48, 48),
        sf::IntRect(816, 320, 48, 48));
    puckmanButton_->LinkedLabel(puckmanLabel_.get());
    puckmanButton_->OnClick([core, menuCore] (TexturedButton &) {
        menuCore->LoadPuckmanPlaylist();
        core->SetNextState(MakeSharedState<PuckmanMainState>());
    });

    tetraminoesButton_.reset(new TexturedButton());
    tetraminoesButton_->Initialize(104, 180, menuCore->Tileset(),
        sf::IntRect(864, 320, 48, 48), sf::IntRect(912, 320, 48, 48),
        sf::IntRect(960, 320, 48, 48));
    tetraminoesButton_->LinkedLabel(tetraminoesLabel_.get());
    tetraminoesButton_->OnClick([core, menuCore] (TexturedButton &) {
        menuCore->LoadTetraminoesPlaylist();
        core->SetNextState(MakeSharedState<TetraminoesIntroState>());
    });

    aboutButton_.reset(new TexturedButton());
    aboutButton_->Initialize(16, 288, menuCore->Tileset(),
        sf::IntRect(144, 272, 48, 48), sf::IntRect(192, 272, 48, 48),
        sf::IntRect(240, 272, 48, 48));
    aboutButton_->LinkedLabel(aboutLabel_.get());
    aboutButton_->OnClick([core] (TexturedButton &) {
        core->SetNextState(MakeSharedState<AboutState>());
    });

    exitButton_.reset(new TexturedButton());
    exitButton_->Initialize(576, 288, menuCore->Tileset(),
        sf::IntRect(0, 272, 48, 48), sf::IntRect(48, 272, 48, 48),
        sf::IntRect(96, 272, 48, 48));
    exitButton_->LinkedLabel(exitLabel_.get());
    exitButton_->OnClick([] (TexturedButton &) {
        ExitDialogState::ChangeStateFrom(ExitDialogState::DESKTOP_STATE);
    });
}

//--------------------------------------------------------------------------------

void DesktopState::Release() {
    spartanPosition_ = sf::Vector2i();
    spartanTexture_.reset(nullptr);
    snakeLabel_.reset(nullptr);
    minesweeperLabel_.reset(nullptr);
    battleshipLabel_.reset(nullptr);
    blackjackLabel_.reset(nullptr);
    tictactoeLabel_.reset(nullptr);
    pongLabel_.reset(nullptr);
    puzzleLabel_.reset(nullptr);
    alienPartyLabel_.reset(nullptr);
    reversiLabel_.reset(nullptr);
    chessLabel_.reset(nullptr);
    checkersLabel_.reset(nullptr);
    puckmanLabel_.reset(nullptr);
    tetraminoesLabel_.reset(nullptr);
    aboutLabel_.reset(nullptr);
    exitLabel_.reset(nullptr);
    snakeButton_.reset(nullptr);
    minesweeperButton_.reset(nullptr);
    battleshipButton_.reset(nullptr);
    blackjackButton_.reset(nullptr);
    tictactoeButton_.reset(nullptr);
    pongButton_.reset(nullptr);
    puzzleButton_.reset(nullptr);
    alienPartyButton_.reset(nullptr);
    reversiButton_.reset(nullptr);
    chessButton_.reset(nullptr);
    checkersButton_.reset(nullptr);
    puckmanButton_.reset(nullptr);
    tetraminoesButton_.reset(nullptr);
    aboutButton_.reset(nullptr);
    exitButton_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void DesktopState::Draw(const sf::Time & timeDelta) {
    spartanTexture_->Draw(spartanPosition_);
    snakeLabel_->Draw();
    minesweeperLabel_->Draw();
    battleshipLabel_->Draw();
    blackjackLabel_->Draw();
    tictactoeLabel_->Draw();
    pongLabel_->Draw();
    puzzleLabel_->Draw();
    alienPartyLabel_->Draw();
    reversiLabel_->Draw();
    chessLabel_->Draw();
    checkersLabel_->Draw();
    puckmanLabel_->Draw();
    tetraminoesLabel_->Draw();
    aboutLabel_->Draw();
    exitLabel_->Draw();
    snakeButton_->Draw();
    minesweeperButton_->Draw();
    battleshipButton_->Draw();
    blackjackButton_->Draw();
    tictactoeButton_->Draw();
    pongButton_->Draw();
    puzzleButton_->Draw();
    alienPartyButton_->Draw();
    reversiButton_->Draw();
    chessButton_->Draw();
    checkersButton_->Draw();
    puckmanButton_->Draw();
    tetraminoesButton_->Draw();
    aboutButton_->Draw();
    exitButton_->Draw();
}

//--------------------------------------------------------------------------------

void DesktopState::Update(const sf::Time & timeDelta) {
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        ExitDialogState::ChangeStateFrom(ExitDialogState::DESKTOP_STATE);
    } else {
        snakeButton_->Update();
        minesweeperButton_->Update();
        battleshipButton_->Update();
        blackjackButton_->Update();
        tictactoeButton_->Update();
        pongButton_->Update();
        puzzleButton_->Update();
        alienPartyButton_->Update();
        reversiButton_->Update();
        chessButton_->Update();
        checkersButton_->Update();
        puckmanButton_->Update();
        tetraminoesButton_->Update();
        aboutButton_->Update();
        exitButton_->Update();
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

DesktopState::DesktopState() : spartanPosition_(), spartanTexture_(nullptr),
    snakeLabel_(nullptr), minesweeperLabel_(nullptr), battleshipLabel_(nullptr),
    blackjackLabel_(nullptr), tictactoeLabel_(nullptr), pongLabel_(nullptr),
    puzzleLabel_(nullptr), alienPartyLabel_(nullptr), reversiLabel_(nullptr),
    chessLabel_(nullptr), checkersLabel_(nullptr), puckmanLabel_(nullptr),
    tetraminoesLabel_(nullptr), aboutLabel_(nullptr), exitLabel_(nullptr),
    snakeButton_(nullptr), minesweeperButton_(nullptr), battleshipButton_(nullptr),
    blackjackButton_(nullptr), tictactoeButton_(nullptr), pongButton_(nullptr),
    puzzleButton_(nullptr), alienPartyButton_(nullptr), reversiButton_(nullptr),
    chessButton_(nullptr), checkersButton_(nullptr), puckmanButton_(nullptr),
    tetraminoesButton_(nullptr), aboutButton_(nullptr), exitButton_(nullptr) {}

//--------------------------------------------------------------------------------

DesktopState::~DesktopState() {}
