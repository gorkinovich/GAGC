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

#ifndef __ALIEN_PLAYER_SHOT_HEADER__
#define __ALIEN_PLAYER_SHOT_HEADER__

#include <System/Texture2D.h>
#include <Games/AlienParty/AlienAnimation.h>

class AlienManager;
class AlienEnemyShot;

/**
 * This class represents an player shot.
 */
class AlienPlayerShot {
public:
    friend class AlienEnemy;
    friend class AlienShield;

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
    void Update();
    bool Collision(AlienEnemyShot & shot);

    void GetCollider(sf::Vector2i & colbeg, sf::Vector2i & colend);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    AlienPlayerShot();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    static Texture2D texture_;

    AlienManager * manager_;
    sf::Vector2i position_;
    sf::Vector2i previousPosition_;
    bool alive_;
};

#endif
