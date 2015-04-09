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

#include "SnakeManager.h"
#include <sstream>
#include <iomanip>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <Menu/DesktopState.h>
#include <Games/SaveManager.h>
#include <Games/Snake/SnakeMenuState.h>
#include <Games/Snake/SnakeGameState.h>
#include <Games/Snake/SnakeGameMenuState.h>
#include <Games/Snake/SnakeGameOverState.h>
#include <Games/Snake/SnakeEnterNameState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  339

#define INIT_STATE   0
#define GAME_STATE   1
#define EXIT_STATE   2
#define PAUSE_STATE  3

#define MAX_PAUSE_INTERVAL    1000
#define EASY_TIME_INTERVAL     150
#define NORMAL_TIME_INTERVAL   100
#define HARD_TIME_INTERVAL      50

#define EASY_DOT_SCORE     1
#define NORMAL_DOT_SCORE   2
#define HARD_DOT_SCORE     4
#define DOT2_FACTOR_SCORE  2

#define DOT2_HIDE_STATE  0
#define DOT2_SHOW_STATE  1

#define DOT2_HIDE_INTERVAL         10000
#define DOT2_SHOW_EASY_INTERVAL    15000
#define DOT2_SHOW_NORMAL_INTERVAL  10000
#define DOT2_SHOW_HARD_INTERVAL     5000

//********************************************************************************
// InnerData
//********************************************************************************

struct SnakeManager::InnerData {
    Texture2D squareTexture_;
    int numberOfPlayers_;
    SnakeManager::Cell world_[SnakeManager::MAX_ROWS][SnakeManager::MAX_COLS];
    SnakeManager::PlayersArray players_;
    SnakeManager::RankingArray ranking_;

    int currentTime_;
    int maxTimeInterval_;
    sf::Vector2i dot2Position_;
    int dot2State_;
    int dot2ShowInterval_;
    int dot2MaxTimeInterval_;
    int dotScoreValue_;

    int substate_;
    std::unique_ptr<SimpleLabel> downLabel_;
    std::unique_ptr<SimpleLabel> player1ScoreLabel_;
    std::unique_ptr<SimpleLabel> player2ScoreLabel_;

    Sound menuUpSound_;
    Sound menuDownSound_;
    Sound menuKeySound_;
    Sound menuNextSound_;

    Sound deathSound_;
    Sound eatDot1Sound_;
    Sound eatDot2Sound_;
    Sound dotSpawnSound_;

    InnerData() : squareTexture_(), numberOfPlayers_(0), players_(),
        world_(), ranking_(), currentTime_(0), maxTimeInterval_(0),
        dot2Position_(), dot2State_(0), dot2ShowInterval_(0),
        dot2MaxTimeInterval_(0), dotScoreValue_(0), substate_(0),
        downLabel_(nullptr), player1ScoreLabel_(nullptr),
        player2ScoreLabel_(nullptr) {}
};

//********************************************************************************
// Properties
//********************************************************************************

Texture2D & SnakeManager::SquareTexture() {
    return data_->squareTexture_;
}

//--------------------------------------------------------------------------------

int SnakeManager::NumberOfPlayers() {
    return data_->numberOfPlayers_;
}

//--------------------------------------------------------------------------------

SnakeManager::PlayersArray & SnakeManager::Players() {
    return data_->players_;
}

//--------------------------------------------------------------------------------

SnakeManager::RankingArray & SnakeManager::Ranking() {
    return data_->ranking_;
}

//--------------------------------------------------------------------------------

SnakeManager::Cell & SnakeManager::World(int x, int y) {
    if (0 <= x && x < MAX_COLS && 0 <= y && y < MAX_ROWS) {
        return data_->world_[y][x];
    } else {
        return data_->world_[0][0];
    }
}

//--------------------------------------------------------------------------------

void SnakeManager::World(int x, int y, int value) {
    World(x, y, value, AtariPalette::Hue00Lum14);
}

//--------------------------------------------------------------------------------

void SnakeManager::World(int x, int y, int value, const sf::Color & color) {
    if (0 <= x && x < MAX_COLS && 0 <= y && y < MAX_ROWS) {
        data_->world_[y][x] = Cell(value, color);
    }
}

//--------------------------------------------------------------------------------

Sound & SnakeManager::MenuUpSound()   { return data_->menuUpSound_;   }
Sound & SnakeManager::MenuDownSound() { return data_->menuDownSound_; }
Sound & SnakeManager::MenuKeySound()  { return data_->menuKeySound_;  }
Sound & SnakeManager::MenuNextSound() { return data_->menuNextSound_; }

Sound & SnakeManager::DeathSound()    { return data_->deathSound_;    }
Sound & SnakeManager::EatDot1Sound()  { return data_->eatDot1Sound_;  }
Sound & SnakeManager::EatDot2Sound()  { return data_->eatDot2Sound_;  }
Sound & SnakeManager::DotSpawnSound() { return data_->dotSpawnSound_; }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void SnakeManager::Initialize() {
    if (!initialized_) {
        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the square texture and set the player entity data.
        data_->squareTexture_.Load(10, 10);
        data_->players_[0].SetPlayer1();
        data_->players_[1].SetPlayer2();

        // Set the HUD data of the game.
        auto * core = CoreManager::Instance();

        data_->downLabel_.reset(new SimpleLabel("", 0, ROW0,
            AtariPalette::Hue01Lum12, core->Retro70Font()));

        data_->player1ScoreLabel_.reset(new SimpleLabel("P1: 00000000",
            10, ROW0, AtariPalette::Hue04Lum02, core->Retro70Font()));

        data_->player2ScoreLabel_.reset(new SimpleLabel("P2: 00000000",
            486, ROW0, AtariPalette::Hue09Lum02, core->Retro70Font()));

        // Load the sounds of the game.
        data_->menuUpSound_.Load("Content/Sounds/SnakeMenuUp.wav");
        data_->menuDownSound_.Load("Content/Sounds/SnakeMenuDown.wav");
        data_->menuKeySound_.Load("Content/Sounds/SnakeMenuKey.wav");
        data_->menuNextSound_.Load("Content/Sounds/SnakeMenuNext.wav");
        data_->deathSound_.Load("Content/Sounds/SnakeDeath.wav");
        data_->eatDot1Sound_.Load("Content/Sounds/SnakeEatDot1.wav");
        data_->eatDot2Sound_.Load("Content/Sounds/SnakeEatDot2.wav");
        data_->dotSpawnSound_.Load("Content/Sounds/SnakeDotSpawn.wav");

        // Loads the previous saved data.
        SaveManager::Instance()->SnakeLoad();

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void SnakeManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->menuKeySound_.WaitUntilStop();

        // Unload the sounds of the game.
        data_->menuUpSound_.Unload();
        data_->menuDownSound_.Unload();
        data_->menuKeySound_.Unload();
        data_->menuNextSound_.Unload();
        data_->deathSound_.Unload();
        data_->eatDot1Sound_.Unload();
        data_->eatDot2Sound_.Unload();
        data_->dotSpawnSound_.Unload();

        // Unload the texture of the game.
        data_->squareTexture_.Unload();

        // Remove the inner data of the game.
        data_.reset(nullptr);
    }

    // Change the initialized flag and set the next state.
    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the border of a menu screen of the game.
 */
void SnakeManager::DrawBorder(const sf::Color & borderColor) {
    // Get some data to paint the border.
    const int TOP_FACTOR = 1, BOTTOM_FACTOR = 4;
    int w = data_->squareTexture_.Width(), h = data_->squareTexture_.Height();

    // Paint the top and bottom lines.
    int end = CoreManager::LOW_WIDTH - w;
    int y1 = TOP_FACTOR * h, y2 = CoreManager::LOW_HEIGHT - BOTTOM_FACTOR * h;
    for (int x = w; x < end; x += w) {
        data_->squareTexture_.Draw(x, y1, borderColor);
        data_->squareTexture_.Draw(x, y2, borderColor);
    }

    // Paint the left and right lines.
    end = CoreManager::LOW_HEIGHT - BOTTOM_FACTOR * h;
    int x1 = w, x2 = CoreManager::LOW_WIDTH - 2 * w;
    for (int y = h * (TOP_FACTOR + 1); y < end; y += h) {
        data_->squareTexture_.Draw(x1, y, borderColor);
        data_->squareTexture_.Draw(x2, y, borderColor);
    }
}

//--------------------------------------------------------------------------------

/**
 * Exits the game over state.
 */
void SnakeManager::ExitGameOver() {
    // Check if the current scores of the game are top score.
    if (IsTopScore(data_->players_[0].Score()) ||
        (data_->numberOfPlayers_ >= 2 && IsTopScore(data_->players_[1].Score()))) {
        // There is a top score in the last game, so we'll go to the enter name state.
        CoreManager::Instance()->SetNextState(MakeSharedState<SnakeEnterNameState>());
    } else {
        // No top score in the last game, so we'll go to the menu of the game.
        CoreManager::Instance()->SetNextState(MakeSharedState<SnakeMenuState>());
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if a score is in the top.
 */
bool SnakeManager::IsTopScore(int score) {
    return data_->ranking_[MAX_RANKING_ENTRIES - 1].Score <= score;
}

//--------------------------------------------------------------------------------

/**
 * Adds a score into the top.
 */
void SnakeManager::AddScore(const std::string & name, int score) {
    for (int i = 0; i < MAX_RANKING_ENTRIES; ++i) {
        if (data_->ranking_[i].Score <= score) {
            // After find the position of the score in the ranking, we'll
            // move down all the other scores in the ranking.
            for (int j = MAX_RANKING_ENTRIES - 1; j > i; --j) {
                data_->ranking_[j].Name = data_->ranking_[j - 1].Name;
                data_->ranking_[j].Score = data_->ranking_[j - 1].Score;
            }
            // Set the last top score and exit.
            data_->ranking_[i].Score = score;
            data_->ranking_[i].Name = name;
            break;
        }
    }
}

//********************************************************************************
// Game methods
//********************************************************************************

/**
 * Starts a new 1 player game.
 */
void SnakeManager::StartGame(int difficulty, bool simpleControl) {
    // Start a new game, set the type of control and the next state.
    startGame(difficulty, 1);
    data_->players_[0].SimpleControl(simpleControl);
    CoreManager::Instance()->SetNextState(MakeSharedState<SnakeGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Starts a new 2 players game.
 */
void SnakeManager::StartGame(int difficulty, bool simpleControl1, bool simpleControl2) {
    // Start a new game, set the type of controls and the next state.
    startGame(difficulty, 2);
    data_->players_[0].SimpleControl(simpleControl1);
    data_->players_[1].SimpleControl(simpleControl2);
    CoreManager::Instance()->SetNextState(MakeSharedState<SnakeGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Starts a new game.
 */
void SnakeManager::startGame(int difficulty, int numberOfPlayers) {
    // Check the difficulty to set some values.
    if (difficulty == DIFFICULTY_HARD) {
        data_->maxTimeInterval_ = HARD_TIME_INTERVAL;
        data_->dotScoreValue_ = HARD_DOT_SCORE;
        data_->dot2ShowInterval_ = DOT2_SHOW_HARD_INTERVAL;
    } else if (difficulty == DIFFICULTY_EASY) {
        data_->maxTimeInterval_ = EASY_TIME_INTERVAL;
        data_->dotScoreValue_ = EASY_DOT_SCORE;
        data_->dot2ShowInterval_ = DOT2_SHOW_EASY_INTERVAL;
    } else {
        data_->maxTimeInterval_ = NORMAL_TIME_INTERVAL;
        data_->dotScoreValue_ = NORMAL_DOT_SCORE;
        data_->dot2ShowInterval_ = DOT2_SHOW_NORMAL_INTERVAL;
    }

    // Set the number of players of this game.
    data_->numberOfPlayers_ = numberOfPlayers;

    // Reset the world matrix.
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            if (i == 0 || i == MAX_ROWS - 1 || j == 0 || j == MAX_COLS - 1) {
                data_->world_[i][j] = Cell(WALL_CELL, AtariPalette::Hue10Lum02);
            } else {
                data_->world_[i][j] = Cell(EMTPY_CELL, AtariPalette::Hue00Lum14);
            }
        }
    }

    // Set the players start data and positions inside the world.
    if (data_->numberOfPlayers_ > 1) {
        data_->players_[0].NewGame(15, 13);
        data_->players_[1].NewGame(45, 13);
    } else {
        data_->players_[0].NewGame(30, 13);
    }

    // Set the dot items data.
    generateDotItem();
    dot2ToHideState();

    // Set the mark of time, the hud and the substate of the game.
    data_->currentTime_ = 0;
    updateHud();
    changeSubstate(INIT_STATE);
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void SnakeManager::DrawGame() {
    // Draw the world of the game.
    int w = data_->squareTexture_.Width(), h = data_->squareTexture_.Height();
    for (int i = 0; i < MAX_ROWS; ++i) {
        for (int j = 0; j < MAX_COLS; ++j) {
            if (data_->world_[i][j].Value != EMTPY_CELL) {
                data_->squareTexture_.Draw(w * (j + 1), h * (i + 1), data_->world_[i][j].Color);
            }
        }
    }
    // Draw the labels of the hud.
    if (data_->substate_ == GAME_STATE) {
        data_->player1ScoreLabel_->Draw();
        if (data_->numberOfPlayers_ > 1) {
            data_->player2ScoreLabel_->Draw();
        }
    }
    data_->downLabel_->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void SnakeManager::UpdateGame(const sf::Time & timeDelta) {
    if (data_->substate_ == EXIT_STATE) {
        // The exit state is when the game is over. We'll need only to wait
        // to any key to be pressed to set the next state of the game.
        if (Keyboard::IsAnyKeyUp()) {
            CoreManager::Instance()->SetNextState(MakeSharedState<SnakeGameOverState>());
        }
    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        // In the other substates the player can press escape and invoke the menu.
        // This will invoke the substate pause and call the game menu state.
        MenuNextSound().Play();
        changeSubstate(PAUSE_STATE);
        CoreManager::Instance()->SetNextState(MakeSharedState<SnakeGameMenuState>());
    } else  if (data_->substate_ == INIT_STATE) {
        // In the initial substate there will be a pause of time, just to
        // make possible to the players that they can see the current state
        // of the world, and locate their avatars.
        data_->currentTime_ += timeDelta.asMilliseconds();
        if (data_->currentTime_ >= MAX_PAUSE_INTERVAL) {
            data_->currentTime_ = 0;
            // After the pause we'll change the substate and "start" the
            // logic of the snakes inside the world.
            MenuNextSound().Play();
            changeSubstate(GAME_STATE);
            data_->players_[0].Start();
            data_->players_[1].Start();
        }
    } else  if (data_->substate_ == PAUSE_STATE) {
        // In the pause substate there will be a pause of time, just to
        // make possible to the players that they can see the current
        // state of the world, and locate their avatars.
        data_->currentTime_ += timeDelta.asMilliseconds();
        if (data_->currentTime_ >= MAX_PAUSE_INTERVAL) {
            data_->currentTime_ = 0;
            // After the pause we'll change the substate.
            MenuNextSound().Play();
            changeSubstate(GAME_STATE);
        }
    } else if (data_->substate_ == GAME_STATE) {
        // In the game substate the first thing to do is update
        // the input of the players.
        data_->players_[0].UpdateInput();
        if (data_->numberOfPlayers_ > 1) {
            data_->players_[1].UpdateInput();
        }

        // After some interval of time, we'll update the internal
        // logic of each snake inside the world.
        data_->currentTime_ += timeDelta.asMilliseconds();
        if (data_->currentTime_ >= data_->maxTimeInterval_) {
            data_->currentTime_ -= data_->maxTimeInterval_;

            data_->players_[0].Update();
            if (data_->numberOfPlayers_ > 1) {
                data_->players_[1].Update();
            }
        }

        // Finally, we'll update the dot 2 logic.
        dot2Update(timeDelta);
    }
}

//--------------------------------------------------------------------------------

/**
 * Kills some entity inside the world.
 */
void SnakeManager::Kill(int cellValue, SnakePlayer * player) {
    if (cellValue == PLAYER1_CELL) {
        // When the entity to kill is the player 1, we'll check the game
        // over condition to change the substate or just remove the snake.
        DeathSound().Play();
        if (checkGameOver()) {
            changeSubstate(EXIT_STATE);
        } else {
            data_->players_[0].RemoveFromWorld();
        }

    } else if (cellValue == PLAYER2_CELL) {
        // When the entity to kill is the player 2, we'll check the game
        // over condition to change the substate or just remove the snake.
        DeathSound().Play();
        if (checkGameOver()) {
            changeSubstate(EXIT_STATE);
        } else {
            data_->players_[1].RemoveFromWorld();
        }

    } else if (cellValue == DOT1_CELL) {
        // When the entity to kill is the dot, we'll add the score to the
        // player that collides with the dot and generate the next one.
        EatDot1Sound().Play();
        player->AddScore(data_->dotScoreValue_);
        generateDotItem();
        updateHud();

    } else if (cellValue == DOT2_CELL) {
        // When the entity to kill is the yellow dot, we'll add the score
        // to the player that collides with the dot and set the internal
        // state of this dot to hide.
        EatDot2Sound().Play();
        player->AddScore(data_->dotScoreValue_ * DOT2_FACTOR_SCORE);
        dot2ToHideState();
        updateHud();
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the game over is here.
 */
bool SnakeManager::checkGameOver() {
    if (data_->numberOfPlayers_ > 1)
        return !data_->players_[0].Alive() && !data_->players_[1].Alive();
    else
        return !data_->players_[0].Alive();
}

//--------------------------------------------------------------------------------

/**
 * Generates a dot item.
 */
void SnakeManager::generateDotItem() {
    // To generate a dot, first we'll get an empty cell from the world.
    auto emptyCell = getEmptyCell();
    if (emptyCell.x != 0 && emptyCell.y != 0) {
        // With the empty cell we'll change the world to put dot inside.
        World(emptyCell.x, emptyCell.y, DOT1_CELL, AtariPalette::Hue00Lum14);
    }
}

//--------------------------------------------------------------------------------

/**
 * Generates a dot item.
 */
void SnakeManager::generateDot2Item() {
    // To generate a dot, first we'll get an empty cell from the world.
    auto emptyCell = getEmptyCell();
    if (emptyCell.x != 0 && emptyCell.y != 0) {
        // With the empty cell we'll change the world to put dot inside.
        DotSpawnSound().Play();
        World(emptyCell.x, emptyCell.y, DOT2_CELL, AtariPalette::Hue01Lum12);
        // And then save the yellow dot position and change its state.
        data_->dot2Position_ = emptyCell;
        dot2ToShowState();
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets an empty cell from the world.
 */
sf::Vector2i SnakeManager::getEmptyCell() {
    // To get an empty cell, the first thing to do is generate some random
    // coordinates and try some luck.
    auto * core = CoreManager::Instance();
    const int MAX_ATTEMPTS = 5;
    int attempts = 0;
    sf::Vector2i victim;
    do {
        victim.x = core->Random(MAX_COLS);
        victim.y = core->Random(MAX_ROWS);
        ++attempts;
    } while (attempts < MAX_ATTEMPTS && World(victim.x, victim.y).Value != EMTPY_CELL);

    // If the generated coordinates are an empty cell, we'll return it.
    if (World(victim.x, victim.y).Value != EMTPY_CELL) {
        return victim;
    } else {
        // If not, we'll seek all the empty cells in the world and choose one.
        std::vector<sf::Vector2i> cells;
        cells.reserve(MAX_ROWS * MAX_COLS);
        for (int i = 0; i < MAX_ROWS; ++i) {
            for (int j = 0; j < MAX_COLS; ++j) {
                if (World(j, i).Value == EMTPY_CELL) {
                    cells.push_back(sf::Vector2i(j, i));
                }
            }
        }
        return cells[core->Random(cells.size())];
    }
}

//--------------------------------------------------------------------------------

/**
 * Sets the dot 2 in hide state.
 */
void SnakeManager::dot2ToHideState() {
    data_->dot2State_ = DOT2_HIDE_STATE;
    data_->dot2MaxTimeInterval_ = DOT2_HIDE_INTERVAL;
}

//--------------------------------------------------------------------------------

/**
 * Sets the dot 2 in show state.
 */
void SnakeManager::dot2ToShowState() {
    data_->dot2State_ = DOT2_SHOW_STATE;
    data_->dot2MaxTimeInterval_ = data_->dot2ShowInterval_;
}

//--------------------------------------------------------------------------------

/**
 * Updates the dot 2 logic.
 */
void SnakeManager::dot2Update(const sf::Time & timeDelta) {
    // When the max time interval of the yellow dot state is over,
    // we'll update the internal state of the entity.
    data_->dot2MaxTimeInterval_ -= timeDelta.asMilliseconds();
    if (data_->dot2MaxTimeInterval_ < 0) {
        if (data_->dot2State_ == DOT2_SHOW_STATE) {
            // If the entity is visible in the world, we'll
            // remove it from the world and set the hide state.
            World(data_->dot2Position_.x, data_->dot2Position_.y, EMTPY_CELL);
            dot2ToHideState();
        } else {
            // If the entity is not visible, we'll show it.
            generateDot2Item();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Changes the substate of the game.
 */
void SnakeManager::changeSubstate(int value) {
    // Get some data and change the substate.
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    data_->substate_ = value;

    // Set the HUD values of the substate.
    switch (data_->substate_) {
    case INIT_STATE:
        data_->downLabel_->Color(AtariPalette::Hue01Lum12);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel_->Text("¡Prepárate para empezar!");
        } else {
            data_->downLabel_->Text("Get ready to start!");
        }
        break;

    case GAME_STATE:
        data_->downLabel_->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel_->Text("Menú (ESC)");
        } else {
            data_->downLabel_->Text("Menu (ESC)");
        }
        break;

    case EXIT_STATE:
        data_->downLabel_->Color(AtariPalette::Hue01Lum12);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel_->Text("Pulse cualquier tecla para continuar...");
        } else {
            data_->downLabel_->Text("Press any key to continue...");
        }
        break;

    case PAUSE_STATE:
        data_->downLabel_->Color(AtariPalette::Hue01Lum12);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel_->Text("¡Prepárate para continuar!");
        } else {
            data_->downLabel_->Text("Get ready to continue!");
        }
        break;
    }

    // Set the right position of some elements of the HUD.
    GUIUtil::CenterLabel(data_->downLabel_, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

/**
 * Updates the hud of the game.
 */
void SnakeManager::updateHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << std::setfill('0') << std::setw(8) << victim;
        return strconv.str();
    };
    // Convert to string the player 1 score.
    std::string score1 = ToString(data_->players_[0].Score());
    data_->player1ScoreLabel_->Text("P1: " + score1);
    // Convert to string the player 2 score.
    if (data_->numberOfPlayers_ > 1) {
        std::string score2 = ToString(data_->players_[1].Score());
        data_->player2ScoreLabel_->Text("P2: " + score2);
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
SnakeManager * SnakeManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
SnakeManager::SnakeManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
SnakeManager::~SnakeManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
SnakeManager * SnakeManager::Instance() {
    if (!instance_) {
        instance_ = new SnakeManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
SnakeManager & SnakeManager::Reference() {
    return *(Instance());
}
