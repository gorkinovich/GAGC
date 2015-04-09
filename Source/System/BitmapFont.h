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

#ifndef __BITMAP_FONT_HEADER__
#define __BITMAP_FONT_HEADER__

#include <vector>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <System/SharedTypes.h>

/**
 * This class represents a bitmap font.
 */
class BitmapFont {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int DEFAULT_TEX_ROWS = 16;
    static const int DEFAULT_TEX_COLS = 16;
    static const int DEFAULT_MAX_CHARS = 256;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    int Width() const { return width_; }
    int Height() const { return height_; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    bool Load(const std::string & path, int texRows = DEFAULT_TEX_ROWS, int texCols = DEFAULT_TEX_COLS);
    void Unload();
    int GetCharWidth(char item);
    int GetCharHeight(char item);
    int GetTextWidth(std::string text);
    int GetTextHeight(std::string text);
    void Draw(char item, int x, int y, const sf::Color & color = sf::Color::White);
    void Draw(const std::string & text, int x, int y, const sf::Color & color = sf::Color::White);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    BitmapFont();
    BitmapFont(const BitmapFont & source);
    ~BitmapFont();
    BitmapFont & operator =(const BitmapFont & source);

private:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    typedef std::vector<sf::IntRect> IntRectVector;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    SharedTexture texture_; // The texture.
    SharedSprite sprite_;   // The sprite.
    int width_;             // The width.
    int height_;            // The height.
    IntRectVector sources_; // The source rectangles.

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void draw(char item, int x, int y, const sf::Color & color);

    //--------------------------------------------------------------------------------
    // Static
    //--------------------------------------------------------------------------------

    static SharedRenderWindow renderWindow_;
};

#endif
