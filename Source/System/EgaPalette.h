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

#ifndef __EGA_PALETTE_HEADER__
#define __EGA_PALETTE_HEADER__

#include <SFML/Graphics/Color.hpp>

/**
 * This static class represents the EGA 16-color palette.
 */
class EgaPalette {
public:
    static const int NumberOfColors     = 16;

    static const int BlackIndex         =  0;
    static const int BlueIndex          =  1;
    static const int GreenIndex         =  2;
    static const int CyanIndex          =  3;
    static const int RedIndex           =  4;
    static const int MagentaIndex       =  5;
    static const int BrownIndex         =  6;
    static const int LightGrayIndex     =  7;
    static const int DarkGrayIndex      =  8;
    static const int BrightBlueIndex    =  9;
    static const int BrightGreenIndex   = 10;
    static const int BrightCyanIndex    = 11;
    static const int BrightRedIndex     = 12;
    static const int BrightMagentaIndex = 13;
    static const int BrightYellowIndex  = 14;
    static const int WhiteIndex         = 15;

    static const sf::Color Black;
    static const sf::Color Blue;
    static const sf::Color Green;
    static const sf::Color Cyan;
    static const sf::Color Red;
    static const sf::Color Magenta;
    static const sf::Color Brown;
    static const sf::Color LightGray;
    static const sf::Color DarkGray;
    static const sf::Color BrightBlue;
    static const sf::Color BrightGreen;
    static const sf::Color BrightCyan;
    static const sf::Color BrightRed;
    static const sf::Color BrightMagenta;
    static const sf::Color BrightYellow;
    static const sf::Color White;

    static const sf::Color Colors[];
};

#endif
