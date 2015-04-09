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

#include "PuckmanGhostController.h"
#include <SFML/Graphics/Rect.hpp>
#include <Games/Puckman/PuckmanPuckmanEntity.h>
#include <Games/Puckman/PuckmanGhost.h>
#include <Games/Puckman/PuckmanMaze.h>

using namespace Puckman;

//********************************************************************************
// Constants
//********************************************************************************

// The number of different ghosts.
const int GHOSTS = 4;

// The id of the red ghost.
const int RED_GHOST = 0;

// The id of the pink ghost.
const int PINK_GHOST = 1;

// The id of the blue ghost.
const int BLUE_GHOST = 2;

// The id of the brown ghost.
const int BROWN_GHOST = 3;

// The amount of time the ghosts are in scatter mode.
const int SCATTER_INTERVAL = 7000;

// The amount of time the ghosts are in chase mode.
const int CHASE_INTERVAL = 30000;

// The amount of time the ghosts are waiting in the house.
const int HOUSE_INTERVALS[] = {
    0,    // Red
    1000, // Pink
    7000, // Blue
    15000 // Brown
};

// The minimum number of dots to exit from the house.
const int MIN_DOTS_TO_EXIT[] = {
    0,  // Red
    7,  // Pink
    17, // Blue
    32  // Brown
};

// The number of state changes.
const int NUMBER_OF_CHANGES = 8;

// The coordinates of the ghosts corners.
const sf::Vector2i CORNERS[] = {
    sf::Vector2i(26,  4), // Red
    sf::Vector2i( 1,  4), // Pink
    sf::Vector2i(26, 32), // Blue
    sf::Vector2i( 1, 32)  // Brown
};

//********************************************************************************
// Static
//********************************************************************************

std::vector<GhostControl> GhostController::control_;

//--------------------------------------------------------------------------------

/**
 * Initializes the controller data.
 */
void GhostController::Initialize(Ghost * red, Ghost * pink, Ghost * blue,
    Ghost * brown, PuckmanEntity * puckman, Maze * maze) {
    auto getMinDotsToExit = [] (int ghost, Maze * maze) -> int {
        int result = MIN_DOTS_TO_EXIT[ghost] + maze->NumberOfEatedDots();
        if(result < Maze::MAXIMUM_DOTS) {
            return result;
        } else {
            return 0;
        }
    };

    auto addGhost = [&] (int id, Ghost * instance, GhostControlStateEnum state) {
        control_.push_back(GhostControl(SCATTER_INTERVAL, CHASE_INTERVAL,
            HOUSE_INTERVALS[id], getMinDotsToExit(id, maze), NUMBER_OF_CHANGES,
            CORNERS[id], instance, state, puckman, maze));
    };

    control_.clear();
    addGhost(RED_GHOST, red, GhostControlState::Scatter);
    addGhost(PINK_GHOST, pink, GhostControlState::House);
    addGhost(BLUE_GHOST, blue, GhostControlState::House);
    addGhost(BROWN_GHOST, brown, GhostControlState::House);
}

//--------------------------------------------------------------------------------

/**
 * Updates the direction of all the ghosts.
 */
void GhostController::Update(int time) {
    for(int i = 0; i < GHOSTS; i++) {
        control_[i].Update(time);
    }
}
