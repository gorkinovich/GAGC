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

#ifndef __PUCKMAN_GAME_STATE_HEADER__
#define __PUCKMAN_GAME_STATE_HEADER__

#include <vector>
#include <Games/Puckman/PuckmanGenericState.h>
#include <Games/Puckman/PuckmanGameLogic.h>

namespace Puckman {
    /**
     * This class represents the game state.
     */
    class GameState : public GenericState {
    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void StartOnePlayerGame();
        void StartTwoPlayersGame();
        virtual void Initialize();
        virtual void Release();
        virtual void Draw(const sf::Time & timeDelta);
        virtual void Update(const sf::Time & timeDelta);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        GameState();
        virtual ~GameState();

    private:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The amount of time in the score blinking interval.
        static const long SCORE_BLINK_DELAY_TIME = 250;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The logic manager of the game.
        GameLogic logic_;

        // The current time passed in the score blinking animation.
        int scoreBlinkInterval_;

        // Tells to the program if the score must be shown or not.
        bool showScore_;

        // An auxiliary list of fruits.
        std::vector<FruitTypeEnum> auxLastFruits_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        FruitTypeEnum getCurrentFruit();
        void startGenericGame();
        void drawGameHud();
        void nextLevel();
        void nextLife();
        void nextTurn();
        void swapLastFruits();
        void getBackToTheMenu();
    };
}

#endif
