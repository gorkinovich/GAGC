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

#include "MinesweeperManager.h"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/TexturedButton.h>
#include <System/Sound.h>
#include <System/Mouse.h>
#include <System/MathUtil.h>
#include <System/ForEach.h>
#include <Menu/DesktopState.h>
#include <Games/Minesweeper/MinesweeperGameState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define INITIAL_STATE     0
#define PLAYING_STATE     1
#define WIN_STATE         2
#define LOSE_STATE        3

#define CELL_BUTTON       0
#define CELL_FLAGGED      1
#define CELL_CLICKED      2

#define EMPTY_VALUE       0
#define BOMB_VALUE        9
#define BOOM_VALUE       10
#define FAIL_VALUE       11
#define MAX_TEX_CELLS    12
#define MAX_TEX_NUMBERS  11

#define CELL_WIDTH       16
#define CELL_HEIGHT      16

#define BUTTON_LEFT       0
#define BUTTON_RIGHT      1
#define BUTTON_BOTH       2

//********************************************************************************
// InnerData
//********************************************************************************

struct MinesweeperManager::InnerData {
    SharedTexture tileset;
    SharedImage tilesetImage;
    Texture2D numbersTextures[MAX_TEX_NUMBERS];
    Texture2D cellsTextures[MAX_TEX_CELLS];
    Texture2D buttonNormalTexture;
    Texture2D buttonClickTexture;
    Texture2D buttonFlaggedTexture;

    Texture2D boardTexture;
    sf::Vector2i boardPosition;
    sf::IntRect tableArea;
    sf::Vector2i minesMarkPosition;
    sf::Vector2i timeMarkPosition;

    struct Cell {
        int state;
        int value;
        Cell() : state(CELL_BUTTON), value(EMPTY_VALUE) {}
        Cell(int s, int v) : state(s), value(v) {}
    };

    int numberOfRows;
    int numberOfColumns;
    int numberOfMines;

    int gameState;
    int currentTime;
    int currentMines;
    int cellsToClick;
    std::vector<std::vector<Cell>> board;

    std::vector<sf::Vector2i> directions;

    std::unique_ptr<TexturedButton> resetButton;
    std::unique_ptr<TexturedButton> resetAfterWinButton;
    std::unique_ptr<TexturedButton> resetAfterLoseButton;

    bool mouseBothDown;
    int mouseDownTimeDif;

    Sound keyboardSound;
    Sound clickSound;
    Sound winSound;
    Sound loseSound;

    sf::Vector2i GetTableCoords() {
        auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
        if (MathUtil::PointInside(mouseCoords, tableArea)) {
            return sf::Vector2i(
                (mouseCoords.x - tableArea.left) / CELL_WIDTH,
                (mouseCoords.y - tableArea.top) / CELL_HEIGHT
            );
        } else {
            return sf::Vector2i(-1, -1);
        }
    }

    bool IsInsideTable(int r, int c) {
        return 0 <= r && r < numberOfRows && 0 <= c && c < numberOfColumns;
    }

    bool IsInsideTable(const sf::Vector2i & pos) {
        return IsInsideTable(pos.y, pos.x);
    }

    bool CanBothClick(const Cell & victim) {
        return victim.state == CELL_CLICKED &&
            0 < victim.value && victim.value < BOMB_VALUE;
    }

    bool CanBothClick(int r, int c) {
        return IsInsideTable(r, c) && CanBothClick(board[r][c]);
    }

    bool CanBothClick(const sf::Vector2i & pos) {
        return CanBothClick(pos.y, pos.x);
    }
};

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & MinesweeperManager::Tileset() { return data_->tileset; }

//--------------------------------------------------------------------------------

Sound & MinesweeperManager::KeyboardSound() { return data_->keyboardSound; }
Sound & MinesweeperManager::ClickSound()    { return data_->clickSound;    }
Sound & MinesweeperManager::WinSound()      { return data_->winSound;      }
Sound & MinesweeperManager::LoseSound()     { return data_->loseSound;     }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void MinesweeperManager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();

        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->tileset = core->LoadTexture("Content/Textures/Minesweeper.png");
        data_->tilesetImage = core->LoadImage("Content/Textures/Minesweeper.png");
        data_->buttonNormalTexture.Load(data_->tileset, sf::IntRect(0, 26, 16, 16));
        data_->buttonClickTexture.Load(data_->tileset, sf::IntRect(16, 26, 16, 16));
        data_->buttonFlaggedTexture.Load(data_->tileset, sf::IntRect(32, 26, 16, 16));

        const int NUM_XOS = 13, NUM_SX = 0, NUM_SY = 42;
        for (int i = 0; i < MAX_TEX_NUMBERS; ++i) {
            data_->numbersTextures[i].Load(data_->tileset,
                sf::IntRect(NUM_SX + i * NUM_XOS, NUM_SY, 13, 23));
        }

        const int CEL_XOS = 16, CEL_SX = 64, CEL_SY = 26;
        for (int i = 0; i < MAX_TEX_CELLS; ++i) {
            data_->cellsTextures[i].Load(data_->tileset,
                sf::IntRect(CEL_SX + i * CEL_XOS, CEL_SY, 16, 16));
        }

        // Set the HUD data of the game.
        data_->resetButton.reset(new TexturedButton());
        data_->resetButton->Initialize(0, 0, data_->tileset,
            sf::IntRect(0, 0, 26, 26), sf::IntRect(0, 0, 26, 26),
            sf::IntRect(78, 0, 26, 26));
        data_->resetButton->OnClick([this] (TexturedButton &) {
            ClickSound().Play();
            resetGame();
        });

        data_->resetAfterWinButton.reset(new TexturedButton());
        data_->resetAfterWinButton->Initialize(0, 0, data_->tileset,
            sf::IntRect(156, 0, 26, 26), sf::IntRect(156, 0, 26, 26),
            sf::IntRect(182, 0, 26, 26));
        data_->resetAfterWinButton->OnClick([this] (TexturedButton &) {
            ClickSound().Play();
            resetGame();
        });

        data_->resetAfterLoseButton.reset(new TexturedButton());
        data_->resetAfterLoseButton->Initialize(0, 0, data_->tileset,
            sf::IntRect(104, 0, 26, 26), sf::IntRect(104, 0, 26, 26),
            sf::IntRect(130, 0, 26, 26));
        data_->resetAfterLoseButton->OnClick([this] (TexturedButton &) {
            ClickSound().Play();
            resetGame();
        });

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");
        data_->winSound.Load("Content/Sounds/MinesweeperWin.wav");
        data_->loseSound.Load("Content/Sounds/MinesweeperLose.wav");

        // Add the directions.
        data_->directions.clear();
        data_->directions.push_back(sf::Vector2i(-1, -1));
        data_->directions.push_back(sf::Vector2i( 0, -1));
        data_->directions.push_back(sf::Vector2i( 1, -1));
        data_->directions.push_back(sf::Vector2i( 1,  0));
        data_->directions.push_back(sf::Vector2i( 1,  1));
        data_->directions.push_back(sf::Vector2i( 0,  1));
        data_->directions.push_back(sf::Vector2i(-1,  1));
        data_->directions.push_back(sf::Vector2i(-1,  0));

        // Mouse control data.
        data_->mouseBothDown = false;
        data_->mouseDownTimeDif = 0;

        // Configure an initial game.
        configGame(9, 9, 10);

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void MinesweeperManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();
        data_->clickSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->clickSound.Unload();
        data_->winSound.Unload();
        data_->loseSound.Unload();

        // Unload the textures of the game.
        data_->tileset = nullptr;
        data_->tilesetImage = nullptr;

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
 * Configures a new game.
 */
void MinesweeperManager::ConfigBeginnerGame() {
    configGame(9, 9, 10);
    CoreManager::Instance()->SetNextState(MakeSharedState<MinesweeperGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Configures a new game.
 */
void MinesweeperManager::ConfigIntermediateGame() {
    configGame(16, 16, 40);
    CoreManager::Instance()->SetNextState(MakeSharedState<MinesweeperGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Configures a new game.
 */
void MinesweeperManager::ConfigAdvanceGame() {
    configGame(16, 30, 99);
    CoreManager::Instance()->SetNextState(MakeSharedState<MinesweeperGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Configures a new game.
 */
void MinesweeperManager::configGame(int rows, int cols, int mines) {
    data_->numberOfRows = rows;
    data_->numberOfColumns = cols;
    data_->numberOfMines = mines;
    makeBoardTexture();
    resetGame();
}

//--------------------------------------------------------------------------------

/**
 * Makes the board texture.
 */
void MinesweeperManager::makeBoardTexture() {
    const int BASE_WIDTH = 24, BASE_HEIGHT = 67;
    int tw = CELL_WIDTH * data_->numberOfColumns;
    int th = CELL_HEIGHT * data_->numberOfRows;
    int bw = BASE_WIDTH + tw;
    int bh = BASE_HEIGHT + th;
    sf::Image boardImage;
    boardImage.create(bw, bh, AtariPalette::Hue00Lum10);

    int x0 = 0, x1 = 3, x2 = 9, x3 = 11, x4 = 16, x5 = bw / 2 - 13;
    int x6 = bw - 59, x7 = bw - 11, x8 = bw - 9, x9 = bw - 3;
    int y0 = 0, y1 = 3, y2 = 9, y3 = 11, y4 = 15, y5 = 44, y6 = 46;
    int y7 = 52, y8 = 55, y9 = bh - 12, y10 = bh - 9, y11 = bh - 3;

    sf::IntRect b1tl(249, 0, 3, 3), b1tr(253, 0, 3, 3), b1bl(249, 4, 3, 3), b1br(253, 4, 3, 3);
    sf::IntRect b1t(252, 0, 1, 3), b1b(252, 4, 1, 3), b1l(249, 3, 3, 1), b1r(253, 3, 3, 1);

    boardImage.copy(*(data_->tilesetImage), x0, y0, b1tl);
    boardImage.copy(*(data_->tilesetImage), x9, y0, b1tr);
    boardImage.copy(*(data_->tilesetImage), x0, y11, b1bl);
    boardImage.copy(*(data_->tilesetImage), x9, y11, b1br);

    for (int x = x1; x < x9; ++x) {
        boardImage.copy(*(data_->tilesetImage), x, y0, b1t);
        boardImage.copy(*(data_->tilesetImage), x, y11, b1b);
    }
    for (int y = y1; y < y11; ++y) {
        boardImage.copy(*(data_->tilesetImage), x0, y, b1l);
        boardImage.copy(*(data_->tilesetImage), x9, y, b1r);
    }

    sf::IntRect b2tl(250, 8, 2, 2), b2tr(253, 8, 2, 2), b2bl(250, 11, 2, 2), b2br(253, 11, 2, 2);
    sf::IntRect b2t(252, 8, 1, 2), b2b(252, 11, 1, 2), b2l(250, 10, 2, 1), b2r(253, 10, 2, 1);

    boardImage.copy(*(data_->tilesetImage), x2, y2, b2tl);
    boardImage.copy(*(data_->tilesetImage), x7, y2, b2tr);
    boardImage.copy(*(data_->tilesetImage), x2, y5, b2bl);
    boardImage.copy(*(data_->tilesetImage), x7, y5, b2br);

    for (int x = x3; x < x7; ++x) {
        boardImage.copy(*(data_->tilesetImage), x, y2, b2t);
        boardImage.copy(*(data_->tilesetImage), x, y5, b2b);
    }
    for (int y = y3; y < y5; ++y) {
        boardImage.copy(*(data_->tilesetImage), x2, y, b2l);
        boardImage.copy(*(data_->tilesetImage), x7, y, b2r);
    }

    sf::IntRect b3tl(249, 7, 3, 3), b3tr(253, 7, 3, 3), b3bl(249, 11, 3, 3), b3br(253, 11, 3, 3);
    sf::IntRect b3t(252, 7, 1, 3), b3b(252, 11, 1, 3), b3l(249, 10, 3, 1), b3r(253, 10, 3, 1);

    int x3b = 12, x7b = bw - 12;

    boardImage.copy(*(data_->tilesetImage), x2, y7, b3tl);
    boardImage.copy(*(data_->tilesetImage), x7b, y7, b3tr);
    boardImage.copy(*(data_->tilesetImage), x2, y9, b3bl);
    boardImage.copy(*(data_->tilesetImage), x7b, y9, b3br);

    for (int x = x3b; x < x7b; ++x) {
        boardImage.copy(*(data_->tilesetImage), x, y7, b3t);
        boardImage.copy(*(data_->tilesetImage), x, y9, b3b);
    }
    for (int y = y8; y < y9; ++y) {
        boardImage.copy(*(data_->tilesetImage), x2, y, b3l);
        boardImage.copy(*(data_->tilesetImage), x7b, y, b3r);
    }

    sf::IntRect led(208, 0, 41, 25);
    boardImage.copy(*(data_->tilesetImage), x4, y4, led);
    boardImage.copy(*(data_->tilesetImage), x6, y4, led);

    SharedTexture boardSurface = std::make_shared<sf::Texture>();
    boardSurface->loadFromImage(boardImage);
    data_->boardTexture.Load(boardSurface);
    data_->boardPosition = sf::Vector2i(
        (CoreManager::LOW_WIDTH - data_->boardTexture.Width()) / 2,
        ((CoreManager::LOW_HEIGHT - 24 - data_->boardTexture.Height()) / 2) + 24
    );

    data_->minesMarkPosition = sf::Vector2i(
        data_->boardPosition.x + x4 + 1,
        data_->boardPosition.y + y4 + 1
    );
    data_->timeMarkPosition = sf::Vector2i(
        data_->boardPosition.x + x6 + 1,
        data_->boardPosition.y + y4 + 1
    );

    data_->resetButton->X(CoreManager::HALF_LOW_WIDTH - 13);
    data_->resetButton->Y(data_->boardPosition.y + y4);

    data_->resetAfterWinButton->X(CoreManager::HALF_LOW_WIDTH - 13);
    data_->resetAfterWinButton->Y(data_->boardPosition.y + y4);

    data_->resetAfterLoseButton->X(CoreManager::HALF_LOW_WIDTH - 13);
    data_->resetAfterLoseButton->Y(data_->boardPosition.y + y4);

    data_->tableArea = sf::IntRect(data_->boardPosition.x + x3b,
        data_->boardPosition.y + y8, tw, th);
}

//--------------------------------------------------------------------------------

/**
 * Resets the configured game.
 */
void MinesweeperManager::resetGame() {
    data_->gameState = INITIAL_STATE;
    data_->currentTime = 0;
    data_->currentMines = data_->numberOfMines;
    data_->cellsToClick = data_->numberOfRows * data_->numberOfColumns - data_->numberOfMines;
    data_->board.clear();
    std::vector<InnerData::Cell> rowOfCells(data_->numberOfColumns);
    for (int i = 0; i < data_->numberOfRows; ++i) {
        data_->board.push_back(rowOfCells);
    }
}

//--------------------------------------------------------------------------------

/**
 * Starts the configured game.
 */
void MinesweeperManager::startGame(int row, int col, int button) {
    // Change the state.
    data_->gameState = PLAYING_STATE;

    // Clean the board.
    std::vector<sf::Vector2i> emptyCells;
    for (int i = 0; i < data_->numberOfRows; ++i) {
        for (int j = 0; j < data_->numberOfColumns; ++j) {
            auto & item = data_->board[i][j];
            item.state = CELL_BUTTON;
            item.value = EMPTY_VALUE;
            if ((row != i && col != j) || (button == BUTTON_RIGHT)) {
                emptyCells.push_back(sf::Vector2i(j, i));
            }
        }
    }

    // Init the random seed.
    auto * core = CoreManager::Instance();
    core->InitRndSeed();

    // Generate the bombs.
    for (int k = 0; k < data_->numberOfMines; ++k) {
        int index = core->Random(emptyCells.size());
        auto & coords = emptyCells[index];
        data_->board[coords.y][coords.x].value = BOMB_VALUE;
        emptyCells.erase(emptyCells.begin() + index);
    }

    // Generate the number indicators.
    for (int i = 0; i < data_->numberOfRows; ++i) {
        for (int j = 0; j < data_->numberOfColumns; ++j) {
            auto & item = data_->board[i][j];
            if (item.value == EMPTY_VALUE) {
                int counter = 0;
                ForEach(data_->directions, [&] (sf::Vector2i & dir) {
                    int r = i + dir.y, c = j + dir.x;
                    if (data_->IsInsideTable(r, c) &&
                        data_->board[r][c].value == BOMB_VALUE) {
                        ++counter;
                    }
                });
                item.value = counter;
            }
        }
    }

    // Execute the click on the cell.
    clickCell(row, col, button);
}

//--------------------------------------------------------------------------------

/**
 * Executes the click over a cell.
 */
void MinesweeperManager::clickCell(int row, int col, int button) {
    auto & item = data_->board[row][col];
    if (button == BUTTON_LEFT) {
        if (item.state == CELL_BUTTON) {
            if (item.value == BOMB_VALUE) {
                item.state = CELL_CLICKED;
                data_->gameState = LOSE_STATE;
                LoseSound().Play();

            } else {
                item.state = CELL_CLICKED;
                data_->cellsToClick--;

                if (data_->cellsToClick <= 0) {
                    data_->gameState = WIN_STATE;
                    WinSound().Play();
                }

                if (item.value == EMPTY_VALUE) {
                    ForEach(data_->directions, [&] (sf::Vector2i & dir) {
                        int r = row + dir.y, c = col + dir.x;
                        if (data_->IsInsideTable(r, c)) {
                            clickCell(r, c, BUTTON_LEFT);
                        }
                    });
                }
            }
        }

    } else if (button == BUTTON_BOTH) {
        if (data_->CanBothClick(item)) {
            int counter = 0;
            ForEach(data_->directions, [&] (sf::Vector2i & dir) {
                int r = row + dir.y, c = col + dir.x;
                if (data_->IsInsideTable(r, c) &&
                    data_->board[r][c].state == CELL_FLAGGED) {
                    ++counter;
                }
            });
            if (counter == item.value) {
                clickCell(row, col, BUTTON_LEFT);
                ForEach(data_->directions, [&] (sf::Vector2i & dir) {
                    int r = row + dir.y, c = col + dir.x;
                    if (data_->IsInsideTable(r, c)) {
                        clickCell(r, c, BUTTON_LEFT);
                    }
                });
            }
        }

    } else {
        if (item.state == CELL_BUTTON) {
            item.state = CELL_FLAGGED;
            data_->currentMines--;
        } else if (item.state == CELL_FLAGGED) {
            item.state = CELL_BUTTON;
            data_->currentMines++;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void MinesweeperManager::DrawGame() {
    // Draw the board.
    data_->boardTexture.Draw(data_->boardPosition);

    // Draw the mines HUD.
    const int NUMBER_WITH = 13;
    int cm = std::abs(data_->currentMines);
    int m0 = cm % 10;
    int m1 = (cm / 10) % 10;
    int m2 = (cm / 100) % 10;

    auto pos = data_->minesMarkPosition;
    if (data_->currentMines < 0) {
        data_->numbersTextures[10].Draw(pos); pos.x += NUMBER_WITH;
        data_->numbersTextures[m1].Draw(pos); pos.x += NUMBER_WITH;
        data_->numbersTextures[m0].Draw(pos);
    } else {
        data_->numbersTextures[m2].Draw(pos); pos.x += NUMBER_WITH;
        data_->numbersTextures[m1].Draw(pos); pos.x += NUMBER_WITH;
        data_->numbersTextures[m0].Draw(pos);
    }

    // Draw the time HUD.
    int ct = std::abs(data_->currentTime / 1000);
    int t0 = ct % 10;
    int t1 = (ct / 10) % 10;
    int t2 = (ct / 100) % 10;

    pos = data_->timeMarkPosition;
    data_->numbersTextures[t2].Draw(pos); pos.x += NUMBER_WITH;
    data_->numbersTextures[t1].Draw(pos); pos.x += NUMBER_WITH;
    data_->numbersTextures[t0].Draw(pos);

    if (data_->gameState == WIN_STATE) {
        // Drawing the win state.
        data_->resetAfterWinButton->Draw();
        for (int i = 0; i < data_->numberOfRows; ++i) {
            for (int j = 0; j < data_->numberOfColumns; ++j) {
                auto & item = data_->board[i][j];
                auto coords = sf::Vector2i(
                    data_->tableArea.left + CELL_WIDTH * j,
                    data_->tableArea.top + CELL_HEIGHT * i
                );
                if (item.state == CELL_BUTTON) {
                    data_->buttonNormalTexture.Draw(coords);
                } else if (item.state == CELL_FLAGGED) {
                    data_->buttonFlaggedTexture.Draw(coords);
                } else {
                    data_->cellsTextures[item.value].Draw(coords);
                }
            }
        }

    } else if (data_->gameState == LOSE_STATE) {
        // Drawing the lose state.
        data_->resetAfterLoseButton->Draw();
        for (int i = 0; i < data_->numberOfRows; ++i) {
            for (int j = 0; j < data_->numberOfColumns; ++j) {
                auto & item = data_->board[i][j];
                auto coords = sf::Vector2i(
                    data_->tableArea.left + CELL_WIDTH * j,
                    data_->tableArea.top + CELL_HEIGHT * i
                );
                if (item.value == BOMB_VALUE) {
                    if (item.state == CELL_BUTTON) {
                        data_->cellsTextures[BOMB_VALUE].Draw(coords);
                    } else if (item.state == CELL_FLAGGED) {
                        data_->buttonFlaggedTexture.Draw(coords);
                    } else {
                        data_->cellsTextures[BOOM_VALUE].Draw(coords);
                    }
                } else {
                    if (item.state == CELL_BUTTON) {
                        data_->buttonNormalTexture.Draw(coords);
                    } else if (item.state == CELL_FLAGGED) {
                        data_->cellsTextures[FAIL_VALUE].Draw(coords);
                    } else {
                        data_->cellsTextures[item.value].Draw(coords);
                    }
                }
            }
        }

    } else {
        // Drawing the other states.
        auto tableCoords = data_->GetTableCoords();
        bool leftPressed = Mouse::isButtonPressed(Mouse::Left);
        bool rightPressed = Mouse::isButtonPressed(Mouse::Right);
        bool allowBothClick = data_->CanBothClick(tableCoords);
        data_->resetButton->Draw();
        for (int i = 0; i < data_->numberOfRows; ++i) {
            for (int j = 0; j < data_->numberOfColumns; ++j) {
                auto & item = data_->board[i][j];
                auto coords = sf::Vector2i(
                    data_->tableArea.left + CELL_WIDTH * j,
                    data_->tableArea.top + CELL_HEIGHT * i
                );
                if (item.state == CELL_BUTTON) {
                    bool normal = true;
                    if (tableCoords.y != -1 && tableCoords.x != -1) {
                        if (leftPressed && rightPressed) {
                            if (allowBothClick) {
                                int difx = std::abs(tableCoords.x - j);
                                int dify = std::abs(tableCoords.y - i);
                                if (difx < 2 && dify < 2) {
                                    data_->buttonClickTexture.Draw(coords);
                                    normal = false;
                                }
                            }
                        } else if (leftPressed) {
                            if (tableCoords.y == i && tableCoords.x == j) {
                                data_->buttonClickTexture.Draw(coords);
                                normal = false;
                            }
                        }
                        if (normal) {
                            data_->buttonNormalTexture.Draw(coords);
                        }
                    } else {
                        data_->buttonNormalTexture.Draw(coords);
                    }
                } else if (item.state == CELL_FLAGGED) {
                    data_->buttonFlaggedTexture.Draw(coords);
                } else {
                    data_->cellsTextures[item.value].Draw(coords);
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void MinesweeperManager::UpdateGame(const sf::Time & timeDelta) {
    // Get some mouse input filters.
    bool mouseBothUp = false;
    bool mouseLeftDown = Mouse::isButtonPressed(Mouse::Left);
    bool mouseRightDown = Mouse::isButtonPressed(Mouse::Right);
    if (mouseLeftDown && mouseRightDown) {
        data_->mouseBothDown = true;
        data_->mouseDownTimeDif = 0;
    } else if (!mouseLeftDown && !mouseRightDown) {
        if (data_->mouseBothDown && data_->mouseDownTimeDif < 100) {
            data_->mouseBothDown = false;
            mouseBothUp = true;
        }
    } else {
        data_->mouseDownTimeDif += timeDelta.asMilliseconds();
    }

    // Execute the logic of each state.
    if (data_->gameState == WIN_STATE) {
        data_->resetAfterWinButton->Update();

    } else if (data_->gameState == LOSE_STATE) {
        data_->resetAfterLoseButton->Update();

    } else if (data_->gameState == PLAYING_STATE) {
        data_->currentTime += timeDelta.asMilliseconds();
        auto tableCoords = data_->GetTableCoords();
        if (tableCoords.y != -1 && tableCoords.x != -1) {
            if (mouseBothUp) {
                clickCell(tableCoords.y, tableCoords.x, BUTTON_BOTH);

            } else if (Mouse::IsButtonUp(Mouse::Left)) {
                clickCell(tableCoords.y, tableCoords.x, BUTTON_LEFT);

            } else if (Mouse::IsButtonUp(Mouse::Right)) {
                clickCell(tableCoords.y, tableCoords.x, BUTTON_RIGHT);
            }
        } else {
            data_->resetButton->Update();
        }

    } else {
        auto tableCoords = data_->GetTableCoords();
        if (tableCoords.y != -1 && tableCoords.x != -1) {
            if (!mouseBothUp) {
                if (Mouse::IsButtonUp(Mouse::Left)) {
                    startGame(tableCoords.y, tableCoords.x, BUTTON_LEFT);

                } else if (Mouse::IsButtonUp(Mouse::Right)) {
                    startGame(tableCoords.y, tableCoords.x, BUTTON_RIGHT);
                }
            }
        } else {
            data_->resetButton->Update();
        }
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
MinesweeperManager * MinesweeperManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
MinesweeperManager::MinesweeperManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
MinesweeperManager::~MinesweeperManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
MinesweeperManager * MinesweeperManager::Instance() {
    if (!instance_) {
        instance_ = new MinesweeperManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
MinesweeperManager & MinesweeperManager::Reference() {
    return *(Instance());
}
