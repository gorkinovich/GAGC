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

#ifndef __BATTLESHIP_PUT_SHIPS_STATE_HEADER__
#define __BATTLESHIP_PUT_SHIPS_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class TextLabel;
class SimpleLabel;
class TexturedButton;
class BattleshipBoard;

/**
 * This class represents the battleship put ships state of the game.
 */
class BattleshipPutShipsState : public AbstractState {
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

    BattleshipPutShipsState();
    virtual ~BattleshipPutShipsState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    std::unique_ptr<BattleshipBoard> board_;

    sf::IntRect ship1Area_;
    sf::IntRect ship2Area_;
    sf::IntRect ship3Area_;
    sf::IntRect ship4Area_;

    std::unique_ptr<Texture2D> ship1Texture_;
    std::unique_ptr<Texture2D> ship2Texture_;
    std::unique_ptr<Texture2D> ship3Texture_;
    std::unique_ptr<Texture2D> ship4Texture_;

    std::unique_ptr<SimpleLabel> titleLabel_;
    std::unique_ptr<SimpleLabel> ship1CountLabel_;
    std::unique_ptr<SimpleLabel> ship2CountLabel_;
    std::unique_ptr<SimpleLabel> ship3CountLabel_;
    std::unique_ptr<SimpleLabel> ship4CountLabel_;
    std::unique_ptr<SimpleLabel> automaticLabel_;
    std::unique_ptr<SimpleLabel> resetLabel_;

    std::unique_ptr<TextLabel> helpText_;

    std::unique_ptr<TexturedButton> automaticButton_;
    std::unique_ptr<TexturedButton> resetButton_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void updateHud();
};

#endif
