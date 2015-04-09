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

#ifndef __CHECKERS_MANAGER_HEADER__
#define __CHECKERS_MANAGER_HEADER__

#include <array>
#include <string>
#include <System/SharedTypes.h>

namespace sf {
    class Time;
}

class Sound;
class CheckersSaveGames;

/**
 * This singleton class represents the checkers game manager.
 */
class CheckersManager {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    SharedTexture & Tileset();

    int Winner();
    CheckersSaveGames & Saves();

    Sound & KeyboardSound();
    Sound & ClickSound();
    Sound & WrongSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    void DrawMouseCursor();

    // Game
    void StartOnePlayerGame(int side, int difficulty);
    void StartTwoPlayersGame();
    void ResetGame();
    bool GameNotFinished();

    void DrawGame();
    void UpdateBoard();
    bool MouseOverBoard();

    void UpdateMachine(const sf::Time & timeDelta);
    bool MachineTurn();

    // Savegames
    void SaveCurrentGame(int index, const std::string & name);
    void LoadPreviousGame(int index);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static CheckersManager * Instance();
    static CheckersManager & Reference();
    ~CheckersManager();

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

    static CheckersManager * instance_;
    CheckersManager();
};

#endif
