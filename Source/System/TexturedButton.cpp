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

#include "TexturedButton.h"
#include <System/CoreManager.h>
#include <System/MathUtil.h>
#include <System/SimpleLabel.h>
#include <System/BitmapFont.h>

//********************************************************************************
// Properties
//********************************************************************************

void TexturedButton::LinkedLabel(SimpleLabel * label) {
    linkedLabel_.Pointer = label;
    if (label != nullptr) {
        auto & coords = label->Position();
        auto * font = label->Font();
        linkedLabel_.Area.left = coords.x;
        linkedLabel_.Area.top = coords.y;
        linkedLabel_.Area.width = font->GetTextWidth(label->Text());
        linkedLabel_.Area.height = font->GetTextHeight(label->Text());
    }
}

//--------------------------------------------------------------------------------

void TexturedButton::LinkedLabel(SimpleLabel * label, int width, int height) {
    linkedLabel_.Pointer = label;
    if (label != nullptr) {
        auto & coords = label->Position();
        linkedLabel_.Area.left = coords.x;
        linkedLabel_.Area.top = coords.y;
        linkedLabel_.Area.width = width;
        linkedLabel_.Area.height = height;
    }
}

//--------------------------------------------------------------------------------

void TexturedButton::LinkedLabel(SimpleLabel * label, int x, int y, int width, int height) {
    linkedLabel_.Pointer = label;
    if (label != nullptr) {
        linkedLabel_.Area.left = x;
        linkedLabel_.Area.top = y;
        linkedLabel_.Area.width = width;
        linkedLabel_.Area.height = height;
    }
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the button.
 */
bool TexturedButton::Initialize(int x, int y, const std::string & texturePath,
    const std::string & clickTexturePath, const std::string & hoverTexturePath) {
    if (!texture_.Load(texturePath) || !clickTexture_.Load(clickTexturePath) ||
        !hoverTexture_.Load(hoverTexturePath)) return false;
    initialize(x, y, texture_.Width(), texture_.Height());
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Initializes the button.
 */
bool TexturedButton::Initialize(int x, int y, SharedTexture & texture,
    const sf::IntRect & textureRect, const sf::IntRect & clickTextureRect,
    const sf::IntRect & hoverTextureRect) {
    if (!texture_.Load(texture, textureRect) ||
        !clickTexture_.Load(texture, clickTextureRect) ||
        !hoverTexture_.Load(texture, hoverTextureRect)) return false;
    initialize(x, y, textureRect.width, textureRect.height);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Initializes the button.
 */
void TexturedButton::initialize(int x, int y, int width, int height) {
    destinationArea_.left = x;
    destinationArea_.top = y;
    destinationArea_.width = width;
    destinationArea_.height = height;
    state_ = TexturedButton::Normal;
    previousLeftButton_ = CoreManager::Instance()->IsButtonPressed(sf::Mouse::Left);
}

//--------------------------------------------------------------------------------

/**
 * Draws the button on the screen.
 */
void TexturedButton::Draw() {
    switch (state_) {
        case TexturedButton::Normal:
            texture_.Draw(destinationArea_.left, destinationArea_.top);
            break;

        case TexturedButton::Hover:
            clickTexture_.Draw(destinationArea_.left, destinationArea_.top);
            break;

        case TexturedButton::Clicked:
            hoverTexture_.Draw(destinationArea_.left, destinationArea_.top);
            break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the logic of the button.
 */
void TexturedButton::Update() {
    auto * core =  CoreManager::Instance();
    auto & mouseCoords = core->GetMousePosition();
    if (MathUtil::PointInside(mouseCoords, destinationArea_) ||
        (linkedLabel_.Pointer && MathUtil::PointInside(mouseCoords, linkedLabel_.Area))) {
        bool currentLeftButton = core->IsButtonPressed(sf::Mouse::Left);
        state_ = currentLeftButton ? TexturedButton::Clicked : TexturedButton::Hover;
        if (!currentLeftButton && previousLeftButton_  && onClick_) {
            onClick_(*this);
        }
    } else {
        state_ = TexturedButton::Normal;
    }
    previousLeftButton_ = core->IsButtonPressed(sf::Mouse::Left);
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

/**
 * Constructs a new object.
 */
TexturedButton::TexturedButton() : state_(TexturedButton::Normal),
    previousLeftButton_(false), destinationArea_(), onClick_(), linkedLabel_() {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
TexturedButton::~TexturedButton() {
    texture_.Unload();
    clickTexture_.Unload();
    hoverTexture_.Unload();
}
