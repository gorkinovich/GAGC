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

#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanSprites.h>
#include <Games/Puckman/PuckmanGameTimer.h>
#include <Games/Puckman/PuckmanEntity.h>

using namespace Puckman;

//********************************************************************************
// Palette
//********************************************************************************

 const sf::Color Palette::Black     = sf::Color(0, 0, 0);
 const sf::Color Palette::Red       = sf::Color(255, 0, 0);
 const sf::Color Palette::DarkBrown = sf::Color(222, 151, 81);
 const sf::Color Palette::Pink      = sf::Color(255, 184, 255);
 const sf::Color Palette::Cyan      = sf::Color(0, 255, 255);
 const sf::Color Palette::Blue      = sf::Color(71, 184, 255);
 const sf::Color Palette::Brown     = sf::Color(255, 184, 81);
 const sf::Color Palette::Yellow    = sf::Color(255, 255, 0);
 const sf::Color Palette::DarkBlue  = sf::Color(33, 33, 255);
 const sf::Color Palette::Green     = sf::Color(0, 255, 0);
 const sf::Color Palette::DarkGreen = sf::Color(71, 184, 174);
 const sf::Color Palette::DarkPink  = sf::Color(255, 184, 174);
 const sf::Color Palette::White     = sf::Color(222, 222, 255);

const sf::Color Palette::Colors[] = {
    Black, Red, DarkBrown, Pink, Black, Cyan, Blue, Brown, Black,
    Yellow, Black, DarkBlue, Green, DarkGreen, DarkPink, White
};

//********************************************************************************
// GameTimer
//********************************************************************************

int GameTimer::time_ = GameTimer::TIME_STOPPED;

//--------------------------------------------------------------------------------

void GameTimer::Stop() {
    time_ = GameTimer::TIME_STOPPED;
}

//--------------------------------------------------------------------------------

void GameTimer::Set(int countdown) {
    time_ = countdown;
}

//--------------------------------------------------------------------------------

bool GameTimer::IsStopped() {
    return (time_ <= GameTimer::TIME_STOPPED);
}

//--------------------------------------------------------------------------------

void GameTimer::Update(int time) {
    if (time_ > GameTimer::TIME_STOPPED) {
        time_ -= time;
    }
}

//********************************************************************************
// Entity
//********************************************************************************

Entity::Entity() : X(0), Y(0), manager_(Manager::Instance()) {}
