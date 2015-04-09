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

#ifndef __ALIEN_PLAYER_HEADER__
#define __ALIEN_PLAYER_HEADER__

#include <System/Texture2D.h>
#include <Games/AlienParty/AlienAnimation.h>

class AlienManager;
class AlienEnemyShot;
class AlienEnemy;

/**
 * This class represents the player.
 */
class AlienPlayer {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    int Life() const { return life_; }
    bool IsDead() const { return life_ <= 0; }
    bool IsDeathAnimationFinished() const { return deathAnimation_.Finished(); }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Reset();
    void Kill();
    void Draw();
    void Update();
    bool Collision(AlienEnemyShot & shot);
    bool Collision(AlienEnemy & alien);

    void GetCollider(sf::IntRect & worldCollider);
    void GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    AlienPlayer();

private:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int MAX_TEXTURES = 4;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    AlienManager * manager_;
    Texture2D textures_[MAX_TEXTURES];
    AlienAnimation deathAnimation_;
    sf::IntRect collider_;
    sf::Vector2i position_;
    int life_;
    int ticksUntilFire_;
};

#endif
