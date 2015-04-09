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

#ifndef __PONG_MANAGER_HEADER__
#define __PONG_MANAGER_HEADER__

#include <memory>

namespace sf {
    class Time;
}

class SimpleLabel;
class Texture2D;
class Sound;

/**
 * This singleton class represents the pong game manager.
 */
class PongManager {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int DIFFICULTY_EASY   = 0;
    static const int DIFFICULTY_NORMAL = 1;
    static const int DIFFICULTY_HARD   = 2;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct Player;
    struct Ball;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    int ScorePlayer1();
    int ScorePlayer2();

    Sound & MenuSelectSound();
    Sound & MenuKeySound();
    Sound & MenuNextSound();
    Sound & BallLeftSound();
    Sound & BallRightSound();
    Sound & PointSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    // Game
    void StartGame(int difficulty, int numberOfPlayers, int numberOfPoints);
    void DrawGame();
    void UpdateGame(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static PongManager * Instance();
    static PongManager & Reference();
    ~PongManager();

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
    // Methods
    //--------------------------------------------------------------------------------

    // Game
    void resetBall();
    void addPoint(Player & player);
    void updateBall();
    void updatePlayer();
    void updateMachine();
    void updatePlayer1();
    void updatePlayer2();
    void updatePlayer(Player & player, int direction);

    void changeSubstate(int value);
    void updateHud();

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static PongManager * instance_;
    PongManager();
};

#endif
