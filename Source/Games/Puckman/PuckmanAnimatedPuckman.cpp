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

#include "PuckmanAnimatedPuckman.h"

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Respawns the puckman entity.
 */
void AnimatedPuckman::Respawn() {
    // MovingEntity data
    SetDirection(MovingDirection::None);
    ResetSpeed();
    SetFast();
    ResetTime();
    // Player data
    neededDirection_ = MovingDirection::None;
    alive_ = true;
}

//--------------------------------------------------------------------------------

/**
 * Kills the puckman entity.
 */
void AnimatedPuckman::Kill() {
    setCurrentSprite(DEATH_SPRITE);
    direction_ = MovingDirection::None;
    alive_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Tests if the entity can change the direction.
 */
void AnimatedPuckman::testNeededDirection() {
    // First, we'll validate the asked direction.
    if(direction_ != neededDirection_) {
        // And if the direction is valid, we'll change the old one.
        if(testUpdateCoordinates(neededDirection_)) {
            SetDirection(neededDirection_);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner state of the entity.
 */
void AnimatedPuckman::Update(int time) {
    if(alive_) {
        // When the entity is alive, we'll control the asked direction.
        testNeededDirection();

        // And then, we'll control the tick logic of the entity.
        currentTime_ += time;
        while(currentTime_ >= maxTimeInterval_) {
            currentTime_ -= maxTimeInterval_;

            // In every tick, we'll update the coordinates and the sprites.
            if(updateCoordinates()) {
                sprites_[currentSprite_].Update(maxTimeInterval_);

                // After move we have to check the current direction.
                if(direction_ != neededDirection_) {
                    if(testUpdateCoordinates(neededDirection_)) {
                        SetDirection(neededDirection_);
                    }
                }
            }
        }
    } else {
        // When the entity isn't alive, we only update the sprites.
        sprites_[currentSprite_].Update(time);
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
AnimatedPuckman::AnimatedPuckman() : PuckmanEntity(),
    neededDirection_(MovingDirection::None), alive_(true) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
AnimatedPuckman::~AnimatedPuckman() {}
