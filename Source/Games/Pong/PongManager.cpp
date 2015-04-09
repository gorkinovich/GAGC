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

#include "PongManager.h"
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
#include <System/MathUtil.h>
#include <Menu/DesktopState.h>
#include <Games/Pong/PongGameState.h>
#include <Games/Pong/PongGameMenuState.h>
#include <Games/Pong/PongGameOverState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define ROW0  336

#define INIT_STATE   0
#define GAME_STATE   1
#define EXIT_STATE   2
#define PAUSE_STATE  3

#define MAX_PAUSE_INTERVAL  1000
#define MAX_TIME_INTERVAL     40

#define SQUARE_WIDTH   10
#define SQUARE_HEIGHT  10
#define PLAYER_WIDTH   10
#define PLAYER_HEIGHT  40
#define SQUARE_WHALF    5
#define SQUARE_HHALF    5
#define PLAYER_WHALF    5
#define PLAYER_HHALF   20

#define BORDER_Y1    10
#define BORDER_Y2   320
#define BORDER_COL  AtariPalette::Hue00Lum04

#define TOP_LIMIT      20
#define BOTTOM_LIMIT  320
#define LEFT_LIMIT      5
#define RIGHT_LIMIT   635

#define LEFT_COLLIDE_POINT    25
#define RIGHT_COLLIDE_POINT  615

#define PLA1_START_X   15
#define PLA1_START_Y  170
#define PLA2_START_X  625
#define PLA2_START_Y  170
#define BALL_START_X  320
#define BALL_START_Y  170

#define BALL_START_VX  5
#define BALL_START_VY  5

#define BALL_MAX_VX  10
#define BALL_MAX_VY  10

#define ENTITY_COL  AtariPalette::Hue00Lum14

//********************************************************************************
// InnerData
//********************************************************************************

struct PongManager::Player {
    int score;
    sf::Vector2i direction;
    sf::Vector2i position;
};

struct PongManager::Ball {
    sf::Vector2i direction;
    sf::Vector2i position;
    bool collided;
};

struct PongManager::InnerData {
    Texture2D squareTexture;
    Texture2D playerTexture;

    int currentTime;
    int difficulty;
    int numberOfPlayers;
    int numberOfPoints;
    PongManager::Player player1;
    PongManager::Player player2;
    PongManager::Ball ball;

    int substate;
    std::unique_ptr<SimpleLabel> downLabel;
    std::unique_ptr<SimpleLabel> player1ScoreLabel;
    std::unique_ptr<SimpleLabel> player2ScoreLabel;

    Sound menuSelectSound;
    Sound menuKeySound;
    Sound menuNextSound;
    Sound ballLeftSound;
    Sound ballRightSound;
    Sound pointSound;
};

//********************************************************************************
// Properties
//********************************************************************************

int PongManager::ScorePlayer1() { return data_->player1.score; }
int PongManager::ScorePlayer2() { return data_->player2.score; }

//--------------------------------------------------------------------------------

Sound & PongManager::MenuSelectSound() { return data_->menuSelectSound; }
Sound & PongManager::MenuKeySound()    { return data_->menuKeySound;    }
Sound & PongManager::MenuNextSound()   { return data_->menuNextSound;   }
Sound & PongManager::BallLeftSound()   { return data_->ballLeftSound;   }
Sound & PongManager::BallRightSound()  { return data_->ballRightSound;  }
Sound & PongManager::PointSound()      { return data_->pointSound;      }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void PongManager::Initialize() {
    if (!initialized_) {
        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->squareTexture.Load(SQUARE_WIDTH, SQUARE_HEIGHT);
        data_->playerTexture.Load(PLAYER_WIDTH, PLAYER_HEIGHT);

        // Set the HUD data of the game.
        auto * core = CoreManager::Instance();

        data_->downLabel.reset(new SimpleLabel("", 0, ROW0,
            AtariPalette::Hue01Lum14, core->Retro70Font()));

        data_->player1ScoreLabel.reset(new SimpleLabel("P1: 00",
            8, ROW0, AtariPalette::Hue04Lum02, core->Retro70Font()));

        data_->player2ScoreLabel.reset(new SimpleLabel("P2: 00",
            584, ROW0, AtariPalette::Hue09Lum02, core->Retro70Font()));

        // Load the sounds of the game.
        data_->menuSelectSound.Load("Content/Sounds/PongMenuSelect.wav");
        data_->menuKeySound.Load("Content/Sounds/PongMenuKey.wav");
        data_->menuNextSound.Load("Content/Sounds/PongMenuNext.wav");
        data_->ballLeftSound.Load("Content/Sounds/PongBallLeft.wav");
        data_->ballRightSound.Load("Content/Sounds/PongBallRight.wav");
        data_->pointSound.Load("Content/Sounds/PongPoint.wav");

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void PongManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->menuKeySound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->menuSelectSound.Unload();
        data_->menuKeySound.Unload();
        data_->menuNextSound.Unload();
        data_->ballLeftSound.Unload();
        data_->ballRightSound.Unload();
        data_->pointSound.Unload();

        // Unload the textures of the game.
        data_->squareTexture.Unload();
        data_->playerTexture.Unload();

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
void PongManager::StartGame(int difficulty, int numberOfPlayers, int numberOfPoints) {
    auto * core = CoreManager::Instance();

    // Check the difficulty to set some values.
    data_->difficulty = difficulty;
    data_->numberOfPlayers = numberOfPlayers;
    data_->numberOfPoints = numberOfPoints;

    // Set the initial data of the entities.
    data_->player1.score = 0;
    data_->player1.direction = sf::Vector2i(0, 0);
    data_->player1.position = sf::Vector2i(PLA1_START_X, PLA1_START_Y);

    data_->player2.score = 0;
    data_->player2.direction = sf::Vector2i(0, 0);
    data_->player2.position = sf::Vector2i(PLA2_START_X, PLA2_START_Y);

    resetBall();

    // Set the mark of time, the hud and the substate of the game.
    data_->currentTime = 0;
    updateHud();
    changeSubstate(INIT_STATE);

    // Go to the game state to run the current new game.
    core->SetNextState(MakeSharedState<PongGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void PongManager::DrawGame() {
    // Draw the world of the game.
    for (int x = 0; x < CoreManager::LOW_WIDTH; x += SQUARE_WIDTH) {
        data_->squareTexture.Draw(x, BORDER_Y1, BORDER_COL);
        data_->squareTexture.Draw(x, BORDER_Y2, BORDER_COL);
    }

    // Draw the entities.
    data_->squareTexture.Draw(
        data_->ball.position.x - SQUARE_WHALF,
        data_->ball.position.y - SQUARE_HHALF,
        ENTITY_COL
    );
    data_->playerTexture.Draw(
        data_->player1.position.x - PLAYER_WHALF,
        data_->player1.position.y - PLAYER_HHALF,
        ENTITY_COL
    );
    data_->playerTexture.Draw(
        data_->player2.position.x - PLAYER_WHALF,
        data_->player2.position.y - PLAYER_HHALF,
        ENTITY_COL
    );

    // Draw the labels of the hud.
    if (data_->substate == GAME_STATE) {
        data_->player1ScoreLabel->Draw();
        data_->player2ScoreLabel->Draw();
    }
    data_->downLabel->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void PongManager::UpdateGame(const sf::Time & timeDelta) {
    if (data_->substate == EXIT_STATE) {
        // The exit state is when the game is over. We'll need only to wait
        // to any key to be pressed to set the next state of the game.
        if (Keyboard::IsAnyKeyUp()) {
            CoreManager::Instance()->SetNextState(MakeSharedState<PongGameOverState>());
        }
    } else if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        // In the other substates the player can press escape and invoke the menu.
        // This will invoke the substate pause and call the game menu state.
        MenuNextSound().Play();
        changeSubstate(PAUSE_STATE);
        CoreManager::Instance()->SetNextState(MakeSharedState<PongGameMenuState>());
    } else  if (data_->substate == INIT_STATE || data_->substate == PAUSE_STATE) {
        // In the initial or the pause substate there will be a pause of time,
        // just to make possible to the players that they can see the current
        // state of the world, and locate their avatars.
        data_->currentTime += timeDelta.asMilliseconds();
        if (data_->currentTime >= MAX_PAUSE_INTERVAL) {
            data_->currentTime = 0;
            // After the pause we'll change the substate and "start" the
            // logic of the snakes inside the world.
            MenuNextSound().Play();
            changeSubstate(GAME_STATE);
        }
    } else if (data_->substate == GAME_STATE) {
        // After some interval of time, we'll update the internal logic.
        data_->currentTime += timeDelta.asMilliseconds();
        if (data_->currentTime >= MAX_TIME_INTERVAL) {
            data_->currentTime -= MAX_TIME_INTERVAL;

            // Update the players.
            if (data_->numberOfPlayers < 2) {
                updatePlayer();
                updateMachine();
            } else {
                updatePlayer1();
                updatePlayer2();
            }

            // Update the ball.
            updateBall();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Resets the ball state.
 */
void PongManager::resetBall() {
    auto * core = CoreManager::Instance();

    data_->ball.direction = sf::Vector2i(BALL_START_VX, BALL_START_VY);
    data_->ball.position = sf::Vector2i(BALL_START_X, BALL_START_Y);
    data_->ball.collided = false;

    if (core->Random(10) < 5) {
        data_->ball.direction.x *= -1;
    }
    if (core->Random(10) < 5) {
        data_->ball.direction.y *= -1;
    }
}

//--------------------------------------------------------------------------------

/**
 * Adds a point to a player.
 */
void PongManager::addPoint(Player & player) {
    PointSound().Play();
    player.score++;
    resetBall();
    updateHud();
    if (data_->player1.score + data_->player2.score >= data_->numberOfPoints) {
        changeSubstate(EXIT_STATE);
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the logic of the ball.
 */
void PongManager::updateBall() {
    if (data_->ball.position.x < LEFT_LIMIT) {
        // Player 2 gets a point.
        addPoint(data_->player2);

    } else if (data_->ball.position.x >= RIGHT_LIMIT) {
        // Player 1 gets a point.
        addPoint(data_->player1);

    } else {
        auto nextPosition = data_->ball.position + data_->ball.direction;
        auto line = LinearEquation<int>(data_->ball.position, nextPosition);
        if (nextPosition.x < LEFT_COLLIDE_POINT) {
            // Check the situation after the left collide point.
            if (data_->ball.direction.x < 0 && !data_->ball.collided) {
                auto & pos = data_->player1.position;
                for (int x = data_->ball.position.x; x != nextPosition.x; --x) {
                    int y = line.GetY(x);
                    sf::Vector2i ppos1(pos.x - PLAYER_WHALF, pos.y - PLAYER_HHALF);
                    sf::Vector2i ppos2(pos.x + PLAYER_WHALF, pos.y + PLAYER_HHALF);
                    sf::Vector2i bpos1(x - SQUARE_WHALF, y - SQUARE_HHALF);
                    sf::Vector2i bpos2(x + SQUARE_WHALF, y + SQUARE_HHALF);
                    if (MathUtil::RectangleCollides(bpos1, bpos2, ppos1, ppos2)) {
                        nextPosition.x = x + 1;
                        nextPosition.y = line.GetY(nextPosition.x);
                        if (ppos2.x <= (nextPosition.x - SQUARE_HHALF)) {
                            const int SIDE_LEN = PLAYER_HHALF + SQUARE_HHALF;
                            int dif = nextPosition.y - pos.y;
                            data_->ball.direction.x = BALL_MAX_VX * (SIDE_LEN - std::abs(dif)) / SIDE_LEN;
                            data_->ball.direction.y = BALL_MAX_VY * dif / SIDE_LEN;
                            if (data_->ball.direction.x < BALL_START_VX) {
                                data_->ball.direction.x = BALL_START_VX;
                            }
                            BallLeftSound().Play();

                        } else if(nextPosition.y < pos.y) {
                            if (data_->ball.direction.y > 0) {
                                data_->ball.direction.y *= -1;
                            }

                        } else if(nextPosition.y > pos.y) {
                            if (data_->ball.direction.y < 0) {
                                data_->ball.direction.y *= -1;
                            }
                        }
                        data_->ball.collided = true;
                        break;
                    }
                }
            }
            if (nextPosition.y - SQUARE_HHALF < TOP_LIMIT) {
                data_->ball.position.y = TOP_LIMIT + SQUARE_HHALF;
                data_->ball.position.x = line.GetX(data_->ball.position.y);
                if (data_->ball.direction.y < 0) {
                    data_->ball.direction.y *= -1;
                }

            } else if (nextPosition.y + SQUARE_HHALF > BOTTOM_LIMIT) {
                data_->ball.position.y = BOTTOM_LIMIT - SQUARE_HHALF;
                data_->ball.position.x = line.GetX(data_->ball.position.y);
                if (data_->ball.direction.y > 0) {
                    data_->ball.direction.y *= -1;
                }

            } else {
                data_->ball.position = nextPosition;
            }

        } else if (nextPosition.x > RIGHT_COLLIDE_POINT) {
            // Check the situation after the right collide point.
            if (data_->ball.direction.x > 0 && !data_->ball.collided) {
                auto & pos = data_->player2.position;
                for (int x = data_->ball.position.x; x != nextPosition.x; ++x) {
                    int y = line.GetY(x);
                    sf::Vector2i ppos1(pos.x - PLAYER_WHALF, pos.y - PLAYER_HHALF);
                    sf::Vector2i ppos2(pos.x + PLAYER_WHALF - 1, pos.y + PLAYER_HHALF - 1);
                    sf::Vector2i bpos1(x - SQUARE_WHALF, y - SQUARE_HHALF);
                    sf::Vector2i bpos2(x + SQUARE_WHALF - 1, y + SQUARE_HHALF - 1);
                    if (MathUtil::RectangleCollides(bpos1, bpos2, ppos1, ppos2)) {
                        nextPosition.x = x - 1;
                        nextPosition.y = line.GetY(nextPosition.x);
                        if (ppos1.x >= (nextPosition.x + SQUARE_HHALF)) {
                            const int SIDE_LEN = PLAYER_HHALF + SQUARE_HHALF;
                            int dif = nextPosition.y - pos.y;
                            data_->ball.direction.x = -(BALL_MAX_VX * (SIDE_LEN - std::abs(dif)) / SIDE_LEN);
                            data_->ball.direction.y = BALL_MAX_VY * dif / SIDE_LEN;
                            if (data_->ball.direction.x > -BALL_START_VX) {
                                data_->ball.direction.x = -BALL_START_VX;
                            }
                            BallRightSound().Play();

                        } else if(nextPosition.y < pos.y) {
                            if (data_->ball.direction.y > 0) {
                                data_->ball.direction.y *= -1;
                            }

                        } else if(nextPosition.y > pos.y) {
                            if (data_->ball.direction.y < 0) {
                                data_->ball.direction.y *= -1;
                            }
                        }
                        data_->ball.collided = true;
                        break;
                    }
                }
            }
            if (nextPosition.y - SQUARE_HHALF < TOP_LIMIT) {
                data_->ball.position.y = TOP_LIMIT + SQUARE_HHALF;
                data_->ball.position.x = line.GetX(data_->ball.position.y);
                if (data_->ball.direction.y < 0) {
                    data_->ball.direction.y *= -1;
                }

            } else if (nextPosition.y + SQUARE_HHALF > BOTTOM_LIMIT) {
                data_->ball.position.y = BOTTOM_LIMIT - SQUARE_HHALF;
                data_->ball.position.x = line.GetX(data_->ball.position.y);
                if (data_->ball.direction.y > 0) {
                    data_->ball.direction.y *= -1;
                }

            } else {
                data_->ball.position = nextPosition;
            }

        } else {
            // Check the situation in the rest of the world.
            if (nextPosition.y - SQUARE_HHALF < TOP_LIMIT) {
                data_->ball.position.y = TOP_LIMIT + SQUARE_HHALF;
                data_->ball.position.x = line.GetX(data_->ball.position.y);
                if (data_->ball.direction.y < 0) {
                    data_->ball.direction.y *= -1;
                }

            } else if (nextPosition.y + SQUARE_HHALF > BOTTOM_LIMIT) {
                data_->ball.position.y = BOTTOM_LIMIT - SQUARE_HHALF;
                data_->ball.position.x = line.GetX(data_->ball.position.y);
                if (data_->ball.direction.y > 0) {
                    data_->ball.direction.y *= -1;
                }

            } else {
                data_->ball.position = nextPosition;
                data_->ball.collided = false;
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the player 1 in a single-player game.
 */
void PongManager::updatePlayer() {
    int movement = 0;
    if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) {
        --movement;
    }
    if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) {
        ++movement;
    }
    updatePlayer(data_->player1, movement);
}

//--------------------------------------------------------------------------------

/**
 * Updates the AI in a single-player game.
 */
void PongManager::updateMachine() {
    int movement = 0;
    if (data_->ball.direction.x >= 0) {
        int chaos = CoreManager::Instance()->Random(20);
        if (chaos != 0) {
            int sideLimit = PLAYER_HHALF - PLAYER_HHALF * std::abs(data_->ball.direction.y) / BALL_MAX_VY;
            if (data_->difficulty == DIFFICULTY_EASY && data_->ball.position.x > 480) {
                if (data_->ball.position.y < data_->player2.position.y - sideLimit) {
                    --movement;
                } else if (data_->ball.position.y > data_->player2.position.y + sideLimit) {
                    ++movement;
                }
            } else if (data_->difficulty == DIFFICULTY_NORMAL && data_->ball.position.x > 320) {
                if (data_->ball.position.y < data_->player2.position.y - sideLimit) {
                    --movement;
                } else if (data_->ball.position.y > data_->player2.position.y + sideLimit) {
                    ++movement;
                }
            } else if (data_->difficulty == DIFFICULTY_HARD && data_->ball.position.x > 160) {
                if (data_->ball.position.y < data_->player2.position.y - sideLimit) {
                    --movement;
                } else if (data_->ball.position.y > data_->player2.position.y + sideLimit) {
                    ++movement;
                }
            }
        }
    }
    updatePlayer(data_->player2, movement);
}

//--------------------------------------------------------------------------------

/**
 * Updates the player 1 in a multi-player game.
 */
void PongManager::updatePlayer1() {
    int movement = 0;
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        --movement;
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        ++movement;
    }
    updatePlayer(data_->player1, movement);
}

//--------------------------------------------------------------------------------

/**
 * Updates the player 2 in a multi-player game.
 */
void PongManager::updatePlayer2() {
    int movement = 0;
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        --movement;
    }
    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        ++movement;
    }
    updatePlayer(data_->player2, movement);
}

//--------------------------------------------------------------------------------

/**
 * Updates the player logic.
 */
void PongManager::updatePlayer(Player & player, int direction) {
    // Calculate the direction.
    if (direction < 0) {
        if (player.direction.y > 0) {
            player.direction.y = -1;
        } else {
            player.direction.y -= 1;
        }
    } else if (direction > 0) {
        if (player.direction.y < 0) {
            player.direction.y = 1;
        } else {
            player.direction.y += 1;
        }
    } else {
        player.direction.y = 0;
    }

    // Calculate the next coordinate.
    int nextY = player.position.y + player.direction.y;
    if (nextY - PLAYER_HHALF < TOP_LIMIT) {
        nextY = PLAYER_HHALF + TOP_LIMIT;
    } else if (nextY + PLAYER_HHALF > BOTTOM_LIMIT) {
        nextY = BOTTOM_LIMIT - PLAYER_HHALF;
    }
    player.position.y = nextY;
}

//--------------------------------------------------------------------------------

/**
 * Changes the substate of the game.
 */
void PongManager::changeSubstate(int value) {
    // Get some data and change the substate.
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    data_->substate = value;

    // Set the HUD values of the substate.
    switch (data_->substate) {
    case INIT_STATE:
        data_->downLabel->Color(AtariPalette::Hue01Lum12);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel->Text("¡Prepárate para empezar!");
        } else {
            data_->downLabel->Text("Get ready to start!");
        }
        break;

    case GAME_STATE:
        data_->downLabel->Color(AtariPalette::Hue11Lum02);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel->Text("Menú (ESC)");
        } else {
            data_->downLabel->Text("Menu (ESC)");
        }
        break;

    case EXIT_STATE:
        data_->downLabel->Color(AtariPalette::Hue01Lum12);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel->Text("Pulse cualquier tecla para continuar...");
        } else {
            data_->downLabel->Text("Press any key to continue...");
        }
        break;

    case PAUSE_STATE:
        data_->downLabel->Color(AtariPalette::Hue01Lum12);
        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->downLabel->Text("¡Prepárate para continuar!");
        } else {
            data_->downLabel->Text("Get ready to continue!");
        }
        break;
    }

    // Set the right position of some elements of the HUD.
    GUIUtil::CenterLabel(data_->downLabel, CoreManager::HALF_LOW_WIDTH);
}

//--------------------------------------------------------------------------------

/**
 * Updates the hud of the game.
 */
void PongManager::updateHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << std::setfill('0') << std::setw(2) << victim;
        return strconv.str();
    };
    data_->player1ScoreLabel->Text("P1: " + ToString(data_->player1.score));
    data_->player2ScoreLabel->Text("P2: " + ToString(data_->player2.score));
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
PongManager * PongManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
PongManager::PongManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
PongManager::~PongManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
PongManager * PongManager::Instance() {
    if (!instance_) {
        instance_ = new PongManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
PongManager & PongManager::Reference() {
    return *(Instance());
}
