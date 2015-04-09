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

#ifndef __PUCKMAN_GENERIC_STATE_HEADER__
#define __PUCKMAN_GENERIC_STATE_HEADER__

#include <System/AbstractState.h>
#include <Games/Puckman/PuckmanFruit.h>

namespace Puckman {
    class Manager;
    class GameData;

    /**
     * This class represents a generic state.
     */
    class GenericState : public AbstractState {
    public:
        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        GenericState();
        virtual ~GenericState();

    protected:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // Tells to the object if the player 2 score must be drawn or not.
        static bool showScorePlayer2_;

        // The manager of the game.
        Manager * manager_;

        // The data manager of the game.
        GameData * data_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void drawFruitsHud();
        void drawLivesHud();
        void drawMenuHud();
    };
}

#endif
