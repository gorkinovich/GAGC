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

#ifndef __ALIEN_MANAGER_HEADER__
#define __ALIEN_MANAGER_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/SharedTypes.h>

namespace sf {
    class Time;
}

class Sound;

/**
 * This singleton class represents the alien party game manager.
 */
class AlienManager {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int LEFT_LIMIT  = 120;
    static const int RIGHT_LIMIT = 520;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    SharedTexture & Tileset();
    int DeathCount();
    Sound & KeyboardSound();
    Sound & FireSound();
    Sound & EnemySound();
    Sound & PlayerSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    // Game
    void StartGame();
    void DrawGame();
    void UpdateGame(const sf::Time & timeDelta);

    void EnemyDies();
    void EnemyLanded();
    void CreatePlayerShot(const sf::Vector2i & coords);
    void CreateEnemyShot(const sf::Vector2i & coords);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static AlienManager * Instance();
    static AlienManager & Reference();
    ~AlienManager();

private:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct InnerData;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    bool initialized_;
    std::unique_ptr<InnerData> data_;

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static AlienManager * instance_;
    AlienManager();
};

#endif
