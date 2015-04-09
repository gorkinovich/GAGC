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

#ifndef __PUCKMAN_PLAYER_HEADER__
#define __PUCKMAN_PLAYER_HEADER__

#include <Games/Puckman/PuckmanMaze.h>
#include <Games/Puckman/PuckmanPlayerPuckman.h>

namespace Puckman {
    /**
     * This class represents the player data.
     */
    class Player {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        int Score() { return score_; }
        void Score(int value) { score_ = value; }

        int Level() { return level_; }
        void Level(int value) { level_ = value; }

        Maze & MazeRef() { return maze_; }
        Maze * MazePtr() { return &maze_; }
        PlayerPuckman & PuckmanRef() { return puckman_; }

        int ExtraLives() { return extraLives_; }
        void ExtraLives(int value);

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void NewGame();
        void NextLevel();

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        Player();
        ~Player();

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        int score_;
        int level_;
        Maze maze_;
        PlayerPuckman puckman_;
        int extraLives_;
    };
}

#endif
