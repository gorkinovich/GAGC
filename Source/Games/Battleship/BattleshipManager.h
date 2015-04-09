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

#ifndef __BATTLESHIP_MANAGER_HEADER__
#define __BATTLESHIP_MANAGER_HEADER__

#include <string>
#include <System/SharedTypes.h>

namespace sf {
    class Time;
}

class Sound;
class BattleshipBoard;

/**
 * This singleton class represents the battleship game manager.
 */
class BattleshipManager {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int NO_WINNER = 0;
    static const int P1_WINNER = 1;
    static const int P2_WINNER = 2;
    static const int AI_WINNER = 3;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct Player;

    struct CurrentData {
        std::string name;
        int turn;
        BattleshipBoard * playerBoard;
        BattleshipBoard * enemyBoard;
    };

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    SharedTexture & Tileset();

    int Winner();

    const std::string & Player1Name();
    const std::string & Player2Name();

    Sound & KeyboardSound();
    Sound & ClickSound();

    Sound & WaterSound();
    Sound & HitSound();
    Sound & SunkSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    // Game
    void StartGame(int numberOfPlayers);

    const std::string & GetPlayerName();
    void AddPlayerName(const std::string & name);
    void AddPlayerBoard(const BattleshipBoard & board);

    void GetCurrentData(CurrentData & data);
    void OnFireUpdate();
    void WaitDone();

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static BattleshipManager * Instance();
    static BattleshipManager & Reference();
    ~BattleshipManager();

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
    void machineTurn();
    void nextSubstate();

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static BattleshipManager * instance_;
    BattleshipManager();
};

#endif
