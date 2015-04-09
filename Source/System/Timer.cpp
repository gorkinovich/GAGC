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

#include "Timer.h"
#include <SFML/System/Time.hpp>

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Updates the inner timer logic.
 */
void Timer::Update(const sf::Time & timeDelta) {
    if (enable_ && action_) {
        currentTime_ += (unsigned int)timeDelta.asMilliseconds();
        while (currentTime_ >= interval_) {
            currentTime_ -= interval_;
            action_(*this);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner timer logic.
 */
void Timer::Update(unsigned int elapsedMilliseconds) {
    if (enable_ && action_) {
        currentTime_ += elapsedMilliseconds;
        while (currentTime_ >= interval_) {
            currentTime_ -= interval_;
            action_(*this);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Resets the current time.
 */
void Timer::ResetTime() {
    currentTime_ = 0;
}

//--------------------------------------------------------------------------------

/**
 * Sets and enable the timer.
 */
void Timer::SetAndEnable(unsigned int interval, ActionEvent action, bool resetTime) {
    if (!action) {
        throw std::exception("The action of the timer can't be null!");
    }
    action_ = action;
    interval_ = interval;
    enable_ = true;
    if (resetTime) ResetTime();
}

//********************************************************************************
// Constructors, destructor and Operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Timer::Timer() : enable_(false), currentTime_(0), interval_(0), action_() {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
Timer::Timer(const Timer & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Timer::~Timer() {}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
Timer & Timer::operator =(const Timer & source) {
    enable_ = source.enable_;
    currentTime_ = source.currentTime_;
    interval_ = source.interval_;
    action_ = source.action_;
    return *this;
}
