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

#include "ReversiManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/Sound.h>
#include <System/TextLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/Mouse.h>
#include <System/MathUtil.h>
#include <System/ForEach.h>
#include <Menu/DesktopState.h>
#include <Games/SaveManager.h>
#include <Games/Reversi/ReversiGameData.h>
#include <Games/Reversi/ReversiSaveGames.h>
#include <Games/Reversi/ReversiGameState.h>
#include <Games/Reversi/ReversiMiniMax.h>
#include <Games/Reversi/ReversiMenuState.h>
#include <Games/Reversi/ReversiGameOverState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int MAX_TEXTURES   = 5;
const int MOUSE_CURSOR   = 0;
const int PRESSED_CURSOR = 1;
const int POSSIBLE_MOVE  = 4;

const int BOARD_BOTTOM_Y = ReversiGameData::BOARD_SIZE * ReversiGameData::CELL_WIDTH;
const int BOARD_LAST_ROW = BOARD_BOTTOM_Y - ReversiGameData::CELL_WIDTH;

const int NO_ERROR          = 0;
const int ERROR_DESTINATION = 1;
const int ERROR_PASS        = 2;

const int AI_TIME_INTERVAL = 200;

//********************************************************************************
// InnerData
//********************************************************************************

struct ReversiManager::InnerData {
    CoreManager * core;
    SharedTexture tileset;
    Texture2D textures[MAX_TEXTURES];
    sf::IntRect boardArea;

    ReversiSaveGames saves;
    ReversiGameData game;

    bool aiExecuteMove;
    bool aiExecutePass;
    int aiCurrentTime;
    ReversiMove aiCurrentMove;

    std::unique_ptr<TextLabel> messageText;
    int errorMessage;

    Sound keyboardSound;
    Sound clickSound;
    Sound wrongSound;

    void MakeMove(const sf::Vector2i & coords);
    void PassMove();

    void ResetHudAndGoToGameState();
    void ClearErrorMessage();
    void SetErrorMessage(int error);
    void UpdateHud();

    sf::Vector2i GetBoardCoords();

    int GetBoardX(int col) {
        return boardArea.left + col * ReversiGameData::CELL_WIDTH;
    }

    int GetBoardY(int row) {
        return boardArea.top + BOARD_LAST_ROW - row * ReversiGameData::CELL_HEIGHT;
    }
};

//--------------------------------------------------------------------------------

/**
 * Makes a move inside the board.
 */
void ReversiManager::InnerData::MakeMove(const sf::Vector2i & coords) {
    game.MakeMove(coords);
    game.NextTurn();
    ClearErrorMessage();
}

//--------------------------------------------------------------------------------

/**
 * Passes the current turn.
 */
void ReversiManager::InnerData::PassMove() {
    if ((game.Turn() == ReversiGameData::WHITE_SIDE && game.WhiteSideBlocked()) ||
        (game.Turn() == ReversiGameData::BLACK_SIDE && game.BlackSideBlocked())) {
        game.NextTurn();
        ClearErrorMessage();
    } else {
        SetErrorMessage(ERROR_PASS);
    }
}

//--------------------------------------------------------------------------------

/**
 * Starts a new game.
 */
void ReversiManager::InnerData::ResetHudAndGoToGameState() {
    ClearErrorMessage();
    aiExecuteMove = false;
    aiExecutePass = false;
    aiCurrentTime = 0;
    aiCurrentMove = ReversiMove();
    core->SetNextState(MakeSharedState<ReversiGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Clears the error message.
 */
void ReversiManager::InnerData::ClearErrorMessage() {
    SetErrorMessage(NO_ERROR);
}

//--------------------------------------------------------------------------------

/**
 * Sets an error message.
 */
void ReversiManager::InnerData::SetErrorMessage(int error) {
    errorMessage = error;
    UpdateHud();
}

//--------------------------------------------------------------------------------

/**
 * Updates the HUD.
 */
void ReversiManager::InnerData::UpdateHud() {
    auto language = core->Language();
    std::string text = "";
    if (language == TEXT_LANGUAGE_ENGLISH) {
        if (game.Turn() == ReversiGameData::WHITE_SIDE) {
            if (game.WhiteSideBlocked()) {
                text += "White side\nwithout moves...";
            } else {
                text += "White side moves.";
            }
        } else {
            if (game.BlackSideBlocked()) {
                text += "Black side\nwithout moves...";
            } else {
                text += "Black side moves.";
            }
        }
        if (errorMessage != NO_ERROR) {
            text += "\n\n";
            if (errorMessage == ERROR_DESTINATION) {
                text += "You can't move there!";
            } else if (errorMessage == ERROR_PASS) {
                text += "You can't pass!";
            }
        }
    } else {
        if (game.Turn() == ReversiGameData::WHITE_SIDE) {
            if (game.WhiteSideBlocked()) {
                text += "Blancas sin\nmovimientos...";
            } else {
                text += "Las blancas mueven.";
            }
        } else {
            if (game.BlackSideBlocked()) {
                text += "Negras sin\nmovimientos...";
            } else {
                text += "Las negras mueven.";
            }
        }
        if (errorMessage != NO_ERROR) {
            text += "\n\n";
            if (errorMessage == ERROR_DESTINATION) {
                text += "¡No puedes mover ahí!";
            } else if (errorMessage == ERROR_PASS) {
                text += "¡No puedes pasar!";
            }
        }
    }
    messageText->Text(text);
}

//--------------------------------------------------------------------------------

/**
 * Gets current cell position inside the board of the mouse coordinates.
 */
sf::Vector2i ReversiManager::InnerData::GetBoardCoords() {
    auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
    if (MathUtil::PointInside(mouseCoords, boardArea)) {
        return sf::Vector2i(
            (mouseCoords.x - boardArea.left) / ReversiGameData::CELL_WIDTH,
            (boardArea.top + BOARD_BOTTOM_Y - mouseCoords.y) / ReversiGameData::CELL_HEIGHT
        );
    } else {
        return sf::Vector2i(-1, -1);
    }
}

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & ReversiManager::Tileset() {
    return data_->tileset;
}

//--------------------------------------------------------------------------------

int ReversiManager::Winner() {
    return data_->game.Winner();
}

//--------------------------------------------------------------------------------

ReversiSaveGames & ReversiManager::Saves() {
    return data_->saves;
}

//--------------------------------------------------------------------------------

Sound & ReversiManager::KeyboardSound() { return data_->keyboardSound; }
Sound & ReversiManager::ClickSound()    { return data_->clickSound;    }
Sound & ReversiManager::WrongSound()    { return data_->wrongSound;    }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void ReversiManager::Initialize() {
    if (!initialized_) {
        // Get memory for the internal data.
        data_.reset(new InnerData());
        data_->core = CoreManager::Instance();

        // Load the textures of the game.
        data_->tileset = data_->core->LoadTexture("Content/Textures/Reversi.png");
        data_->textures[MOUSE_CURSOR].Load(data_->tileset, sf::IntRect(256, 408, 32, 32));
        data_->textures[PRESSED_CURSOR].Load(data_->tileset, sf::IntRect(288, 408, 32, 32));
        data_->textures[ReversiGameData::WHITE_SIDE].Load(data_->tileset, sf::IntRect(360, 0, 40, 40));
        data_->textures[ReversiGameData::BLACK_SIDE].Load(data_->tileset, sf::IntRect(400, 0, 40, 40));
        data_->textures[POSSIBLE_MOVE].Load(data_->tileset, sf::IntRect(440, 0, 40, 40));
        data_->boardArea = sf::IntRect(64, 20, 320, 320);

        // Set the HUD data of the game.
        data_->messageText.reset(new TextLabel("", 464, 228,
            AtariPalette::Hue01Lum14, data_->core->Retro70Font()));

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");
        data_->wrongSound.Load("Content/Sounds/SharedWrong.wav");

        // Loads the previous saved data.
        SaveManager::Instance()->ReversiLoad();

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void ReversiManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();
        data_->clickSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->clickSound.Unload();
        data_->wrongSound.Unload();

        // Unload the textures of the game.
        data_->tileset = nullptr;

        // Remove the inner data of the game.
        data_.reset(nullptr);
    }

    // Change the initialized flag and set the next state.
    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the mouse cursor.
 */
void ReversiManager::DrawMouseCursor() {
    auto & mouseCoords = data_->core->GetMousePosition();
    if (Mouse::isButtonPressed(Mouse::Left)) {
        data_->textures[PRESSED_CURSOR].Draw(mouseCoords);
    } else {
        data_->textures[MOUSE_CURSOR].Draw(mouseCoords);
    }
}

//********************************************************************************
// Game methods
//********************************************************************************

/**
 * Starts a 1 player new game.
 */
void ReversiManager::StartOnePlayerGame(int side, int difficulty) {
    data_->game.Start(true, difficulty, side);
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Starts a 2 players new game.
 */
void ReversiManager::StartTwoPlayersGame() {
    data_->game.Start(false, 0, 0);
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Resets the current game.
 */
void ReversiManager::ResetGame() {
    data_->game.Reset();
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Checks if the game is not finished.
 */
bool ReversiManager::GameNotFinished() {
    if (data_->game.GameOver()) {
        data_->messageText->Text("");
        data_->core->SetNextState(MakeSharedState<ReversiGameOverState>());
        return false;
    } else {
        return true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void ReversiManager::DrawGame() {
    ForEach(data_->game.Candidates(), [&] (const sf::Vector2i & victim) {
        data_->textures[POSSIBLE_MOVE].Draw(data_->GetBoardX(victim.x),
            data_->GetBoardY(victim.y));
    });
    data_->game.ForEachInBoard([&] (int item, int i, int j) {
        if (item != ReversiGameData::EMPTY_CELL) {
            data_->textures[item].Draw(data_->GetBoardX(j), data_->GetBoardY(i));
        }
    });
    data_->messageText->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void ReversiManager::UpdateBoard() {
    auto boardCoords = data_->GetBoardCoords();
    if (data_->game.IsInside(boardCoords) && Mouse::IsButtonUp(Mouse::Left)) {
        if (data_->game.IsCandidate(boardCoords)) {
            ClickSound().Play();
            data_->MakeMove(boardCoords);
        } else {
            WrongSound().Play();
            data_->SetErrorMessage(ERROR_DESTINATION);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the mouse is over the board.
 */
bool ReversiManager::MouseOverBoard() {
    auto boardCoords = data_->GetBoardCoords();
    return boardCoords.x != -1 && boardCoords.y != -1;
}

//--------------------------------------------------------------------------------

/**
 * Passes the current turn when the side have no moves.
 */
void ReversiManager::PassTurn() {
    data_->PassMove();
}

//--------------------------------------------------------------------------------

/**
 * Updates the machine move.
 */
void ReversiManager::UpdateMachine(const sf::Time & timeDelta) {
    if (!data_->game.GameOver()) {
        if (data_->aiExecuteMove) {
            data_->aiCurrentTime += timeDelta.asMilliseconds();
            if (data_->aiCurrentTime >= AI_TIME_INTERVAL) {
                data_->aiCurrentTime -= AI_TIME_INTERVAL;
                if (data_->aiExecutePass) {
                    data_->PassMove();
                } else {
                    data_->MakeMove(data_->aiCurrentMove.step);
                }
                data_->aiExecuteMove = false;
            }
        } else {
            if (data_->game.PlayerSide() == ReversiGameData::WHITE_SIDE) {
                data_->aiExecutePass = data_->game.BlackSideBlocked();
            } else {
                data_->aiExecutePass = data_->game.WhiteSideBlocked();
            }
            if (!data_->aiExecutePass) {
                if (data_->game.BeginningState()) {
                    auto index = data_->core->Random(data_->game.Candidates().size());
                    data_->aiCurrentMove = ReversiMove();
                    data_->aiCurrentMove.step = data_->game.Candidates()[index];
                } else {
                    ReversiMiniMax solver;
                    data_->aiCurrentMove = solver.Execute(data_->game);
                }
            }
            data_->aiCurrentTime = 0;
            data_->aiExecuteMove = true;
        }
    }
    data_->UpdateHud();
}

//--------------------------------------------------------------------------------

/**
 * Checks if the current turn is the machine's turn.
 */
bool ReversiManager::MachineTurn() {
    auto & game = data_->game;
    return game.SinglePlayer() && game.Turn() != game.PlayerSide();
}

//********************************************************************************
// Savegames methods
//********************************************************************************

/**
 * Saves the current game data.
 */
void ReversiManager::SaveCurrentGame(int index, const std::string & name) {
    if (0 <= index && index < ReversiSaveGames::MAX_ENTRIES) {
        auto & savedata = data_->saves.GetData()[index];
        savedata.used = true;
        savedata.name = name;
        savedata.data = data_->game;
        SaveManager::Instance()->ReversiSave();
    }
    data_->core->SetNextState(MakeSharedState<ReversiGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Loads a previous game data.
 */
void ReversiManager::LoadPreviousGame(int index) {
    if (0 <= index && index < ReversiSaveGames::MAX_ENTRIES) {
        auto & savedata = data_->saves.GetData()[index];
        if (savedata.used) {
            data_->game = savedata.data;
            data_->ResetHudAndGoToGameState();
        } else {
            data_->core->SetNextState(MakeSharedState<ReversiMenuState>());
        }
    } else {
        data_->core->SetNextState(MakeSharedState<ReversiMenuState>());
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
ReversiManager * ReversiManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
ReversiManager::ReversiManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
ReversiManager::~ReversiManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
ReversiManager * ReversiManager::Instance() {
    if (!instance_) {
        instance_ = new ReversiManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
ReversiManager & ReversiManager::Reference() {
    return *(Instance());
}
