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

#include "TextUtil.h"
#include <System/BitmapFont.h>
#include <System/ForEach.h>

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Gets the width of a line of text.
 */
int TextUtil::GetLineWidth(BitmapFont * font, const std::string & text) {
    int width = 0;
    ForEach(text, [&](char x) {
        width += font->GetCharWidth(x);
    });
    return width;
}

//--------------------------------------------------------------------------------

/**
 * Gets the height of a line of text.
 */
int TextUtil::GetLineHeight(BitmapFont * font, const std::string & text) {
    int height = 0;
    ForEach(text, [&](char x) {
        auto value = font->GetCharHeight(x);
        if (value > height) height = value;
    });
    return height;
}

//--------------------------------------------------------------------------------

/**
 * Gets the sizes of a line of text.
 */
sf::Vector2i TextUtil::GetLineSizes(BitmapFont * font, const std::string & text) {
    return sf::Vector2i(GetLineWidth(font, text), GetLineHeight(font, text));
}

//--------------------------------------------------------------------------------

/**
 * Gets the sizes of a text (with 1 or more lines).
 */
sf::Vector2i TextUtil::GetTextSizes(BitmapFont * font, const std::string & text) {
    return sf::Vector2i(font->GetTextWidth(text), font->GetTextHeight(text));
}

//--------------------------------------------------------------------------------

/**
 * Gets the final point from a center coordinates of a line of text.
 */
sf::Vector2i TextUtil::GetLinePointFromCenter(BitmapFont * font, const std::string & text,
    const sf::Vector2i & center) {
    return sf::Vector2i(
        center.x - GetLineWidth(font, text) / 2,
        center.y - GetLineHeight(font, text) / 2
    );
}

//--------------------------------------------------------------------------------

/**
 * Gets the final point from a center coordinates of a text (with 1 or more lines).
 */
sf::Vector2i TextUtil::GetTextPointFromCenter(BitmapFont * font, const std::string & text,
    const sf::Vector2i & center) {
    return sf::Vector2i(
        center.x - font->GetTextWidth(text) / 2,
        center.y - font->GetTextHeight(text) / 2
    );
}

//--------------------------------------------------------------------------------

/**
 *  Gets the final coordinate value from a center X coordinate of a line of text.
 */
int TextUtil::GetLineCoordFromCenterX(BitmapFont * font, const std::string & text, int centerX) {
    return centerX - GetLineWidth(font, text) / 2;
}

//--------------------------------------------------------------------------------

/**
 * Gets the final coordinate value from a center Y coordinate of a line of text.
 */
int TextUtil::GetLineCoordFromCenterY(BitmapFont * font, const std::string & text, int centerY) {
    return centerY - GetLineHeight(font, text) / 2;
}

//--------------------------------------------------------------------------------

/**
 * Gets the final coordinate value from a center X coordinate of a text (with 1 or more lines).
 */
int TextUtil::GetTextCoordFromCenterX(BitmapFont * font, const std::string & text, int centerX) {
    return centerX - font->GetTextWidth(text) / 2;
}

//--------------------------------------------------------------------------------

/**
 * Gets the final coordinate value from a center Y coordinate of a text (with 1 or more lines).
 */
int TextUtil::GetTextCoordFromCenterY(BitmapFont * font, const std::string & text, int centerY) {
    return centerY - font->GetTextHeight(text) / 2;
}
