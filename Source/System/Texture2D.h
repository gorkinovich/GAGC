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

#ifndef __TEXTURE_2D_HEADER__
#define __TEXTURE_2D_HEADER__

#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <System/SharedTypes.h>

/**
 * This class represents a 2D texture surface.
 */
class Texture2D {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    int Width() const;
    int Height() const;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    bool Load(int width, int height, const sf::Color & color = sf::Color::White);
    bool Load(const std::string & path, bool smooth = false);
    bool Load(SharedTexture & texture);
    bool Load(SharedTexture & texture, const sf::IntRect & sourceArea);
    void Unload();
    void Draw(int x, int y);
    void Draw(const sf::Vector2i & position);
    void Draw(int x, int y, const sf::Color & color);
    void Draw(const sf::Vector2i & position, const sf::Color & color);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    Texture2D();
    Texture2D(const Texture2D & source);
    ~Texture2D();
    Texture2D & operator =(const Texture2D & source);

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    SharedTexture texture_; // The texture of the surface.
    SharedSprite sprite_;   // The sprite of the surface.

    //--------------------------------------------------------------------------------
    // Static
    //--------------------------------------------------------------------------------

    static SharedRenderWindow renderWindow_;
};

#endif
