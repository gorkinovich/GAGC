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

#ifndef __MINESWEEPER_MANAGER_HEADER__
#define __MINESWEEPER_MANAGER_HEADER__

#include <System/SharedTypes.h>

namespace sf {
    class Time;
}

class Sound;

/**
 * This singleton class represents the minesweeper game manager.
 */
class MinesweeperManager {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    SharedTexture & Tileset();

    Sound & KeyboardSound();
    Sound & ClickSound();
    Sound & WinSound();
    Sound & LoseSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    // Game
    void ConfigBeginnerGame();
    void ConfigIntermediateGame();
    void ConfigAdvanceGame();

    void DrawGame();
    void UpdateGame(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static MinesweeperManager * Instance();
    static MinesweeperManager & Reference();
    ~MinesweeperManager();

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
    void configGame(int rows, int cols, int mines);
    void makeBoardTexture();
    void resetGame();
    void startGame(int row, int col, int button);
    void clickCell(int row, int col, int button);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static MinesweeperManager * instance_;
    MinesweeperManager();
};

#endif
