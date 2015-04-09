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

#include "PuzzleManager.h"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/Sound.h>
#include <System/Mouse.h>
#include <System/MathUtil.h>
#include <Menu/DesktopState.h>
#include <Games/Puzzle/PuzzleGameState.h>
#include <Games/Puzzle/Solver/Generator.h>
#include <Games/Puzzle/Solver/Solver.h>

//********************************************************************************
// Defines
//********************************************************************************

#define INITIAL_STATE 0
#define PLAYING_STATE 1

#define MAX_PIECES 16

#define SIDE_LENGTH NPuzzle::Puzzle::RANGE

//********************************************************************************
// InnerData
//********************************************************************************

struct PuzzleManager::InnerData {
    SharedTexture tileset;
    SharedTexture puzzleSurface;
    Texture2D puzzleTexture;
    Texture2D puzzleTextures[MAX_PIECES];

    int gameState;
    NPuzzle::Puzzle board;
    sf::IntRect puzzleArea;
    sf::Vector2i pieceSizes;

    Sound keyboardSound;
    Sound clickSound;
    Sound winSound;

    sf::Vector2i GetTableCoords() {
        auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
        if (MathUtil::PointInside(mouseCoords, puzzleArea)) {
            return sf::Vector2i(
                (mouseCoords.x - puzzleArea.left) / pieceSizes.x,
                (mouseCoords.y - puzzleArea.top) / pieceSizes.y
            );
        } else {
            return sf::Vector2i(-1, -1);
        }
    }
};

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & PuzzleManager::Tileset() { return data_->tileset; }

//--------------------------------------------------------------------------------

Sound & PuzzleManager::KeyboardSound() { return data_->keyboardSound; }
Sound & PuzzleManager::ClickSound()    { return data_->clickSound;    }
Sound & PuzzleManager::WinSound()      { return data_->winSound;      }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void PuzzleManager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();

        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->tileset = core->LoadTexture("Content/Textures/Puzzle01.png");
        data_->puzzleSurface = core->LoadTexture("Content/Textures/Puzzle02.png");
        data_->puzzleTexture.Load(data_->puzzleSurface);

        // Set some initial data.
        int w = data_->puzzleTexture.Width(), h = data_->puzzleTexture.Height();
        data_->pieceSizes = sf::Vector2i(w / SIDE_LENGTH, h / SIDE_LENGTH);

        data_->puzzleArea = sf::IntRect(
            (CoreManager::LOW_WIDTH - w) / 2,
            ((CoreManager::LOW_HEIGHT - 24 - h) / 2) + 24,
            data_->puzzleTexture.Width(),
            data_->puzzleTexture.Height()
        );

        data_->gameState = INITIAL_STATE;
        data_->board = NPuzzle::Puzzle();

        // Setting the blank piece texture.
        const auto BORDER_COLOR = AtariPalette::Hue00Lum00;
        w = data_->pieceSizes.x, h = data_->pieceSizes.y;

        sf::Image blankImage;
        blankImage.create(w, h, AtariPalette::Hue00Lum14);

        for (int i = 0; i < w; ++i) {
            blankImage.setPixel(i, 0, BORDER_COLOR);
            blankImage.setPixel(i, h - 1, BORDER_COLOR);
        }
        for (int i = 0; i < h; ++i) {
            blankImage.setPixel(0, i, BORDER_COLOR);
            blankImage.setPixel(w - 1, i, BORDER_COLOR);
        }

        SharedTexture blankSurface = std::make_shared<sf::Texture>();
        blankSurface->loadFromImage(blankImage);
        data_->puzzleTextures[0].Load(blankSurface);

        // Setting the pieces textures.
        int x = w, y = 0;
        for (int i = 1; i < MAX_PIECES; ++i) {
            auto area = sf::IntRect(x, y, w, h);
            data_->puzzleTextures[i].Load(data_->puzzleSurface, area);
            x += w;
            if (x >= data_->puzzleArea.width) {
                x = 0; y += h;
            }
        }

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");
        data_->winSound.Load("Content/Sounds/PuzzleWin.wav");

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void PuzzleManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();
        data_->clickSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->clickSound.Unload();
        data_->winSound.Unload();

        // Unload the textures of the game.
        data_->tileset = nullptr;
        data_->puzzleSurface = nullptr;

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
void PuzzleManager::StartGame() {
    data_->gameState = PLAYING_STATE;

    auto candidates = NPuzzle::GenerateRandomCandidate();
    NPuzzle::Puzzle puzzle(candidates);
    NPuzzle::Solver solver(puzzle);
    if (solver.Solve()) {
        data_->board = puzzle;
    } else {
        const auto EMPTY = NPuzzle::Puzzle::EMPTY;
        if (candidates[0] != EMPTY && candidates[1] != EMPTY) {
            std::swap(candidates[0], candidates[1]);
        } else {
            std::swap(candidates[2], candidates[3]);
        }
        data_->board = NPuzzle::Puzzle(candidates);
    }

    CoreManager::Instance()->SetNextState(MakeSharedState<PuzzleGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void PuzzleManager::DrawGame() {
    if (data_->gameState == PLAYING_STATE) {
        int limit = data_->puzzleArea.left + data_->puzzleArea.width;
        int x = data_->puzzleArea.left, y = data_->puzzleArea.top;
        for (int i = 0; i < SIDE_LENGTH; ++i) {
            for (int j = 0; j < SIDE_LENGTH; ++j) {
                auto index = data_->board.GetData(i, j);
                data_->puzzleTextures[index].Draw(x, y);
                x += data_->pieceSizes.x;
                if (x >= limit) {
                    x = data_->puzzleArea.left;
                    y += data_->pieceSizes.y;
                }
            }
        }
    } else {
        data_->puzzleTexture.Draw(data_->puzzleArea.left, data_->puzzleArea.top);
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void PuzzleManager::UpdateGame(const sf::Time & timeDelta) {
    if (data_->gameState == PLAYING_STATE) {
        auto coords = data_->GetTableCoords();
        if (coords.x != -1 && coords.y != -1 && Mouse::IsButtonUp(Mouse::Left)) {
            const auto EMPTY = NPuzzle::Puzzle::EMPTY;
            if (data_->board.GetData(coords.y - 1, coords.x) == EMPTY) {
                data_->board.MoveDown();
            } else if (data_->board.GetData(coords.y, coords.x + 1) == EMPTY) {
                data_->board.MoveLeft();
            } else if (data_->board.GetData(coords.y + 1, coords.x) == EMPTY) {
                data_->board.MoveUp();
            } else if (data_->board.GetData(coords.y, coords.x - 1) == EMPTY) {
                data_->board.MoveRight();
            }
            if (data_->board.Solved()) {
                data_->gameState = INITIAL_STATE;
                WinSound().Play();
            }
        }
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
PuzzleManager * PuzzleManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
PuzzleManager::PuzzleManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
PuzzleManager::~PuzzleManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
PuzzleManager * PuzzleManager::Instance() {
    if (!instance_) {
        instance_ = new PuzzleManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
PuzzleManager & PuzzleManager::Reference() {
    return *(Instance());
}
