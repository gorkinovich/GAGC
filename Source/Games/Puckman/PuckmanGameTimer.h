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

#ifndef __PUCKMAN_GAME_TIMER_HEADER__
#define __PUCKMAN_GAME_TIMER_HEADER__

namespace Puckman {
    /**
     * This static class is a simple timer to add some countdowns to the game.
     */
    class GameTimer {
    private:
        GameTimer() {}
        ~GameTimer() {}

        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        /** This represents when the timer is stopped. */
        static const int TIME_STOPPED = 0;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        /** The current time of the timer's countdown. */
        static int time_;

    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        /** Stops the timer's countdown. */
        static void Stop();

        /** Sets a new timer's countdown. */
        static void Set(int countdown);

        /** Checks if the timer's countdown is stopped or not. */
        static bool IsStopped();

        /** Updates the current timer's countdown. */
        static void Update(int time);
    };
}

#endif
