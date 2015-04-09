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

#include "TicTacToeManager.h"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/MathUtil.h>
#include <Menu/DesktopState.h>
#include <Games/TicTacToe/TicTacToeGameState.h>
#include <Games/TicTacToe/TicTacToeGameMenuState.h>
#include <Games/TicTacToe/TicTacToeMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define P1SEL_STATE   0
#define P1PUT_STATE   1
#define P2SEL_STATE   2
#define P2PUT_STATE   3
#define AISEL_STATE   4
#define AIPUT_STATE   5
#define P1WIN_STATE   6
#define P2WIN_STATE   7
#define AIWIN_STATE   8

#define MAX_PAUSE_INTERVAL  500

#define BOARD_SIDE  3
#define MAX_PIECES  3

#define EMPTY_CELL    0
#define PLAYER1_CELL  1
#define PLAYER2_CELL  2

//********************************************************************************
// InnerData
//********************************************************************************

struct TicTacToeManager::InnerData {
    SharedTexture tileset;
    Texture2D crossTexture;
    Texture2D circleTexture;
    Texture2D boardTexture;
    Texture2D selectorTexture;

    int currentTime;
    int numberOfPlayers;
    int playerOnePieces;
    int playerTwoPieces;
    int board[BOARD_SIDE][BOARD_SIDE];
    sf::IntRect areas[BOARD_SIDE][BOARD_SIDE];
    sf::Vector2i selectedCell;
    sf::Vector2i aiNextCell;

    bool previousMouseLeft;
    bool mouseLeft;

    int substate;
    std::unique_ptr<SimpleLabel> upLabel;
    std::unique_ptr<SimpleLabel> downLabel;

    Sound keyboardSound;
    Sound clickSound;

    bool IsMouseLeftUp() {
        return !mouseLeft && previousMouseLeft;
    }
};

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & TicTacToeManager::Tileset() { return data_->tileset; }

//--------------------------------------------------------------------------------

Sound & TicTacToeManager::KeyboardSound() { return data_->keyboardSound; }
Sound & TicTacToeManager::ClickSound()    { return data_->clickSound;    }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void TicTacToeManager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();

        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->tileset = core->LoadTexture("Content/Textures/TicTacToe.png");
        data_->crossTexture.Load(data_->tileset, sf::IntRect(408, 122, 100, 100));
        data_->circleTexture.Load(data_->tileset, sf::IntRect(308, 122, 100, 100));
        data_->boardTexture.Load(data_->tileset, sf::IntRect(0, 122, 308, 308));
        data_->selectorTexture.Load(data_->tileset, sf::IntRect(103, 225, 102, 102));

        // Set the HUD data of the game.
        data_->upLabel.reset(new SimpleLabel("", 0, 7,
            AtariPalette::Hue00Lum14, core->Retro70Font()));

        data_->downLabel.reset(new SimpleLabel("", 0, 341,
            AtariPalette::Hue01Lum14, core->Retro70Font()));

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void TicTacToeManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();
        data_->clickSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->clickSound.Unload();

        // Unload the textures of the game.
        data_->tileset = nullptr;

        // Remove the inner data of the game.
        data_.reset(nullptr);
    }

    // Change the initialized flag and set the next state.
    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//********************************************************************************
// Game methods
//********************************************************************************

/**
 * Starts a new game.
 */
void TicTacToeManager::StartGame(int numberOfPlayers) {
    auto * core = CoreManager::Instance();

    // Initialize the current game values.
    data_->numberOfPlayers = numberOfPlayers;
    data_->playerOnePieces = 0;
    data_->playerTwoPieces = 0;
    data_->selectedCell = sf::Vector2i(-1, -1);

    const int X_BASE = 167, Y_BASE = 27;
    int w = data_->selectorTexture.Width(),
        h = data_->selectorTexture.Height();
    for (int i = 0; i < BOARD_SIDE; ++i) {
        for (int j = 0; j < BOARD_SIDE; ++j) {
            data_->board[i][j] = 0;
            data_->areas[i][j] = sf::IntRect(
                X_BASE + w * j,
                Y_BASE + h * i,
                w, h
            );
        }
    }

    data_->previousMouseLeft = false;
    data_->mouseLeft = core->IsButtonPressed(sf::Mouse::Left);
    data_->currentTime = 0;
    changeSubstate(P1PUT_STATE);

    // Go to the game state to run the current new game.
    core->SetNextState(MakeSharedState<TicTacToeGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void TicTacToeManager::DrawGame() {
    const int X_BASE = 166, Y_BASE = 26;
    const int X_OFFSET = 1, Y_OFFSET = 1;

    data_->upLabel->Draw();
    data_->boardTexture.Draw(X_BASE, Y_BASE, AtariPalette::Hue00Lum04);

    auto * core = CoreManager::Instance();
    auto & mousePosition = core->GetMousePosition();
    for (int i = 0; i < BOARD_SIDE; ++i) {
        for (int j = 0; j < BOARD_SIDE; ++j) {
            // Get the cell data.
            auto item = data_->board[i][j];
            auto rect = data_->areas[i][j];
            // Draw the current piece.
            if (item == 1) {
                data_->crossTexture.Draw(rect.left + X_OFFSET, rect.top + Y_OFFSET);
            } else if (item == 2) {
                data_->circleTexture.Draw(rect.left + X_OFFSET, rect.top + Y_OFFSET);
            }
            // Draw the selector.
            if (data_->selectedCell.x == j && data_->selectedCell.y == i) {
                data_->selectorTexture.Draw(rect.left, rect.top, AtariPalette::Hue10Lum02);
            }
            if (data_->substate < AISEL_STATE && MathUtil::PointInside(mousePosition, rect)) {
                data_->selectorTexture.Draw(rect.left, rect.top, AtariPalette::Hue10Lum06);
            }
        }
    }

    data_->downLabel->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void TicTacToeManager::UpdateGame(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto & mousePosition = core->GetMousePosition();
    data_->previousMouseLeft = data_->mouseLeft;
    data_->mouseLeft = core->IsButtonPressed(sf::Mouse::Left);

    if (data_->substate == P1WIN_STATE || data_->substate == P2WIN_STATE ||
        data_->substate == AIWIN_STATE) {
        // The exit state is when the game is over. We'll need only to wait
        // to any button to be pressed to set the next state of the game.
        if (Keyboard::IsAnyKeyUp() || data_->IsMouseLeftUp()) {
            core->SetNextState(MakeSharedState<TicTacToeMenuState>());
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        // In the other substates the player can press escape and invoke the menu.
        // This will invoke the substate pause and call the game menu state.
        KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<TicTacToeGameMenuState>());

    } else if (data_->substate == P1SEL_STATE) {
        // Check if the player have selected a player cell.
        if (data_->IsMouseLeftUp()) {
            auto coords = getBoardCoords(mousePosition);
            if (coords.x != -1 && data_->board[coords.y][coords.x] == PLAYER1_CELL) {
                data_->selectedCell = coords;
                changeSubstate(P1PUT_STATE);
            }
        }

    } else if (data_->substate == P1PUT_STATE) {
        // Check if the player have selected an empty cell.
        if (data_->IsMouseLeftUp()) {
            auto coords = getBoardCoords(mousePosition);
            if (coords.x != -1) {
                if (data_->board[coords.y][coords.x] == EMPTY_CELL) {
                    // Make the move.
                    if (data_->playerOnePieces < MAX_PIECES) {
                        data_->board[coords.y][coords.x] = PLAYER1_CELL;
                        data_->playerOnePieces++;
                    } else {
                        data_->board[data_->selectedCell.y][data_->selectedCell.x] = EMPTY_CELL;
                        data_->board[coords.y][coords.x] = PLAYER1_CELL;
                        data_->selectedCell = sf::Vector2i(-1, -1);
                    }
                    // Check the victory condition and change to the next state.
                    if (checkVictory(PLAYER1_CELL)) {
                        changeSubstate(P1WIN_STATE);
                    } else if (data_->numberOfPlayers < 2) {
                        changeSubstate(data_->playerTwoPieces < MAX_PIECES ?
                                       AIPUT_STATE : AISEL_STATE);
                    } else {
                        changeSubstate(data_->playerTwoPieces < MAX_PIECES ?
                                       P2PUT_STATE : P2SEL_STATE);
                    }
                } else if (coords == data_->selectedCell) {
                    // Unselect the selected cell.
                    data_->selectedCell = sf::Vector2i(-1, -1);
                    changeSubstate(P1SEL_STATE);
                }
            }
        }

    } else if (data_->substate == P2SEL_STATE) {
        // Check if the player have selected a player cell.
        if (data_->IsMouseLeftUp()) {
            auto coords = getBoardCoords(mousePosition);
            if (coords.x != -1 && data_->board[coords.y][coords.x] == PLAYER2_CELL) {
                data_->selectedCell = coords;
                changeSubstate(P2PUT_STATE);
            }
        }

    } else if (data_->substate == P2PUT_STATE) {
        // Check if the player have selected an empty cell.
        if (data_->IsMouseLeftUp()) {
            auto coords = getBoardCoords(mousePosition);
            if (coords.x != -1) {
                if (data_->board[coords.y][coords.x] == EMPTY_CELL) {
                    // Make the move.
                    if (data_->playerTwoPieces < MAX_PIECES) {
                        data_->board[coords.y][coords.x] = PLAYER2_CELL;
                        data_->playerTwoPieces++;
                    } else {
                        data_->board[data_->selectedCell.y][data_->selectedCell.x] = EMPTY_CELL;
                        data_->board[coords.y][coords.x] = PLAYER2_CELL;
                        data_->selectedCell = sf::Vector2i(-1, -1);
                    }
                    // Check the victory condition and change to the next state.
                    if (checkVictory(PLAYER2_CELL)) {
                        changeSubstate(P2WIN_STATE);
                    } else {
                        changeSubstate(data_->playerOnePieces < MAX_PIECES ?
                                       P1PUT_STATE : P1SEL_STATE);
                    }
                } else if (coords == data_->selectedCell) {
                    // Unselect the selected cell.
                    data_->selectedCell = sf::Vector2i(-1, -1);
                    changeSubstate(P1SEL_STATE);
                }
            }
        }

    } else if (data_->substate == AISEL_STATE) {
        data_->currentTime += timeDelta.asMilliseconds();
        if (data_->currentTime >= MAX_PAUSE_INTERVAL) {
            data_->currentTime = 0;

            // Select the next move.
            aiTakePiece();
            changeSubstate(AIPUT_STATE);
        }

    } else if (data_->substate == AIPUT_STATE) {
        data_->currentTime += timeDelta.asMilliseconds();
        if (data_->currentTime >= MAX_PAUSE_INTERVAL) {
            data_->currentTime = 0;

            // Make the move.
            aiMovePiece();
            if (data_->playerTwoPieces < MAX_PIECES) {
                data_->board[data_->aiNextCell.y][data_->aiNextCell.x] = PLAYER2_CELL;
                data_->playerTwoPieces++;
            } else {
                data_->board[data_->selectedCell.y][data_->selectedCell.x] = EMPTY_CELL;
                data_->board[data_->aiNextCell.y][data_->aiNextCell.x] = PLAYER2_CELL;
                data_->selectedCell = sf::Vector2i(-1, -1);
            }
            // Check the victory condition and change to the next state.
            if (checkVictory(PLAYER2_CELL)) {
                changeSubstate(AIWIN_STATE);
            } else {
                changeSubstate(data_->playerOnePieces < MAX_PIECES ?
                               P1PUT_STATE : P1SEL_STATE);
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Converts a point coordinates to the board coordinates.
 */
sf::Vector2i TicTacToeManager::getBoardCoords(const sf::Vector2i & point) {
    for (int i = 0; i < BOARD_SIDE; ++i) {
        for (int j = 0; j < BOARD_SIDE; ++j) {
            if (MathUtil::PointInside(point, data_->areas[i][j])) {
                return sf::Vector2i(j, i);
            }
        }
    }
    return sf::Vector2i(-1, -1);
}

//--------------------------------------------------------------------------------

/**
 * Checks if a player has the victory condition.
 */
bool TicTacToeManager::checkVictory(int player) {
    auto & board = data_->board;

    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }

    for (int i = 0; i < BOARD_SIDE; ++i) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------

/**
 * Checks if a couple of point has any kind of potential to the machine.
 */
bool TicTacToeManager::checkPotential(int enemy, const sf::Vector2i & p1, const sf::Vector2i & p2) {
    if (p1.x == p2.x) {
        // Both pieces are in the same column.
        for (int i = 0; i < BOARD_SIDE; ++i) {
            if (data_->board[i][p1.x] == enemy) {
                return false;
            }
        }
        return true;
    } else if (p1.y == p2.y) {
        // Both pieces are in the same row.
        for (int i = 0; i < BOARD_SIDE; ++i) {
            if (data_->board[p1.y][i] == enemy) {
                return false;
            }
        }
        return true;
    } else {
        auto dif = p2 - p1;
        if (std::abs(dif.x) == std::abs(dif.y)) {
            if (p1.x == p1.y && p2.x == p2.y) {
                // Both pieces are in the same diagonal.
                for (int i = 0; i < BOARD_SIDE; ++i) {
                    if (data_->board[i][i] == enemy) {
                        return false;
                    }
                }
                return true;
            } else if (p1.x == (2 - p1.y) && p2.x == (2 - p2.y)) {
                // Both pieces are in the same diagonal.
                for (int i = 0; i < BOARD_SIDE; ++i) {
                    if (data_->board[2 - i][i] == enemy) {
                        return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

/**
 * The machine selects a piece to move.
 */
void TicTacToeManager::aiTakePiece() {
    std::vector<sf::Vector2i> pieces, safePieces;
    for (int i = 0; i < BOARD_SIDE; ++i) {
        for (int j = 0; j < BOARD_SIDE; ++j) {
            if (data_->board[i][j] == PLAYER2_CELL) {
                auto coords = sf::Vector2i(j, i);
                // Add the current coordinates.
                pieces.push_back(coords);
                // Check if the player 1 gets the victory.
                data_->board[i][j] = PLAYER1_CELL;
                bool p1victory = checkVictory(PLAYER1_CELL);
                data_->board[i][j] = PLAYER2_CELL;
                // When there is no victory, we'll add the coordinates.
                if (!p1victory) {
                    safePieces.push_back(coords);
                }
            }
        }
    }

    auto * core = CoreManager::Instance();
    if (safePieces.empty()) {
        // No safe moves, get a random one.
        data_->selectedCell = pieces[core->Random(pieces.size())];

    } else {
        data_->selectedCell = sf::Vector2i(-1, -1);

        // Check if there is a safe move with potential.
        for (unsigned int i = 0; i < safePieces.size(); ++i) {
            bool hasPotential = false;
            for (unsigned int j = 0; j < pieces.size(); ++j) {
                if (safePieces[i] != pieces[j] &&
                    checkPotential(PLAYER1_CELL, safePieces[i], pieces[j])) {
                    hasPotential = true;
                    break;
                }
            }
            if (!hasPotential) {
                data_->selectedCell = safePieces[i];
                break;
            }
        }

        // If there is no potential move choose a random one.
        if (data_->selectedCell.x == -1) {
            data_->selectedCell = safePieces[core->Random(safePieces.size())];
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * The machine selects the destination of the piece to move.
 */
void TicTacToeManager::aiMovePiece() {
    std::vector<sf::Vector2i> piecesCoords, enemyCoords, emptyCoords, winCoords, loseCoords;
    for (int i = 0; i < BOARD_SIDE; ++i) {
        for (int j = 0; j < BOARD_SIDE; ++j) {
            if (data_->board[i][j] == EMPTY_CELL) {
                // Add the current coordinates.
                auto coords = sf::Vector2i(j, i);
                emptyCoords.push_back(coords);

                // Check if the player 2 gets the victory.
                data_->board[data_->selectedCell.y][data_->selectedCell.x] = EMPTY_CELL;
                data_->board[i][j] = PLAYER2_CELL;
                bool victory = checkVictory(PLAYER2_CELL);
                data_->board[i][j] = EMPTY_CELL;
                data_->board[data_->selectedCell.y][data_->selectedCell.x] = PLAYER2_CELL;
                // When there is victory, we'll add the coordinates.
                if (victory) {
                    winCoords.push_back(coords);
                }

                // Check if the player 1 gets the victory.
                data_->board[i][j] = PLAYER1_CELL;
                victory = checkVictory(PLAYER1_CELL);
                data_->board[i][j] = EMPTY_CELL;
                // When there is victory, we'll add the coordinates.
                if (victory) {
                    loseCoords.push_back(coords);
                }
            } else if (data_->board[i][j] == PLAYER2_CELL) {
                // Get the pieces that are not going to move.
                auto coords = sf::Vector2i(j, i);
                if (coords != data_->selectedCell) {
                    piecesCoords.push_back(coords);
                }
            } else if (data_->board[i][j] == PLAYER1_CELL) {
                // Get the enemy pieces.
                auto coords = sf::Vector2i(j, i);
                enemyCoords.push_back(coords);
            }
        }
    }

    auto * core = CoreManager::Instance();
    if (!winCoords.empty()) {
        // Can you win? Choose victory!
        data_->aiNextCell = winCoords[core->Random(winCoords.size())];

    } else if (!loseCoords.empty()) {
        // Are you going to lose? Try to avoid it.
        data_->aiNextCell = loseCoords[core->Random(loseCoords.size())];

    } else {
        // I don't know what to do.
        data_->aiNextCell = sf::Vector2i(-1, -1);
        if (data_->playerTwoPieces == 0) {
            // I don't have any piece inside the board.
            if (data_->board[0][0] == PLAYER1_CELL) {
                // Take a side to the player 1.
                if (core->Random(2)) {
                    data_->aiNextCell = sf::Vector2i(0, 1);
                } else {
                    data_->aiNextCell = sf::Vector2i(1, 0);
                }

            } else if (data_->board[0][2] == PLAYER1_CELL) {
                // Take a side to the player 1.
                if (core->Random(2)) {
                    data_->aiNextCell = sf::Vector2i(0, 1);
                } else {
                    data_->aiNextCell = sf::Vector2i(1, 2);
                }

            } else if (data_->board[2][0] == PLAYER1_CELL) {
                // Take a side to the player 1.
                if (core->Random(2)) {
                    data_->aiNextCell = sf::Vector2i(2, 1);
                } else {
                    data_->aiNextCell = sf::Vector2i(1, 0);
                }

            } else if (data_->board[2][2] == PLAYER1_CELL) {
                // Take a side to the player 1.
                if (core->Random(2)) {
                    data_->aiNextCell = sf::Vector2i(2, 1);
                } else {
                    data_->aiNextCell = sf::Vector2i(1, 2);
                }

            } else if (data_->board[1][1] == EMPTY_CELL) {
                // Take the center.
                data_->aiNextCell = sf::Vector2i(1, 1);

            } else {
                // Take a corner.
                std::vector<sf::Vector2i> emptyCornerCoords;
                emptyCornerCoords.push_back(sf::Vector2i(0, 0));
                emptyCornerCoords.push_back(sf::Vector2i(2, 0));
                emptyCornerCoords.push_back(sf::Vector2i(0, 2));
                emptyCornerCoords.push_back(sf::Vector2i(2, 2));
                data_->aiNextCell = emptyCornerCoords[core->Random(emptyCornerCoords.size())];
            }

        } else if (data_->playerTwoPieces == 1) {
            // I have 1 piece inside the board, lets find the good place of the enemy.
            for (unsigned int i = 0; i < emptyCoords.size(); ++i) {
                int totalPotential = 0;
                for (unsigned int j = 0; j < enemyCoords.size(); ++j) {
                    if (checkPotential(PLAYER2_CELL, emptyCoords[i], enemyCoords[j])) {
                        ++totalPotential;
                    }
                }
                if (totalPotential > 1) {
                    data_->aiNextCell = emptyCoords[i];
                    break;
                }
            }

        } else {
            // I have 2 or more pieces inside the board, lets find a good place.
            for (unsigned int i = 0; i < emptyCoords.size(); ++i) {
                int totalPotential = 0;
                for (unsigned int j = 0; j < piecesCoords.size(); ++j) {
                    if (checkPotential(PLAYER1_CELL, emptyCoords[i], piecesCoords[j])) {
                        ++totalPotential;
                    }
                }
                if (totalPotential > 1) {
                    data_->aiNextCell = emptyCoords[i];
                    break;
                }
            }

            if (data_->aiNextCell.x == -1) {
                // No good place? Lets find the good place of the enemy.
                for (unsigned int i = 0; i < emptyCoords.size(); ++i) {
                    int totalPotential = 0;
                    for (unsigned int j = 0; j < enemyCoords.size(); ++j) {
                        if (checkPotential(PLAYER2_CELL, emptyCoords[i], enemyCoords[j])) {
                            ++totalPotential;
                        }
                    }
                    if (totalPotential > 1) {
                        data_->aiNextCell = emptyCoords[i];
                        break;
                    }
                }
            }
        }

        // No solution? Choose a random one.
        if (data_->aiNextCell.x == -1) {
            data_->aiNextCell = emptyCoords[core->Random(emptyCoords.size())];
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Changes the substate of the game.
 */
void TicTacToeManager::changeSubstate(int value) {
    // Get some data and change the substate.
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    data_->substate = value;

    // Set the HUD values of the substate.
    switch (data_->substate) {
    case P1SEL_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("Player 1, select a piece...");
            data_->downLabel->Text("Menu (ESC)");
        } else {
            data_->upLabel->Text("Jugador 1, seleccione una ficha...");
            data_->downLabel->Text("Menú (ESC)");
        }
        break;

    case P1PUT_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text(data_->playerOnePieces < MAX_PIECES ?
                "Player 1, make your move..." :
                "Player 1, move the selected piece...");
            data_->downLabel->Text("Menu (ESC)");
        } else {
            data_->upLabel->Text(data_->playerOnePieces < MAX_PIECES ?
                "Jugador 1, haga su movimiento..." :
                "Jugador 1, mueva la ficha seleccionada...");
            data_->downLabel->Text("Menú (ESC)");
        }
        break;

    case P2SEL_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("Player 2, select a piece...");
            data_->downLabel->Text("Menu (ESC)");
        } else {
            data_->upLabel->Text("Jugador 2, seleccione una ficha...");
            data_->downLabel->Text("Menú (ESC)");
        }
        break;

    case P2PUT_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text(data_->playerTwoPieces < MAX_PIECES ?
                "Player 2, make your move..." :
                "Player 2, move the selected piece...");
            data_->downLabel->Text("Menu (ESC)");
        } else {
            data_->upLabel->Text(data_->playerTwoPieces < MAX_PIECES ?
                "Jugador 2, haga su movimiento..." :
                "Jugador 2, mueva la ficha seleccionada...");
            data_->downLabel->Text("Menú (ESC)");
        }
        break;

    case AISEL_STATE:
        data_->currentTime = 0;
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("The machine is going to select a piece...");
            data_->downLabel->Text("Menu (ESC)");
        } else {
            data_->upLabel->Text("La máquina va a seleccionar ficha...");
            data_->downLabel->Text("Menú (ESC)");
        }
        break;

    case AIPUT_STATE:
        data_->currentTime = 0;
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("The machine is going to move...");
            data_->downLabel->Text("Menu (ESC)");
        } else {
            data_->upLabel->Text("La máquina va a mover...");
            data_->downLabel->Text("Menú (ESC)");
        }
        break;

    case P1WIN_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue01Lum14);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("Player 1 wins!");
            data_->downLabel->Text("Press any key to continue...");
        } else {
            data_->upLabel->Text("¡Jugador 1 gana!");
            data_->downLabel->Text("Pulse cualquier tecla para continuar...");
        }
        break;

    case P2WIN_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue01Lum14);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("Player 2 wins!");
            data_->downLabel->Text("Press any key to continue...");
        } else {
            data_->upLabel->Text("¡Jugador 2 gana!");
            data_->downLabel->Text("Pulse cualquier tecla para continuar...");
        }
        break;

    case AIWIN_STATE:
        data_->upLabel->Color(AtariPalette::Hue00Lum14);
        data_->downLabel->Color(AtariPalette::Hue01Lum14);
        if (language == TEXT_LANGUAGE_ENGLISH) {
            data_->upLabel->Text("Machine wins!");
            data_->downLabel->Text("Press any key to continue...");
        } else {
            data_->upLabel->Text("¡La máquina gana!");
            data_->downLabel->Text("Pulse cualquier tecla para continuar...");
        }
        break;
    }

    // Set the right position of some elements of the HUD.
    GUIUtil::CenterLabel(data_->upLabel, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(data_->downLabel, CoreManager::HALF_LOW_WIDTH);
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
TicTacToeManager * TicTacToeManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
TicTacToeManager::TicTacToeManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
TicTacToeManager::~TicTacToeManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
TicTacToeManager * TicTacToeManager::Instance() {
    if (!instance_) {
        instance_ = new TicTacToeManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
TicTacToeManager & TicTacToeManager::Reference() {
    return *(Instance());
}
