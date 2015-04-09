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

#ifndef __PUCKMAN_MAZE_HEADER__
#define __PUCKMAN_MAZE_HEADER__

#include <SFML/Graphics/Rect.hpp>

namespace Puckman {
    class Ghost;
    class Manager;

    /**
     * This class represents the maze of the game.
     */
    class Maze {
    public:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // This is the id of the empty cells in the maze.
        static const int CLEAN = 93;

        // This is the id of the cells with a dot inside.
        static const int DOT = 94;

        // This is the id of the cells with a big dot inside.
        static const int BIG_DOT = 96;

        // The maximum number of dots inside the maze.
        static const int MAXIMUM_DOTS = 244;

        // This is the separation between the sprites and the cells.
        static const int SPRITE_SEP_X = 4;

        // This is the separation between the sprites and the cells.
        static const int SPRITE_SEP_Y = 4;

        // This is start position in the x-axis of Puckman.
        static const int PUCKMAN_START_X = 105;

        // This is start position in the y-axis of Puckman.
        static const int PUCKMAN_START_Y = 204;

        // This is start position in the x-axis of the red ghost.
        static const int RED_GHOST_START_X = 104;

        // This is start position in the y-axis of the red ghost.
        static const int RED_GHOST_START_Y = 108;

        // This is start position in the x-axis of the pink ghost.
        static const int PINK_GHOST_START_X = 104;

        // This is start position in the y-axis of the pink ghost.
        static const int PINK_GHOST_START_Y = 132;

        // This is start position in the x-axis of the blue ghost.
        static const int BLUE_GHOST_START_X = 88;

        // This is start position in the y-axis of the blue ghost.
        static const int BLUE_GHOST_START_Y = 132;

        // This is start position in the x-axis of the brown ghost.
        static const int BROWN_GHOST_START_X = 120;

        // This is start position in the y-axis of the brown ghost.
        static const int BROWN_GHOST_START_Y = 132;

        // This is start position in the x-axis of the fruit.
        static const int FRUIT_START_X = 104;

        // This is start position in the y-axis of the fruit.
        static const int FRUIT_START_Y = 156;

        // This is the start X coordinate of the ghost house area.
        static const int GHOST_HOUSE_START_X = 88;

        // This is the start Y coordinate of the ghost house area.
        static const int GHOST_HOUSE_START_Y = 128;

        // This is the end X coordinate of the ghost house area.
        static const int GHOST_HOUSE_END_X = 120;

        // This is the end Y coordinate of the ghost house area.
        static const int GHOST_HOUSE_END_Y = 136;

        // This is the center Y coordinate of the ghost house area.
        static const int GHOST_HOUSE_CENTER_Y = 132;

        // This is the X coordinate of the ghost door area.
        static const int GHOST_DOOR_X = 104;

        // This is the start Y coordinate of the ghost door area.
        static const int GHOST_DOOR_START_Y = 120;

        // This is the end Y coordinate of the ghost door area.
        static const int GHOST_DOOR_END_Y = 127;

        // This is the limit Y coordinate when the ghost is getting out the house.
        static const int GHOST_DOOR_LIMIT_Y = 108;

        // This is the X coordinate of the ghost start point.
        static const int GHOST_START_X = 104;

        // This is the Y coordinate of the ghost start point.
        static const int GHOST_START_Y = 108;

        static const int ROWS   = 36; // The number of rows in the maze.
        static const int COLUMS = 28; // The number of columns in the maze.

        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        typedef int Table[ROWS][COLUMS];

        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        // The data of the maze.
        Table & Data() { return data_; }

        // This tells to the program if the door is open for the ghosts.
        bool DoorOpen() { return doorOpen_; }
        void DoorOpen(bool value) { doorOpen_ = value; }

        // The number of dots inside the maze.
        int NumberOfDots() { return numberOfDots_; }
        void NumberOfDots(int value) { numberOfDots_ = value; }

        // The number of eated dots inside the maze.
        int NumberOfEatedDots() { return MAXIMUM_DOTS - numberOfDots_; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        static sf::Vector2i SpriteCoordsToMaze(int x, int y);
        static bool IsGhostInsideTheHouse(const Ghost & ghost);
        static bool IsGhostInTheStartPoint(const Ghost & ghost);

        void Initialize();
        void Draw(Manager * manager);
        bool CheckSpriteCoords(int x, int y);
        bool CheckGhostCoords(int x, int y);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        Maze();
        Maze(const Maze & source);
        virtual ~Maze();

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        Table data_;
        bool doorOpen_;
        int numberOfDots_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        bool isCellWalkable(int row, int col);
    };
}

#endif
