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

#include "PuckmanGenericState.h"
#include <sstream>
#include <iomanip>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Puckman/PuckmanSprites.h>

using namespace Puckman;

//********************************************************************************
// Static
//********************************************************************************

bool GenericState::showScorePlayer2_ = false;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Draws the fruits HUD.
 */
void GenericState::drawFruitsHud() {
    static Fruit auxFruit_;

    if(!data_->LastFruits().empty()) {
        const int START_X = 192, START_Y = 272;
        for(unsigned int i = 0; i < data_->LastFruits().size(); i++) {
            auxFruit_.Type(data_->LastFruits()[i]);
            auxFruit_.SetCoordinates(START_X - Sprites::Width * i, START_Y);
            auxFruit_.Draw();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the lives HUD.
 */
void GenericState::drawLivesHud() {
    const int START_X = 16, START_Y = 272;
    for(int i = 0; i < data_->ExtraLives(); i++) {
        manager_->DrawSprite(START_X + Sprites::Width * i, START_Y, Sprites::LifeIcon);
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the menu HUD.
 */
void GenericState::drawMenuHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };
    auto ToString2 = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << std::setfill('0') << std::setw(2) << victim;
        return strconv.str();
    };

    manager_->DrawText(0, 3, "1UP");
    manager_->DrawTextLeft(1, 6, ToString2(data_->Player1().Score()));

    manager_->DrawText(0, 9, "HIGH SCORE");
    if(data_->HighScore() > 0) {
        manager_->DrawTextLeft(1, 16, ToString2(data_->HighScore()));
    }

    manager_->DrawText(0, 22, "2UP");
    if(showScorePlayer2_) {
        manager_->DrawTextLeft(1, 25, ToString2(data_->Player2().Score()));
    }

    manager_->DrawText(35, 2, "CREDIT");
    manager_->DrawTextLeft(35, 10, ToString(data_->Coins()));

    drawFruitsHud();
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
GenericState::GenericState() : AbstractState(),
    manager_(Manager::Instance()),
    data_(Manager::Instance()->DataInstance()) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
GenericState::~GenericState() {}
