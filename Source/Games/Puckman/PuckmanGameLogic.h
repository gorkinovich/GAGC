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

#ifndef __PUCKMAN_GAME_LOGIC_HEADER__
#define __PUCKMAN_GAME_LOGIC_HEADER__

#include <Games/Puckman/PuckmanGenericLogic.h>

namespace Puckman {
    class GameData;
    class Manager;

    /**
     * This class represents the game logic.
     */
    class GameLogic : public GenericLogic {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        bool Victory() const { return victory_; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Initialize(FruitTypeEnum currentFruit, bool firstPlay, bool clearMaze);
        void Release();
        void Draw();
        void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        GameLogic();
        virtual ~GameLogic();

    protected:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The amount of time when the game begins.
        static const int BEGIN_DELAY_TIME = 1800;

        // The amount of time when the puckman is killed.
        static const int KILL_DELAY_TIME = 1000;

        // The amount of time when the logic ends.
        static const int SHORT_END_DELAY_TIME = 500;

        // The amount of time when the points of an eated fruit is shown.
        static const int SHOW_FRUIT_POINTS_DELAY_TIME = 1000;

        // The amount of time when the puckman is victorious.
        static const int VICTORY_DELAY_TIME = 1500;

        // The amount of time when the logic ends and puckman is victorious.
        static const int END_VICTORY_DELAY_TIME = 1500;

        // This is the state id when the logic is paused at the beginning.
        static const int INITIAL_SOUND_STATE = 10;

        // This is the state id when the logic is paused at the beginning.
        static const int INITIAL_NO_SOUND_STATE = 11;

        // This is the state id when puckman is victorious.
        static const int VICTORY_STATE = 12;

        // This is the state id when puckman is victorious and ends the game.
        static const int END_VICTORY_STATE = 13;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The data manager of the game.
        GameData * data_;

        // The manager of the game.
        Manager * manager_;

        // The time controller when the points of an eated fruit is shown.
        int showFruitPointsTime_;

        // Tells if the points of an eated fruit must be shown or not.
        bool showFruitPoints_;

        // Tells to the program if the puckman have been victorious.
        bool victory_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void addScore(int points);
        void drawFruitPoints();
        void updateFruitPoints(int time);
        void checkNumberOfDots();
        void puckmanUpdate();
        virtual void killPuckman();
        virtual void checkFruitCollision(PuckmanEntity & puckman);
    };
}

#endif
