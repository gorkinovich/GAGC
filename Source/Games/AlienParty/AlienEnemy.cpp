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

#include "AlienEnemy.h"
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/MathUtil.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienPlayerShot.h>

//********************************************************************************
// Constants
//********************************************************************************

const int SPEED = 4, LIMIT_Y = 334;
const int WIDTH = 13, HEIGHT = 8;
const int HALF_WIDTH = 6, HALF_HEIGHT = 4;
const int MAX_TICKS_TO_FIRE = 230;
const int MIN_TICKS_TO_FIRE =  20;

#define SKIN_COLOR AtariPalette::Hue10Lum02

//********************************************************************************
// Properties
//********************************************************************************

bool AlienEnemy::LeftReached() const {
    return position_.x <= AlienManager::LEFT_LIMIT;
}

//--------------------------------------------------------------------------------

bool AlienEnemy::RightReached() const {
    return AlienManager::RIGHT_LIMIT <= position_.x;
}

//--------------------------------------------------------------------------------

int AlienEnemy::Top() const {
    return (position_.y - HALF_HEIGHT) + collider_.top;
}

//--------------------------------------------------------------------------------

int AlienEnemy::Bottom() const {
    return (position_.y - HALF_HEIGHT) + collider_.top + collider_.height;
}

//--------------------------------------------------------------------------------

int AlienEnemy::Left() const {
    return (position_.x - HALF_WIDTH) + collider_.left;
}

//--------------------------------------------------------------------------------

int AlienEnemy::Right() const {
    return (position_.x - HALF_WIDTH) + collider_.left + collider_.width;
}

//********************************************************************************
// Static
//********************************************************************************

Texture2D AlienEnemy::textures_[MAX_TYPES][MAX_TEXTURES];

int AlienEnemy::Level = 0;
int AlienEnemy::Speed = 0;

//--------------------------------------------------------------------------------

void AlienEnemy::LoadContent() {
    auto & tileset = AlienManager::Instance()->Tileset();
    textures_[TYPE_1][0].Load(tileset, sf::IntRect(18, 1, WIDTH, HEIGHT));
    textures_[TYPE_1][1].Load(tileset, sf::IntRect(32, 1, WIDTH, HEIGHT));
    textures_[TYPE_2][0].Load(tileset, sf::IntRect(46, 1, WIDTH, HEIGHT));
    textures_[TYPE_2][1].Load(tileset, sf::IntRect(60, 1, WIDTH, HEIGHT));
    textures_[TYPE_3][0].Load(tileset, sf::IntRect(74, 1, WIDTH, HEIGHT));
    textures_[TYPE_3][1].Load(tileset, sf::IntRect(88, 1, WIDTH, HEIGHT));
    textures_[TYPE_D][0].Load(tileset, sf::IntRect(102, 1, WIDTH, HEIGHT));
    textures_[TYPE_D][1].Load(tileset, sf::IntRect(59, 16, WIDTH, HEIGHT));
}

//--------------------------------------------------------------------------------

void AlienEnemy::UnloadContent() {
    textures_[TYPE_1][0].Unload();
    textures_[TYPE_1][1].Unload();
    textures_[TYPE_2][0].Unload();
    textures_[TYPE_2][1].Unload();
    textures_[TYPE_3][0].Unload();
    textures_[TYPE_3][1].Unload();
    textures_[TYPE_D][0].Unload();
    textures_[TYPE_D][1].Unload();
}

//********************************************************************************
// Methods
//********************************************************************************

void AlienEnemy::Reset(int type, const sf::Vector2i & coords) {
    normalAnimation_.Frames.clear();
    normalAnimation_.Frames.push_back(AlienAnimation::Node(0, 16));
    normalAnimation_.Frames.push_back(AlienAnimation::Node(1, 16));
    normalAnimation_.Reset(true);

    deathAnimation_.Frames.clear();
    deathAnimation_.Frames.push_back(AlienAnimation::Node(0, 20));
    deathAnimation_.Frames.push_back(AlienAnimation::Node(1, 1));
    deathAnimation_.Reset();

    type_ = type;
    previousPosition_ = position_ = coords;
    alive_ = true;
    if (type_ == TYPE_1) {
        collider_ = sf::IntRect(1, 2, 12, 4);
    } else if (type_ == TYPE_2) {
        collider_ = sf::IntRect(1, 2, 11, 4);
    } else {
        collider_ = sf::IntRect(2, 2, 8, 4);
    }
    SetNextFire();
}

//--------------------------------------------------------------------------------

void AlienEnemy::Draw() {
    sf::Vector2i coords(position_.x - HALF_WIDTH, position_.y - HALF_HEIGHT);
    if (alive_) {
        textures_[type_][normalAnimation_.GetIndex()].Draw(coords, SKIN_COLOR);
    } else {
         textures_[TYPE_D][deathAnimation_.GetIndex()].Draw(coords, SKIN_COLOR);
    }
}

//--------------------------------------------------------------------------------

void AlienEnemy::Update() {
    if (alive_) {
        previousPosition_ = position_;
        normalAnimation_.Update();
        if (ticksUntilFire_ <= 0) {
            SetNextFire();
            manager_->CreateEnemyShot(sf::Vector2i(position_.x, position_.y + 3));
        } else {
            --ticksUntilFire_;
        }
    } else {
        deathAnimation_.Update();
    }
}

//--------------------------------------------------------------------------------

void AlienEnemy::UpdateAnimation() {
    if (alive_) {
        normalAnimation_.Update();
    } else {
        deathAnimation_.Update();
    }
}

//--------------------------------------------------------------------------------

bool AlienEnemy::Collision(AlienPlayerShot & shot) {
    if (alive_ && shot.alive_) {
        int TOP = position_.y - 1, BOT = position_.y + 3;
        if (shot.previousPosition_.y >= TOP && shot.position_.y <= BOT) {
            int LEFT = position_.x - 6, RIGHT = position_.x + 6;
            if (shot.position_.x >= LEFT && shot.position_.x <= RIGHT) {
                alive_ = false;
                manager_->EnemySound().Play();
                manager_->EnemyDies();
                shot.alive_ = false;
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

void AlienEnemy::SetNextFire() {
    if (MAX_TICKS_TO_FIRE > Level) {
        ticksUntilFire_ = core_->Random(MAX_TICKS_TO_FIRE - Level);
    } else {
        ticksUntilFire_ = 0;
    }
    ticksUntilFire_ += MIN_TICKS_TO_FIRE;
}

//--------------------------------------------------------------------------------

void AlienEnemy::MoveDown() {
    if (position_.y + HALF_WIDTH >= LIMIT_Y) {
        manager_->EnemyLanded();
    } else {
        position_.y += HEIGHT;
    }
}

//--------------------------------------------------------------------------------

void AlienEnemy::MoveLeft() {
    position_.x -= Speed;
    if (position_.x < AlienManager::LEFT_LIMIT)
        position_.x = AlienManager::LEFT_LIMIT;
}

//--------------------------------------------------------------------------------

void AlienEnemy::MoveRight() {
    position_.x += Speed;
    if (position_.x > AlienManager::RIGHT_LIMIT)
        position_.x = AlienManager::RIGHT_LIMIT;
}

//--------------------------------------------------------------------------------

void AlienEnemy::MiniMoveLeft() {
    position_.x--;
}

//--------------------------------------------------------------------------------

void AlienEnemy::MiniMoveRight() {
    position_.x++;
}

//--------------------------------------------------------------------------------

void AlienEnemy::GetCollider(sf::IntRect & worldCollider) {
    worldCollider = sf::IntRect(
        position_.x - HALF_WIDTH + collider_.left,
        position_.y - HALF_HEIGHT + collider_.top,
        collider_.width,
        collider_.height
    );
}

//--------------------------------------------------------------------------------

void AlienEnemy::GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend) {
    colbeg = sf::Vector2i(
        position_.x - HALF_WIDTH + collider_.left,
        position_.y - HALF_HEIGHT + collider_.top
    );
    colend = sf::Vector2i(
        colbeg.x + collider_.width - 1,
        colbeg.y + collider_.height - 1
    );
}

//--------------------------------------------------------------------------------

void AlienEnemy::GetBigCollider(sf::Vector2i & colbeg, sf::Vector2i & colend) {
    if (previousPosition_.x < position_.x) {
        colbeg = sf::Vector2i(
            previousPosition_.x - HALF_WIDTH + collider_.left,
            position_.y - HALF_HEIGHT
        );
    } else {
        colbeg = sf::Vector2i(
            position_.x - HALF_WIDTH + collider_.left,
            position_.y - HALF_HEIGHT
        );
    }
    int extraWidth = std::abs(previousPosition_.x - position_.x);
    colend = sf::Vector2i(
        colbeg.x + extraWidth + collider_.width - 1,
        colbeg.y + HEIGHT
    );
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

AlienEnemy::AlienEnemy() : core_(CoreManager::Instance()),
    manager_(AlienManager::Instance()), alive_(false) {}
