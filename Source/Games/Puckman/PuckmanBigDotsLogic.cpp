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

#include "PuckmanBigDotsLogic.h"
#include <System/ForEach.h>
#include <Games/Puckman/PuckmanManager.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Resets the data inside the controller.
 */
void BigDotsLogic::Reset() {
    coords_.clear();
    currentTime_ = 0;
    hideDots_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Resets the time inside the controller.
 */
void BigDotsLogic::ResetTime() {
    currentTime_ = 0;
    hideDots_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Register a big dot inside the controller.
 */
void BigDotsLogic::Register(int row, int col) {
    coords_.push_back(sf::Vector2i(col, row));
}

//--------------------------------------------------------------------------------

/**
 * Allow the big dots to appear or not in the screen.
 */
void BigDotsLogic::Draw() {
    if(hideDots_) {
        ForEach(coords_, [&] (sf::Vector2i & p) {
            manager_->ClearCell(p.y, p.x);
        });
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner state of the controller.
 */
void BigDotsLogic::Update(int time) {
    currentTime_ += time;
    while(currentTime_ >= MAX_TIME_INTERVAL) {
        hideDots_ = !hideDots_;
        currentTime_ -= MAX_TIME_INTERVAL;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
BigDotsLogic::BigDotsLogic() : coords_(), currentTime_(0), hideDots_(false),
    manager_(Manager::Instance())  {}
