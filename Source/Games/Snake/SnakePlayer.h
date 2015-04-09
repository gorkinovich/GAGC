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

#ifndef __SNAKE_PLAYER_HEADER__
#define __SNAKE_PLAYER_HEADER__

#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>

/**
 * This class represents the player data of the snake game.
 */
class SnakePlayer {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    int Score() { return score_; }

    bool Alive() { return alive_; }

    bool SimpleControl() { return simpleControl_; }
    void SimpleControl(bool value) { simpleControl_ = value; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void SetPlayer1();
    void SetPlayer2();

    void NewGame(int x, int y);
    void Start();
    void UpdateInput();
    void Update();
    void RemoveFromWorld();
    void AddScore(int value);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    SnakePlayer();
    SnakePlayer(const SnakePlayer & source);
    virtual ~SnakePlayer();
    SnakePlayer & operator =(const SnakePlayer & source);

private:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const sf::Vector2i NONE_DIRECTION;
    static const sf::Vector2i NORTH_DIRECTION;
    static const sf::Vector2i SOUTH_DIRECTION;
    static const sf::Vector2i WEST_DIRECTION;
    static const sf::Vector2i EAST_DIRECTION;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    unsigned int score_;             // The score of the last game.
    std::vector<sf::Vector2i> body_; // The body of the snake.
    sf::Vector2i direction_;         // The direction of the snake.
    sf::Vector2i nextDirection_;     // The next direction of the snake.
    bool alive_;                     // The alive flag.
    bool simpleControl_;             // The simple control flag.
    int cellValue_;                  // The cell value in the world.
    sf::Color headColor_;            // The head color.
    sf::Color bodyColor_;            // The body color.

    sf::Keyboard::Key upKey_;        // The up key value.
    sf::Keyboard::Key downKey_;      // The down key value.
    sf::Keyboard::Key leftKey_;      // The left key value.
    sf::Keyboard::Key rightKey_;     // The right key value.
};

#endif
