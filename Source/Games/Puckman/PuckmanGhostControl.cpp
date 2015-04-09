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

#include "PuckmanGhostControl.h"
#include <System/MathUtil.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPuckmanEntity.h>
#include <Games/Puckman/PuckmanGhost.h>

using namespace Puckman;

//********************************************************************************
// Static
//********************************************************************************

MazeGraph GhostControl::graph_;
sf::Vector2i GhostControl::houseEnter_(13, 14);
Ghost * GhostControl::red_ = nullptr;
Ghost * GhostControl::pink_ = nullptr;
Ghost * GhostControl::blue_ = nullptr;
Ghost * GhostControl::brown_ = nullptr;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Sets the time counter.
 */
void GhostControl::setTimeCounter() {
    switch(state_) {
    case GhostControlState::House:   timeCounter_ = maxHouseInterval_;   break;
    case GhostControlState::Scatter: timeCounter_ = maxScatterInterval_; break;
    case GhostControlState::Chase:   timeCounter_ = maxChaseInterval_;   break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Changes the state.
 */
void GhostControl::changeState(GhostControlStateEnum state) {
    if(state_ != state) {
        state_ = state;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the ghost is on the corner.
 */
bool GhostControl::isGhostOnCorner() {
    return (ghost_->X + Maze::SPRITE_SEP_X == corner_.x * Manager::CELL_WIDTH) &&
           (ghost_->Y + Maze::SPRITE_SEP_Y == corner_.y * Manager::CELL_HEIGHT);
}

//--------------------------------------------------------------------------------

/**
 * Checks the state of a ghost.
 */
void GhostControl::checkState(int time) {
    // First, we'll check if the ghost is alive.
    if(ghost_->Alive()) {
        // If the ghost is alive, check if the state of the ghost.
        if(ghost_->State() == GhostState::Normal) {
            // If the ghost is in normal mode, we'll check the control state.
            switch(state_) {
            case GhostControlState::House:
                // If the ghost is in house state, we'll check the time counter
                // first, to remove time from it, only when it have time.
                if(timeCounter_ > 0 && maze_->NumberOfEatedDots() < minDotsToExit_) {
                    timeCounter_ -= time;
                } else {
                    // When the time is over, we have to check if the ghost is outside
                    // the house and in the start coordinates.
                    if(!Maze::IsGhostInsideTheHouse(*ghost_) &&
                        Maze::IsGhostInTheStartPoint(*ghost_)) {
                        // Then, we'll change the "is in house" flag and the ghost direction.
                        isInHouse_ = false;
                        ghost_->NeededDirection(MovingDirection::Left);
                        // After that, we'll have to check if the ghost is chasing the
                        // puckman or not, to change the state and the time counter.
                        if(isChasing_) {
                            state_ = GhostControlState::Chase;
                            timeCounter_ = maxChaseInterval_;
                        } else {
                            state_ = GhostControlState::Scatter;
                            timeCounter_ = maxScatterInterval_;
                        }
                    }
                }
                break;


            case GhostControlState::Scatter:
                // If the ghost is in scatter state, we'll check the time counter
                // first, to remove time from it, only when it have time. We also
                // have to check if the ghost is in its corner.
                if(timeCounter_ > 0 && !isGhostOnCorner()) {
                    timeCounter_ -= time;
                } else {
                    // When the time is over, we have to check if we can continue
                    // changing between the chase and the scatter state.
                    if(changes_ < maxNumberOfChanges_) {
                        // If we can make a change, we'll change the state to chase.
                        state_ = GhostControlState::Chase;
                        isChasing_ = true;
                        // Then we'll set the time counter and add a change mark.
                        timeCounter_ = maxChaseInterval_;
                        changes_++;
                        isDirectionGetted_ = false;
                    }
                }
                break;


            case GhostControlState::Chase:
                // If the ghost is in chase state, we'll check the time counter
                // first, to remove time from it, only when it have time.
                if(timeCounter_ > 0) {
                    timeCounter_ -= time;
                } else {
                    // When the time is over, we have to check if we can continue
                    // changing between the chase and the scatter state.
                    if(changes_ < maxNumberOfChanges_) {
                        // If we can make a change, we'll change the state to scatter.
                        state_ = GhostControlState::Scatter;
                        isChasing_ = false;
                        // Then we'll set the time counter and add a change mark.
                        timeCounter_ = maxScatterInterval_;
                        changes_++;
                        isDirectionGetted_ = false;
                    }
                }
                break;


            case GhostControlState::Frightened:
                // If the ghost is in frightened state, is because it was in
                // vulnerable mode and now is back to normal. So we'll check
                // in which state the ghost was before the vulnerable mode.
                if(isInHouse_) {
                    // If the ghost is inside the house, we'll have to check
                    // the "is in house" flag, to continue with the countdown.
                    state_ = GhostControlState::House;
                } else {
                    // If the ghost is out the house, we'll have to check
                    // if the ghost is chasing the puckman or not.
                    if(isChasing_) {
                        state_ = GhostControlState::Chase;
                    } else {
                        state_ = GhostControlState::Scatter;
                    }
                    isDirectionGetted_ = false;
                }
                break;


            case GhostControlState::Dead:
                // If the ghost is in dead state, is because it was killed by
                // puckman and now after get back to life inside the house,
                // the ghost is still in dead state but it is alive. So we'll
                // change the current state to the house control state.
                state_ = GhostControlState::House;
                isInHouse_ = true;
                // Because the ghost was out when it was killed, it make no
                // sense to wait some time to get out the house.
                timeCounter_ = 0;
                break;

            }
        } else {
            // If the ghost isn't in normal mode, it will be frightened.
            changeState(GhostControlState::Frightened);
            // We also need to check if the ghost is still in the house.
            if(isInHouse_) {
                // If the ghost was in house state, we'll check the time counter
                // first, to remove time from it, only when it have time.
                if(timeCounter_ > 0) {
                    timeCounter_ -= time;
                } else {
                    // When the time is over, we have to check if the ghost is outside
                    // the house and in the start coordinates.
                    if(!Maze::IsGhostInsideTheHouse(*ghost_) &&
                        Maze::IsGhostInTheStartPoint(*ghost_)) {
                        // Then, we'll change the "is in house" flag and the ghost direction.
                        isInHouse_ = false;
                        ghost_->NeededDirection(MovingDirection::Left);
                        // After that, we'll have to check if the ghost is chasing the
                        // puckman or not, to change the state.
                        if(isChasing_) {
                            timeCounter_ = maxChaseInterval_;
                        } else {
                            timeCounter_ = maxScatterInterval_;
                        }
                    }
                }
            }
        }
    } else {
        // If the ghost isn't alive, we'll be in dead state.
        if(state_ != GhostControlState::Dead) {
            state_ = GhostControlState::Dead;
            isDirectionGetted_ = false;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction when the ghost is in the house state.
 */
void GhostControl::houseUpdate() {
    // First, we'll check the time counter.
    if(timeCounter_ > 0 && maze_->NumberOfEatedDots() < minDotsToExit_) {
        // When the time isn't over, we'll check if the ghost have
        // reached the limit inside the house, to move in the Y-axis.
        if(ghost_->Y <= Maze::GHOST_HOUSE_START_Y) {
            ghost_->NeededDirection(MovingDirection::Down);
        } else if(ghost_->Y >= Maze::GHOST_HOUSE_END_Y) {
            ghost_->NeededDirection(MovingDirection::Up);
        }
    } else {
        // When the time is over, we'll have to get the ghost out the house.
        // So if the ghost is in the same line that the start x coordinate,
        // we'll set the ghost direction to up.
        if(ghost_->X == Maze::GHOST_START_X) {
            ghost_->NeededDirection(MovingDirection::Up);
        } else {
            // If not, we'll try to put the ghost in the middle of the house,
            // to move it to the center start x coordinate line.
            if(ghost_->Y <= Maze::GHOST_HOUSE_START_Y) {
                ghost_->NeededDirection(MovingDirection::Down);
            } else if(ghost_->Y >= Maze::GHOST_HOUSE_END_Y) {
                ghost_->NeededDirection(MovingDirection::Up);
            } else if(ghost_->Y >= Maze::GHOST_HOUSE_CENTER_Y) {
                // When the ghost is in the center of the house, we'll move
                // it from the right or the left to the center.
                if(ghost_->X <= Maze::BLUE_GHOST_START_X) {
                    ghost_->NeededDirection(MovingDirection::Right);
                } else if(ghost_->X >= Maze::BROWN_GHOST_START_X) {
                    ghost_->NeededDirection(MovingDirection::Left);
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction when the ghost is in the scatter state.
 */
void GhostControl::scatterUpdate() {
    // First, we'll check if we're on an intersection.
    if(graph_.IsOnIntersection(*ghost_)) {
        // If we're on an intersection, we'll check that we didn't select
        // a direction in a previous update.
        if(!isDirectionGetted_) {
            // If we can select a direction, we'll find a new one.
            ghost_->NeededDirection(graph_.GetDirection(*ghost_, corner_));
            // And then, we'll set this flag to avoid to make the same
            // algorithim for the same data.
            if(ghost_->NeededDirection() != MovingDirection::None) {
                isDirectionGetted_ = true;
            }
        }
    } else {
        // If we're not on an intersection, we'll set this flag to make
        // possible in a future update to select a new direction.
        isDirectionGetted_ = false;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction when the ghost is in the chase state.
 */
void GhostControl::chaseUpdate() {
    // First, we'll check if we're on an intersection.
    if(graph_.IsOnIntersection(*ghost_)) {
        // If we're on an intersection, we'll check that we didn't select
        // a direction in a previous update.
        if(!isDirectionGetted_) {
            // If we can select a direction, we'll find a new one.
            sf::Vector2i dest = Maze::SpriteCoordsToMaze(puckman_->X, puckman_->Y);
            switch(ghost_->TypeOfGhost()) {
            case GhostType::Red:
                // The red ghost will always try to track puckman directly.
                ghost_->NeededDirection(graph_.GetDirection(*ghost_, dest));
                // This ghost will be behind puckman and sometimes will enter the same cell
                // that the puckman is exiting. The path finder will return the none direction
                // because they both are in the same cell, so we'll asign the same direction
                // that the player have chosen.
                if(ghost_->NeededDirection() == MovingDirection::None) {
                    ghost_->NeededDirection(puckman_->Direction());
                }
                break;


            case GhostType::Pink:
                // The pink ghost will always try anticipate puckman location.
                switch(puckman_->Direction()) {
                case MovingDirection::Up:    dest.y -= 4; break;
                case MovingDirection::Down:  dest.y += 4; break;
                case MovingDirection::Left:  dest.x -= 4; break;
                case MovingDirection::Right: dest.x += 4; break;
                }
                ghost_->NeededDirection(graph_.GetDirection(*ghost_, dest));
                // And if the ghost get stopped in the destination cell, will be next to the
                // puckam and we'll set the direction to finally cut the player's path.
                if(ghost_->NeededDirection() == MovingDirection::None) {
                    ghost_->NeededDirection(graph_.GetOppositeDirection(puckman_->Direction()));
                }
                break;


            case GhostType::Blue:
                {
                    // The blue ghost will always try to be the least predictable.
                    switch(puckman_->Direction()) {
                    case MovingDirection::Up:    dest.y -= 2; dest.x -= 2; break;
                    case MovingDirection::Down:  dest.y += 2; break;
                    case MovingDirection::Left:  dest.x -= 2; break;
                    case MovingDirection::Right: dest.x += 2; break;
                    }
                    sf::Vector2i redOrig = Maze::SpriteCoordsToMaze(red_->X, red_->Y);
                    sf::Vector2i vector(dest.x - redOrig.x, dest.y - redOrig.y);
                    dest.x += vector.x;
                    dest.y += vector.y;
                    // After calculate the destination, we'll get the direction.
                    ghost_->NeededDirection(graph_.GetDirection(*ghost_, dest));
                    // And if the ghost get stopped in the destination cell, we'll get a random direction.
                    if(ghost_->NeededDirection() == MovingDirection::None) {
                        ghost_->NeededDirection(graph_.GetRandomDirection(*ghost_));
                    }
                }
                break;


            case GhostType::Brown:
                // The brown ghost will always try to pretend ignorance.
                sf::Vector2i cell = Maze::SpriteCoordsToMaze(ghost_->X, ghost_->Y);
                int sideX2 = std::abs(dest.x - cell.x) * std::abs(dest.x - cell.x);
                int sideY2 = std::abs(dest.y - cell.y) * std::abs(dest.y - cell.y);
                int distance = (int)std::sqrt(static_cast<float>(sideX2 + sideY2));
                // After calculate the distance between the puckman and the ghost,
                // we'll get the direction to chase puckam or to get back to the corner.
                if(distance <= 8) {
                    ghost_->NeededDirection(graph_.GetDirection(*ghost_, corner_));
                } else {
                    ghost_->NeededDirection(graph_.GetDirection(*ghost_, dest));
                }
                // And if the ghost get stopped in the destination cell, we'll get a random direction.
                if(ghost_->NeededDirection() == MovingDirection::None) {
                    ghost_->NeededDirection(graph_.GetRandomDirection(*ghost_));
                }
                break;

            }
            // And then, we'll set this flag to avoid to make the same
            // algorithim for the same data.
            if(ghost_->NeededDirection() != MovingDirection::None) {
                isDirectionGetted_ = true;
            }
        }
    } else {
        // If we're not on an intersection, we'll set this flag to make
        // possible in a future update to select a new direction.
        isDirectionGetted_ = false;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction when the ghost is in the frightened state.
 */
void GhostControl::frightenedUpdate() {
    if(isInHouse_) {
        if(Maze::IsGhostInsideTheHouse(*ghost_)) {
            // If the ghost is in the same line that the start x coordinate,
            // we'll set the ghost direction to up.
            if(ghost_->X == Maze::GHOST_START_X) {
                ghost_->NeededDirection(MovingDirection::Up);
            } else {
                // If not, we'll try to put the ghost in the middle of the house,
                // to move it to the center start x coordinate line.
                if(ghost_->Y <= Maze::GHOST_HOUSE_START_Y) {
                    ghost_->NeededDirection(MovingDirection::Down);
                } else if(ghost_->Y >= Maze::GHOST_HOUSE_END_Y) {
                    ghost_->NeededDirection(MovingDirection::Up);
                } else if(ghost_->Y >= Maze::GHOST_HOUSE_CENTER_Y) {
                    // When the ghost is in the center of the house, we'll move
                    // it from the right or the left to the center.
                    if(ghost_->X <= Maze::BLUE_GHOST_START_X) {
                        ghost_->NeededDirection(MovingDirection::Right);
                    } else if(ghost_->X >= Maze::BROWN_GHOST_START_X) {
                        ghost_->NeededDirection(MovingDirection::Left);
                    }
                }
            }
        } else if(Maze::IsGhostInTheStartPoint(*ghost_)) {
            isInHouse_ = false;
            ghost_->NeededDirection(MovingDirection::Left);
        }
    } else {
        // Here we'll check if we're on an intersection.
        if(graph_.IsOnIntersection(*ghost_)) {
            // If we're on an intersection, we'll check that we didn't select
            // a direction in a previous update.
            if(!isDirectionGetted_) {
                // If we can select a direction, we'll find a new one.
                ghost_->NeededDirection(graph_.GetEscapeDirection(*ghost_, *puckman_));
                // And then, we'll set this flag to avoid to make the same
                // algorithim for the same data.
                if(ghost_->NeededDirection() != MovingDirection::None) {
                    isDirectionGetted_ = true;
                }
            }
        } else {
            // If we're not on an intersection, we'll set this flag to make
            // possible in a future update to select a new direction.
            isDirectionGetted_ = false;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction when the ghost is in the dead state.
 */
void GhostControl::deadUpdate() {
    // First, we'll check if the ghost is inside the house or not.
    if(Maze::IsGhostInsideTheHouse(*ghost_)) {
        // If we're inside the house, we'll check this cases:
        if(ghost_->TypeOfGhost() == GhostType::Blue &&
            ghost_->Y >= Maze::GHOST_HOUSE_CENTER_Y) {
            // If we are the blue ghost and it's in the center of the house,
            // we'll change the direction to move that ghost to the left.
            ghost_->NeededDirection(MovingDirection::Left);

        } else if(ghost_->TypeOfGhost() == GhostType::Brown &&
            ghost_->Y >= Maze::GHOST_HOUSE_CENTER_Y) {
            // If we are the brown ghost and it's in the center of the house,
            // we'll change the direction to move that ghost to the right.
            ghost_->NeededDirection(MovingDirection::Right);
        }
    } else {
        // If we're not inside the house, we'll check if the ghost is
        // in the house enter or not.
        sf::Vector2i orig = Maze::SpriteCoordsToMaze(ghost_->X, ghost_->Y);
        if(orig == houseEnter_) {
            // If we're at the house enter, we'll set the direction
            // to make enter the ghost inside the house.
            ghost_->NeededDirection(MovingDirection::Down);

        } else {
            // If we're not at the house enter, we'll check if we're on an intersection.
            if(graph_.IsOnIntersection(*ghost_)) {
                // If we're on an intersection, we'll check that we didn't
                // select a direction in a previous update.
                if(!isDirectionGetted_) {
                    // If we can select a direction, we'll find a new one.
                    ghost_->NeededDirection(graph_.GetDirection(*ghost_, houseEnter_));
                    // And then, we'll set this flag to avoid to make the same
                    // algorithim for the same data.
                    if(ghost_->NeededDirection() != MovingDirection::None) {
                        isDirectionGetted_ = true;
                    }
                }
            } else {
                // If we're not on an intersection, we'll set this flag to make
                // possible in a future update to select a new direction.
                isDirectionGetted_ = false;
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction of the ghost.
 */
void GhostControl::Update(int time) {
    // First, we'll check the state of the ghost controller.
    checkState(time);
    // And then, we'll update the direction.
    switch(state_) {
    case GhostControlState::House:      houseUpdate();      break;
    case GhostControlState::Scatter:    scatterUpdate();    break;
    case GhostControlState::Chase:      chaseUpdate();      break;
    case GhostControlState::Frightened: frightenedUpdate(); break;
    case GhostControlState::Dead:       deadUpdate();       break;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
GhostControl::GhostControl(int maxScatterInterval, int maxChaseInterval, int maxHouseInterval,
    int minDotsToExit, int maxNumberOfChanges, const sf::Vector2i & corner,
    Ghost * ghost, GhostControlStateEnum state, PuckmanEntity * puckman, Maze * maze)
    : maxScatterInterval_(maxScatterInterval), maxChaseInterval_(maxChaseInterval),
    maxHouseInterval_(maxHouseInterval), minDotsToExit_(minDotsToExit),
    maxNumberOfChanges_(maxNumberOfChanges), corner_(corner), ghost_(ghost), state_(state),
    isChasing_(state == GhostControlState::Chase), isInHouse_(state == GhostControlState::House),
    timeCounter_(0), changes_(0), puckman_(puckman), maze_(maze), isDirectionGetted_(false) {
    setTimeCounter();
    switch(ghost_->TypeOfGhost()) {
    case GhostType::Red:   GhostControl::red_   = ghost; break;
    case GhostType::Pink:  GhostControl::pink_  = ghost; break;
    case GhostType::Blue:  GhostControl::blue_  = ghost; break;
    case GhostType::Brown: GhostControl::brown_ = ghost; break;
    }
}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
GhostControl::GhostControl(const GhostControl & source)
    : maxScatterInterval_(source.maxScatterInterval_),
    maxChaseInterval_(source.maxChaseInterval_),
    maxHouseInterval_(source.maxHouseInterval_),
    minDotsToExit_(source.minDotsToExit_),
    maxNumberOfChanges_(source.maxNumberOfChanges_),
    corner_(source.corner_), ghost_(source.ghost_), state_(source.state_),
    isChasing_(source.isChasing_), isInHouse_(source.isInHouse_),
    timeCounter_(source.timeCounter_), changes_(source.changes_),
    puckman_(source.puckman_), maze_(source.maze_),
    isDirectionGetted_(source.isDirectionGetted_) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
GhostControl::~GhostControl() {}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
GhostControl & GhostControl::operator =(const GhostControl & source) {
    maxScatterInterval_ = source.maxScatterInterval_;
    maxChaseInterval_ = source.maxChaseInterval_;
    maxHouseInterval_ = source.maxHouseInterval_;
    minDotsToExit_ = source.minDotsToExit_;
    maxNumberOfChanges_ = source.maxNumberOfChanges_;
    corner_ = source.corner_;
    ghost_ = source.ghost_;
    state_ = source.state_;
    isChasing_ = source.isChasing_;
    isInHouse_ = source.isInHouse_;
    timeCounter_ = source.timeCounter_;
    changes_ = source.changes_;
    puckman_ = source.puckman_;
    maze_ = source.maze_;
    isDirectionGetted_ = source.isDirectionGetted_;
    return *this;
}
