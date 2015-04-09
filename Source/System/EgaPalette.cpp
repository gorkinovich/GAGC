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

#include "EgaPalette.h"

//********************************************************************************
// Constants
//********************************************************************************

/*
 * Default EGA 16-color palette
 * (set up to match the standard CGA colors)
 * Color                                   	rgbRGB 	Decimal
 * 0  – Black                    (#000000) 	000000 	0
 * 1  – Blue                     (#0000AA) 	000001 	1
 * 2  – Green                    (#00AA00) 	000010 	2
 * 3  – Cyan                     (#00AAAA) 	000011 	3
 * 4  – Red                      (#AA0000) 	000100 	4
 * 5  – Magenta                  (#AA00AA) 	000101 	5
 * 6  – Brown                    (#AA5500) 	010100 	20
 * 7  – White / Light gray       (#AAAAAA) 	000111 	7
 * 8  – Dark gray / Bright black (#555555) 	111000 	56
 * 9  – Bright blue              (#5555FF) 	111001 	57
 * 10 – Bright green             (#55FF55) 	111010 	58
 * 11 – Bright cyan              (#55FFFF) 	111011 	59
 * 12 – Bright red               (#FF5555) 	111100 	60
 * 13 – Bright magenta           (#FF55FF) 	111101 	61
 * 14 – Bright yellow            (#FFFF55) 	111110 	62
 * 15 – Bright white             (#FFFFFF) 	111111 	63
 */

const sf::Color EgaPalette::Black         = sf::Color(0x00, 0x00, 0x00);
const sf::Color EgaPalette::Blue          = sf::Color(0x00, 0x00, 0xAA);
const sf::Color EgaPalette::Green         = sf::Color(0x00, 0xAA, 0x00);
const sf::Color EgaPalette::Cyan          = sf::Color(0x00, 0xAA, 0xAA);
const sf::Color EgaPalette::Red           = sf::Color(0xAA, 0x00, 0x00);
const sf::Color EgaPalette::Magenta       = sf::Color(0xAA, 0x00, 0xAA);
const sf::Color EgaPalette::Brown         = sf::Color(0xAA, 0x55, 0x00);
const sf::Color EgaPalette::LightGray     = sf::Color(0xAA, 0xAA, 0xAA);
const sf::Color EgaPalette::DarkGray      = sf::Color(0x55, 0x55, 0x55);
const sf::Color EgaPalette::BrightBlue    = sf::Color(0x55, 0x55, 0xFF);
const sf::Color EgaPalette::BrightGreen   = sf::Color(0x55, 0xFF, 0x55);
const sf::Color EgaPalette::BrightCyan    = sf::Color(0x55, 0xFF, 0xFF);
const sf::Color EgaPalette::BrightRed     = sf::Color(0xFF, 0x55, 0x55);
const sf::Color EgaPalette::BrightMagenta = sf::Color(0xFF, 0x55, 0xFF);
const sf::Color EgaPalette::BrightYellow  = sf::Color(0xFF, 0xFF, 0x55);
const sf::Color EgaPalette::White         = sf::Color(0xFF, 0xFF, 0xFF);

const sf::Color EgaPalette::Colors[] = {
    Black, Blue, Green, Cyan,
    Red, Magenta, Brown, LightGray,
    DarkGray, BrightBlue, BrightGreen, BrightCyan,
    BrightRed, BrightMagenta, BrightYellow, White
};
