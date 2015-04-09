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

#ifndef __TEXT_UTIL_HEADER__
#define __TEXT_UTIL_HEADER__

#include <string>
#include <SFML/Graphics/Rect.hpp>

class BitmapFont;

/**
 * This static class is a collection of text utility functions.
 */
class TextUtil {
private:
    TextUtil() {}
    ~TextUtil() {}

public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    static int GetLineWidth(BitmapFont * font, const std::string & text);
    static int GetLineHeight(BitmapFont * font, const std::string & text);
    static sf::Vector2i GetLineSizes(BitmapFont * font, const std::string & text);
    static sf::Vector2i GetTextSizes(BitmapFont * font, const std::string & text);

    static sf::Vector2i GetLinePointFromCenter(BitmapFont * font, const std::string & text,
        const sf::Vector2i & center);
    static sf::Vector2i GetTextPointFromCenter(BitmapFont * font, const std::string & text,
        const sf::Vector2i & center);

    static int GetLineCoordFromCenterX(BitmapFont * font, const std::string & text, int centerX);
    static int GetLineCoordFromCenterY(BitmapFont * font, const std::string & text, int centerY);

    static int GetTextCoordFromCenterX(BitmapFont * font, const std::string & text, int centerX);
    static int GetTextCoordFromCenterY(BitmapFont * font, const std::string & text, int centerY);
};

#endif
