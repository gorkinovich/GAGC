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

#ifndef __PUCKMAN_INSERT_COIN_STATE_HEADER__
#define __PUCKMAN_INSERT_COIN_STATE_HEADER__

#include <Games/Puckman/PuckmanGenericState.h>
#include <Games/Puckman/PuckmanBigDotsLogic.h>
#include <Games/Puckman/PuckmanAnimatedPuckman.h>
#include <Games/Puckman/PuckmanGhost.h>
#include <Games/Puckman/PuckmanDemoLogic.h>

namespace Puckman {
    /**
     * This class represents the insert coin state.
     */
    class InsertCoinState : public GenericState {
    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        virtual void Initialize();
        virtual void Release();
        virtual void Draw(const sf::Time & timeDelta);
        virtual void Update(const sf::Time & timeDelta);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        InsertCoinState();
        virtual ~InsertCoinState();

    private:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The amount of time between one substate and another, until this state
        // gets to the animation substate.
        static const int DELAY_TIME = 600;

        // The amount of time when puckman eats a ghost in the animation.
        static const int POINTS_DELAY_TIME = 600;

        // A fast amount of time that the puckman interval can have.
        static const int PUCKMAN_FAST_INTERVAL = 14;

        // A slow amount of time that the puckman interval can have.
        static const int PUCKMAN_SLOW_INTERVAL = 16;

        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        // This enumeration represents the inner states of this class.
        enum Substates {
            INITIAL_STATE,
            SHOW_RED_GHOST_STATE, SHOW_RED_NAME_STATE, SHOW_RED_NICK_STATE,
            SHOW_PINK_GHOST_STATE, SHOW_PINK_NAME_STATE, SHOW_PINK_NICK_STATE,
            SHOW_BLUE_GHOST_STATE, SHOW_BLUE_NAME_STATE, SHOW_BLUE_NICK_STATE,
            SHOW_BROWN_GHOST_STATE, SHOW_BROWN_NAME_STATE, SHOW_BROWN_NICK_STATE,
            SHOW_DOTS_STATE, SHOW_FINAL_TEXT_STATE,
            ANIMATION_STATE, DEMO_STATE
        };

        // This enumeration represents the states of the animation.
        enum AnimationStates {
            NORMAL, RED_DIE, PINK_DIE, BLUE_DIE, BROWN_DIE
        };

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The inner substate of the class.
        Substates innerState_;

        // This is the controller of the big dots blinking.
        BigDotsLogic bigDotsLogic_;

        // Tells to the state if the big dot of the animation is eated or not.
        bool bigDotEated_;

        // This is the puckman used in the animation.
        AnimatedPuckman puckman_;

        // This is the red ghost used in the animation.
        Ghost red_;

        // This is the pink ghost used in the animation.
        Ghost pink_;

        // This is the blue ghost used in the animation.
        Ghost blue_;

        // This is the brown ghost used in the animation.
        Ghost brown_;

        // The inner substate of the animation.
        AnimationStates aniState_;

        // The logic manager of the demo.
        DemoLogic demo_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void changeInnerState(Substates newState);
        void drawText();
        void changeToNextState(Substates nextState);
    };
}

#endif
