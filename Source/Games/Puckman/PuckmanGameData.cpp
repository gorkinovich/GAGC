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

#include "PuckmanGameData.h"
#include <Games/SaveManager.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Adds a coin to the game.
 */
void GameData::AddCoin() {
    coins_ += 1;
}

//--------------------------------------------------------------------------------

/**
 * Adds a fruit to the last fruits array.
 */
void GameData::AddFruit(FruitTypeEnum fruit) {
    if(static_cast<int>(lastFruits_.size()) < MAX_FRUITS) {
        lastFruits_.push_back(fruit);

    } else {
        // If we have get the maximum number of fruits inside the array, we'll
        // erase the oldest, and add the new in the last position.
        std::vector<FruitTypeEnum> nextVector;
        for(int i = 1; i < MAX_FRUITS; ++i) {
            nextVector.push_back(lastFruits_[i]);
        }
        nextVector.push_back(fruit);
        lastFruits_ = nextVector;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the current score is the high score.
 */
void GameData::CheckHighScore() {
    if(Score() > highScore_) {
        highScore_ = Score();
        SaveManager::Instance()->PuckmanSave();
    }
}

//--------------------------------------------------------------------------------

/**
 * Change the player turn.
 */
void GameData::ChangeTurn() {
    if(playerTurn_ == 1) {
        playerTurn_ = 2;
    } else {
        playerTurn_ = 1;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
GameData::GameData() : highScore_(0), coins_(MIN_COINS), player1_(), player2_(),
    lastFruits_(), numberOfPlayers_(1), playerTurn_(1) {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
GameData::GameData(const GameData & source) : highScore_(source.highScore_),
    coins_(source.coins_), player1_(source.player1_), player2_(source.player2_),
    lastFruits_(source.lastFruits_), numberOfPlayers_(source.numberOfPlayers_),
    playerTurn_(source.playerTurn_) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
GameData::~GameData() {}
