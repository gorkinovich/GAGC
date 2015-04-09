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

#include "PuckmanAnimation.h"
#include <Games/Puckman/PuckmanManager.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Draws the current frame of the animation.
 */
void Animation::Draw(int x, int y) {
    manager_->DrawSprite(x, y, frames_[currentFrame_]);
}

//--------------------------------------------------------------------------------

/**
 * Updates the current frame of the animation.
 */
void Animation::Update(int milliseconds) {
    // First we have to add the passed time.
    currentTime_ += milliseconds;
    // Then we check if we have wait enough to change the current frame.
    while(currentTime_ >= maxTimeInterval_) {
        // Second, we'll check if we have a loop animation or not. If we haven't a
        // loop animation, we'll check if we aren't at the last frame.
        if(loop_) {
            // If this is a loop animation, we'll change the current frame.
            currentFrame_ = (currentFrame_ + 1) % frames_.size();
        } else if(currentFrame_ + 1 < static_cast<int>(frames_.size())) {
            // Only if we aren't at the end, we'll change the current frame.
            currentFrame_++;
        }
        // Finally we'll remove an interval from the passed time.
        currentTime_ -= maxTimeInterval_;
    }
}

//--------------------------------------------------------------------------------

/**
 * Resets the whole animation.
 */
void Animation::Reset() {
    currentTime_ = 0;
    currentFrame_ = 0;
}

//--------------------------------------------------------------------------------

/**
 * Resets only the passed time.
 */
void Animation::ResetTime() {
    currentTime_ = 0;
}

//--------------------------------------------------------------------------------

/**
 * Sets the frames per second of the animation.
 */
void Animation::SetFps(int fps) {
    maxTimeInterval_ = 1000 / fps;
}

//--------------------------------------------------------------------------------

/**
 * Checks if the current frame is the final one.
 */
bool Animation::IsOnFinalFrame() {
    return frames_.size() - 1 == currentFrame_;
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Animation::Animation(const std::vector<int> & frames, int fps, bool loop)
    : manager_(Manager::Instance()), frames_(frames),
    currentTime_(0), currentFrame_(0), loop_(loop) { SetFps(fps); }
