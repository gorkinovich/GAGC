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

#ifndef __PUCKMAN_MANAGER_HEADER__
#define __PUCKMAN_MANAGER_HEADER__

#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <System/SharedTypes.h>
#include <System/AbstractState.h>
#include <Games/Puckman/PuckmanEnums.h>

namespace Puckman {
    class GameData;
    class InsertCoinState;
    class SelectPlayerState;
    class GameState;
}

class Sound;
class AbstractState;

namespace Puckman {
    /**
     * This singleton class represents the puckman game manager.
     */
    class Manager {
    public:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        static const int WIDTH  = 224;
        static const int HEIGHT = 288;

        static const int CELL_WIDTH  = 8;
        static const int CELL_HEIGHT = 8;

        static const int SPRITE_WIDTH  = 16;
        static const int SPRITE_HEIGHT = 16;

        static const int ROWS   = HEIGHT / CELL_HEIGHT; // 36
        static const int COLUMS = WIDTH / CELL_WIDTH;   // 28

        static const int WORMHOLE_LX = -16;
        static const int WORMHOLE_RX = 224;
        static const int WORMHOLE_Y  = 132;

        static const int MIN_COINS = 0;

        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        // Screen
        SharedTexture & Tileset();

        // Sound
        Sound & Beginning();
        Sound & Chomp();
        Sound & Death();
        Sound & ExtraPac();
        Sound & FruitEat();
        Sound & GhostEat();
        Sound & Siren();

        // Data
        GameData * DataInstance();

        // States
        AbstractState * State() { return state_; }
        InsertCoinState * InsertCoinStateInstance();
        SelectPlayerState * SelectPlayerStateInstance();
        GameState * GameStateInstance();

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Initialize();
        void Release();
        void ResetGame();

        // Screen
        sf::Vector2i CellCoordsToVector2(int row, int col);
        void ClearCell(int row, int col);
        void DrawTile(int row, int col, int index, const sf::Color & color);
        void DrawTile(int row, int col, int index);

        int GetIndexFromChar(char c);
        void DrawText(int row, int col, const std::string & text, const sf::Color & color);
        void DrawText(int row, int col, const std::string & text);
        void DrawTextLeft(int row, int col, const std::string & text, const sf::Color & color);
        void DrawTextLeft(int row, int col, const std::string & text);
        void DrawSymbol(int row, int col, FontSymbolEnum symbol, const sf::Color & color);
        void DrawSymbol(int row, int col, FontSymbolEnum symbol);

        sf::Vector2i SpriteCoordsToVector2(int x, int y);
        void ClearSprite(int x, int y);
        void DrawSprite(int x, int y, int index);

        // Sound
        void StopAll();

        // States
        void ChangeToNextState();
        void SetNextState(AbstractState * state);

        //--------------------------------------------------------------------------------
        // Singleton pattern
        //--------------------------------------------------------------------------------

        static Manager * Instance();
        static Manager & Reference();
        ~Manager();

    private:
        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        struct InnerData;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        bool initialized_;
        AbstractState * nextState_;
        AbstractState * state_;
        std::unique_ptr<InnerData> data_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        // States
        void changeState(AbstractState * victim);

        //--------------------------------------------------------------------------------
        // Singleton pattern
        //--------------------------------------------------------------------------------

        Manager();
    };
}

#endif
