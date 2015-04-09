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

#include "AlienShield.h"
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/MathUtil.h>
#include <Games/AlienParty/AlienManager.h>
#include <Games/AlienParty/AlienPlayerShot.h>
#include <Games/AlienParty/AlienEnemyShot.h>
#include <Games/AlienParty/AlienEnemy.h>

//********************************************************************************
// Constants
//********************************************************************************

const int EMPTY =  0;
const int B1_DR =  1, B1_DL =  2, B1_UL =  3, B1_UR =  4;
const int B2_DO =  5, B2_UP =  6, B2_LE =  7, B2_RI =  8, B2_LR = 9, B2_RL = 10;
const int B3_DR = 11, B3_DL = 12, B3_UL = 13, B3_UR = 14;
const int B4_AL = 15;

const int INITIAL_DATA[AlienShield::ROWS][AlienShield::COLS] = {
    { EMPTY, B1_DR, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B1_DL, EMPTY },
    { B1_DR, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B1_DL },
    { B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL },
    { B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL },
    { B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL },
    { B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL, B4_AL },
    { B4_AL, B4_AL, B4_AL, B1_UL, EMPTY, EMPTY, EMPTY, B3_UR, B4_AL, B4_AL, B4_AL },
    { B4_AL, B4_AL, B2_LE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, B4_AL, B4_AL, B4_AL }
};

#define SKIN_COLOR AtariPalette::Hue00Lum04

//********************************************************************************
// Static
//********************************************************************************

Texture2D AlienShield::textures_[MAX_TEXTURES];
std::vector<int> AlienShield::rndIdx_[MAX_TEXTURES];

//--------------------------------------------------------------------------------

void AlienShield::LoadContent() {
    auto & tileset = AlienManager::Instance()->Tileset();
    textures_[B4_AL].Load(tileset, sf::IntRect(60, 12, SIDE, SIDE));
    textures_[B3_DR].Load(tileset, sf::IntRect(63, 12, SIDE, SIDE));
    textures_[B3_DL].Load(tileset, sf::IntRect(66, 12, SIDE, SIDE));
    textures_[B3_UL].Load(tileset, sf::IntRect(69, 12, SIDE, SIDE));
    textures_[B3_UR].Load(tileset, sf::IntRect(72, 12, SIDE, SIDE));
    textures_[B2_DO].Load(tileset, sf::IntRect(75, 12, SIDE, SIDE));
    textures_[B2_LE].Load(tileset, sf::IntRect(78, 12, SIDE, SIDE));
    textures_[B2_UP].Load(tileset, sf::IntRect(81, 12, SIDE, SIDE));
    textures_[B2_RI].Load(tileset, sf::IntRect(84, 12, SIDE, SIDE));
    textures_[B2_LR].Load(tileset, sf::IntRect(87, 12, SIDE, SIDE));
    textures_[B2_RL].Load(tileset, sf::IntRect(90, 12, SIDE, SIDE));
    textures_[B1_DL].Load(tileset, sf::IntRect(93, 12, SIDE, SIDE));
    textures_[B1_UL].Load(tileset, sf::IntRect(96, 12, SIDE, SIDE));
    textures_[B1_UR].Load(tileset, sf::IntRect(99, 12, SIDE, SIDE));
    textures_[B1_DR].Load(tileset, sf::IntRect(102, 12, SIDE, SIDE));
    textures_[EMPTY].Load(tileset, sf::IntRect(105, 12, SIDE, SIDE));

    const int B4_IDX[] = {
        B3_DR, B3_DL, B3_UL, B3_UR,
        B2_DO, B2_LE, B2_UP, B2_RI, B2_LR, B2_RL
    };
    std::vector<int> b4_idx(std::begin(B4_IDX), std::end(B4_IDX));

    rndIdx_[B4_AL] = b4_idx;

    const int B3_IDX[] = { 
        B2_DO, B2_LE, B2_UP, B2_RI, B2_LR, B2_RL,
        B1_DL, B1_UL, B1_UR, B1_DR
    };
    std::vector<int> b3_idx(std::begin(B3_IDX), std::end(B3_IDX));

    rndIdx_[B3_DR] = b3_idx;
    rndIdx_[B3_DL] = b3_idx;
    rndIdx_[B3_UL] = b3_idx;
    rndIdx_[B3_UR] = b3_idx;

    const int B2_IDX[] = { EMPTY, B1_DL, B1_UL, B1_UR, B1_DR };
    std::vector<int> b2_idx(std::begin(B2_IDX), std::end(B2_IDX));

    rndIdx_[B2_DO] = b2_idx;
    rndIdx_[B2_LE] = b2_idx;
    rndIdx_[B2_UP] = b2_idx;
    rndIdx_[B2_RI] = b2_idx;
    rndIdx_[B2_LR] = b2_idx;
    rndIdx_[B2_RL] = b2_idx;

    const int B1_IDX[] = { EMPTY };
    std::vector<int> b1_idx(std::begin(B1_IDX), std::end(B1_IDX));

    rndIdx_[B1_DL] = b1_idx;
    rndIdx_[B1_UL] = b1_idx;
    rndIdx_[B1_UR] = b1_idx;
    rndIdx_[B1_DR] = b1_idx;
    rndIdx_[EMPTY] = b1_idx;
}

//--------------------------------------------------------------------------------

void AlienShield::UnloadContent() {
    for (int i = 0; i < MAX_TEXTURES; ++i) {
        textures_[i].Unload();
        rndIdx_[i].clear();
    }
}

//********************************************************************************
// Methods
//********************************************************************************

void AlienShield::Reset(const sf::Vector2i & coords) {
    collider_ = sf::IntRect(coords.x, coords.y, WIDTH, HEIGHT);
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            data_[i][j] = INITIAL_DATA[i][j];
        }
    }
    alive_ = true;
}

//--------------------------------------------------------------------------------

void AlienShield::Draw() {
    if (alive_) {
        sf::Vector2i coords(collider_.left, collider_.top);
        for (int i = 0; i < ROWS; ++i) {
            coords.x = collider_.left;
            for (int j = 0; j < COLS; ++j) {
                textures_[data_[i][j]].Draw(coords, SKIN_COLOR);
                coords.x += SIDE;
            }
            coords.y += SIDE;
        }
    }
}

//--------------------------------------------------------------------------------

bool AlienShield::Collision(AlienPlayerShot & victim) {
    if (alive_ && victim.Alive()) {
        sf::Vector2i colbeg, colend;
        victim.GetCollider(colbeg, colend);
        if (collision(colbeg, colend)) {
            victim.alive_ = false;
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool AlienShield::Collision(AlienEnemyShot & victim) {
    if (alive_ && victim.Alive()) {
        sf::Vector2i colbeg, colend;
        victim.GetCollider(colbeg, colend);
        if (collision(colbeg, colend)) {
            victim.alive_ = false;
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

bool AlienShield::Collision(AlienEnemy & victim) {
    if (alive_ && victim.Alive()) {
        sf::Vector2i colbeg, colend;
        victim.GetBigCollider(colbeg, colend);
        if (collision(colbeg, colend, false)) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

sf::Vector2i AlienShield::getRowCol(const sf::Vector2i & coords) {
    int c = (coords.x - collider_.left) / SIDE;
    int r = (coords.y - collider_.top) / SIDE;
    if (c < 0) c = 0; if (c >= COLS) c = COLS - 1;
    if (r < 0) r = 0; if (r >= ROWS) r = ROWS - 1;
    return sf::Vector2i(c, r);
}

//--------------------------------------------------------------------------------

bool AlienShield::collision(const sf::Vector2i & colbeg, const sf::Vector2i & colend, bool useRnd) {
    if (colend.y >= collider_.top && colbeg.y <= collider_.top + collider_.height &&
        colend.x >= collider_.left && colbeg.x <= collider_.left + collider_.width) {
        sf::Vector2i tabbeg = getRowCol(colbeg);
        sf::Vector2i tabend = getRowCol(colend);
        bool something = false;
        for (int i = tabbeg.y; i <= tabend.y; ++i) {
            for (int j = tabbeg.x; j <= tabend.x; ++j) {
                if (data_[i][j] != EMPTY) {
                    something = true;
                    if (useRnd) {
                        int idx = core_->Random(rndIdx_[data_[i][j]].size());
                        data_[i][j] = rndIdx_[data_[i][j]][idx];
                    } else {
                        data_[i][j] = EMPTY;
                    }
                }
            }
        }
        return something;
    }
    return false;
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

AlienShield::AlienShield() : core_(CoreManager::Instance()),
    manager_(AlienManager::Instance()), alive_(false) {}
