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

#include "TextLabel.h"
#include <System/BitmapFont.h>

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Updates the current coordinates after draw a character.
 */
void TextLabel::updateCoords(char item, sf::Vector2i & coords) {
    if (item == '\n') {
        coords.x = position_.x;
        coords.y += font_->GetCharHeight(item);
    } else if (item == '\r') {
        coords.x = position_.x;
    } else {
        coords.x += font_->GetCharWidth(item);
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

/**
 * Constructs a new object.
 */
TextLabel::TextLabel() {}

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
TextLabel::TextLabel(const std::string & text, int x, int y, const sf::Color & color,
    BitmapFont * font) : SimpleLabel(text, x, y, color, font) {}

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
TextLabel::TextLabel(const std::string & text, const sf::Vector2i & position,
    const sf::Color & color, BitmapFont * font) : SimpleLabel(text, position,
    color, font) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
TextLabel::~TextLabel() {}
