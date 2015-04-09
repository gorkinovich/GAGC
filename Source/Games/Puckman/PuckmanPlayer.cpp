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

#include "PuckmanPlayer.h"

using namespace Puckman;

//********************************************************************************
// Properties
//********************************************************************************

void Player::ExtraLives(int value) {
    if(0 <= value && value <= 6) {
        extraLives_ = value;
    }
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Sets the data to a new game situation.
 */
void Player::NewGame() {
    score_ = 0;
    level_ = 1;
    extraLives_ = 3;
    maze_.Initialize();
}

//--------------------------------------------------------------------------------

/**
 * Sets the data to a next level situation.
 */
void Player::NextLevel() {
    ++level_;
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Player::Player() : maze_(), puckman_() {
    puckman_.CheckCoordinates = std::bind(&Maze::CheckSpriteCoords, &maze_,
        std::placeholders::_1, std::placeholders::_2);
    NewGame();
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Player::~Player() {}
