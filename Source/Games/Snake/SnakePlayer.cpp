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

#include "SnakePlayer.h"
#include <System/AtariPalette.h>
#include <Games/Snake/SnakeManager.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const sf::Vector2i SnakePlayer::NONE_DIRECTION(0, 0);
const sf::Vector2i SnakePlayer::NORTH_DIRECTION(0, -1);
const sf::Vector2i SnakePlayer::SOUTH_DIRECTION(0, 1);
const sf::Vector2i SnakePlayer::WEST_DIRECTION(-1, 0);
const sf::Vector2i SnakePlayer::EAST_DIRECTION(1, 0);

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Sets the controls of the player 1.
 */
void SnakePlayer::SetPlayer1() {
    upKey_ = sf::Keyboard::Up;
    downKey_ = sf::Keyboard::Down;
    leftKey_ = sf::Keyboard::Left;
    rightKey_ = sf::Keyboard::Right;

    cellValue_ = SnakeManager::PLAYER1_CELL;
    headColor_ = AtariPalette::Hue04Lum02;
    bodyColor_ = AtariPalette::Hue11Lum02;
}

//--------------------------------------------------------------------------------

/**
 * Sets the controls of the player 2.
 */
void SnakePlayer::SetPlayer2() {
    upKey_ = sf::Keyboard::W;
    downKey_ = sf::Keyboard::S;
    leftKey_ = sf::Keyboard::A;
    rightKey_ = sf::Keyboard::D;

    cellValue_ = SnakeManager::PLAYER2_CELL;
    headColor_ = AtariPalette::Hue09Lum02;
    bodyColor_ = AtariPalette::Hue01Lum02;
}

//--------------------------------------------------------------------------------

/**
 * Initializes the player data in a new game.
 */
void SnakePlayer::NewGame(int x, int y) {
    // Initialize some data of the player.
    score_ = 0;
    direction_ = NONE_DIRECTION;
    nextDirection_ = NONE_DIRECTION;

    // Create the body of the snake.
    auto * manager = SnakeManager::Instance();
    int startPieces = 4;
    body_.clear();
    for (int i = startPieces - 1; i >= 0; --i) {
        auto currentY = y + i;
        manager->World(x, currentY, cellValue_, i == 0 ? headColor_ : bodyColor_);
        body_.push_back(sf::Vector2i(x, currentY));
    }

    // Set the alive flag.
    alive_ = true;
}

//--------------------------------------------------------------------------------

/**
 * Starts the snake to run.
 */
void SnakePlayer::Start() {
    // Set the current direction to the snake.
    nextDirection_ = NORTH_DIRECTION;
    direction_ = nextDirection_;
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction using the keyboard input.
 */
void SnakePlayer::UpdateInput() {
    if (simpleControl_) {
        // The simple control input.
        if (Keyboard::IsKeyUp(leftKey_)) {
            if (direction_ == NORTH_DIRECTION) {
                nextDirection_ = WEST_DIRECTION;
            } else if (direction_ == WEST_DIRECTION) {
                nextDirection_ = SOUTH_DIRECTION;
            } else if (direction_ == SOUTH_DIRECTION) {
                nextDirection_ = EAST_DIRECTION;
            } else if (direction_ == EAST_DIRECTION) {
                nextDirection_ = NORTH_DIRECTION;
            } else {
                nextDirection_ = NONE_DIRECTION;
            }
        } else if (Keyboard::IsKeyUp(rightKey_)) {
            if (direction_ == NORTH_DIRECTION) {
                nextDirection_ = EAST_DIRECTION;
            } else if (direction_ == EAST_DIRECTION) {
                nextDirection_ = SOUTH_DIRECTION;
            } else if (direction_ == SOUTH_DIRECTION) {
                nextDirection_ = WEST_DIRECTION;
            } else if (direction_ == WEST_DIRECTION) {
                nextDirection_ = NORTH_DIRECTION;
            } else {
                nextDirection_ = NONE_DIRECTION;
            }
        }
    } else {
        // The standard control input.
        if (Keyboard::IsKeyUp(upKey_)) {
            if (direction_ != SOUTH_DIRECTION) {
                nextDirection_ = NORTH_DIRECTION;
            }
        } else if (Keyboard::IsKeyUp(downKey_)) {
            if (direction_ != NORTH_DIRECTION) {
                nextDirection_ = SOUTH_DIRECTION;
            }
        } else if (Keyboard::IsKeyUp(leftKey_)) {
            if (direction_ != EAST_DIRECTION) {
                nextDirection_ = WEST_DIRECTION;
            }
        } else if (Keyboard::IsKeyUp(rightKey_)) {
            if (direction_ != WEST_DIRECTION) {
                nextDirection_ = EAST_DIRECTION;
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the logic of the snake.
 */
void SnakePlayer::Update() {
    // First, check that the snake is alive.
    if (!alive_) return;

    // Set the current direction from the next one.
    direction_ = nextDirection_;
    if (direction_ == NONE_DIRECTION) return;

    // Calculate the next head position.
    auto currentHead = *(body_.rbegin());
    auto nextHead = currentHead + direction_;

    // Check what is inside that location.
    auto * manager = SnakeManager::Instance();
    auto nextCellValue = manager->World(nextHead.x, nextHead.y).Value;
    if (nextCellValue == SnakeManager::WALL_CELL ||
        nextCellValue == SnakeManager::PLAYER1_CELL ||
        nextCellValue == SnakeManager::PLAYER2_CELL) {
        // When the cell has a wall or an snake the player dies.
        alive_ = false;
        manager->Kill(cellValue_, this);
    } else if (nextCellValue == SnakeManager::DOT1_CELL ||
        nextCellValue == SnakeManager::DOT2_CELL) {
        // When the cell has a dot the player eats it.
        manager->Kill(nextCellValue, this);
        manager->World(currentHead.x, currentHead.y, cellValue_, bodyColor_);
        manager->World(nextHead.x, nextHead.y, cellValue_, headColor_);
        body_.push_back(nextHead);
    } else {
        // Otherwise, the player move forward.
        auto len = body_.size();
        if (len > 0) {
            auto currentTail = *(body_.begin());
            manager->World(currentTail.x, currentTail.y, SnakeManager::EMTPY_CELL);
            manager->World(currentHead.x, currentHead.y, cellValue_, bodyColor_);
            manager->World(nextHead.x, nextHead.y, cellValue_, headColor_);
            auto top = len - 1;
            for (unsigned int i = 0; i < top; ++i) {
                body_[i] = body_[i + 1];
            }
            body_[top] = nextHead;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Removes the snake from the world.
 */
void SnakePlayer::RemoveFromWorld() {
    auto * manager = SnakeManager::Instance();
    ForEach(body_, [manager] (sf::Vector2i & item) {
        manager->World(item.x, item.y, SnakeManager::EMTPY_CELL);
    });
}

//--------------------------------------------------------------------------------

/**
 * Adds some score to the player.
 */
void SnakePlayer::AddScore(int value) {
    if (value > 0) {
        score_ += value;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
SnakePlayer::SnakePlayer() : score_(0), body_(),
    direction_(NONE_DIRECTION), nextDirection_(NONE_DIRECTION), alive_(false),
    simpleControl_(false), cellValue_(SnakeManager::EMTPY_CELL), headColor_(),
    bodyColor_(), upKey_(sf::Keyboard::Up), downKey_(sf::Keyboard::Down),
    leftKey_(sf::Keyboard::Left), rightKey_(sf::Keyboard::Right) {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
SnakePlayer::SnakePlayer(const SnakePlayer & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
SnakePlayer::~SnakePlayer() {}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
SnakePlayer & SnakePlayer::operator =(const SnakePlayer & source) {
    score_ = source.score_;
    body_ = source.body_;
    direction_ = source.direction_;
    nextDirection_ = source.nextDirection_;
    simpleControl_ = source.simpleControl_;
    cellValue_ = source.cellValue_;
    headColor_ = source.headColor_;
    bodyColor_ = source.bodyColor_;
    upKey_ = source.upKey_;
    downKey_ = source.downKey_;
    leftKey_ = source.leftKey_;
    rightKey_ = source.rightKey_;
    return *this;
}
