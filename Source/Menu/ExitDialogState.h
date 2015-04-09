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

#ifndef __EXIT_DIALOG_STATE_HEADER__
#define __EXIT_DIALOG_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class TextLabel;
class SimpleLabel;
class TexturedButton;

/**
 * This class represents the exit dialog state of the game.
 */
class ExitDialogState : public AbstractState {
public:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    enum PreviousState {
        LOGON_STATE,
        DESKTOP_STATE
    };

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    virtual void Initialize();
    virtual void Release();
    virtual void Draw(const sf::Time & timeDelta);
    virtual void Update(const sf::Time & timeDelta);

    static void ChangeStateFrom(PreviousState state);

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    ExitDialogState();
    virtual ~ExitDialogState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    static PreviousState previousState_;            // The previous state caller.
    sf::Vector2i panelPosition_;                    // The panel background position.
    std::unique_ptr<Texture2D> panelBackground_;    // The panel background texture.
    std::unique_ptr<TextLabel> messageLabel_;       // The message label.
    std::unique_ptr<SimpleLabel> yesLabel_;         // The yes label.
    std::unique_ptr<TexturedButton> yesButton_;     // The yes button.
    std::unique_ptr<SimpleLabel> noLabel_;          // The no label.
    std::unique_ptr<TexturedButton> noButton_;      // The no button.
};

#endif
