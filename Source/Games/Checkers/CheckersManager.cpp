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

#include "CheckersManager.h"
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
#include <Games/Checkers/CheckersGameData.h>
#include <Games/Checkers/CheckersSaveGames.h>
#include <Games/Checkers/CheckersGameState.h>
#include <Games/Checkers/CheckersMiniMax.h>
#include <Games/Checkers/CheckersMenuState.h>
#include <Games/Checkers/CheckersGameOverState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int MAX_TEXTURES   = 8;
const int MOUSE_CURSOR   = 0;
const int PRESSED_CURSOR = 1;
const int SELECTED_PIECE = 2;
const int POSSIBLE_MOVE  = 3;

const int BOARD_BOTTOM_Y = CheckersGameData::BOARD_SIZE * CheckersGameData::CELL_WIDTH;
const int BOARD_LAST_ROW = BOARD_BOTTOM_Y - CheckersGameData::CELL_WIDTH;

const int NO_ERROR          = 0;
const int ERROR_CANDIDATE   = 1;
const int ERROR_DESTINATION = 2;

const int AI_TIME_INTERVAL = 200;

//********************************************************************************
// InnerData
//********************************************************************************

struct CheckersManager::InnerData {
    CoreManager * core;
    SharedTexture tileset;
    Texture2D textures[MAX_TEXTURES];
    sf::IntRect boardArea;

    CheckersSaveGames saves;

    CheckersGameData game;
    sf::Vector2i selectedCell;
    std::vector<sf::Vector2i> destinationCells;

    bool aiExecuteMove;
    int aiCurrentTime;
    int aiMoveIndex;
    CheckersMove aiCurrentMove;

    std::unique_ptr<TextLabel> messageText;
    int errorMessage;

    Sound keyboardSound;
    Sound clickSound;
    Sound wrongSound;

    void ClearSelectCell();
    void SetSelectCell(const sf::Vector2i & coords);
    void MakeMove(const sf::Vector2i & coords);

    void ResetHudAndGoToGameState();
    void ClearErrorMessage();
    void SetErrorMessage(int error);
    void UpdateHud();

    sf::Vector2i GetBoardCoords();
    bool IsDestination(const sf::Vector2i & coords);

    int GetBoardX(int col) {
        return boardArea.left + col * CheckersGameData::CELL_WIDTH;
    }

    int GetBoardY(int row) {
        return boardArea.top + BOARD_LAST_ROW - row * CheckersGameData::CELL_HEIGHT;
    }
};

//--------------------------------------------------------------------------------

/**
 * Clears the selected cell data.
 */
void CheckersManager::InnerData::ClearSelectCell() {
    ClearErrorMessage();
    selectedCell = sf::Vector2i(-1, -1);
    destinationCells.clear();
}

//--------------------------------------------------------------------------------

/**
 * Sets the selected cell data.
 */
void CheckersManager::InnerData::SetSelectCell(const sf::Vector2i & coords) {
    ClearErrorMessage();
    selectedCell = coords;
    game.GetPossibleMoves(destinationCells, coords);
}

//--------------------------------------------------------------------------------

/**
 * Makes a move inside the board.
 */
void CheckersManager::InnerData::MakeMove(const sf::Vector2i & coords) {
    game.MakeMove(selectedCell, coords);
    game.NextTurn();
    ClearSelectCell();
}

//--------------------------------------------------------------------------------

/**
 * Starts a new game.
 */
void CheckersManager::InnerData::ResetHudAndGoToGameState() {
    selectedCell = sf::Vector2i(-1, -1);
    destinationCells.clear();
    ClearErrorMessage();
    aiExecuteMove = false;
    aiCurrentTime = 0;
    aiMoveIndex = 0;
    aiCurrentMove = CheckersMove();
    core->SetNextState(MakeSharedState<CheckersGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Clears the error message.
 */
void CheckersManager::InnerData::ClearErrorMessage() {
    SetErrorMessage(NO_ERROR);
}

//--------------------------------------------------------------------------------

/**
 * Sets an error message.
 */
void CheckersManager::InnerData::SetErrorMessage(int error) {
    errorMessage = error;
    UpdateHud();
}

//--------------------------------------------------------------------------------

/**
 * Updates the HUD.
 */
void CheckersManager::InnerData::UpdateHud() {
    auto language = core->Language();
    std::string text = "";
    if (language == TEXT_LANGUAGE_ENGLISH) {
        if (game.Turn() == CheckersGameData::WHITE_SIDE) {
            if (game.LastTurnFinished())
                text += "White side moves.";
            else
                text += "White side still\nmoves...";
        } else {
            if (game.LastTurnFinished())
                text += "Black side moves.";
            else
                text += "Black side still\nmoves...";
        }
        if (errorMessage != NO_ERROR) {
            text += "\n\n";
            if (errorMessage == ERROR_CANDIDATE) {
                text += "You can't select\nthat cell!";
            } else if (errorMessage == ERROR_DESTINATION) {
                text += "You can't move there!";
            }
        }
    } else {
        if (game.Turn() == CheckersGameData::WHITE_SIDE) {
            if (game.LastTurnFinished())
                text += "Las blancas mueven.";
            else
                text += "Las blancas siguen\nmoviendo...";
        } else {
            if (game.LastTurnFinished())
                text += "Las negras mueven.";
            else
                text += "Las negras siguen\nmoviendo...";
        }
        if (errorMessage != NO_ERROR) {
            text += "\n\n";
            if (errorMessage == ERROR_CANDIDATE) {
                text += "¡Esa celda no se\npuede seleccionar!";
            } else if (errorMessage == ERROR_DESTINATION) {
                text += "¡No puedes mover ahí!";
            }
        }
    }
    messageText->Text(text);
}

//--------------------------------------------------------------------------------

/**
 * Gets current cell position inside the board of the mouse coordinates.
 */
sf::Vector2i CheckersManager::InnerData::GetBoardCoords() {
    auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
    if (MathUtil::PointInside(mouseCoords, boardArea)) {
        return sf::Vector2i(
            (mouseCoords.x - boardArea.left) / CheckersGameData::CELL_WIDTH,
            (boardArea.top + BOARD_BOTTOM_Y - mouseCoords.y) / CheckersGameData::CELL_HEIGHT
        );
    } else {
        return sf::Vector2i(-1, -1);
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if a coordinates are inside the destination list.
 */
bool CheckersManager::InnerData::IsDestination(const sf::Vector2i & coords) {
    if (!destinationCells.empty()) {
        auto end = destinationCells.end();
        auto it = std::find(destinationCells.begin(), end, coords);
        if (it != end) return true;
    }
    return false;
}

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & CheckersManager::Tileset() {
    return data_->tileset;
}

//--------------------------------------------------------------------------------

int CheckersManager::Winner() {
    return data_->game.Winner();
}

//--------------------------------------------------------------------------------

CheckersSaveGames & CheckersManager::Saves() {
    return data_->saves;
}

//--------------------------------------------------------------------------------

Sound & CheckersManager::KeyboardSound() { return data_->keyboardSound; }
Sound & CheckersManager::ClickSound()    { return data_->clickSound;    }
Sound & CheckersManager::WrongSound()    { return data_->wrongSound;    }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void CheckersManager::Initialize() {
    if (!initialized_) {
        // Get memory for the internal data.
        data_.reset(new InnerData());
        data_->core = CoreManager::Instance();

        // Load the textures of the game.
        data_->tileset = data_->core->LoadTexture("Content/Textures/Checkers.png");
        data_->textures[MOUSE_CURSOR].Load(data_->tileset, sf::IntRect(256, 408, 32, 32));
        data_->textures[PRESSED_CURSOR].Load(data_->tileset, sf::IntRect(288, 408, 32, 32));
        data_->textures[CheckersGameData::WHITE_PIECE].Load(data_->tileset, sf::IntRect(360, 0, 32, 32));
        data_->textures[CheckersGameData::BLACK_PIECE].Load(data_->tileset, sf::IntRect(392, 0, 32, 32));
        data_->textures[CheckersGameData::WHITE_QUEEN_PIECE].Load(data_->tileset, sf::IntRect(424, 0, 32, 32));
        data_->textures[CheckersGameData::BLACK_QUEEN_PIECE].Load(data_->tileset, sf::IntRect(456, 0, 32, 32));
        data_->textures[SELECTED_PIECE].Load(data_->tileset, sf::IntRect(360, 64, 32, 32));
        data_->textures[POSSIBLE_MOVE].Load(data_->tileset, sf::IntRect(392, 64, 32, 32));
        data_->boardArea = sf::IntRect(64, 20, 320, 320);

        // Set the HUD data of the game.
        data_->messageText.reset(new TextLabel("", 464, 192,
            AtariPalette::Hue01Lum14, data_->core->Retro70Font()));

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");
        data_->wrongSound.Load("Content/Sounds/SharedWrong.wav");

        // Loads the previous saved data.
        SaveManager::Instance()->CheckersLoad();

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void CheckersManager::Release() {
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
void CheckersManager::DrawMouseCursor() {
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
void CheckersManager::StartOnePlayerGame(int side, int difficulty) {
    data_->game.Start(true, difficulty, side);
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Starts a 2 players new game.
 */
void CheckersManager::StartTwoPlayersGame() {
    data_->game.Start(false, 0, 0);
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Resets the current game.
 */
void CheckersManager::ResetGame() {
    data_->game.Reset();
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Checks if the game is not finished.
 */
bool CheckersManager::GameNotFinished() {
    if (data_->game.GameOver()) {
        data_->messageText->Text("");
        data_->core->SetNextState(MakeSharedState<CheckersGameOverState>());
        return false;
    } else {
        return true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void CheckersManager::DrawGame() {
    if (data_->game.IsInside(data_->selectedCell.y, data_->selectedCell.x)) {
        ForEach(data_->destinationCells, [&] (const sf::Vector2i & victim) {
            data_->textures[POSSIBLE_MOVE].Draw(data_->GetBoardX(victim.x),
                data_->GetBoardY(victim.y));
        });
        data_->textures[SELECTED_PIECE].Draw(data_->GetBoardX(data_->selectedCell.x),
            data_->GetBoardY(data_->selectedCell.y));
    }
    data_->game.ForEachInBoard([&] (int item, int i, int j) {
        if (item != CheckersGameData::EMPTY_CELL) {
            data_->textures[item].Draw(data_->GetBoardX(j), data_->GetBoardY(i));
        }
    });
    data_->messageText->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void CheckersManager::UpdateBoard() {
    auto boardCoords = data_->GetBoardCoords();
    if (data_->game.IsInside(boardCoords) && Mouse::IsButtonUp(Mouse::Left)) {
        if (data_->game.IsInside(data_->selectedCell)) {
            if (data_->selectedCell == boardCoords) {
                ClickSound().Play();
                data_->ClearSelectCell();

            } else if (data_->IsDestination(boardCoords)) {
                ClickSound().Play();
                data_->MakeMove(boardCoords);

            } else {
                WrongSound().Play();
                data_->SetErrorMessage(ERROR_DESTINATION);
            }

        } else if (data_->game.IsCandidate(boardCoords)) {
            ClickSound().Play();
            data_->SetSelectCell(boardCoords);

        } else {
            WrongSound().Play();
            data_->SetErrorMessage(ERROR_CANDIDATE);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the mouse is over the board.
 */
bool CheckersManager::MouseOverBoard() {
    auto boardCoords = data_->GetBoardCoords();
    return boardCoords.x != -1 && boardCoords.y != -1;
}

//--------------------------------------------------------------------------------

/**
 * Updates the machine move.
 */
void CheckersManager::UpdateMachine(const sf::Time & timeDelta) {
    if (!data_->game.GameOver()) {
        if (data_->aiExecuteMove) {
            data_->aiCurrentTime += timeDelta.asMilliseconds();
            if (data_->aiCurrentTime >= AI_TIME_INTERVAL) {
                data_->aiCurrentTime -= AI_TIME_INTERVAL;
                auto & move = data_->aiCurrentMove.steps[data_->aiMoveIndex];
                data_->aiMoveIndex++;
                data_->game.MakeMove(move.origin, move.destination);
                data_->game.NextTurn();
                if (data_->game.LastTurnFinished()) {
                    data_->aiExecuteMove = false;
                }
            }
        } else {
            CheckersMiniMax solver;
            data_->aiCurrentMove = solver.Execute(data_->game);
            data_->aiMoveIndex = 0;
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
bool CheckersManager::MachineTurn() {
    auto & game = data_->game;
    return game.SinglePlayer() && game.Turn() != game.PlayerSide();
}

//********************************************************************************
// Savegames methods
//********************************************************************************

/**
 * Saves the current game data.
 */
void CheckersManager::SaveCurrentGame(int index, const std::string & name) {
    if (0 <= index && index < CheckersSaveGames::MAX_ENTRIES) {
        auto & savedata = data_->saves.GetData()[index];
        savedata.used = true;
        savedata.name = name;
        savedata.data = data_->game;
        SaveManager::Instance()->CheckersSave();
    }
    data_->core->SetNextState(MakeSharedState<CheckersGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Loads a previous game data.
 */
void CheckersManager::LoadPreviousGame(int index) {
    if (0 <= index && index < CheckersSaveGames::MAX_ENTRIES) {
        auto & savedata = data_->saves.GetData()[index];
        if (savedata.used) {
            data_->game = savedata.data;
            data_->ResetHudAndGoToGameState();
        } else {
            data_->core->SetNextState(MakeSharedState<CheckersMenuState>());
        }
    } else {
        data_->core->SetNextState(MakeSharedState<CheckersMenuState>());
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
CheckersManager * CheckersManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
CheckersManager::CheckersManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
CheckersManager::~CheckersManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
CheckersManager * CheckersManager::Instance() {
    if (!instance_) {
        instance_ = new CheckersManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
CheckersManager & CheckersManager::Reference() {
    return *(Instance());
}
