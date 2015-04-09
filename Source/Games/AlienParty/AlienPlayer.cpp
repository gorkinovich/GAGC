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

#include "AlienPlayer.h"
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Keyboard.h>
#include <System/MathUtil.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienEnemyShot.h>
#include <Games/AlienParty/AlienEnemy.h>

//********************************************************************************
// Constants
//********************************************************************************

const int SPEED = 4, MAX_DAMAGE = 10;
const int WIDTH = 16, HEIGHT = 8;
const int HALF_WIDTH = 8, HALF_HEIGHT = 4;
const int MAX_TICKS_UNTIL_FIRE = 10;

#define SKIN_COLOR AtariPalette::Hue00Lum14

//********************************************************************************
// Methods
//********************************************************************************

void AlienPlayer::Initialize() {
    textures_[0].Load(manager_->Tileset(), sf::IntRect( 1, 10, WIDTH, HEIGHT));
    textures_[1].Load(manager_->Tileset(), sf::IntRect(18, 10, WIDTH, HEIGHT));
    textures_[2].Load(manager_->Tileset(), sf::IntRect( 1, 19, WIDTH, HEIGHT));
    textures_[3].Load(manager_->Tileset(), sf::IntRect(59, 16, WIDTH, HEIGHT));

    deathAnimation_.Frames.clear();
    for (int i = 0; i < 8; ++i) {
        deathAnimation_.Frames.push_back(AlienAnimation::Node(1, 5));
        deathAnimation_.Frames.push_back(AlienAnimation::Node(2, 5));
    }
    deathAnimation_.Frames.push_back(AlienAnimation::Node(3, 10));

    collider_ = sf::IntRect(2, 3, 13, 5);
    Reset();
}

//--------------------------------------------------------------------------------

void AlienPlayer::Reset() {
    deathAnimation_.Reset();
    position_ = sf::Vector2i(312, 320);
    life_ = 100;
    ticksUntilFire_ = 0;
}

//--------------------------------------------------------------------------------

void AlienPlayer::Kill() {
    manager_->PlayerSound().Play();
    life_ = 0;
}

//--------------------------------------------------------------------------------

void AlienPlayer::Draw() {
    sf::Vector2i coords(position_.x - HALF_WIDTH, position_.y - HALF_HEIGHT);
    if (IsDead()) {
        textures_[deathAnimation_.GetIndex()].Draw(coords, SKIN_COLOR);
    } else {
        textures_[0].Draw(coords, SKIN_COLOR);
    }
}

//--------------------------------------------------------------------------------

void AlienPlayer::Update() {
    if (IsDead()) {
        deathAnimation_.Update();
    } else {
        // Update the position.
        int x = position_.x;
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            x -= SPEED;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            x += SPEED;
        }
        if (x < AlienManager::LEFT_LIMIT)  x = AlienManager::LEFT_LIMIT;
        if (x > AlienManager::RIGHT_LIMIT) x = AlienManager::RIGHT_LIMIT;
        position_.x = x;

        // Update the fire operation.
        if (ticksUntilFire_ <= 0) {
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                manager_->FireSound().Play();
                manager_->CreatePlayerShot(sf::Vector2i(position_.x, position_.y - 5));
                ticksUntilFire_ = MAX_TICKS_UNTIL_FIRE;
            }
        } else {
            --ticksUntilFire_;
        }
    }
}

//--------------------------------------------------------------------------------

bool AlienPlayer::Collision(AlienEnemyShot & shot) {
    if (shot.alive_) {
        int TOP = position_.y - 1, BOT = position_.y + 3;
        if (shot.position_.y >= TOP && shot.previousPosition_.y <= BOT) {
            int lsp = shot.position_.x - 1, rsp = shot.position_.x + 1;
            int LEFT = position_.x - 6, RIGHT = position_.x + 6;
            if (lsp >= LEFT && rsp <= RIGHT) {
                life_ -= (MAX_DAMAGE - std::abs(position_.x - shot.position_.x));
                if (life_ <= 0) {
                    Kill();
                }
                shot.alive_ = false;
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool AlienPlayer::Collision(AlienEnemy & alien) {
    if (life_ > 0 && alien.alive_) {
        sf::Vector2i alienColbeg, alienColend;
        alien.GetCollider(alienColbeg, alienColend);
        sf::Vector2i playerColbeg, playerColend;
        GetCollider(playerColbeg, playerColend);
        if (alienColend.y >= playerColbeg.y && alienColbeg.y <= playerColend.y &&
            alienColend.x >= playerColbeg.x && alienColbeg.x <= playerColend.x) {
            Kill();
            alien.alive_ = false;
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

void AlienPlayer::GetCollider(sf::IntRect & worldCollider) {
    worldCollider = sf::IntRect(
        position_.x - HALF_WIDTH + collider_.left,
        position_.y - HALF_HEIGHT + collider_.top,
        collider_.width,
        collider_.height
    );
}

//--------------------------------------------------------------------------------

void AlienPlayer::GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend) {
    colbeg = sf::Vector2i(
        position_.x - HALF_WIDTH + collider_.left,
        position_.y - HALF_HEIGHT + collider_.top
    );
    colend = sf::Vector2i(
        colbeg.x + collider_.width - 1,
        colbeg.y + collider_.height - 1
    );
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

AlienPlayer::AlienPlayer() : manager_(AlienManager::Instance()) {}
