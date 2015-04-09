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

#ifndef __CHECKERS_CONFIG_GAME_STATE_HEADER__
#define __CHECKERS_CONFIG_GAME_STATE_HEADER__

#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;

/**
 * This class represents the checkers config one player new game state of the game.
 */
class CheckersConfigGameState : public AbstractState {
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

    CheckersConfigGameState();
    virtual ~CheckersConfigGameState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int side_;
    int difficulty_;

    std::unique_ptr<Texture2D> selectedTexture_;

    std::unique_ptr<SimpleLabel> chooseSideLabel_;
    std::unique_ptr<SimpleLabel> whiteSideLabel_;
    std::unique_ptr<SimpleLabel> blackSideLabel_;
    std::unique_ptr<SimpleLabel> chooseDifficultyLabel_;
    std::unique_ptr<SimpleLabel> easyLabel_;
    std::unique_ptr<SimpleLabel> normalLabel_;
    std::unique_ptr<SimpleLabel> hardLabel_;
    std::unique_ptr<SimpleLabel> startLabel_;
    std::unique_ptr<SimpleLabel> cancelLabel_;

    std::unique_ptr<TexturedButton> whiteSideButton_;
    std::unique_ptr<TexturedButton> blackSideButton_;
    std::unique_ptr<TexturedButton> easyButton_;
    std::unique_ptr<TexturedButton> normalButton_;
    std::unique_ptr<TexturedButton> hardButton_;
    std::unique_ptr<TexturedButton> startButton_;
    std::unique_ptr<TexturedButton> cancelButton_;
};

#endif
