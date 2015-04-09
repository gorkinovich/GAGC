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

#include "AlienManager.h"
#include <sstream>
#include <iomanip>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Menu/DesktopState.h>
#include <Games/AlienParty/AlienGameState.h>
#include <Games/AlienParty/AlienGameMenuState.h>
#include <Games/AlienParty/AlienGameOverState.h>
#include <Games/AlienParty/AlienPlayer.h>
#include <Games/AlienParty/AlienPlayerShot.h>
#include <Games/AlienParty/AlienEnemy.h>
#include <Games/AlienParty/AlienEnemyShot.h>
#include <Games/AlienParty/AlienShield.h>

//********************************************************************************
// Defines
//********************************************************************************

const int INIT_STATE  = 0;
const int GAME_STATE  = 1;
const int EXIT_STATE  = 2;
const int PAUSE_STATE = 3;

const int MAX_PAUSE_INTERVAL = 500;
const int MAX_TIME_INTERVAL  =  40;

const int BAR_POSITION_Y = 336;

const int ROWS = 5, COLS = 11;
const int MAX_ENEMIES = ROWS * COLS;
const int MAX_SHIELDS = 4;

//********************************************************************************
// InnerData
//********************************************************************************

struct AlienManager::InnerData {
    SharedTexture tileset;
    Texture2D barTexture;

    int substate;
    int currentTime;
    int deathCount;
    AlienPlayer player;
    AlienShield shields[MAX_SHIELDS];
    bool goingToTheLeft;
    int enemiesAlive;
    AlienEnemy enemies[ROWS][COLS];
    sf::Vector2i enemiesStart, enemiesEnd;
    std::vector<AlienPlayerShot> playerShots;
    std::vector<AlienEnemyShot> enemyShots;

    std::unique_ptr<SimpleLabel> downLabel;

    Sound keyboardSound;
    Sound fireSound;
    Sound enemySound;
    Sound playerSound;

    template<class T>
    void CreateShot(std::vector<T> & shots, const sf::Vector2i & coords) {
        int size = shots.size(), index = -1;
        for (int i = 0; i < size; ++i) {
            if (!shots[i].Alive()) {
                index = i; break;
            }
        }
        if (index == -1) {
            shots.push_back(T());
            shots[size].Reset(coords);
        } else {
            shots[index].Reset(coords);
        }
    }

    void DrawEnemies();
    void UpdateEnemies();
    void UpdateAnimationEnemies();
    void CheckEnemies();
    void CollisionEnemies(AlienPlayerShot & shot);
    void CollisionEnemyShots(AlienPlayerShot & shot);
    void NextLevel();
    void UpdateSpeed();
    void UpdateHud();
};

//--------------------------------------------------------------------------------

void AlienManager::InnerData::DrawEnemies() {
    for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
        for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
            enemies[i][j].Draw();
        }
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::UpdateEnemies() {
    const int OFFSET_X = 16;
    bool leftReached = enemies[enemiesStart.y][enemiesStart.x].LeftReached();
    bool rightReached = enemies[enemiesStart.y][enemiesEnd.x].RightReached();

    if (leftReached || rightReached) {
        goingToTheLeft = !goingToTheLeft;
        for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
            for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
                enemies[i][j].Update();
                enemies[i][j].MoveDown();
                if (leftReached) {
                    enemies[i][j].MiniMoveRight();
                } else {
                    enemies[i][j].MiniMoveLeft();
                }
                for (int k = 0; k < MAX_SHIELDS; ++k) {
                    if (shields[k].Collision(enemies[i][j])) break;
                }
            }
        }
    } else if (goingToTheLeft) {
        for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
            for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
                enemies[i][j].Update();
                enemies[i][j].MoveLeft();
                if (j > enemiesStart.x) {
                    enemies[i][j].X(enemies[i][j - 1].X() + OFFSET_X);
                }
                for (int k = 0; k < MAX_SHIELDS; ++k) {
                    if (shields[k].Collision(enemies[i][j])) break;
                }
            }
        }
    } else {
        for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
            for (int j = enemiesEnd.x; j >= enemiesStart.x; --j) {
                enemies[i][j].Update();
                enemies[i][j].MoveRight();
                if (j < enemiesEnd.x) {
                    enemies[i][j].X(enemies[i][j + 1].X() - OFFSET_X);
                }
                for (int k = 0; k < MAX_SHIELDS; ++k) {
                    if (shields[k].Collision(enemies[i][j])) break;
                }
            }
        }
    }

    for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
        if (player.Collision(enemies[enemiesEnd.y][j])) break;
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::UpdateAnimationEnemies() {
    for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
        for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
            enemies[i][j].UpdateAnimation();
        }
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::CheckEnemies() {
    // Close the circle.
    for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
        bool allDead = true;
        for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
            auto & item = enemies[i][j];
            if (item.Alive() || !item.IsDeathAnimationFinished()) {
                allDead = false; break;
            }
        }
        if (allDead) {
            enemiesStart.x++;
        } else {
            break;
        }
    }
    for (int j = enemiesEnd.x; j >= enemiesStart.x; --j) {
        bool allDead = true;
        for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
            auto & item = enemies[i][j];
            if (item.Alive() || !item.IsDeathAnimationFinished()) {
                allDead = false; break;
            }
        }
        if (allDead) {
            enemiesEnd.x--;
        } else {
            break;
        }
    }
    for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
        bool allDead = true;
        for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
            auto & item = enemies[i][j];
            if (item.Alive() || !item.IsDeathAnimationFinished()) {
                allDead = false; break;
            }
        }
        if (allDead) {
            enemiesStart.y++;
        } else {
            break;
        }
    }
    for (int i = enemiesEnd.y; i >= enemiesStart.y; --i) {
        bool allDead = true;
        for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
            auto & item = enemies[i][j];
            if (item.Alive() || !item.IsDeathAnimationFinished()) {
                allDead = false; break;
            }
        }
        if (allDead) {
            enemiesEnd.y--;
        } else {
            break;
        }
    }

    // Update the speed.
    UpdateSpeed();

    // Check if player collision.
    for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
        for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
            player.Collision(enemies[i][j]);
        }
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::CollisionEnemies(AlienPlayerShot & shot) {
    for (int i = enemiesStart.y; i <= enemiesEnd.y; ++i) {
        for (int j = enemiesStart.x; j <= enemiesEnd.x; ++j) {
            enemies[i][j].Collision(shot);
        }
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::CollisionEnemyShots(AlienPlayerShot & shot) {
    auto it = enemyShots.begin(), end = enemyShots.end();
    for (; it != end; ++it) {
        if (shot.Collision(*it)) {
            break;
        }
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::NextLevel() {
    playerShots.clear();
    enemyShots.clear();

    shields[0].Reset(sf::Vector2i(182, 289));
    shields[1].Reset(sf::Vector2i(266, 289));
    shields[2].Reset(sf::Vector2i(352, 289));
    shields[3].Reset(sf::Vector2i(436, 289));

    AlienEnemy::Level++;
    goingToTheLeft = true;
    enemiesAlive = MAX_ENEMIES;
    const int BASE_Y = 48, OFFSET_Y = 16;
    const int BASE_X = LEFT_LIMIT + 118, OFFSET_X = 16;
    const int TYPES[] = {
        AlienEnemy::TYPE_3, AlienEnemy::TYPE_2, AlienEnemy::TYPE_2,
        AlienEnemy::TYPE_1, AlienEnemy::TYPE_1
    };
    for (int i = 0; i < ROWS; ++i) {
        int y = BASE_Y + OFFSET_Y * i;
        for (int j = 0; j < COLS; ++j) {
            int x = BASE_X + OFFSET_X * j;
            enemies[i][j].Reset(TYPES[i], sf::Vector2i(x, y));
        }
    }

    enemiesStart = sf::Vector2i(0, 0);
    enemiesEnd = sf::Vector2i(COLS - 1, ROWS - 1);
    UpdateSpeed();
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::UpdateSpeed() {
    if (enemiesAlive <= 1) {
        AlienEnemy::Speed = 8;
    } else if (enemiesAlive <= 4) {
        AlienEnemy::Speed = 7;
    } else if (enemiesAlive <= 7) {
        AlienEnemy::Speed = 6;
    } else if (enemiesAlive <= 11) {
        AlienEnemy::Speed = 5;
    } else if (enemiesAlive <= 22) {
        AlienEnemy::Speed = 4;
    } else if (enemiesAlive <= 33) {
        AlienEnemy::Speed = 3;
    } else if (enemiesAlive <= 44) {
        AlienEnemy::Speed = 2;
    } else {
        AlienEnemy::Speed = 1;
    }
}

//--------------------------------------------------------------------------------

void AlienManager::InnerData::UpdateHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << std::setfill(' ') << std::setw(3) << victim;
        return strconv.str();
    };
    downLabel->Text("Life: " + ToString(player.Life()));
    GUIUtil::CenterLabel(downLabel, CoreManager::HALF_LOW_WIDTH);
}

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & AlienManager::Tileset() { return data_->tileset; }

int AlienManager::DeathCount() { return data_->deathCount; }

Sound & AlienManager::KeyboardSound() { return data_->keyboardSound; }
Sound & AlienManager::FireSound()     { return data_->fireSound;     }
Sound & AlienManager::EnemySound()    { return data_->enemySound;    }
Sound & AlienManager::PlayerSound()   { return data_->playerSound;   }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void AlienManager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();

        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->tileset = core->LoadTexture("Content/Textures/AlienParty.png");
        data_->barTexture.Load(data_->tileset, sf::IntRect(191, 29, 320, 1));

        // Set the HUD data of the game.
        data_->downLabel.reset(new SimpleLabel("", 0, 342,
            AtariPalette::Hue00Lum14, core->Retro70Font()));

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->fireSound.Load("Content/Sounds/AlienFire.wav");
        data_->enemySound.Load("Content/Sounds/AlienEnemy.wav");
        data_->playerSound.Load("Content/Sounds/AlienPlayer.wav");

        // Initialize the logic data.
        data_->player.Initialize();
        AlienEnemy::LoadContent();
        AlienPlayerShot::LoadContent();
        AlienEnemyShot::LoadContent();
        AlienShield::LoadContent();

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void AlienManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->fireSound.Unload();
        data_->enemySound.Unload();
        data_->playerSound.Unload();

        // Unload the textures of the game.
        AlienShield::UnloadContent();
        AlienEnemy::UnloadContent();
        AlienPlayerShot::UnloadContent();
        AlienEnemyShot::UnloadContent();
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

void AlienManager::StartGame() {
    auto * core = CoreManager::Instance();

    data_->substate = INIT_STATE;
    data_->currentTime = 0;
    data_->deathCount = 0;
    data_->player.Reset();

    AlienEnemy::Level = 0;
    data_->NextLevel();
    data_->UpdateHud();
    core->SetNextState(MakeSharedState<AlienGameState>());
}

//--------------------------------------------------------------------------------

void AlienManager::DrawGame() {
    for (int i = 0; i < MAX_SHIELDS; ++i) {
        data_->shields[i].Draw();
    }
    ForEach(data_->playerShots, [] (AlienPlayerShot & item) {
        item.Draw();
    });
    ForEach(data_->enemyShots, [] (AlienEnemyShot & item) {
        item.Draw();
    });
    data_->player.Draw();
    data_->DrawEnemies();

    data_->barTexture.Draw(0, BAR_POSITION_Y, AtariPalette::Hue00Lum14);
    data_->barTexture.Draw(CoreManager::HALF_LOW_WIDTH, BAR_POSITION_Y, AtariPalette::Hue00Lum14);
    data_->downLabel->Draw();
}

//--------------------------------------------------------------------------------

void AlienManager::UpdateGame(const sf::Time & timeDelta) {
    if (data_->substate == EXIT_STATE) {
        if (data_->player.IsDeathAnimationFinished()) {
            CoreManager::Instance()->SetNextState(MakeSharedState<AlienGameOverState>());
        } else {
            data_->player.Update();
            data_->UpdateAnimationEnemies();
        }

    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        KeyboardSound().Play();
        data_->substate = PAUSE_STATE;
        CoreManager::Instance()->SetNextState(MakeSharedState<AlienGameMenuState>());

    } else  if (data_->substate == INIT_STATE || data_->substate == PAUSE_STATE) {
        data_->currentTime += timeDelta.asMilliseconds();
        if (data_->currentTime >= MAX_PAUSE_INTERVAL) {
            data_->currentTime = 0;
            data_->substate = GAME_STATE;
        }

    } else if (data_->substate == GAME_STATE) {
        data_->currentTime += timeDelta.asMilliseconds();
        if (data_->currentTime >= MAX_TIME_INTERVAL) {
            data_->currentTime -= MAX_TIME_INTERVAL;

            data_->player.Update();
            data_->UpdateEnemies();
            ForEach(data_->enemyShots, [&] (AlienEnemyShot & item) {
                item.Update();
                if (data_->player.Collision(item)) {
                    data_->UpdateHud();
                }
                for (int i = 0; i < MAX_SHIELDS; ++i) {
                    if (data_->shields[i].Collision(item)) break;
                }
            });
            ForEach(data_->playerShots, [&] (AlienPlayerShot & item) {
                item.Update();
                data_->CollisionEnemies(item);
                data_->CollisionEnemyShots(item);
                for (int i = 0; i < MAX_SHIELDS; ++i) {
                    if (data_->shields[i].Collision(item)) break;
                }
            });
            data_->CheckEnemies();
        }

        if (data_->player.IsDead()) {
            data_->substate = EXIT_STATE;
        } else if (data_->enemiesAlive <= 0) {
            data_->NextLevel();
        }
    }
}

//--------------------------------------------------------------------------------

void AlienManager::EnemyDies() {
    data_->deathCount++;
    data_->enemiesAlive--;
}

//--------------------------------------------------------------------------------

void AlienManager::EnemyLanded() {
    data_->player.Kill();
}
//--------------------------------------------------------------------------------

void AlienManager::CreatePlayerShot(const sf::Vector2i & coords) {
    data_->CreateShot(data_->playerShots, coords);
}

//--------------------------------------------------------------------------------

void AlienManager::CreateEnemyShot(const sf::Vector2i & coords) {
    data_->CreateShot(data_->enemyShots, coords);
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
AlienManager * AlienManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
AlienManager::AlienManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
AlienManager::~AlienManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
AlienManager * AlienManager::Instance() {
    if (!instance_) {
        instance_ = new AlienManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
AlienManager & AlienManager::Reference() {
    return *(Instance());
}
