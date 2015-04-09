/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#include "PuckmanSequencedPuckman.h"

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Updates the inner state of the entity.
 */
void SequencedPuckman::Update(int time) {
    if(alive_) {
        // When the entity is alive, we'll control the tick logic of the entity.
        currentTime_ += time;
        while(currentTime_ >= maxTimeInterval_) {
            currentTime_ -= maxTimeInterval_;

            // In every tick, we'll check the sequence and the direction first.
            MoveSequence.Check(*this);
            testNeededDirection();

            // And finally, we'll update the coordinates and the sprites.
            if(updateCoordinates()) {
                sprites_[currentSprite_].Update(maxTimeInterval_);
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
SequencedPuckman::SequencedPuckman(const Sequence & sequence)
    : AnimatedPuckman(), MoveSequence(sequence) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
SequencedPuckman::~SequencedPuckman() {}
