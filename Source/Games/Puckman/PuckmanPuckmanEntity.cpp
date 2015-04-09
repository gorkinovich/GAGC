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

#include "PuckmanPuckmanEntity.h"

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Sets the speed of the entity to fast.
 */
void PuckmanEntity::SetFast() {
    maxTimeInterval_ = FAST_INTERVAL;
}

//--------------------------------------------------------------------------------

/**
 * Sets the speed of the entity to slow.
 */
void PuckmanEntity::SetSlow() {
    maxTimeInterval_ = SLOW_INTERVAL;
}

//--------------------------------------------------------------------------------

/**
 * Sets the current sprite of the entity.
 */
void PuckmanEntity::setCurrentSprite(int index) {
    currentSprite_ = index;
    sprites_[currentSprite_].Reset();
}

//--------------------------------------------------------------------------------

/**
 * Sets the direction of the entity.
 */
void PuckmanEntity::SetDirection(MovingDirectionEnum dir) {
    direction_ = dir;
    switch(dir) {
    case MovingDirection::Up:    setCurrentSprite(UP_SPRITE);    break;
    case MovingDirection::Down:  setCurrentSprite(DOWN_SPRITE);  break;
    case MovingDirection::Left:  setCurrentSprite(LEFT_SPRITE);  break;
    case MovingDirection::Right: setCurrentSprite(RIGHT_SPRITE); break;
    case MovingDirection::None:  setCurrentSprite(STOP_SPRITE);  break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the Puckman inside the screen.
 */
void PuckmanEntity::Draw() {
    sprites_[currentSprite_].Draw(X, Y);
}

//--------------------------------------------------------------------------------

/**
 * Checks if the death animation have ended.
 */
bool PuckmanEntity::IsDeathSpriteEnded() {
    return sprites_[DEATH_SPRITE].IsOnFinalFrame();
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
PuckmanEntity::PuckmanEntity() : MovingEntity() {
    const int RIGHT[] = { 16, 17, 18 };
    const int DOWN[]  = { 16, 19, 20 };
    const int LEFT[]  = { 16, 21, 22 };
    const int UP[]    = { 16, 23, 24 };
    const int DEATH[] = { 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };
    const int STOP[]  = { 16 };
    // First, set each sprite with an animation.
    sprites_.push_back(Animation(ARRAY_TO_FRAMES(RIGHT), MAX_FPS));
    sprites_.push_back(Animation(ARRAY_TO_FRAMES(DOWN), MAX_FPS));
    sprites_.push_back(Animation(ARRAY_TO_FRAMES(LEFT), MAX_FPS));
    sprites_.push_back(Animation(ARRAY_TO_FRAMES(UP), MAX_FPS));
    sprites_.push_back(Animation(ARRAY_TO_FRAMES(DEATH), MAX_FPS));
    sprites_.push_back(Animation(ARRAY_TO_FRAMES(STOP), MAX_FPS));
    // And finally, set the current sprite.
    currentSprite_ = STOP_SPRITE;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
PuckmanEntity::~PuckmanEntity() {}
