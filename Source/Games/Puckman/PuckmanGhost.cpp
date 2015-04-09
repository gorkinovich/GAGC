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

#include "PuckmanGhost.h"
#include <System/ForEach.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanSprites.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Controls that the speed interval tick of the ghost is right.
 */
void Ghost::speedController(Ghost & ghost) {
    if(ghost.alive_) {
        if(ghost.state_ == GhostState::Normal) {
            ghost.SetSpeedInterval(NORMAL_SPEED_INTERVAL);
        } else {
            ghost.SetSpeedInterval(VULNERABLE_SPEED_INTERVAL);
        }
    } else {
        ghost.SetSpeedInterval(DEAD_SPEED_INTERVAL);
    }
}

//--------------------------------------------------------------------------------

/**
 * Resets all the sprites.
 */
void Ghost::resetSprites() {
    ForEach(normalSprites_, [] (Animation & victim) {
        victim.Reset();
    });
    ForEach(otherSprites_, [] (Animation & victim) {
        victim.Reset();
    });
}

//--------------------------------------------------------------------------------

/**
 * Updates all the sprites.
 */
void Ghost::updateSprites(int time) {
    ForEach(normalSprites_, [&] (Animation & victim) {
        victim.Update(time);
    });
    ForEach(otherSprites_, [&] (Animation & victim) {
        victim.Update(time);
    });
}

//--------------------------------------------------------------------------------

/**
 * Respawns the ghost entity.
 */
void Ghost::BackToLife() {
    alive_ = true;
    state_ = GhostState::Normal;
}

//--------------------------------------------------------------------------------

/**
 * Kills the ghost entity.
 */
void Ghost::Kill() {
    alive_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Changes the state of the ghost.
 */
void Ghost::ChangeState(GhostStateEnum state) {
    if(alive_) {
        state_ = state;
        if(state_ == GhostState::Normal) {

        } else if(state_ == GhostState::Vulnerable) {
            currentOtherSprite_ = BLUE_SPRITE;

        } else if(state_ == GhostState::Ending) {
            if(otherSprites_[currentOtherSprite_].CurrentFrame() == 0) {
                currentOtherSprite_ = WHITE1_SPRITE;
            } else {
                currentOtherSprite_ = WHITE2_SPRITE;
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Sets the current sprite of the entity.
 */
void Ghost::setCurrentSprite(int index) {
    currentNormalSprite_ = index;
}

//--------------------------------------------------------------------------------

/**
 * Sets the direction of the entity.
 */
void Ghost::SetDirection(MovingDirectionEnum dir) {
    direction_ = dir;
    switch(dir) {
    case MovingDirection::Up:    setCurrentSprite(UP_SPRITE);    break;
    case MovingDirection::Down:  setCurrentSprite(DOWN_SPRITE);  break;
    case MovingDirection::Left:  setCurrentSprite(LEFT_SPRITE);  break;
    case MovingDirection::Right: setCurrentSprite(RIGHT_SPRITE); break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Tests if the entity can change the direction.
 */
void Ghost::testNeededDirection() {
    // First, we'll validate the asked direction.
    if(testUpdateCoordinates(neededDirection_)) {
        // And if the direction is valid, we'll change the old one.
        if(direction_ != neededDirection_) {
            SetDirection(neededDirection_);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the ghost inside the screen.
 */
void Ghost::Draw() {
    if(alive_) {
        // Here we'll draw the ghost when it's alive.
        switch(state_) {
        case GhostState::Vulnerable:
            otherSprites_[currentOtherSprite_].Draw(X, Y);
            break;
        case GhostState::Ending:
            otherSprites_[currentOtherSprite_].Draw(X, Y);
            break;
        case GhostState::Normal:
            normalSprites_[currentNormalSprite_].Draw(X, Y);
            break;
        }
    } else {
        // Here we'll draw the ghost when it isn't alive.
        switch(direction_) {
            case MovingDirection::Up:
                manager_->DrawSprite(X, Y, Sprites::DeadGhostUp);
                break;
            case MovingDirection::Down:
                manager_->DrawSprite(X, Y, Sprites::DeadGhostDown);
                break;
            case MovingDirection::Left:
                manager_->DrawSprite(X, Y, Sprites::DeadGhostLeft);
                break;
            case MovingDirection::Right:
                manager_->DrawSprite(X, Y, Sprites::DeadGhostRight);
                break;
            case MovingDirection::None:
                manager_->DrawSprite(X, Y, Sprites::DeadGhostRight);
                break;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner state of the entity.
 */
void Ghost::Update(int time) {
    // First, we'll control the speed, the asked direction and we'll update the sprites.
    SpeedController(*this);
    testNeededDirection();
    updateSprites(time);

    // Second, we'll control the tick logic of the entity.
    currentTime_ += time;
    while(currentTime_ >= maxTimeInterval_) {
        currentTime_ -= maxTimeInterval_;
        // And in every tick, we'll update the coordinates.
        updateCoordinates();

        // After move we have to check the current direction.
        if(direction_ != neededDirection_) {
            if(testUpdateCoordinates(neededDirection_)) {
                SetDirection(neededDirection_);
            }
        }
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Ghost::Ghost(GhostTypeEnum type) : MovingEntity(), typeOfGhost_(type),
    currentNormalSprite_(RIGHT_SPRITE), currentOtherSprite_(BLUE_SPRITE),
    neededDirection_(MovingDirection::None), alive_(true), state_(GhostState::Normal) {
    // Then define the normal state array of animations.
    const int RERS_FRAMES[] = { 37, 38 };
    const int REDS_FRAMES[] = { 39, 40 };
    const int RELS_FRAMES[] = { 41, 42 };
    const int REUS_FRAMES[] = { 43, 44 };

    const int PIRS_FRAMES[] = { 53, 54 };
    const int PIDS_FRAMES[] = { 55, 56 };
    const int PILS_FRAMES[] = { 57, 58 };
    const int PIUS_FRAMES[] = { 59, 60 };

    const int BLRS_FRAMES[] = { 45, 46 };
    const int BLDS_FRAMES[] = { 47, 48 };
    const int BLLS_FRAMES[] = { 49, 50 };
    const int BLUS_FRAMES[] = { 51, 52 };

    const int BRRS_FRAMES[] = { 61, 62 };
    const int BRDS_FRAMES[] = { 63, 64 };
    const int BRLS_FRAMES[] = { 65, 66 };
    const int BRUS_FRAMES[] = { 67, 68 };

    if (typeOfGhost_ == GhostType::Red) {
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(RERS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(REDS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(RELS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(REUS_FRAMES), MAX_FPS));
    } else if (typeOfGhost_ == GhostType::Pink) {
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(PIRS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(PIDS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(PILS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(PIUS_FRAMES), MAX_FPS));
    } else if (typeOfGhost_ == GhostType::Blue) {
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BLRS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BLDS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BLLS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BLUS_FRAMES), MAX_FPS));
    } else if (typeOfGhost_ == GhostType::Brown) {
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BRRS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BRDS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BRLS_FRAMES), MAX_FPS));
        normalSprites_.push_back(Animation(ARRAY_TO_FRAMES(BRUS_FRAMES), MAX_FPS));
    }

    // Then define the other states array of animations.
    const int BLUE_FRAMES[] = { 69, 70 };
    const int WHI1_FRAMES[] = { 69, 72 };
    const int WHI2_FRAMES[] = { 71, 70 };

    otherSprites_.push_back(Animation(ARRAY_TO_FRAMES(BLUE_FRAMES), MAX_FPS));
    otherSprites_.push_back(Animation(ARRAY_TO_FRAMES(WHI1_FRAMES), MAX_FPS));
    otherSprites_.push_back(Animation(ARRAY_TO_FRAMES(WHI2_FRAMES), MAX_FPS));

    // Second, set the other stuff.
    SpeedController = std::bind(&Ghost::speedController, this, std::placeholders::_1);
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Ghost::~Ghost() {
    normalSprites_.clear();
    otherSprites_.clear();
}
