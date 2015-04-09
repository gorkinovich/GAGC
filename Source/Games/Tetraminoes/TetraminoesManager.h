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

#ifndef __TETRAMINOES_MANAGER_HEADER__
#define __TETRAMINOES_MANAGER_HEADER__

#include <array>
#include <string>
#include <System/SharedTypes.h>

namespace sf {
    class Time;
}

class Sound;

/**
 * This singleton class represents the tetraminoes game manager.
 */
class TetraminoesManager {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int DIFFICULTY_EASY   = 1;
    static const int DIFFICULTY_NORMAL = 5;
    static const int DIFFICULTY_HARD   = 10;

    static const int MAX_RANKING_ENTRIES = 8;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct RankingEntry {
        std::string Name;
        int Score;
        RankingEntry() : Name("----------------"), Score(0) {}
        RankingEntry(const std::string & name, int score)
            : Name(name), Score(score) {}
    };

    typedef std::array<RankingEntry, MAX_RANKING_ENTRIES> RankingArray;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    SharedTexture & Tileset();
    SharedTexture & Menu1Background();
    SharedTexture & Menu2Background();
    SharedTexture & GameBackground();

    RankingArray & Ranking();
    int Score();

    Sound & KeyboardSound();
    Sound & HitSound();
    Sound & FallSound();
    Sound & LineSound();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    bool IsTopScore(int score);
    void AddScore(const std::string & name, int score);

    // Game
    void StartGame(int difficulty);
    void DrawGame();
    void UpdateGame(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static TetraminoesManager * Instance();
    static TetraminoesManager & Reference();
    ~TetraminoesManager();

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
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static TetraminoesManager * instance_;
    TetraminoesManager();
};

#endif
