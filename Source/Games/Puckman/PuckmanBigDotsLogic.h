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

#ifndef __PUCKMAN_BIG_DOTS_LOGIC_HEADER__
#define __PUCKMAN_BIG_DOTS_LOGIC_HEADER__

#include <vector>
#include <SFML/Graphics/Rect.hpp>

namespace sf {
    class Time;
}

namespace Puckman {
    class Manager;

    /**
     * This class represents the controller behind the big dots blinking.
     */
    class BigDotsLogic {
    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Reset();
        void ResetTime();
        void Register(int row, int col);
        void Draw();
        void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        BigDotsLogic();

    private:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The maximum time interval to blink.
        static const int MAX_TIME_INTERVAL = 200;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The coordinates of the big dots on the screen.
        std::vector<sf::Vector2i> coords_;

        // The current time passed.
        int currentTime_;

        // Tells the game if the big dots must be hidden or not.
        bool hideDots_;

        // The manager of the game.
        Manager * manager_;
    };
}

#endif
