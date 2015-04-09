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

#ifndef __SNAKE_MANAGER_HEADER__
#define __SNAKE_MANAGER_HEADER__

#include <array>
#include <string>
#include <Games/Snake/SnakePlayer.h>

namespace sf {
    class Time;
}

class Texture2D;
class Sound;

/**
 * This singleton class represents the snake game manager.
 */
class SnakeManager {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int MAX_PLAYERS         = 2;
    static const int MAX_RANKING_ENTRIES = 8;

    static const int DIFFICULTY_EASY   = 0;
    static const int DIFFICULTY_NORMAL = 1;
    static const int DIFFICULTY_HARD   = 2;

    static const int MAX_ROWS = 32;
    static const int MAX_COLS = 62;

    static const int EMTPY_CELL   =  0;
    static const int WALL_CELL    =  4;
    static const int PLAYER1_CELL =  8;
    static const int PLAYER2_CELL = 15;
    static const int DOT1_CELL    = 16;
    static const int DOT2_CELL    = 24;
    static const int TOWEL_CELL   = 42;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct Cell {
        int Value;
        sf::Color Color;
        Cell() : Value(EMTPY_CELL), Color(sf::Color::White) {}
        Cell(int v, const sf::Color & c) : Value(v), Color(c) {}
    };

    struct RankingEntry {
        std::string Name;
        int Score;
        RankingEntry() : Name("--------"), Score(0) {}
        RankingEntry(const std::string & name, int score)
            : Name(name), Score(score) {}
    };

    typedef std::array<SnakePlayer, MAX_PLAYERS> PlayersArray;
    typedef std::array<RankingEntry, MAX_RANKING_ENTRIES> RankingArray;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    Texture2D & SquareTexture();
    int NumberOfPlayers();
    PlayersArray & Players();
    RankingArray & Ranking();

    Cell & World(int x, int y);
    void World(int x, int y, int value);
    void World(int x, int y, int value, const sf::Color & color);

    Sound & MenuUpSound();
    Sound & MenuDownSound();
    Sound & MenuKeySound();
    Sound & MenuNextSound();

    Sound & DeathSound();
    Sound & EatDot1Sound();
    Sound & EatDot2Sound();
    Sound & DotSpawnSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    void DrawBorder(const sf::Color & borderColor);

    void ExitGameOver();

    bool IsTopScore(int score);
    void AddScore(const std::string & name, int score);

    // Game
    void StartGame(int difficulty, bool simpleControl);
    void StartGame(int difficulty, bool simpleControl1, bool simpleControl2);

    void DrawGame();
    void UpdateGame(const sf::Time & timeDelta);

    void Kill(int cellValue, SnakePlayer * player);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static SnakeManager * Instance();
    static SnakeManager & Reference();
    ~SnakeManager();

private:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct InnerData;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    bool initialized_;
    std::unique_ptr<InnerData> data_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    // Game
    void startGame(int difficulty, int numberOfPlayers);
    bool checkGameOver();
    void generateDotItem();
    void generateDot2Item();
    sf::Vector2i getEmptyCell();
    void dot2ToHideState();
    void dot2ToShowState();
    void dot2Update(const sf::Time & timeDelta);
    void changeSubstate(int value);
    void updateHud();

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static SnakeManager * instance_;
    SnakeManager();
};

#endif
