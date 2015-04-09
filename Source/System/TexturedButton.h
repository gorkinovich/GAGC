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

#ifndef __TEXTURED_BUTTON_HEADER__
#define __TEXTURED_BUTTON_HEADER__

#include <functional>
#include <SFML/Graphics/Rect.hpp>
#include <System/Texture2D.h>

class SimpleLabel;

/**
 * This class represents a textured button. This button has three textures,
 * each one used with the current inner state in the button.
 */
class TexturedButton {
public:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    typedef std::function<void (TexturedButton &)> OnClickAction;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    int X() const { return destinationArea_.left; }
    void X(int value) { destinationArea_.left = value; }

    int Y() const { return destinationArea_.top; }
    void Y(int value) { destinationArea_.top = value; }

    void OnClick(OnClickAction value) { onClick_ = value; }

    void LinkedLabel(SimpleLabel * label);
    void LinkedLabel(SimpleLabel * label, int width, int height);
    void LinkedLabel(SimpleLabel * label, int x, int y, int width, int height);

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    bool Initialize(int x, int y, const std::string & texturePath,
        const std::string & clickTexturePath, const std::string & hoverTexturePath);
    bool TexturedButton::Initialize(int x, int y, SharedTexture & texture,
        const sf::IntRect & textureRect, const sf::IntRect & clickTextureRect,
        const sf::IntRect & hoverTextureRect);

    void Draw();
    void Update();

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    TexturedButton();
    virtual ~TexturedButton();

private:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    enum ButtonInnerState {
        Normal, Hover, Clicked
    };

    struct LinkedLabelData {
        SimpleLabel * Pointer;
        sf::IntRect Area;
        LinkedLabelData() : Pointer(nullptr), Area() {}
    };

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    ButtonInnerState state_;        // The inner state.
    bool previousLeftButton_;       // The previous mouse left button value.
    Texture2D texture_;             // The main texture.
    Texture2D clickTexture_;        // The "on click" texture.
    Texture2D hoverTexture_;        // The "on hover" texture.
    sf::IntRect destinationArea_;   // The destination rectangle area.
    OnClickAction onClick_;         // The on click callback.
    LinkedLabelData linkedLabel_;   // The label attached to the button.

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void initialize(int x, int y, int width, int height);
};

#endif
