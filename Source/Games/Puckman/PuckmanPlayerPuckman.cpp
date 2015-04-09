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

#include "PuckmanPlayerPuckman.h"
#include <System/Sound.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Respawns the puckman entity.
 */
void PlayerPuckman::Respawn() {
    // MovingEntity data
    SetDirection(MovingDirection::None);
    ResetSpeed();
    SetFast();
    ResetTime();
    // Player data
    askedDirection_ = MovingDirection::None;
    alive_ = true;
}

//--------------------------------------------------------------------------------

/**
 * Kills the puckman entity.
 */
void PlayerPuckman::Kill() {
    setCurrentSprite(DEATH_SPRITE);
    direction_ = MovingDirection::None;
    alive_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Tests if the entity can change the direction.
 */
void PlayerPuckman::testUserInput() {
    // First, we'll check the user input.
    if(Keyboard::isKeyPressed(Keyboard::Up)) {
        askedDirection_ = MovingDirection::Up;
    }
    if(Keyboard::isKeyPressed(Keyboard::Down)) {
        askedDirection_ = MovingDirection::Down;
    }
    if(Keyboard::isKeyPressed(Keyboard::Right)) {
        askedDirection_ = MovingDirection::Right;
    }
    if(Keyboard::isKeyPressed(Keyboard::Left)) {
        askedDirection_ = MovingDirection::Left;
    }

    // And then, we'll validate the asked direction.
    if(testUpdateCoordinates(askedDirection_)) {
        // When the asked direction is valid, we'll set the new course.
        if(direction_ != askedDirection_) {
            SetDirection(askedDirection_);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner state of the entity.
 */
void PlayerPuckman::Update(int time) {
    if(alive_) {
        // When the entity is alive, we'll control the user input.
        testUserInput();

        // And then, we'll control the tick logic of the entity.
        currentTime_ += time;
        while(currentTime_ >= maxTimeInterval_) {
            currentTime_ -= maxTimeInterval_;

            // In every tick, we'll update the coordinates and the sprites.
            if(updateCoordinates()) {
                manager_->Chomp().Play();
                sprites_[currentSprite_].Update(maxTimeInterval_);

                // After move we have to check the current direction.
                if(direction_ != askedDirection_) {
                    if(testUpdateCoordinates(askedDirection_)) {
                        SetDirection(askedDirection_);
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
PlayerPuckman::PlayerPuckman() : PuckmanEntity(), askedDirection_(MovingDirection::None),
    alive_(true) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
PlayerPuckman::~PlayerPuckman() {}
