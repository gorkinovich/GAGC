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

#ifndef __ALIEN_ENEMY_HEADER__
#define __ALIEN_ENEMY_HEADER__

#include <System/Texture2D.h>
#include <Games/AlienParty/AlienAnimation.h>

class CoreManager;
class AlienManager;
class AlienPlayerShot;

/**
 * This class represents the enemy.
 */
class AlienEnemy {
public:
    friend class AlienPlayer;

    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int TYPE_1 = 0;
    static const int TYPE_2 = 1;
    static const int TYPE_3 = 2;
    static const int TYPE_D = 3;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool Alive() const { return alive_; }
    bool IsDeathAnimationFinished() const { return deathAnimation_.Finished(); }

    int X() const { return position_.x; }
    int Y() const { return position_.y; }
    void X(int x) { position_.x = x; }
    void Y(int y) { position_.y = y; }

    bool LeftReached() const;
    bool RightReached() const;

    int Top() const;
    int Bottom() const;
    int Left() const;
    int Right() const;

    static int Level;
    static int Speed;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    static void LoadContent();
    static void UnloadContent();

    void Reset(int type, const sf::Vector2i & coords);
    void Draw();
    void Update();
    void UpdateAnimation();
    bool Collision(AlienPlayerShot & shot);
    void SetNextFire();

    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void MiniMoveLeft();
    void MiniMoveRight();

    void GetCollider(sf::IntRect & worldCollider);
    void GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend);
    void GetBigCollider(sf::Vector2i & colbeg, sf::Vector2i & colend);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    AlienEnemy();

private:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int MAX_TYPES    = 4;
    static const int MAX_TEXTURES = 2;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    static Texture2D textures_[MAX_TYPES][MAX_TEXTURES];

    CoreManager * core_;
    AlienManager * manager_;
    AlienAnimation normalAnimation_;
    AlienAnimation deathAnimation_;
    sf::IntRect collider_;
    sf::Vector2i position_;
    sf::Vector2i previousPosition_;
    bool alive_;
    int type_;
    int ticksUntilFire_;
};

#endif
