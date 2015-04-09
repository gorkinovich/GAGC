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

#include "ChessManager.h"
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
#include <Games/Chess/ChessGameData.h>
#include <Games/Chess/ChessSaveGames.h>
#include <Games/Chess/ChessGameState.h>
#include <Games/Chess/ChessMiniMax.h>
#include <Games/Chess/ChessMenuState.h>
#include <Games/Chess/ChessGameOverState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int MAX_TEXTURES   = 4;
const int MOUSE_CURSOR   = 0;
const int PRESSED_CURSOR = 1;
const int SELECTED_PIECE = 2;
const int POSSIBLE_MOVE  = 3;

const int BOARD_BOTTOM_Y = ChessGameData::BOARD_SIZE * ChessGameData::CELL_WIDTH;
const int BOARD_LAST_ROW = BOARD_BOTTOM_Y - ChessGameData::CELL_WIDTH;

const int AI_TIME_INTERVAL = 200;

//********************************************************************************
// InnerData
//********************************************************************************

struct ChessManager::InnerData {
    CoreManager * core;
    SharedTexture tileset;
    Texture2D textures[MAX_TEXTURES];
    Texture2D pieceTextures[ChessGameData::MAX_SIDES][ChessGameData::PIECE_TYPES];
    sf::IntRect boardArea;

    ChessSaveGames saves;

    ChessGameData game;
    sf::Vector2i selectedCell;
    ChessGameData::CoordsVector candidateCells;
    ChessGameData::CoordsVector destinationCells;
    bool anyPawnToChange;

    bool aiExecuteMove;
    int aiCurrentTime;
    ChessMove aiCurrentMove;
    ChessMiniMax solver;

    std::unique_ptr<TextLabel> messageText;
    int errorMessage;

    Sound keyboardSound;
    Sound clickSound;
    Sound wrongSound;

    void ClearSelectCell();
    void SetSelectCell(const sf::Vector2i & coords);
    void MakeMove(const sf::Vector2i & coords);
    void FinishPawnChange();

    void ResetHudAndGoToGameState();
    void UpdateHud();

    sf::Vector2i GetBoardCoords();
    bool IsCandidate(const sf::Vector2i & coords);
    bool IsDestination(const sf::Vector2i & coords);

    int GetBoardX(int col) {
        return boardArea.left + col * ChessGameData::CELL_WIDTH;
    }

    int GetBoardY(int row) {
        return boardArea.top + BOARD_LAST_ROW - row * ChessGameData::CELL_HEIGHT;
    }

    void ClearAllErrorMessages() {
        errorMessage = NO_ERROR;
        UpdateHud();
    }

    void ClearErrorMessage(int error) {
        errorMessage &= (~error);
        UpdateHud();
    }

    void SetErrorMessage(int error) {
        errorMessage |= error;
        UpdateHud();
    }
};

//--------------------------------------------------------------------------------

void ChessManager::InnerData::ClearSelectCell() {
    ClearErrorMessage(ERROR_CANDIDATE | ERROR_DESTINATION);
    selectedCell = sf::Vector2i(-1, -1);
    destinationCells.clear();
}

//--------------------------------------------------------------------------------

void ChessManager::InnerData::SetSelectCell(const sf::Vector2i & coords) {
    ClearErrorMessage(ERROR_CANDIDATE | ERROR_DESTINATION);
    selectedCell = coords;
    game.GetPossibleMoves(destinationCells, coords);
}

//--------------------------------------------------------------------------------

void ChessManager::InnerData::MakeMove(const sf::Vector2i & coords) {
    game.MakeMove(selectedCell, coords);
    if (game.AnyPawnToConvert()) {
        anyPawnToChange = true;
    } else {
        ClearAllErrorMessages();
        game.NextTurn();
        game.GetCandidates(candidateCells);
    }
    ClearSelectCell();
}

//--------------------------------------------------------------------------------

void ChessManager::InnerData::FinishPawnChange() {
    anyPawnToChange = false;
    ClearAllErrorMessages();
    game.NextTurn();
    game.GetCandidates(candidateCells);
}

//--------------------------------------------------------------------------------

void ChessManager::InnerData::ResetHudAndGoToGameState() {
    selectedCell = sf::Vector2i(-1, -1);
    destinationCells.clear();
    aiExecuteMove = false;
    aiCurrentTime = 0;
    aiCurrentMove = ChessMove();
    solver.Initialize(game.Difficulty());
    anyPawnToChange = game.AnyPawnToConvert();
    ClearAllErrorMessages();
    if (game.WhiteCheck()) SetErrorMessage(ERROR_WHITE_CHECK);
    if (game.BlackCheck()) SetErrorMessage(ERROR_BLACK_CHECK);
    game.GetCandidates(candidateCells);
    core->SetNextState(MakeSharedState<ChessGameState>());
}

//--------------------------------------------------------------------------------

void ChessManager::InnerData::UpdateHud() {
    auto language = core->Language();
    std::string text = "";
    if (language == TEXT_LANGUAGE_ENGLISH) {
        if (anyPawnToChange) {
            text += "Choose a type:\n(1) Rook\n(2) Knight\n(3) Bishop\n(4) Queen";
        } else {
            if (game.Turn() == ChessGameData::WHITE_SIDE) {
                text += "White side moves.";
            } else {
                text += "Black side moves.";
            }
            if (errorMessage != NO_ERROR) {
                text += "\n";
                if (game.Turn() == ChessGameData::WHITE_SIDE) {
                    if (errorMessage & ERROR_WHITE_CHECKMATE) {
                        text += "\nYou are in checkmate!";
                    } else if (errorMessage & ERROR_WHITE_CHECK) {
                        text += "\nYou are in check!";
                    }
                } else {
                    if (errorMessage & ERROR_BLACK_CHECKMATE) {
                        text += "\nYou are in checkmate!";
                    } else if (errorMessage & ERROR_BLACK_CHECK) {
                        text += "\nYou are in check!";
                    }
                }
                if (errorMessage & ERROR_CANDIDATE) {
                    text += "\nYou can't select\nthat cell!";
                }
                if (errorMessage & ERROR_DESTINATION) {
                    text += "\nYou can't move there!";
                }
                if (errorMessage & ERROR_CHECK_IF_MOVE) {
                    text += "\nYou get check if\nyou move there!";
                }
                if (errorMessage & ERROR_CANT_CASTLING) {
                    text += "\nYou can't do\nthe castling there!";
                }
            }
        }
    } else {
        if (anyPawnToChange) {
            text += "Elije un tipo:\n(1) Torre\n(2) Caballo\n(3) Alfil\n(4) Reina";
        } else {
            if (game.Turn() == ChessGameData::WHITE_SIDE) {
                text += "Las blancas mueven.";
            } else {
                text += "Las negras mueven.";
            }
            if (errorMessage != NO_ERROR) {
                text += "\n";
                if (game.Turn() == ChessGameData::WHITE_SIDE) {
                    if (errorMessage & ERROR_WHITE_CHECKMATE) {
                        text += "\n¡Estás en jaquemate!";
                    } else if (errorMessage & ERROR_WHITE_CHECK) {
                        text += "\n¡Estás en jaque!";
                    }
                } else {
                    if (errorMessage & ERROR_BLACK_CHECKMATE) {
                        text += "\n¡Estás en jaquemate!";
                    } else if (errorMessage & ERROR_BLACK_CHECK) {
                        text += "\n¡Estás en jaque!";
                    }
                }
                if (errorMessage & ERROR_CANDIDATE) {
                    text += "\n¡Esa celda no se\npuede seleccionar!";
                }
                if (errorMessage & ERROR_DESTINATION) {
                    text += "\n¡No puedes mover ahí!";
                }
                if (errorMessage & ERROR_CHECK_IF_MOVE) {
                    text += "\n¡Si mieves ahí\nestarás en jaque!";
                }
                if (errorMessage & ERROR_CANT_CASTLING) {
                    text += "\n¡No puedes hacer el\nenrroque ahí!";
                }
            }
        }
    }
    messageText->Text(text);
}

//--------------------------------------------------------------------------------

sf::Vector2i ChessManager::InnerData::GetBoardCoords() {
    auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
    if (MathUtil::PointInside(mouseCoords, boardArea)) {
        return sf::Vector2i(
            (mouseCoords.x - boardArea.left) / ChessGameData::CELL_WIDTH,
            (boardArea.top + BOARD_BOTTOM_Y - mouseCoords.y) / ChessGameData::CELL_HEIGHT
        );
    } else {
        return sf::Vector2i(-1, -1);
    }
}

//--------------------------------------------------------------------------------

bool ChessManager::InnerData::IsCandidate(const sf::Vector2i & coords) {
    if (!candidateCells.empty()) {
        auto end = candidateCells.end();
        auto it = std::find(candidateCells.begin(), end, coords);
        if (it != end) return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

bool ChessManager::InnerData::IsDestination(const sf::Vector2i & coords) {
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

SharedTexture & ChessManager::Tileset() {
    return data_->tileset;
}

//--------------------------------------------------------------------------------

int ChessManager::Winner() {
    return data_->game.Winner();
}

//--------------------------------------------------------------------------------

ChessSaveGames & ChessManager::Saves() {
    return data_->saves;
}

//--------------------------------------------------------------------------------

Sound & ChessManager::KeyboardSound() { return data_->keyboardSound; }
Sound & ChessManager::ClickSound()    { return data_->clickSound;    }
Sound & ChessManager::WrongSound()    { return data_->wrongSound;    }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void ChessManager::Initialize() {
    if (!initialized_) {
        // Get memory for the internal data.
        data_.reset(new InnerData());
        data_->core = CoreManager::Instance();

        // Load the textures of the game.
        const int SIDE = ChessGameData::CELL_WIDTH;
        data_->tileset = data_->core->LoadTexture("Content/Textures/Chess.png");
        data_->textures[MOUSE_CURSOR].Load(data_->tileset, sf::IntRect(256, 408, 32, 32));
        data_->textures[PRESSED_CURSOR].Load(data_->tileset, sf::IntRect(288, 408, 32, 32));
        data_->textures[SELECTED_PIECE].Load(data_->tileset, sf::IntRect(360, 0, SIDE, SIDE));
        data_->textures[POSSIBLE_MOVE].Load(data_->tileset, sf::IntRect(400, 0, SIDE, SIDE));
        for (int i = 0; i < ChessGameData::MAX_SIDES; ++i) {
            for (int j = 0; j < ChessGameData::PIECE_TYPES; ++j) {
                data_->pieceTextures[i][j].Load(data_->tileset,
                    sf::IntRect(360 + i * SIDE, 40 + j * SIDE, SIDE, SIDE));
            }
        }
        data_->boardArea = sf::IntRect(64, 20, 320, 320);

        // Set the HUD data of the game.
        data_->messageText.reset(new TextLabel("", 464, 192,
            AtariPalette::Hue01Lum14, data_->core->Retro70Font()));

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");
        data_->wrongSound.Load("Content/Sounds/SharedWrong.wav");

        // Loads the previous saved data.
        SaveManager::Instance()->ChessLoad();

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void ChessManager::Release() {
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
void ChessManager::DrawMouseCursor() {
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
void ChessManager::StartOnePlayerGame(int side, int difficulty) {
    data_->game.Start(true, difficulty, side);
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Starts a 2 players new game.
 */
void ChessManager::StartTwoPlayersGame() {
    data_->game.Start(false, 0, 0);
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Resets the current game.
 */
void ChessManager::ResetGame() {
    data_->game.Reset();
    data_->ResetHudAndGoToGameState();
}

//--------------------------------------------------------------------------------

/**
 * Checks if the game is not finished.
 */
bool ChessManager::GameNotFinished() {
    if (data_->game.GameOver()) {
        data_->messageText->Text("");
        data_->core->SetNextState(MakeSharedState<ChessGameOverState>());
        return false;
    } else {
        return true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void ChessManager::DrawGame() {
    if (data_->game.IsInside(data_->selectedCell.y, data_->selectedCell.x)) {
        ForEach(data_->destinationCells, [&] (const sf::Vector2i & victim) {
            data_->textures[POSSIBLE_MOVE].Draw(data_->GetBoardX(victim.x),
                data_->GetBoardY(victim.y));
        });
        data_->textures[SELECTED_PIECE].Draw(data_->GetBoardX(data_->selectedCell.x),
            data_->GetBoardY(data_->selectedCell.y));
    }
    data_->game.ForEachInPieces([&] (ChessGameData::Piece & item, int) {
        if (item.NotDead()) {
            data_->pieceTextures[item.side][item.type].Draw(
                data_->GetBoardX(item.position.x),
                data_->GetBoardY(item.position.y)
            );
        }
    });
    data_->messageText->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void ChessManager::UpdateBoard() {
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

        } else if (data_->IsCandidate(boardCoords)) {
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
bool ChessManager::MouseOverBoard() {
    auto boardCoords = data_->GetBoardCoords();
    return boardCoords.x != -1 && boardCoords.y != -1;
}

//--------------------------------------------------------------------------------

/**
 * Gives the user the option to select the transformatio of the pawn.
 */
void ChessManager::SelectPiece() {
    if (data_->anyPawnToChange) {
        if (Keyboard::IsKeyUp(Keyboard::Num1)) {
            data_->game.ConvertPawnToRook();
            data_->FinishPawnChange();

        } else if (Keyboard::IsKeyUp(Keyboard::Num2)) {
            data_->game.ConvertPawnToKnight();
            data_->FinishPawnChange();

        } else if (Keyboard::IsKeyUp(Keyboard::Num3)) {
            data_->game.ConvertPawnToBishop();
            data_->FinishPawnChange();

        } else if (Keyboard::IsKeyUp(Keyboard::Num4)) {
            data_->game.ConvertPawnToQueen();
            data_->FinishPawnChange();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets if there is any pawn to change.
 */
bool ChessManager::PawnToChange() {
    return data_->anyPawnToChange;
}

//--------------------------------------------------------------------------------

/**
 * Updates the machine move.
 */
void ChessManager::UpdateMachine(const sf::Time & timeDelta) {
    if (!data_->game.GameOver()) {
        if (data_->aiExecuteMove) {
            data_->aiCurrentTime += timeDelta.asMilliseconds();
            if (data_->aiCurrentTime >= AI_TIME_INTERVAL) {
                data_->aiCurrentTime -= AI_TIME_INTERVAL;
                data_->ClearAllErrorMessages();
                data_->aiCurrentMove.MakeMove(data_->game);
                data_->game.GetCandidates(data_->candidateCells);
                data_->aiExecuteMove = false;
            }
        } else {
            data_->aiCurrentMove = data_->solver.Execute(data_->game);
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
bool ChessManager::MachineTurn() {
    auto & game = data_->game;
    return game.SinglePlayer() && game.Turn() != game.PlayerSide();
}

//********************************************************************************
// Errors methods
//********************************************************************************

void ChessManager::ClearAllErrorMessages() {
    if (!MachineTurn()) {
        data_->ClearAllErrorMessages();
    }
}

//--------------------------------------------------------------------------------

void ChessManager::ClearErrorMessage(int error) {
    if (!MachineTurn()) {
        data_->ClearErrorMessage(error);
    }
}

//--------------------------------------------------------------------------------

void ChessManager::SetErrorMessage(int error) {
    if (!MachineTurn()) {
        data_->SetErrorMessage(error);
    }
}

//********************************************************************************
// Savegames methods
//********************************************************************************

/**
 * Saves the current game data.
 */
void ChessManager::SaveCurrentGame(int index, const std::string & name) {
    if (0 <= index && index < ChessSaveGames::MAX_ENTRIES) {
        auto & savedata = data_->saves.GetData()[index];
        savedata.used = true;
        savedata.name = name;
        savedata.data = data_->game;
        SaveManager::Instance()->ChessSave();
    }
    data_->core->SetNextState(MakeSharedState<ChessGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Loads a previous game data.
 */
void ChessManager::LoadPreviousGame(int index) {
    if (0 <= index && index < ChessSaveGames::MAX_ENTRIES) {
        auto & savedata = data_->saves.GetData()[index];
        if (savedata.used) {
            data_->game = savedata.data;
            data_->ResetHudAndGoToGameState();
        } else {
            data_->core->SetNextState(MakeSharedState<ChessMenuState>());
        }
    } else {
        data_->core->SetNextState(MakeSharedState<ChessMenuState>());
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
ChessManager * ChessManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
ChessManager::ChessManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
ChessManager::~ChessManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
ChessManager * ChessManager::Instance() {
    if (!instance_) {
        instance_ = new ChessManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
ChessManager & ChessManager::Reference() {
    return *(Instance());
}
