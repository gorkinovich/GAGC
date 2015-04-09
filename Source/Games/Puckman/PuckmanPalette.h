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

#ifndef __PUCKMAN_PALETTE_HEADER__
#define __PUCKMAN_PALETTE_HEADER__

#include <SFML/Graphics/Color.hpp>

namespace Puckman {
    /**
     * This static class is the palette of colors suported in the game.
     */
    class Palette {
    private:
        Palette() {}
        ~Palette() {}

    public:
        static const int NumberOfColors = 16;

        static const int BlackIndex     = 0;
        static const int RedIndex       = 1;
        static const int DarkBrownIndex = 2;
        static const int PinkIndex      = 3;
        static const int CyanIndex      = 5;
        static const int BlueIndex      = 6;
        static const int BrownIndex     = 7;
        static const int YellowIndex    = 9;
        static const int DarkBlueIndex  = 11;
        static const int GreenIndex     = 12;
        static const int DarkGreenIndex = 13;
        static const int DarkPinkIndex  = 14;
        static const int WhiteIndex     = 15;

        static const sf::Color Black;
        static const sf::Color Red;
        static const sf::Color DarkBrown;
        static const sf::Color Pink;
        static const sf::Color Cyan;
        static const sf::Color Blue;
        static const sf::Color Brown;
        static const sf::Color Yellow;
        static const sf::Color DarkBlue;
        static const sf::Color Green;
        static const sf::Color DarkGreen;
        static const sf::Color DarkPink;
        static const sf::Color White;

        static const sf::Color Colors[];
    };
}

#endif
