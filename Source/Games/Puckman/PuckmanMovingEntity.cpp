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

#include "PuckmanMovingEntity.h"
#include <Games/Puckman/PuckmanManager.h>

using namespace Puckman;

//********************************************************************************
// Properties
//********************************************************************************

void MovingEntity::Speed(int value) {
    speed_ = value == MovingEntity::NO_SPEED || value == MovingEntity::MIN_SPEED ?
        value : MovingEntity::NO_SPEED;
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * The generic check function of the entity, must be changed for a proper one.
 */
bool MovingEntity::checkSpriteCoords(int x, int y) {
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Tests if the coordinates can be updated.
 */
bool MovingEntity::testUpdateCoordinates(MovingDirectionEnum d) {
    switch(d) {
    case MovingDirection::Up:    return CheckCoordinates(X, Y - speed_);
    case MovingDirection::Down:  return CheckCoordinates(X, Y + speed_);
    case MovingDirection::Left:  return CheckCoordinates(X - speed_, Y);
    case MovingDirection::Right: return CheckCoordinates(X + speed_, Y);
    }
    return false;
}

//--------------------------------------------------------------------------------

/**
 * Tries to update the coordinates of the entity.
 */
bool MovingEntity::updateCoordinates() {
    // We'll check if the entity can move through the maze.
    if(testUpdateCoordinates(direction_)) {
        // If the test returns true, we'll move the entity.
        Move();
        return true;
    } else {
        // If we're in the maze but we can't pass the test, then it could be that
        // we're going through the wormhole, so we'll check that chance.
        if(direction_ == MovingDirection::Left && X <= Manager::WORMHOLE_LX) {
            X = Manager::WORMHOLE_RX;
            return true;
        } else if(direction_ == MovingDirection::Right && X >= Manager::WORMHOLE_RX) {
            X = Manager::WORMHOLE_LX;
            return true;
        } else {
            return false;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Resets the time of the entity.
 */
void MovingEntity::ResetTime() {
    currentTime_ = 0;
}

//--------------------------------------------------------------------------------

/**
 * Resets the speed of the entity.
 */
void MovingEntity::ResetSpeed() {
    speed_ = MovingEntity::MIN_SPEED;
}

//--------------------------------------------------------------------------------

/**
 * Stops the speed of the entity.
 */
void MovingEntity::StopSpeed() {
    speed_ = MovingEntity::NO_SPEED;
}

//--------------------------------------------------------------------------------

/**
 * Sets the speed interval of the entity.
 */
void MovingEntity::SetSpeedInterval(int interval) {
    maxTimeInterval_ = interval;
}

//--------------------------------------------------------------------------------

/**
 * Moves the entity to the current direction.
 */
void MovingEntity::Move() {
    switch(direction_) {
    case MovingDirection::Up:    Y -= speed_; break;
    case MovingDirection::Down:  Y += speed_; break;
    case MovingDirection::Left:  X -= speed_; break;
    case MovingDirection::Right: X += speed_; break;
    }
    if (!OnMove.empty()) {
        for (unsigned int i = 0; i < OnMove.size(); ++i) {
            OnMove[i](*this);
        }
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
MovingEntity::MovingEntity() : Entity() {
    direction_ = MovingDirection::None;
    ResetSpeed();
    maxTimeInterval_ = MovingEntity::MIN_INTERVAL;
    ResetTime();
    CheckCoordinates = std::bind(&MovingEntity::checkSpriteCoords, this,
        std::placeholders::_1, std::placeholders::_2);
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
MovingEntity::~MovingEntity() {}
