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

#include "Texture2D.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <System/CoreManager.h>

//********************************************************************************
// Static
//********************************************************************************

SharedRenderWindow Texture2D::renderWindow_ = nullptr;

//********************************************************************************
// Properties
//********************************************************************************

/**
 * Gets the width of the surface.
 */
int Texture2D::Width() const {
    return texture_ ? sprite_->getTextureRect().width : 0;
}

//--------------------------------------------------------------------------------

/**
 * Gets the height of the surface.
 */
int Texture2D::Height() const {
    return texture_ ? sprite_->getTextureRect().height : 0;
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Creates a surface from a color.
 */
bool Texture2D::Load(int width, int height, const sf::Color & color) {
    auto * core = CoreManager::Instance();
    renderWindow_ = core->window_;
    Unload();

    sf::Image image;
    image.create(width, height, color);
    texture_ = std::make_shared<sf::Texture>();
    if (!texture_->loadFromImage(image)) return false;
    sprite_ = std::make_shared<sf::Sprite>(*texture_);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Loads a surface from a file.
 */
bool Texture2D::Load(const std::string & path, bool smooth) {
    auto * core = CoreManager::Instance();
    renderWindow_ = core->window_;
    Unload();

    texture_ = core->LoadTexture(path, smooth);
    if (!texture_) return false;
    sprite_ = std::make_shared<sf::Sprite>(*texture_);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Loads a surface from the memory.
 */
bool Texture2D::Load(SharedTexture & texture) {
    renderWindow_ = CoreManager::Instance()->window_;
    Unload();

    texture_ = texture;
    sprite_ = std::make_shared<sf::Sprite>(*texture_);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Loads a surface from the memory.
 */
bool Texture2D::Load(SharedTexture & texture, const sf::IntRect & sourceArea) {
    renderWindow_ = CoreManager::Instance()->window_;
    Unload();

    texture_ = texture;
    sprite_ = std::make_shared<sf::Sprite>(*texture_, sourceArea);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Unloads the current surface.
 */
void Texture2D::Unload() {
    sprite_ = nullptr;
    texture_ = nullptr;
}

//--------------------------------------------------------------------------------

/**
 * Draws a surface on the screen.
 */
void Texture2D::Draw(int x, int y) {
    Draw(x, y, sf::Color::White);
}

//--------------------------------------------------------------------------------

/**
 * Draws a surface on the screen.
 */
void Texture2D::Draw(const sf::Vector2i & position) {
    Draw(position.x, position.y, sf::Color::White);
}

//--------------------------------------------------------------------------------

/**
 * Draws a surface on the screen.
 */
void Texture2D::Draw(int x, int y, const sf::Color & color) {
    sprite_->setColor(color);
    sprite_->setPosition((float)x, (float)y);
    renderWindow_->draw(*sprite_);
}

//--------------------------------------------------------------------------------

/**
 * Draws a surface on the screen.
 */
void Texture2D::Draw(const sf::Vector2i & position, const sf::Color & color) {
    Draw(position.x, position.y, color);
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Texture2D::Texture2D() : texture_(nullptr), sprite_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
Texture2D::Texture2D(const Texture2D & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Texture2D::~Texture2D() {
    Unload();
}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
Texture2D & Texture2D::operator =(const Texture2D & source) {
    texture_ = source.texture_;
    sprite_ = source.sprite_;
    return *this;
}
