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

#include "PuckmanSequence.h"
#include <Games/Puckman/PuckmanMaze.h>
#include <Games/Puckman/PuckmanAnimatedPuckman.h>
#include <Games/Puckman/PuckmanGhost.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Resets the sequence.
 */
void Sequence::Reset() {
    currentNode_ = 0;
}

//--------------------------------------------------------------------------------

/**
 * Resets the sequence.
 */
void Sequence::Reset(const std::vector<Node> & data) {
    data_ = data;
    currentNode_ = 0;
}

//--------------------------------------------------------------------------------

/**
 * Checks if an entity is in the right cell.
 */
bool Sequence::checkCoordinates(MovingEntity & entity) {
    if(data_[currentNode_].PixelCoords == false) {
        sf::Vector2i cell = Maze::SpriteCoordsToMaze(entity.X, entity.Y);
        return data_[currentNode_].Row == cell.y &&
               data_[currentNode_].Column == cell.x;
    } else {
        return data_[currentNode_].Row == entity.Y &&
               data_[currentNode_].Column == entity.X;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks the sequence have ended or not.
 */
bool Sequence::IsEnded() {
    return currentNode_ == data_.size();
}

//--------------------------------------------------------------------------------

/**
 * Sets the current sequence in the next node.
 */
void Sequence::nextNode() {
    if(currentNode_ < static_cast<int>(data_.size())) {
        ++currentNode_;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks and update an entity's direction.
 */
void Sequence::Check(Ghost & entity) {
    if(IsEnded()) return;
    if(checkCoordinates(entity)) {
        entity.NeededDirection(data_[currentNode_].Direction);
        nextNode();
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks and update an entity's direction.
 */
void Sequence::Check(AnimatedPuckman & entity) {
    if(IsEnded()) return;
    if(checkCoordinates(entity)) {
        entity.NeededDirection(data_[currentNode_].Direction);
        nextNode();
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Sequence::Sequence() : data_(), currentNode_(0) {}

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
Sequence::Sequence(const std::vector<Node> & data)
    : data_(data), currentNode_(0) {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
Sequence::Sequence(const Sequence & source) : data_(source.data_),
    currentNode_(source.currentNode_) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Sequence::~Sequence() {}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
Sequence & Sequence::operator =(const Sequence & source) {
    data_ = source.data_;
    currentNode_ = source.currentNode_;
    return *this;
}
