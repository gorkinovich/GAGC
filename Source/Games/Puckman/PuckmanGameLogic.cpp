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

#include "PuckmanGameLogic.h"
#include <System/Sound.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Puckman/PuckmanGhost.h>
#include <Games/Puckman/PuckmanGameTimer.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanGhostController.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the game logic object.
 */
void GameLogic::Initialize(FruitTypeEnum currentFruit, bool firstPlay, bool clearMaze) {
    // First, we'll get the current used maze.
    maze_ = data_->MazePtr();

    // Then, we'll set the collision controler of the ghosts.
    red_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    pink_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    blue_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    brown_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);

    // And, we'll initialize the current maze data.
    initializeMaze(clearMaze);

    // Second, we'll initialize the ghost data.
    initializeGhosts();
    if(data_->Level() > 20) {
        hurryMode_ = true;
    }
    GhostController::Initialize(red_.get(), pink_.get(), blue_.get(),
        brown_.get(), &(data_->PuckmanRef()), maze_);

    // Third, we'll initialize the fruit data.
    initializeFruit(currentFruit);

    // Fourth, we'll initialize the puckman data.
    data_->PuckmanRef().Respawn();
    initializePuckman(data_->PuckmanRef());

    // And finally, we'll initialize the logic data.
    victory_ = false;
    ended_ = false;
    showFruitPoints_ = false;
    showFruitPointsTime_ = 0;
    if(firstPlay) {
        state_ = INITIAL_STATE;
        manager_->Beginning().Play();
    } else {
        state_ = INITIAL_NO_SOUND_STATE;
    }
    GameTimer::Set(BEGIN_DELAY_TIME);
}

//--------------------------------------------------------------------------------

/**
 * Releases the game logic object.
 */
void GameLogic::Release() {
}

//--------------------------------------------------------------------------------

/**
 * Draws the content of the game.
 */
void GameLogic::Draw() {
    switch(state_) {
    case INITIAL_STATE:
        // First, we'll draw the maze.
        drawMaze();
        // Second, we'll draw the player turn message.
        if(data_->PlayerTurn() == 1) {
            manager_->DrawText(14, 9, "PLAYER ONE", Palette::Cyan);
        } else {
            manager_->DrawText(14, 9, "PLAYER TWO", Palette::Cyan);
        }
        // And finally, we'll draw the ready message.
        manager_->DrawText(20, 11, "READY!", Palette::Yellow);
        break;

    case INITIAL_NO_SOUND_STATE:
    case INITIAL_SOUND_STATE:
        // First, we'll draw the maze.
        drawMaze();
        // Second, we'll draw the entities.
        brown_->Draw();
        blue_->Draw();
        pink_->Draw();
        red_->Draw();
        data_->PuckmanRef().Draw();
        // And finally, we'll draw the ready message.
        manager_->DrawText(20, 11, "READY!", Palette::Yellow);
        break;

    case NORMAL_STATE:
    case KILL_STATE:
        // First, we'll draw the maze and the fruit.
        drawMaze();
        drawFruit();
        drawFruitPoints();

        // Second, we'll check if the ghost are normal or not, because we have
        // to draw ghost when they are normal over puckman and when they are
        // vulnerable or dead under puckman.
        if(ghostsState_ == GhostState::Normal) {
            drawDeadGhosts();
            data_->PuckmanRef().Draw();
            drawAliveGhosts();

        } else {
            // If the ghosts are in vulnerable state and puckman eats one, there
            // will be a pause to show the points of the last killed ghost, that's
            // the reason why the timer is used in this state. When the timer is
            // stopped there isn't any pause and everybody is moving.
            if(GameTimer::IsStopped()) {
                drawVulnerableGhosts();
                data_->PuckmanRef().Draw();
                drawNormalGhosts();

            } else {
                // But if there is a pause the puckman and one ghost can't be drawn.
                // So we'll check one by one each ghost to find the eat one, to draw
                // the point over it.
                drawGhostsWhenPuckmanEats();
            }
        }
        break;

    case DEATH_STATE:
        drawMaze();
        data_->PuckmanRef().Draw();
        break;

    case END_STATE:
        drawMaze();
        if(data_->ExtraLives() <= 0) {
            if(data_->NumberOfPlayers() > 1) {
                if(data_->PlayerTurn() == 1) {
                    manager_->DrawText(14, 9, "PLAYER ONE", Palette::Cyan);
                } else {
                    manager_->DrawText(14, 9, "PLAYER TWO", Palette::Cyan);
                }
            }
            manager_->DrawText(20, 9, "GAME  OVER", Palette::Red);
        }
        break;

    case VICTORY_STATE:
        drawMaze();
        drawFruit();
        drawFruitPoints();

        if(ghostsState_ == GhostState::Normal) {
            drawDeadGhosts();
            data_->PuckmanRef().Draw();
            drawAliveGhosts();
        } else {
            drawVulnerableGhosts();
            data_->PuckmanRef().Draw();
            drawNormalGhosts();
        }
        break;

    case END_VICTORY_STATE:
        drawMaze();
        data_->PuckmanRef().Draw();
        manager_->DrawText(20, 10, "VICTORY!", Palette::Yellow);
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner logic of the game.
 */
void GameLogic::Update(int time) {
    switch(state_) {
    case INITIAL_STATE:
        // In this state we'll take a pause in the game.
        if(GameTimer::IsStopped()) {
            state_ = INITIAL_SOUND_STATE;
        }
        break;

    case INITIAL_SOUND_STATE:
        // In this state we'll take a pause in the game.
        if(manager_->Beginning().IsStoped()) {
            state_ = NORMAL_STATE;
            manager_->Siren().Play(true);
        }
        break;

    case INITIAL_NO_SOUND_STATE:
        // In this state we'll take a pause in the game.
        if(GameTimer::IsStopped()) {
            state_ = NORMAL_STATE;
            manager_->Siren().Play(true);
        }
        break;

    case NORMAL_STATE:
        // In this state we'll update the big dots blink controller and we'll
        // check if any dead ghost is in the house, to bring it back to life.
        bigDotsLogic_.Update(time);
        checksDeadGhostInHouse();

        if(maze_->NumberOfDots() <= 0) {
            // If all the dots are eated puckman will be victorious.
            victory_ = true;
            manager_->Siren().Stop();
            // We'll stop all the entities.
            data_->PuckmanRef().StopSpeed();
            red_->StopSpeed();
            pink_->StopSpeed();
            blue_->StopSpeed();
            brown_->StopSpeed();
            // And change the inner state of the logic.
            state_ = VICTORY_STATE;
            GameTimer::Set(VICTORY_DELAY_TIME);

        } else {
            // And if the game isn't paused, we'll update the ghost state,
            // the fruit logic, the collisions and all the entities.
            if(GameTimer::IsStopped()) {
                updateGhostState(time);
                updateFruit(time);
                updateFruitPoints(time);

                int aux = numberOfGhostsKilled_;
                checkCollisions(data_->PuckmanRef());
                if(aux != numberOfGhostsKilled_) {
                    manager_->GhostEat().Play();
                    switch(numberOfGhostsKilled_) {
                    case 1: addScore(200);  break;
                    case 2: addScore(400);  break;
                    case 3: addScore(800);  break;
                    case 4: addScore(1600); break;
                    }
                }

                data_->PuckmanRef().Update(time);
                GhostController::Update(time);
                red_->Update(time);
                pink_->Update(time);
                blue_->Update(time);
                brown_->Update(time);

            } else {
                // If the game is paused, we'll only update the ghosts
                // that are not alive and are not the last one killed.
                GhostController::Update(time);
                updateGhostsWhenPuckmanEats(time);
            }
        }
        break;

    case KILL_STATE:
        // Here the puckman have been caught and we're waiting some time.
        if(GameTimer::IsStopped()) {
            state_ = DEATH_STATE;
            data_->PuckmanRef().Kill();
            manager_->Death().Play();
        } else {
            bigDotsLogic_.Update(time);
            red_->Update(time);
            pink_->Update(time);
            blue_->Update(time);
            brown_->Update(time);
        }
        break;

    case DEATH_STATE:
        // After wait, we'll see the puckman death animation.
        if(data_->PuckmanRef().IsDeathSpriteEnded()) {
            state_ = END_STATE;
            if(data_->ExtraLives() > 0) {
                GameTimer::Set(SHORT_END_DELAY_TIME);
            } else {
                GameTimer::Set(END_DELAY_TIME);
            }
        } else {
            bigDotsLogic_.Update(time);
            data_->PuckmanRef().Update(time);
        }
        break;

    case END_STATE:
        // And finally we'll have a pause and end the game.
        if(GameTimer::IsStopped()) {
            ended_ = true;
        }
        break;

    case VICTORY_STATE:
        // Here the puckman have been victorious and we're waiting some time.
        if(GameTimer::IsStopped()) {
            state_ = END_VICTORY_STATE;
            GameTimer::Set(END_VICTORY_DELAY_TIME);
        } else {
            bigDotsLogic_.Update(time);
            red_->Update(time);
            pink_->Update(time);
            blue_->Update(time);
            brown_->Update(time);
        }
        break;

    case END_VICTORY_STATE:
        // And finally we'll have a pause and end the game.
        if(GameTimer::IsStopped()) {
            ended_ = true;
        }
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Adds points to the current score.
 */
void GameLogic::addScore(int points) {
    const int bonusMark = 10000;
    auto score = data_->Score();
    int prev = score / bonusMark;
    score += points;
    data_->Score(score);
    int next = data_->Score() / bonusMark;
    if((next > prev) && (data_->ExtraLives() < 6)) {
        manager_->ExtraPac().Play();
        data_->ExtraLives(data_->ExtraLives() + 1);
    }
    data_->CheckHighScore();
}

//--------------------------------------------------------------------------------

/**
 * Update the state of the logic to kill the puckman.
 */
void GameLogic::killPuckman() {
    manager_->Siren().Stop();
    data_->PuckmanRef().StopSpeed();
    genericKillPuckman();
    GameTimer::Set(KILL_DELAY_TIME);
}

//--------------------------------------------------------------------------------

/**
 * Draws the points of an eated fruit.
 */
void GameLogic::drawFruitPoints() {
    if(showFruitPoints_) {
        int col = 13;
        switch(fruit_.Symbol()) {
        case FontSymbol::N100:  col = 13; break;
        case FontSymbol::N300:  col = 13; break;
        case FontSymbol::N500:  col = 13; break;
        case FontSymbol::N700:  col = 13; break;
        case FontSymbol::N1000: col = 12; break;
        case FontSymbol::N2000: col = 12; break;
        case FontSymbol::N3000: col = 12; break;
        case FontSymbol::N5000: col = 12; break;
        }
        manager_->DrawSymbol(20, col, fruit_.Symbol(), Palette::Pink);
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the points of an eated fruit.
 */
void GameLogic::updateFruitPoints(int time) {
    if(showFruitPoints_) {
        showFruitPointsTime_ -= time;
        if(showFruitPointsTime_ <= 0) {
            showFruitPoints_ = false;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Check the collision between the puckman and a fruit in the maze.
 */
void GameLogic::checkFruitCollision(PuckmanEntity & puckman) {
    if(showFruit_) {
        if(checkCollision(data_->PuckmanRef(), fruit_)) {
            showFruit_ = false;
            showFruitPoints_ = true;
            manager_->FruitEat().Play();
            addScore(fruit_.Value());
            showFruitPointsTime_ = SHOW_FRUIT_POINTS_DELAY_TIME;
            fruitTime_ += SHOW_FRUIT_DELAY_TIME;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks the number of dots to update the logic of the game.
 */
void GameLogic::checkNumberOfDots() {
    maze_->NumberOfDots(maze_->NumberOfDots() - 1);
    switch(data_->Level()) {
    case  1: if(maze_->NumberOfDots() <=  10) hurryMode_ = true; break;
    case  2: if(maze_->NumberOfDots() <=  20) hurryMode_ = true; break;
    case  3: if(maze_->NumberOfDots() <=  30) hurryMode_ = true; break;
    case  4: if(maze_->NumberOfDots() <=  40) hurryMode_ = true; break;
    case  5: if(maze_->NumberOfDots() <=  50) hurryMode_ = true; break;
    case  6: if(maze_->NumberOfDots() <=  60) hurryMode_ = true; break;
    case  7: if(maze_->NumberOfDots() <=  70) hurryMode_ = true; break;
    case  8: if(maze_->NumberOfDots() <=  80) hurryMode_ = true; break;
    case  9: if(maze_->NumberOfDots() <=  90) hurryMode_ = true; break;
    case 10: if(maze_->NumberOfDots() <= 100) hurryMode_ = true; break;
    case 11: if(maze_->NumberOfDots() <= 110) hurryMode_ = true; break;
    case 12: if(maze_->NumberOfDots() <= 120) hurryMode_ = true; break;
    case 13: if(maze_->NumberOfDots() <= 130) hurryMode_ = true; break;
    case 14: if(maze_->NumberOfDots() <= 140) hurryMode_ = true; break;
    case 15: if(maze_->NumberOfDots() <= 150) hurryMode_ = true; break;
    case 16: if(maze_->NumberOfDots() <= 160) hurryMode_ = true; break;
    case 17: if(maze_->NumberOfDots() <= 170) hurryMode_ = true; break;
    case 18: if(maze_->NumberOfDots() <= 180) hurryMode_ = true; break;
    case 19: if(maze_->NumberOfDots() <= 190) hurryMode_ = true; break;
    case 20: if(maze_->NumberOfDots() <= 200) hurryMode_ = true; break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the puckman entity.
 */
void GameLogic::puckmanUpdate() {
    // First, we'll get the current cell of the puckman entity.
    sf::Vector2i cell = Maze::SpriteCoordsToMaze(data_->PuckmanRef().X, data_->PuckmanRef().Y);

    // Second, if the coordinates are inside the maze.
    if(0 <= cell.x && cell.x < Manager::COLUMS) {
        // We'll get the value of that cell.
        int cellValue = maze_->Data()[cell.y][cell.x];

        // And then we'll check that value.
        if(cellValue == Maze::CLEAN) {
            // If the cell is empty and isn't the last cell cleared, the
            // puckman will go in fast mode.
            if(cell != lastCellCleared_) {
                data_->PuckmanRef().SetFast();
            }

        } else if(cellValue == Maze::DOT) {
            // If the cell have a dot, we'll clear the cell and set
            // puckman in slow mode.
            clearCell(data_->PuckmanRef(), cell);
            checkNumberOfDots();
            addScore(10);

        } else if(cellValue == Maze::BIG_DOT) {
            // If the cell have a big dot, we'll clear the cell, set
            // puckman in slow mode and set the ghosts in vulnerable mode.
            clearCell(data_->PuckmanRef(), cell);
            setGhostsToVulnerable();
            checkNumberOfDots();
            addScore(50);
        }
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
GameLogic::GameLogic() : GenericLogic(), data_(Manager::Instance()->DataInstance()),
    manager_(Manager::Instance()), showFruitPointsTime_(0),
    showFruitPoints_(false), victory_(false) {
    red_ = std::make_shared<Ghost>(GhostType::Red);
    pink_ = std::make_shared<Ghost>(GhostType::Pink);
    blue_ = std::make_shared<Ghost>(GhostType::Blue);
    brown_ = std::make_shared<Ghost>(GhostType::Brown);

    data_->Player1().PuckmanRef().OnMove.push_back([this] (MovingEntity &) { puckmanUpdate(); });
    data_->Player2().PuckmanRef().OnMove.push_back([this] (MovingEntity &) { puckmanUpdate(); });
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
GameLogic::~GameLogic() {}
