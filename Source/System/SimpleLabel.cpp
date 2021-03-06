/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#include "SimpleLabel.h"
#include <System/BitmapFont.h>

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Draws the current state of the label.
 */
void SimpleLabel::Draw() {
    if (font_ != nullptr && !text_.empty()) {
        sf::Vector2i coords(position_);
        for (int i = 0, len = text_.size(); i < len; ++i) {
            font_->Draw(text_[i], coords.x, coords.y, color_);
            updateCoords(text_[i], coords);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the current coordinates after draw a character.
 */
void SimpleLabel::updateCoords(char item, sf::Vector2i & coords) {
    coords.x += font_->GetCharWidth(item);
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

/**
 * Constructs a new object.
 */
SimpleLabel::SimpleLabel() : text_(), position_(0, 0),
    color_(sf::Color::White), font_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
SimpleLabel::SimpleLabel(const std::string & text, int x, int y,
    const sf::Color & color, BitmapFont * font) : text_(text),
    position_(x, y), color_(color), font_(font) {}

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
SimpleLabel::SimpleLabel(const std::string & text, const sf::Vector2i & position,
    const sf::Color & color, BitmapFont * font) : text_(text), position_(position),
    color_(color), font_(font) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
SimpleLabel::~SimpleLabel() {}
