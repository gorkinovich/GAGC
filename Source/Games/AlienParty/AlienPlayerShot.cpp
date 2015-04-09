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

#include "AlienPlayerShot.h"
#include <System/AtariPalette.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienEnemyShot.h>

//********************************************************************************
// Constants
//********************************************************************************

const int SPEED = 8;
const int LIMIT_Y = -4;

#define SKIN_COLOR AtariPalette::Hue04Lum02

//********************************************************************************
// Static
//********************************************************************************

Texture2D AlienPlayerShot::texture_;

//--------------------------------------------------------------------------------

void AlienPlayerShot::LoadContent() {
    auto & tileset = AlienManager::Instance()->Tileset();
    texture_.Load(tileset, sf::IntRect(18, 19, 1, 4));
}

//--------------------------------------------------------------------------------

void AlienPlayerShot::UnloadContent() {
    texture_.Unload();
}

//********************************************************************************
// Methods
//********************************************************************************

void AlienPlayerShot::Reset(const sf::Vector2i & coords) {
    previousPosition_ = position_ = coords;
    alive_ = true;
}

//--------------------------------------------------------------------------------

void AlienPlayerShot::Draw() {
    if (alive_) {
        texture_.Draw(position_, SKIN_COLOR);
    }
}

//--------------------------------------------------------------------------------

void AlienPlayerShot::Update() {
    if (alive_) {
        previousPosition_ = position_;
        position_.y -= SPEED;
        if (position_.y < LIMIT_Y) {
            alive_ = false;
        }
    }
}

//--------------------------------------------------------------------------------

bool AlienPlayerShot::Collision(AlienEnemyShot & shot) {
    if (alive_ && shot.alive_ && position_.y <= shot.position_.y &&
        previousPosition_.y >= shot.previousPosition_.y) {
        int lsp = shot.position_.x - 1, rsp = shot.position_.x + 1;
        if (lsp <= position_.x && position_.x <= rsp) {
            alive_ = shot.alive_ = false;
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

void AlienPlayerShot::GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend) {
    colbeg = position_;
    colend = previousPosition_;
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

AlienPlayerShot::AlienPlayerShot() : manager_(AlienManager::Instance()), alive_(false) {}
