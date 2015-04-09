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

#ifndef __BATTLESHIP_GAME_STATE_HEADER__
#define __BATTLESHIP_GAME_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;
class BattleshipBoard;
class BattleshipManager;

/**
 * This class represents the battleship game state of the game.
 */
class BattleshipGameState : public AbstractState {
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

    BattleshipGameState();
    virtual ~BattleshipGameState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    BattleshipManager * manager_;
    BattleshipBoard * playerBoard_;
    BattleshipBoard * enemyBoard_;

    sf::Vector2i playerPanelPosition_;
    sf::Vector2i infoPanelPosition_;

    std::unique_ptr<Texture2D> playerPanelTexture_;
    std::unique_ptr<Texture2D> infoPanelTexture_;

    std::unique_ptr<SimpleLabel> playerBoardLabel_;
    std::unique_ptr<SimpleLabel> enemyBoardLabel_;
    std::unique_ptr<SimpleLabel> playerLabel_;
    std::unique_ptr<SimpleLabel> turnLabel_;
    std::unique_ptr<SimpleLabel> playerNameLabel_;
    std::unique_ptr<SimpleLabel> playerTurnLabel_;
    std::unique_ptr<SimpleLabel> waterLabel_;
    std::unique_ptr<SimpleLabel> shipLabel_;
    std::unique_ptr<SimpleLabel> hitLabel_;
    std::unique_ptr<SimpleLabel> menuLabel_;

    std::unique_ptr<TexturedButton> menuButton_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void updateHud();
};

#endif
