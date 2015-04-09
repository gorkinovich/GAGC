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

#ifndef __BLACKJACK_MENU_STATE_HEADER__
#define __BLACKJACK_MENU_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;

/**
 * This class represents the blackjack menu state of the game.
 */
class BlackjackMenuState : public AbstractState {
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

    BlackjackMenuState();
    virtual ~BlackjackMenuState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    sf::Vector2i titlePosition_;
    sf::Vector2i spadePosition_;
    sf::Vector2i heartPosition_;
    sf::Vector2i clubPosition_;
    sf::Vector2i diamondPosition_;

    std::unique_ptr<Texture2D> titleTexture_;
    std::unique_ptr<Texture2D> spadeTexture_;
    std::unique_ptr<Texture2D> heartTexture_;
    std::unique_ptr<Texture2D> clubTexture_;
    std::unique_ptr<Texture2D> diamondTexture_;

    std::unique_ptr<SimpleLabel> newGameLabel_;
    std::unique_ptr<SimpleLabel> helpLabel_;
    std::unique_ptr<SimpleLabel> creditsLabel_;
    std::unique_ptr<SimpleLabel> exitLabel_;

    std::unique_ptr<TexturedButton> newGameButton_;
    std::unique_ptr<TexturedButton> helpButton_;
    std::unique_ptr<TexturedButton> creditsButton_;
    std::unique_ptr<TexturedButton> exitButton_;
};

#endif
