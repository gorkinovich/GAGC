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

#ifndef __ALIEN_SHIELD_HEADER__
#define __ALIEN_SHIELD_HEADER__

#include <vector>
#include <System/Texture2D.h>

class CoreManager;
class AlienManager;
class AlienPlayerShot;
class AlienEnemyShot;
class AlienEnemy;

/**
 * This class represents the shield.
 */
class AlienShield {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int ROWS = 8, COLS = 11;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool Alive() const { return alive_; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    static void LoadContent();
    static void UnloadContent();

    void Reset(const sf::Vector2i & coords);
    void Draw();
    bool Collision(AlienPlayerShot & victim);
    bool Collision(AlienEnemyShot & victim);
    bool Collision(AlienEnemy & victim);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    AlienShield();

private:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int MAX_TEXTURES = 16;

    static const int SIDE = 2;
    static const int WIDTH = SIDE * COLS;
    static const int HEIGHT = SIDE * ROWS;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    static Texture2D textures_[MAX_TEXTURES];
    static std::vector<int> rndIdx_[MAX_TEXTURES];

    CoreManager * core_;
    AlienManager * manager_;
    sf::IntRect collider_;
    int data_[ROWS][COLS];
    bool alive_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    sf::Vector2i getRowCol(const sf::Vector2i & coords);
    bool collision(const sf::Vector2i & colbeg, const sf::Vector2i & colend,
        bool useRnd = true);
};

#endif
