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

#include "AlienEnemyShot.h"
#include <System/AtariPalette.h>
#include <Games/AlienParty/AlienManager.h>

//********************************************************************************
// Constants
//********************************************************************************

const int SPEED = 6;
const int LIMIT_Y = 336;

#define SKIN_COLOR AtariPalette::Hue12Lum02

//********************************************************************************
// Static
//********************************************************************************

Texture2D AlienEnemyShot::textures_[MAX_TEXTURES];

//--------------------------------------------------------------------------------

void AlienEnemyShot::LoadContent() {
    auto & tileset = AlienManager::Instance()->Tileset();
    textures_[0].Load(tileset, sf::IntRect(26, 19, 3, 7));
    textures_[1].Load(tileset, sf::IntRect(30, 19, 3, 7));
}

//--------------------------------------------------------------------------------

void AlienEnemyShot::UnloadContent() {
    textures_[0].Unload();
    textures_[1].Unload();
}

//********************************************************************************
// Methods
//********************************************************************************

void AlienEnemyShot::Reset(const sf::Vector2i & coords) {
    previousPosition_ = position_ = coords;
    alive_ = true;

    if (normalAnimation_.Frames.empty()) {
        normalAnimation_.Frames.push_back(AlienAnimation::Node(0, 5));
        normalAnimation_.Frames.push_back(AlienAnimation::Node(1, 5));
    }
    normalAnimation_.Reset(true);
}

//--------------------------------------------------------------------------------

void AlienEnemyShot::Draw() {
    if (alive_) {
        sf::Vector2i coords(position_.x - 1, position_.y - 6);
        textures_[normalAnimation_.GetIndex()].Draw(coords, SKIN_COLOR);
    }
}

//--------------------------------------------------------------------------------

void AlienEnemyShot::Update() {
    if (alive_) {
        previousPosition_ = position_;
        normalAnimation_.Update();
        position_.y += SPEED;
        if (position_.y > LIMIT_Y) {
            alive_ = false;
        }
    }
}

//--------------------------------------------------------------------------------

void AlienEnemyShot::GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend) {
    colbeg = sf::Vector2i(previousPosition_.x - 1, previousPosition_.y);
    colend = sf::Vector2i(position_.x + 1, position_.y);
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

AlienEnemyShot::AlienEnemyShot() : manager_(AlienManager::Instance()), alive_(false) {}
