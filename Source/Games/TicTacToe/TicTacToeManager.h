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

#ifndef __TICTACTOE_MANAGER_HEADER__
#define __TICTACTOE_MANAGER_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/SharedTypes.h>

namespace sf {
    class Time;
}

class Sound;

/**
 * This singleton class represents the tic-tac-toe game manager.
 */
class TicTacToeManager {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    SharedTexture & Tileset();

    Sound & KeyboardSound();
    Sound & ClickSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    // Game
    void StartGame(int numberOfPlayers);
    void DrawGame();
    void UpdateGame(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static TicTacToeManager * Instance();
    static TicTacToeManager & Reference();
    ~TicTacToeManager();

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
    sf::Vector2i getBoardCoords(const sf::Vector2i & point);
    bool checkVictory(int player);
    bool checkPotential(int enemy, const sf::Vector2i & p1, const sf::Vector2i & p2);
    void aiTakePiece();
    void aiMovePiece();
    void changeSubstate(int value);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static TicTacToeManager * instance_;
    TicTacToeManager();
};

#endif
