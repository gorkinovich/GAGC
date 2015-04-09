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

#ifndef __SNAKE_NEW_GAME_STATE_HEADER__
#define __SNAKE_NEW_GAME_STATE_HEADER__

#include <System/AbstractState.h>

class SimpleLabel;

/**
 * This class represents the snake new game state of the game.
 */
class SnakeNewGameState : public AbstractState {
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

    SnakeNewGameState();
    virtual ~SnakeNewGameState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int substate_;
    int difficulty_;
    int numberOfPlayers_;
    bool simpleControlPlayer1_;
    bool simpleControlPlayer2_;
    std::unique_ptr<SimpleLabel> titleLabel_;
    std::unique_ptr<SimpleLabel> difficultyLabel_;
    std::unique_ptr<SimpleLabel> easyLabel_;
    std::unique_ptr<SimpleLabel> normalLabel_;
    std::unique_ptr<SimpleLabel> hardLabel_;
    std::unique_ptr<SimpleLabel> numberOfPlayersLabel_;
    std::unique_ptr<SimpleLabel> onePlayerLabel_;
    std::unique_ptr<SimpleLabel> twoPlayersLabel_;
    std::unique_ptr<SimpleLabel> controlTypePlayer1Label_;
    std::unique_ptr<SimpleLabel> standard1Label_;
    std::unique_ptr<SimpleLabel> simple1Label_;
    std::unique_ptr<SimpleLabel> controlTypePlayer2Label_;
    std::unique_ptr<SimpleLabel> standard2Label_;
    std::unique_ptr<SimpleLabel> simple2Label_;
    std::unique_ptr<SimpleLabel> startGameLabel_;
    std::unique_ptr<SimpleLabel> exitLabel_;
    std::unique_ptr<SimpleLabel> cancelLabel_;
};

#endif
