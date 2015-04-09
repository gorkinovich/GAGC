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

#include "PuckmanGenericLogic.h"
#include <System/MathUtil.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPuckmanEntity.h>
#include <Games/Puckman/PuckmanGhost.h>
#include <Games/Puckman/PuckmanGameTimer.h>
#include <Games/Puckman/PuckmanSprites.h>

using namespace Puckman;

//********************************************************************************
// Static
//********************************************************************************

Maze GenericLogic::defaultMaze_;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the maze.
 */
void GenericLogic::initializeMaze(bool clearMaze) {
    lastCellCleared_ = sf::Vector2i(0, 0);
    bigDotsLogic_.ResetTime();
    if (clearMaze) {
        maze_->Initialize();
    }
    maze_->DoorOpen(true);
}

//--------------------------------------------------------------------------------

/**
 * Initializes the puckman.
 */
void GenericLogic::initializePuckman(PuckmanEntity & puckman) {
    puckman.SetCoordinates(Maze::PUCKMAN_START_X, Maze::PUCKMAN_START_Y);
    puckman.SetDirection(MovingDirection::Left);
    puckman.ResetSpeed();
    puckman.SetFast();
    puckman.ResetTime();
}

//--------------------------------------------------------------------------------

/**
 * Controls that the speed interval tick of the ghost is right.
 */
void GenericLogic::ghostSpeedController(Ghost & ghost) {
    // First, we'll check if the ghost is alive.
    if (ghost.Alive()) {
        // Second, we'll check if the ghost is in normal mode.
        if (ghost.State() == GhostState::Normal) {
            sf::Vector2i cell = Maze::SpriteCoordsToMaze(ghost.X, ghost.Y);
            const int wormholeRow = 17;
            const int wormholeLeftCol = 4;
            const int wormholeRightCol = 23;

            // Third, we'll check if the ghost is inside the wormhole or the house.
            if (cell.y == wormholeRow && (cell.x <= wormholeLeftCol || cell.x >= wormholeRightCol)) {
                ghost.SetSpeedInterval(WORMHOLE_SPEED_INTERVAL);

            } else if (Maze::GHOST_HOUSE_START_X <= ghost.X && ghost.X <= Maze::GHOST_HOUSE_END_X &&
                       Maze::GHOST_HOUSE_START_Y <= ghost.Y && ghost.Y <= Maze::GHOST_HOUSE_END_Y) {
                ghost.SetSpeedInterval(HOUSE_SPEED_INTERVAL);

            } else {
                // And finally, we'll check if the ghosts are in hurry mode.
                if (hurryMode_) {
                    ghost.SetSpeedInterval(HURRY_SPEED_INTERVAL);
                } else {
                    ghost.SetSpeedInterval(NORMAL_SPEED_INTERVAL);
                }
            }
        } else {
            ghost.SetSpeedInterval(VULNERABLE_SPEED_INTERVAL);
        }
    } else {
        if (Maze::GHOST_HOUSE_START_X <= ghost.X && ghost.X <= Maze::GHOST_HOUSE_END_X &&
            Maze::GHOST_HOUSE_START_Y <= ghost.Y && ghost.Y <= Maze::GHOST_HOUSE_END_Y) {
            ghost.SetSpeedInterval(HOUSE_SPEED_INTERVAL);

        } else {
            ghost.SetSpeedInterval(DEAD_SPEED_INTERVAL);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Initializes one ghost.
 */
void GenericLogic::initializeGhost(Ghost & ghost, int x, int y, MovingDirectionEnum dir) {
    ghost.SetCoordinates(x, y);
    ghost.SetDirection(dir);
    ghost.NeededDirection(dir);
    ghost.BackToLife();
    ghost.ResetTime();
    ghost.ResetSpeed();
    ghost.SpeedController = std::bind(&GenericLogic::ghostSpeedController, this, std::placeholders::_1);
}

//--------------------------------------------------------------------------------

/**
 * Initializes all the ghosts.
 */
void GenericLogic::initializeGhosts() {
    initializeGhost(*red_, Maze::RED_GHOST_START_X, Maze::RED_GHOST_START_Y, MovingDirection::Left);
    initializeGhost(*pink_, Maze::PINK_GHOST_START_X, Maze::PINK_GHOST_START_Y, MovingDirection::Down);
    initializeGhost(*blue_, Maze::BLUE_GHOST_START_X, Maze::BLUE_GHOST_START_Y, MovingDirection::Up);
    initializeGhost(*brown_, Maze::BROWN_GHOST_START_X, Maze::BROWN_GHOST_START_Y, MovingDirection::Up);

    setGhostsToNormal();
    lastGhostKilled_ = NONE_LAST_KILLED;
    numberOfGhostsKilled_ = 0;
    hurryMode_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Updates the state of all the ghosts to the current one.
 */
void GenericLogic::updateGhostsState() {
    ghostTime_ = 0;
    red_->ChangeState(ghostsState_);
    pink_->ChangeState(ghostsState_);
    blue_->ChangeState(ghostsState_);
    brown_->ChangeState(ghostsState_);
}

//--------------------------------------------------------------------------------

/**
 * Sets all the ghosts to normal state.
 */
void GenericLogic::setGhostsToNormal() {
    ghostsState_ = GhostState::Normal;
    updateGhostsState();
}

//--------------------------------------------------------------------------------

/**
 * Sets all the ghosts to vulnerable state.
 */
void GenericLogic::setGhostsToVulnerable() {
    // Here we'll set the data used when puckman eats a ghost.
    lastGhostKilled_ = NONE_LAST_KILLED;
    numberOfGhostsKilled_ = 0;
    // And here we'll change the state of the ghosts.
    ghostsState_ = GhostState::Vulnerable;
    updateGhostsState();
}

//--------------------------------------------------------------------------------

/**
 * Sets all the ghosts to ending vulnerable state.
 */
void GenericLogic::setGhostsToEnding() {
    ghostsState_ = GhostState::Ending;
    ghostTime_ = 0;

    if (red_->State() == GhostState::Vulnerable)
        red_->ChangeState(ghostsState_);

    if (pink_->State() == GhostState::Vulnerable)
        pink_->ChangeState(ghostsState_);

    if (blue_->State() == GhostState::Vulnerable)
        blue_->ChangeState(ghostsState_);

    if (brown_->State() == GhostState::Vulnerable)
        brown_->ChangeState(ghostsState_);
}

//--------------------------------------------------------------------------------

/**
 * Resets the last killed ghost identification.
 */
void GenericLogic::resetLastKilledGhost(int ghost) {
    if (lastGhostKilled_ == ghost) {
        lastGhostKilled_ = NONE_LAST_KILLED;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if a dead ghost is inside the house.
 */
void GenericLogic::checksDeadGhostInHouse(Ghost & ghost) {
    if (!ghost.Alive() && Maze::IsGhostInsideTheHouse(ghost)) {
        switch(ghost.TypeOfGhost()) {
            case GhostType::Red:
                if (ghost.Y >= Maze::GHOST_HOUSE_CENTER_Y) {
                    ghost.BackToLife();
                    resetLastKilledGhost(RED_LAST_KILLED);
                }
                break;

            case GhostType::Pink:
                if (ghost.Y >= Maze::GHOST_HOUSE_CENTER_Y) {
                    ghost.BackToLife();
                    resetLastKilledGhost(PINK_LAST_KILLED);
                }
                break;

            case GhostType::Blue:
                if (ghost.Y >= Maze::GHOST_HOUSE_CENTER_Y &&
                    ghost.X <= Maze::BLUE_GHOST_START_X) {
                    ghost.BackToLife();
                    resetLastKilledGhost(BLUE_LAST_KILLED);
                }
                break;

            case GhostType::Brown:
                if (ghost.Y >= Maze::GHOST_HOUSE_CENTER_Y &&
                    ghost.X >= Maze::BROWN_GHOST_START_X) {
                    ghost.BackToLife();
                    resetLastKilledGhost(BROWN_LAST_KILLED);
                }
                break;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if any dead ghost is inside the house.
 */
void GenericLogic::checksDeadGhostInHouse() {
    checksDeadGhostInHouse(*red_);
    checksDeadGhostInHouse(*pink_);
    checksDeadGhostInHouse(*blue_);
    checksDeadGhostInHouse(*brown_);
}

//--------------------------------------------------------------------------------

/**
 * Draws the dead ghosts.
 */
void GenericLogic::drawDeadGhosts() {
    if (!brown_->Alive()) { brown_->Draw(); }
    if (!blue_->Alive()) { blue_->Draw(); }
    if (!pink_->Alive()) { pink_->Draw(); }
    if (!red_->Alive()) { red_->Draw(); }
}

//--------------------------------------------------------------------------------

/**
 * Draws the ghosts.
 */
void GenericLogic::drawAliveGhosts() {
    if (brown_->Alive()) { brown_->Draw(); }
    if (blue_->Alive()) { blue_->Draw(); }
    if (pink_->Alive()) { pink_->Draw(); }
    if (red_->Alive()) { red_->Draw(); }
}

//--------------------------------------------------------------------------------

/**
 * Draws the vulnerable ghosts.
 */
void GenericLogic::drawVulnerableGhosts() {
    if (brown_->State() != GhostState::Normal) { brown_->Draw(); }
    if (blue_->State() != GhostState::Normal) { blue_->Draw(); }
    if (pink_->State() != GhostState::Normal) { pink_->Draw(); }
    if (red_->State() != GhostState::Normal) { red_->Draw(); }
}

//--------------------------------------------------------------------------------

/**
 * Draws the normal ghosts.
 */
void GenericLogic::drawNormalGhosts() {
    if (brown_->State() == GhostState::Normal) { brown_->Draw(); }
    if (blue_->State() == GhostState::Normal) { blue_->Draw(); }
    if (pink_->State() == GhostState::Normal) { pink_->Draw(); }
    if (red_->State() == GhostState::Normal) { red_->Draw(); }
}

//--------------------------------------------------------------------------------

/**
 * Draws a ghost when the puckman have eated one.
 */
void GenericLogic::drawGhostWhenPuckmanEats(Ghost & ghost, int lastKilledId) {
    if (lastGhostKilled_ != lastKilledId) {
        ghost.Draw();
    } else {
        drawPoints(ghost.X, ghost.Y);
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the ghosts when the puckman have eated one.
 */
void GenericLogic::drawGhostsWhenPuckmanEats() {
    drawGhostWhenPuckmanEats(*brown_, BROWN_LAST_KILLED);
    drawGhostWhenPuckmanEats(*blue_, BLUE_LAST_KILLED);
    drawGhostWhenPuckmanEats(*pink_, PINK_LAST_KILLED);
    drawGhostWhenPuckmanEats(*red_, RED_LAST_KILLED);
}

//--------------------------------------------------------------------------------

/**
 * Updates a ghost when the puckman have eated one.
 */
void GenericLogic::updateGhostWhenPuckmanEats(int time, Ghost & ghost, int lastKilledId) {
    if (!ghost.Alive()) {
        if (lastGhostKilled_ != lastKilledId) {
            ghost.Update(time);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the ghosts when the puckman have eated one.
 */
void GenericLogic::updateGhostsWhenPuckmanEats(int time) {
    updateGhostWhenPuckmanEats(time, *red_, RED_LAST_KILLED);
    updateGhostWhenPuckmanEats(time, *pink_, PINK_LAST_KILLED);
    updateGhostWhenPuckmanEats(time, *blue_, BLUE_LAST_KILLED);
    updateGhostWhenPuckmanEats(time, *brown_, BROWN_LAST_KILLED);
}

//--------------------------------------------------------------------------------

/**
 * Draws the maze of the game.
 */
void GenericLogic::drawMaze() {
    // First, we'll draw the maze.
    maze_->Draw(manager_);
    // And then the big dots blink controller will hide them or not.
    bigDotsLogic_.Draw();
}

//--------------------------------------------------------------------------------

/**
 * Initializes the fruit of the maze.
 */
void GenericLogic::initializeFruit(FruitTypeEnum type) {
    fruit_.Type(type);
    fruit_.SetCoordinates(Maze::FRUIT_START_X, Maze::FRUIT_START_Y);
    fruitTime_ = FRUIT_DELAY_TIME;
    showFruit_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Draws the fruit of the game.
 */
void GenericLogic::drawFruit() {
    // If the fruit is active we'll draw it into the screen.
    if (showFruit_) {
        fruit_.Draw();
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the fruit state.
 */
void GenericLogic::updateFruit(int time) {
    fruitTime_ -= time;
    if (fruitTime_ <= 0) {
        if (showFruit_) {
            fruitTime_ += SHOW_FRUIT_DELAY_TIME;
        } else {
            fruitTime_ += FRUIT_DELAY_TIME;
        }
        showFruit_ = !showFruit_;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the ghosts state.
 */
void GenericLogic::updateGhostState(int time) {
    // Here we'll check when the vulnerable state of the ghost ends.
    if (ghostsState_ == GhostState::Vulnerable) {
        // If the ghost is in vulnerable state we'll check if that have ended or not.
        ghostTime_ += time;
        if (ghostTime_ >= VULNERABLE_INTERVAL) {
            setGhostsToEnding();
        }
    } else if (ghostsState_ == GhostState::Ending) {
        // If the ghost is in ending state we'll check if that have ended or not.
        ghostTime_ += time;
        if (ghostTime_ >= VULNERABLE_ENDING_INTERVAL) {
            setGhostsToNormal();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Clears a cell in the maze and set the puckman's speed.
 */
void GenericLogic::clearCell(PuckmanEntity & puckman, const sf::Vector2i & cell) {
    puckman.SetSlow();
    maze_->Data()[cell.y][cell.x] = Maze::CLEAN;
    lastCellCleared_ = cell;
}

//--------------------------------------------------------------------------------

/**
 * Checks if there is a collision between two entities.
 */
bool GenericLogic::checkCollision(Entity & a, Entity & b) {
    const int collisionSeparation = 8;
    return (a.X == b.X && std::abs(a.Y - b.Y) <= collisionSeparation) ||
           (a.Y == b.Y && std::abs(a.X - b.X) <= collisionSeparation);
}

//--------------------------------------------------------------------------------

/**
 * Update the state of the logic to kill the puckman.
 */
void GenericLogic::genericKillPuckman() {
    // First, we'll stop all the ghosts.
    red_->StopSpeed();
    pink_->StopSpeed();
    blue_->StopSpeed();
    brown_->StopSpeed();
    // And then, we'll set the new state of the logic.
    state_ = KILL_STATE;
}

//--------------------------------------------------------------------------------

/**
 * Check the collision between the puckman and a ghost in the maze.
 */
void GenericLogic::checkGhostCollision(PuckmanEntity & puckman, Ghost & ghost, int lastKilledId) {
    // If the ghost is alive and collides with puckman, we'll check the ghost state.
    if (ghost.Alive() && checkCollision(puckman, ghost)) {
        // When the ghost is in normal state the puckman is killed.
        if (ghost.State() == GhostState::Normal) {
            killPuckman();
        } else {
            // But in any other case the ghost is eated by puckman.
            ghost.Kill();
            // After kill the ghost, the counter of killed ghost must be increased.
            numberOfGhostsKilled_++;
            // And the last ghost killed identification must be set.
            lastGhostKilled_ = lastKilledId;
            // Finally, we'll set a pause in the game to show the points.
            GameTimer::Set(KILL_PAUSE_INTERVAL);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks all the collisions between the entities in the maze.
 */
void GenericLogic::checkCollisions(PuckmanEntity & puckman) {
    checkGhostCollision(puckman, *red_, RED_LAST_KILLED);
    checkGhostCollision(puckman, *pink_, PINK_LAST_KILLED);
    checkGhostCollision(puckman, *blue_, BLUE_LAST_KILLED);
    checkGhostCollision(puckman, *brown_, BROWN_LAST_KILLED);
    checkFruitCollision(puckman);
}

//--------------------------------------------------------------------------------

/**
 * Draws the points of a killed ghost.
 */
void GenericLogic::drawPoints(int x, int y) {
    switch(numberOfGhostsKilled_) {
    case 1: manager_->DrawSprite(x, y, Sprites::N200);  break;
    case 2: manager_->DrawSprite(x, y, Sprites::N400);  break;
    case 3: manager_->DrawSprite(x, y, Sprites::N800);  break;
    case 4: manager_->DrawSprite(x, y, Sprites::N1600); break;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
GenericLogic::GenericLogic() : manager_(Manager::Instance()),
    ended_(false), state_(INITIAL_STATE), ghostsState_(GhostState::Normal),
    ghostTime_(0), red_(nullptr), pink_(nullptr), blue_(nullptr), brown_(nullptr),
    hurryMode_(false), lastGhostKilled_(NONE_LAST_KILLED), numberOfGhostsKilled_(0),
    fruit_(), fruitTime_(0), showFruit_(false), maze_(&defaultMaze_), bigDotsLogic_(),
    lastCellCleared_() {
    bigDotsLogic_.Register(6, 1);
    bigDotsLogic_.Register(6, 26);
    bigDotsLogic_.Register(26, 1);
    bigDotsLogic_.Register(26, 26);
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
GenericLogic::~GenericLogic() {}
