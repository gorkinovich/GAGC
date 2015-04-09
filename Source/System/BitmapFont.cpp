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

#include "BitmapFont.h"
#include <SFML/Graphics.hpp>
#include <System/CoreManager.h>
#include <System/ForEach.h>

//********************************************************************************
// Static
//********************************************************************************

SharedRenderWindow BitmapFont::renderWindow_ = nullptr;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Loads a font from a file.
 */
bool BitmapFont::Load(const std::string & path, int texRows, int texCols) {
    renderWindow_ = CoreManager::Instance()->window_;

    // Load the texture from the file.
    texture_ = std::make_shared<sf::Texture>();
    if (!texture_->loadFromFile(path)) {
        Unload();
        return false;
    }
    texture_->setSmooth(false);

    // Create the sprite of the font.
    sprite_ = std::make_shared<sf::Sprite>(*texture_);

    // Get the sizes of the texture.
    auto size = texture_->getSize();
    width_ = size.x / texCols;
    height_ = size.y / texRows;

    // Create the source rectangles.
    sources_.clear();
    sources_.reserve(texRows * texCols);
    for (int i = 0; i < texRows; ++i) {
        for (int j = 0; j < texCols; ++j) {
            sources_.push_back(sf::IntRect(j * width_, i * height_, width_, height_));
        }
    }

    return true;
}

//--------------------------------------------------------------------------------

/**
 * Unloads the current font.
 */
void BitmapFont::Unload() {
    width_ = 0;
    height_ = 0;
    sources_.clear();
    sprite_ = nullptr;
    texture_ = nullptr;
}

//--------------------------------------------------------------------------------

/**
 * Gets the width of a character.
 */
int BitmapFont::GetCharWidth(char item) {
    return width_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the height of a character.
 */
int BitmapFont::GetCharHeight(char item) {
    return height_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the width of a text.
 */
int BitmapFont::GetTextWidth(std::string text) {
    if (text.size() > 0) {
        int maxLen = 0, len = 0;
        ForEach(text, [&](char x) {
            if (x == '\n') {
                if (maxLen < len) {
                    maxLen = len;
                }
                len = 0;
            } else {
                ++len;
            }
        });
        if (maxLen < len) {
            maxLen = len;
        }
        return maxLen * width_;
    } else {
        return 0;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets the height of a text.
 */
int BitmapFont::GetTextHeight(std::string text) {
    if (text.size() > 0) {
        int maxLen = 1;
        ForEach(text, [&](char x) {
            if (x == '\n') {
                ++maxLen;
            }
        });
        return maxLen * height_;
    } else {
        return 0;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws a character on the screen.
 */
void BitmapFont::Draw(char item, int x, int y, const sf::Color & color) {
    draw(item, x, y, color);
}

//--------------------------------------------------------------------------------

/**
 * Draws a character on the screen.
 */
void BitmapFont::Draw(const std::string & text, int x, int y, const sf::Color & color) {
    int cx = x, cy = y;
    for (int i = 0, len = text.size(); i < len; ++i) {
        auto item = text[i];
        if (item == '\n') {
            cx = x;
            cy += height_;
        } else {
            draw(item, cx, cy, color);
            cx += width_;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws a character on the screen.
 */
void BitmapFont::draw(char item, int x, int y, const sf::Color & color) {
    unsigned int index = static_cast<unsigned int>(item) & 0xFF;
    if (0 <= index && index < sources_.size()) {
        sprite_->setColor(color);
        sprite_->setPosition((float)x, (float)y);
        sprite_->setTextureRect(sources_[index]);
        renderWindow_->draw(*sprite_);
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
BitmapFont::BitmapFont() : texture_(nullptr), sprite_(nullptr), width_(0),
    height_(0), sources_() {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
BitmapFont::BitmapFont(const BitmapFont & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
BitmapFont::~BitmapFont() {
    Unload();
}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
BitmapFont & BitmapFont::operator =(const BitmapFont & source) {
    texture_ = source.texture_;
    sprite_ = source.sprite_;
    width_ = source.width_;
    height_ = source.height_;
    sources_ = source.sources_;
    return *this;
}
