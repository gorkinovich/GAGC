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

#include "PuckmanMaze.h"
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanGhost.h>
#include <Games/Puckman/PuckmanSprites.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Transforms a sprite coordinates to the maze coordinates system.
 */
sf::Vector2i Maze::SpriteCoordsToMaze(int x, int y) {
    return sf::Vector2i(
        (x + SPRITE_SEP_X) / Manager::CELL_WIDTH,
        (y + SPRITE_SEP_Y) / Manager::CELL_HEIGHT
    );
}

//--------------------------------------------------------------------------------

/**
 * Checks if a ghost is inside the house of the maze.
 */
bool Maze::IsGhostInsideTheHouse(const Ghost & ghost) {
    return GHOST_HOUSE_START_X <= ghost.X && ghost.X <= GHOST_HOUSE_END_X &&
           GHOST_HOUSE_START_Y <= ghost.Y && ghost.Y <= GHOST_HOUSE_END_Y;
}

//--------------------------------------------------------------------------------

/**
 * Checks if a ghost is in the start point of the maze.
 */
bool Maze::IsGhostInTheStartPoint(const Ghost & ghost) {
    return GHOST_START_X == ghost.X && ghost.Y == GHOST_START_Y;
}

//--------------------------------------------------------------------------------

/**
 * Initializes the maze.
 */
void Maze::Initialize() {
    const int INITIAL_DATA[ROWS][COLUMS] = {
        { 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93},
        { 97,101,101,101,101,101,101,101,101,101,101,101,101,109,110,101,101,101,101,101,101,101,101,101,101,101,101, 98},
        {104, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,128,130, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,102},
        {104, 94,125,129,129,126, 94,125,129,129,129,126, 94,128,130, 94,125,129,129,129,126, 94,125,129,129,126, 94,102},
        {104, 96,128, 93, 93,130, 94,128, 93, 93, 93,130, 94,128,130, 94,128, 93, 93, 93,130, 94,128, 93, 93,130, 96,102},
        {104, 94,124,127,127,123, 94,124,127,127,127,123, 94,124,123, 94,124,127,127,127,123, 94,124,127,127,123, 94,102},
        {104, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,102},
        {104, 94,125,129,129,126, 94,125,126, 94,125,129,129,129,129,129,129,126, 94,125,126, 94,125,129,129,126, 94,102},
        {104, 94,124,127,127,123, 94,128,130, 94,124,127,127,132,131,127,127,123, 94,128,130, 94,124,127,127,123, 94,102},
        {104, 94, 94, 94, 94, 94, 94,128,130, 94, 94, 94, 94,128,130, 94, 94, 94, 94,128,130, 94, 94, 94, 94, 94, 94,102},
        {100,103,103,103,103,120, 94,128,133,129,129,126, 93,128,130, 93,125,129,129,134,130, 94,119,103,103,103,103, 99},
        { 93, 93, 93, 93, 93,104, 94,128,131,127,127,123, 93,124,123, 93,124,127,127,132,130, 94,102, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93,104, 94,128,130, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,128,130, 94,102, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93,104, 94,128,130, 93,113,103,115,121,122,116,103,114, 93,128,130, 94,102, 93, 93, 93, 93, 93},
        {101,101,101,101,101,117, 94,124,123, 93,102, 93, 93, 93, 93, 93, 93,104, 93,124,123, 94,118,101,101,101,101,101},
        { 93, 93, 93, 93, 93, 93, 94, 93, 93, 93,102, 93, 93, 93, 93, 93, 93,104, 93, 93, 93, 94, 93, 93, 93, 93, 93, 93},
        {103,103,103,103,103,120, 94,125,126, 93,102, 93, 93, 93, 93, 93, 93,104, 93,125,126, 94,119,103,103,103,103,103},
        { 93, 93, 93, 93, 93,104, 94,128,130, 93,112,101,101,101,101,101,101,111, 93,128,130, 94,102, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93,104, 94,128,130, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93,128,130, 94,102, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93,104, 94,128,130, 93,125,129,129,129,129,129,129,126, 93,128,130, 94,102, 93, 93, 93, 93, 93},
        { 97,101,101,101,101,117, 94,124,123, 93,124,127,127,132,131,127,127,123, 93,124,123, 94,118,101,101,101,101, 98},
        {104, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,128,130, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,102},
        {104, 94,125,129,129,126, 94,125,129,129,129,126, 94,128,130, 94,125,129,129,129,126, 94,125,129,129,126, 94,102},
        {104, 94,124,127,132,130, 94,124,127,127,127,123, 94,124,123, 94,124,127,127,127,123, 94,128,131,127,123, 94,102},
        {104, 96, 94, 94,128,130, 94, 94, 94, 94, 94, 94, 94, 93, 93, 94, 94, 94, 94, 94, 94, 94,128,130, 94, 94, 96,102},
        {108,129,126, 94,128,130, 94,125,126, 94,125,129,129,129,129,129,129,126, 94,125,126, 94,128,130, 94,125,129,107},
        {105,127,123, 94,124,123, 94,128,130, 94,124,127,127,132,131,127,127,123, 94,128,130, 94,124,123, 94,124,127,106},
        {104, 94, 94, 94, 94, 94, 94,128,130, 94, 94, 94, 94,128,130, 94, 94, 94, 94,128,130, 94, 94, 94, 94, 94, 94,102},
        {104, 94,125,129,129,129,129,134,133,129,129,126, 94,128,130, 94,125,129,129,134,133,129,129,129,129,126, 94,102},
        {104, 94,124,127,127,127,127,127,127,127,127,123, 94,124,123, 94,124,127,127,127,127,127,127,127,127,123, 94,102},
        {104, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94,102},
        {100,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103, 99},
        { 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93},
        { 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93, 93}
    };
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMS; ++j) {
            data_[i][j] = INITIAL_DATA[i][j];
        }
    }
    doorOpen_ = false;
    numberOfDots_ = MAXIMUM_DOTS;
}

//--------------------------------------------------------------------------------

/**
 * Draws the maze on the screen.
 */
void Maze::Draw(Manager * manager) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMS; ++j) {
            manager->DrawTile(i, j, data_[i][j]);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if a cell is walkable or not.
 */
bool Maze::isCellWalkable(int row, int col) {
    return data_[row][col] == CLEAN || data_[row][col] == DOT ||
           data_[row][col] == BIG_DOT;
}

//--------------------------------------------------------------------------------

/**
 * Checks if a sprite coordinates are valid or not.
 */
bool Maze::CheckSpriteCoords(int x, int y) {
    // First, we have to get the maze coordinates.
    sf::Vector2i coords = SpriteCoordsToMaze(x, y);
    // Second, we check if we are inside the maze dimensions and if the
    // cell is a walkable one for the player and the entities.
    if (0 <= coords.x && coords.x < COLUMS && 0 <= coords.y &&
        coords.y < ROWS && isCellWalkable(coords.y, coords.x)) {
        // Here we obtain the coordinates of the current cell.
        int cx = coords.x * Manager::CELL_WIDTH;
        int cy = coords.y * Manager::CELL_HEIGHT;
        // Because we need it to check this:
        if ((x + SPRITE_SEP_X == cx) && (y + SPRITE_SEP_Y == cy)) {
            // We're in the same coordinates, in the up-left cell's corner.
            return true;
        } else if ((x + SPRITE_SEP_X == cx) && (y + SPRITE_SEP_Y != cy)) {
            // We're in the left side of the cell, so we could be moving to up or down.
            // So we have to check if the up and the down cell are walkable, but the up
            // cell would be checked if we try to enter on it, se we just need only to check
            // if we're trying to move down, to avoid to see an entity out of the right rail.
            return isCellWalkable(coords.y + 1, coords.x);
        } else if ((x + SPRITE_SEP_X != cx) && (y + SPRITE_SEP_Y == cy)) {
            // We're in the up side of the cell, so we could be moving to the right or
            // to the left. So we have to check this:
            if (COLUMS > coords.x + 1) {
                // If we aren't in the last column, we have to check if the right cell
                // is walkable. Just like the up cell, the left one would be checked if
                // we enter on it.
                return isCellWalkable(coords.y, coords.x + 1);
            } else {
                // If we are in the last column, we'll check if we're in the row 17
                // and if we're in the correct y coordinate, because we can't move
                // up or down, just to the left or to the right.
                if (coords.y == 17 && coords.x <= 28 &&
                    (coords.y * Manager::CELL_HEIGHT) == (y + SPRITE_SEP_Y)) {
                    return true;
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    } else if (coords.y == 17) {
        // If we're out of the maze we must be in the row 17, the only place in the
        // maze that is allowed to be out of it, and we'll have to check this:
        if (coords.x >= -1 && coords.x <= 0 &&
            (coords.y * Manager::CELL_HEIGHT) == (y + SPRITE_SEP_Y) &&
            x >= Manager::WORMHOLE_LX) {
            // Here we are in the left side and we can't move to up or down, only to
            // the left or to the right, and we must be to the right of the left
            // wormhole X coordinate limit.
            return true;
        } else if (coords.x <= 28 && coords.x >= 27 &&
            (coords.y * Manager::CELL_HEIGHT) == (y + SPRITE_SEP_Y) &&
            x <= Manager::WORMHOLE_RX) {
            // Here we are in the right side and we can't move to up or down, only to
            // the left or to the right, and we must be to the left of the right
            // wormhole X coordinate limit.
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if a ghost coordinates are valid or not.
 */
bool Maze::CheckGhostCoords(int x, int y) {
    // First, we'll check if the ghost is inside the house.
    if (GHOST_HOUSE_START_X <= x && x <= GHOST_HOUSE_END_X &&
        GHOST_HOUSE_START_Y <= y && y <= GHOST_HOUSE_END_Y) {
        return true;

    } else if (doorOpen_ && GHOST_DOOR_X == x &&
        GHOST_DOOR_START_Y <= y && y <= GHOST_DOOR_END_Y) {
        // Second, we'll check if the ghost is crossing the door.
        return true;

    } else if(GHOST_DOOR_X == x && GHOST_DOOR_LIMIT_Y < y &&
        y <= GHOST_DOOR_START_Y) {
        // Third, we'll check if the ghost is out the door.
        return true;

    } else {
        // And finally, we'll check the rest.
        return CheckSpriteCoords(x, y);
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Maze::Maze() {
    Initialize();
}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
Maze::Maze(const Maze & source) : doorOpen_(source.doorOpen_),
    numberOfDots_(source.numberOfDots_) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLUMS; ++j) {
            data_[i][j] = source.data_[i][j];
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Maze::~Maze() {}
