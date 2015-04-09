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

#ifndef __LOGON_STATE_HEADER__
#define __LOGON_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;

/**
 * This class represents the logon state of the game.
 */
class LogonState : public AbstractState {
public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    virtual void Initialize();
    virtual void Release();
    virtual void Draw(const sf::Time & timeDelta);
    virtual void Update(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    LogonState();
    virtual ~LogonState();

private:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const float MAX_RADIUS;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    std::unique_ptr<Texture2D> eyesBackground_;          // The texture of the eyes background.
    std::unique_ptr<Texture2D> eye_;                     // The texture of an eye.

    sf::Vector2i eyesBackgroundDestination_;             // The eyes background destination point.
    sf::Vector2i leftEyeBase_;                           // The left eye base destination point.
    sf::Vector2i rightEyeBase_;                          // The right eye base destination point.
    sf::Vector2i leftEyeDestination_;                    // The left eye destination point.
    sf::Vector2i rightEyeDestination_;                   // The right eye destination point.

    std::unique_ptr<SimpleLabel> welcomeLabel_;          // The welcome label.
    std::unique_ptr<SimpleLabel> userLabel_;             // The user label.
    std::unique_ptr<SimpleLabel> startSessionLabel_;     // The start session label.
    std::unique_ptr<SimpleLabel> exitLabel_;             // The exit label.

    std::unique_ptr<TexturedButton> avatarButton_;       // The avatar button.
    std::unique_ptr<TexturedButton> startSessionButton_; // The start session button.
    std::unique_ptr<TexturedButton> exitButton_;         // The exit button.
};

#endif
