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

#ifndef __REVERSI_GAME_STATE_HEADER__
#define __REVERSI_GAME_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;
class ReversiManager;

/**
 * This class represents the reversi game state of the game.
 */
class ReversiGameState : public AbstractState {
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

    ReversiGameState();
    virtual ~ReversiGameState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    ReversiManager * manager_;

    sf::Vector2i boardPosition_;
    std::unique_ptr<Texture2D> boardTexture_;

    std::unique_ptr<SimpleLabel> resetLabel_;
    std::unique_ptr<SimpleLabel> saveLabel_;
    std::unique_ptr<SimpleLabel> loadLabel_;
    std::unique_ptr<SimpleLabel> passLabel_;
    std::unique_ptr<SimpleLabel> exitLabel_;

    std::unique_ptr<TexturedButton> resetButton_;
    std::unique_ptr<TexturedButton> saveButton_;
    std::unique_ptr<TexturedButton> loadButton_;
    std::unique_ptr<TexturedButton> passButton_;
    std::unique_ptr<TexturedButton> exitButton_;
};

#endif
