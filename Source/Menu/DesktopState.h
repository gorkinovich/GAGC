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

#ifndef __DESKTOP_STATE_HEADER__
#define __DESKTOP_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;

/**
 * This class represents the desktop state of the game.
 */
class DesktopState : public AbstractState {
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

    DesktopState();
    virtual ~DesktopState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    sf::Vector2i spartanPosition_;              // The spartan position.
    std::unique_ptr<Texture2D> spartanTexture_; // The spartan texture.

    std::unique_ptr<SimpleLabel> snakeLabel_;       // The snake label.
    std::unique_ptr<SimpleLabel> minesweeperLabel_; // The minesweeper label.
    std::unique_ptr<SimpleLabel> battleshipLabel_;  // The battleship label.
    std::unique_ptr<SimpleLabel> blackjackLabel_;   // The blackjack label.
    std::unique_ptr<SimpleLabel> tictactoeLabel_;   // The tictactoe label.
    std::unique_ptr<SimpleLabel> pongLabel_;        // The pong label.
    std::unique_ptr<SimpleLabel> puzzleLabel_;      // The puzzle label.
    std::unique_ptr<SimpleLabel> alienPartyLabel_;  // The alien party label.
    std::unique_ptr<SimpleLabel> reversiLabel_;     // The reversi label.
    std::unique_ptr<SimpleLabel> chessLabel_;       // The chess label.
    std::unique_ptr<SimpleLabel> checkersLabel_;    // The checkers label.
    std::unique_ptr<SimpleLabel> puckmanLabel_;     // The puckman label.
    std::unique_ptr<SimpleLabel> tetraminoesLabel_; // The tetraminoes label.
    std::unique_ptr<SimpleLabel> aboutLabel_;       // The about label.
    std::unique_ptr<SimpleLabel> exitLabel_;        // The exit label.

    std::unique_ptr<TexturedButton> snakeButton_;       // The snake button.
    std::unique_ptr<TexturedButton> minesweeperButton_; // The minesweeper button.
    std::unique_ptr<TexturedButton> battleshipButton_;  // The battleship button.
    std::unique_ptr<TexturedButton> blackjackButton_;   // The blackjack button.
    std::unique_ptr<TexturedButton> tictactoeButton_;   // The tictactoe button.
    std::unique_ptr<TexturedButton> pongButton_;        // The pong button.
    std::unique_ptr<TexturedButton> puzzleButton_;      // The puzzle button.
    std::unique_ptr<TexturedButton> alienPartyButton_;  // The alien party button.
    std::unique_ptr<TexturedButton> reversiButton_;     // The reversi button.
    std::unique_ptr<TexturedButton> chessButton_;       // The chess button.
    std::unique_ptr<TexturedButton> checkersButton_;    // The checkers button.
    std::unique_ptr<TexturedButton> puckmanButton_;     // The puckman button.
    std::unique_ptr<TexturedButton> tetraminoesButton_; // The tetraminoes button.
    std::unique_ptr<TexturedButton> aboutButton_;       // The about button.
    std::unique_ptr<TexturedButton> exitButton_;        // The exit button.
};

#endif
