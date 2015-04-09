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

#ifndef __SIMPLE_LABEL_HEADER__
#define __SIMPLE_LABEL_HEADER__

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

class BitmapFont;

/**
 * This class represents a simple text label.
 */
class SimpleLabel {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    const std::string & Text() const { return text_; }
    void Text(const std::string & value) { text_ = value; }

    const sf::Vector2i & Position() const { return position_; }
    void Position(const sf::Vector2i & value) { position_ = value; }

    const sf::Color & Color() const { return color_; }
    void Color(const sf::Color & value) { color_ = value; }

    BitmapFont * Font() { return font_; }
    void Font(BitmapFont * value) { font_ = value; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Draw();

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    SimpleLabel();
    SimpleLabel(const std::string & text, int x, int y,
        const sf::Color & color, BitmapFont * font);
    SimpleLabel(const std::string & text, const sf::Vector2i & position,
        const sf::Color & color, BitmapFont * font);
    virtual ~SimpleLabel();

protected:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    std::string text_;      // The text of the label.
    sf::Vector2i position_; // The coordinates of the label.
    sf::Color color_;       // The color of the label.
    BitmapFont * font_;     // The font of the label.

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    virtual void updateCoords(char item, sf::Vector2i & coords);
};

#endif
