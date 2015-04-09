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

#include "PuckmanMazeGraph.h"
#include <System/CoreManager.h>
#include <System/MathUtil.h>
#include <System/ForEach.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPuckmanEntity.h>
#include <Games/Puckman/PuckmanGhost.h>

using namespace Puckman;

//********************************************************************************
// Constants
//********************************************************************************

const int MazeGraph::COLS[] = { 6, 8, 8, 4, 4, 2, 8, 10, 10, 4 };

const MazeGraph::Node MazeGraph::NOT_FOUND(0, 0);

//********************************************************************************
// Types
//********************************************************************************

MazeGraph::Node::Node(int row, int col) {
    Row = row;
    Col = col;
    X = col * Manager::CELL_WIDTH - Maze::SPRITE_SEP_X;
    Y = row * Manager::CELL_HEIGHT - Maze::SPRITE_SEP_Y;
    Dirs.clear();
}

//--------------------------------------------------------------------------------

bool MazeGraph::Node::IsValid() const {
    return Row != 0 && Col != 0;
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Resets the maze map.
 */
void MazeGraph::resetMap() {
    const bool INITIAL_DATA[Maze::ROWS][Maze::COLUMS] = {
        {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
        {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
        {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
        {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
        {false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,true,true,true,true,true,true,true,true,true,true,true,false},
        {false,true,false,false,false,false,true,false,false,false,false,false,true,false,false,true,false,false,false,false,false,true,false,false,false,false,true,false},
        {false,true,false,true,true,false,true,false,true,true,true,false,true,false,false,true,false,true,true,true,false,true,false,true,true,false,true,false},
        {false,true,false,false,false,false,true,false,false,false,false,false,true,false,false,true,false,false,false,false,false,true,false,false,false,false,true,false},
        {false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,false},
        {false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,false,false,false,true,false},
        {false,true,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,false,false,false,true,false},
        {false,true,true,true,true,true,true,false,false,true,true,true,true,false,false,true,true,true,true,false,false,true,true,true,true,true,true,false},
        {false,false,false,false,false,false,true,false,false,false,false,false,true,false,false,true,false,false,false,false,false,true,false,false,false,false,false,false},
        {true,true,true,true,true,false,true,false,false,false,false,false,true,false,false,true,false,false,false,false,false,true,false,true,true,true,true,true},
        {true,true,true,true,true,false,true,false,false,true,true,true,true,true,true,true,true,true,true,false,false,true,false,true,true,true,true,true},
        {true,true,true,true,true,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,true,true,true,true,true},
        {false,false,false,false,false,false,true,false,false,true,false,true,true,true,true,true,true,false,true,false,false,true,false,false,false,false,false,false},
        {true,true,true,true,true,true,true,true,true,true,false,true,true,true,true,true,true,false,true,true,true,true,true,true,true,true,true,true},
        {false,false,false,false,false,false,true,false,false,true,false,true,true,true,true,true,true,false,true,false,false,true,false,false,false,false,false,false},
        {true,true,true,true,true,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,true,true,true,true,true},
        {true,true,true,true,true,false,true,false,false,true,true,true,true,true,true,true,true,true,true,false,false,true,false,true,true,true,true,true},
        {true,true,true,true,true,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,true,true,true,true,true},
        {false,false,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,false,false,false,false,false},
        {false,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,true,true,true,true,true,true,true,true,true,true,true,false},
        {false,true,false,false,false,false,true,false,false,false,false,false,true,false,false,true,false,false,false,false,false,true,false,false,false,false,true,false},
        {false,true,false,false,false,false,true,false,false,false,false,false,true,false,false,true,false,false,false,false,false,true,false,false,false,false,true,false},
        {false,true,true,true,false,false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,false,false,true,true,true,false},
        {false,false,false,true,false,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,false,true,false,false,false},
        {false,false,false,true,false,false,true,false,false,true,false,false,false,false,false,false,false,false,true,false,false,true,false,false,true,false,false,false},
        {false,true,true,true,true,true,true,false,false,true,true,true,true,false,false,true,true,true,true,false,false,true,true,true,true,true,true,false},
        {false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false},
        {false,true,false,false,false,false,false,false,false,false,false,false,true,false,false,true,false,false,false,false,false,false,false,false,false,false,true,false},
        {false,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,false},
        {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
        {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true},
        {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true}
    };
    for (int i = 0; i < Maze::ROWS; ++i) {
        for (int j = 0; j < Maze::COLUMS; ++j) {
            map_[i][j] = INITIAL_DATA[i][j];
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Sets a cell of the map with a value.
 */
void MazeGraph::setMapValue(int row, int col, bool value) {
    if(row < 0) row += Manager::ROWS;
    if(col < 0) col += Manager::COLUMS;
    map_[row % Manager::ROWS][col % Manager::COLUMS] = value;
}

//--------------------------------------------------------------------------------

/**
 * Gets the value from a cell of the map.
 */
bool MazeGraph::getMapValue(int row, int col) {
    if(row < 0) row += Manager::ROWS;
    if(col < 0) col += Manager::COLUMS;
    return map_[row % Manager::ROWS][col % Manager::COLUMS];
}

//--------------------------------------------------------------------------------

/**
 * Locks a cell in the map.
 */
void MazeGraph::lockCell(const sf::Vector2i & cell) {
    setMapValue(cell.y, cell.x, false);
}

//--------------------------------------------------------------------------------

/**
 * Unlocks a cell in the map.
 */
void MazeGraph::unlockCell(const sf::Vector2i & cell) {
    setMapValue(cell.y, cell.x, true);
}

//--------------------------------------------------------------------------------

/**
 * Gets all the posible directions from a cell.
 */
MazeGraph::MovingDirections MazeGraph::GetDirections(int row, int col) {
    MovingDirections list;
    if(getMapValue(row - 1, col)) list.push_back(MovingDirection::Up);
    if(getMapValue(row + 1, col)) list.push_back(MovingDirection::Down);
    if(getMapValue(row, col - 1)) list.push_back(MovingDirection::Left);
    if(getMapValue(row, col + 1)) list.push_back(MovingDirection::Right);
    return list;
}

//--------------------------------------------------------------------------------

/**
 * Gets all the posible directions from a cell.
 */
MazeGraph::MovingDirections MazeGraph::GetDirections(const sf::Vector2i & orig,
    const sf::Vector2i & dest) {
    MovingDirections list;
    if(orig.y < dest.y) {
        if(orig.x < dest.x) { // NW
            if((dest.y - orig.y) <= (dest.x - orig.x)) {
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
            } else {
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
            }
        } else if(orig.x == dest.x) { // N
            if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
            if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
            if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
            if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
        } else { // NE
            if((dest.y - orig.y) <= (orig.x - dest.x)) {
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
            } else {
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
            }
        }
    } else if(orig.y == dest.y) {
        if(orig.x < dest.x) { // W
            if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
            if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
            if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
            if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
        } else if(orig.x > dest.x) { // E
            if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
            if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
            if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
            if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
        }
    } else {
        if(orig.x < dest.x) { // SW
            if((orig.y - dest.y) <= (dest.x - orig.x)) {
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
            } else {
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
            }
        } else if(orig.x == dest.x) { // S
            if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
            if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
            if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
            if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
        } else { // SE
            if((orig.y - dest.y) <= (orig.x - dest.x)) {
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
            } else {
                if(getMapValue(orig.y, orig.x - 1)) list.push_back(MovingDirection::Left);
                if(getMapValue(orig.y - 1, orig.x)) list.push_back(MovingDirection::Up);
                if(getMapValue(orig.y + 1, orig.x)) list.push_back(MovingDirection::Down);
                if(getMapValue(orig.y, orig.x + 1)) list.push_back(MovingDirection::Right);
            }
        }
    }
    return list;
}

//--------------------------------------------------------------------------------

/**
 * Gets the next point when we have a direction.
 */
sf::Vector2i MazeGraph::GetNextPoint(const sf::Vector2i & orig, MovingDirectionEnum dir) {
    sf::Vector2i aux(orig.x, orig.y);
    switch(dir) {
    case MovingDirection::Up:
        aux.y--;
        if(aux.y < 0) {
            aux.y += Manager::ROWS;
        }
        break;
    case MovingDirection::Down:
        aux.y++;
        if(aux.y >= Manager::ROWS) {
            aux.y -= Manager::ROWS;
        }
        break;
    case MovingDirection::Left:
        aux.x--;
        if(aux.x < 0) {
            aux.x += Manager::COLUMS;
        }
        break;
    case MovingDirection::Right:
        aux.x++;
        if(aux.x >= Manager::COLUMS) {
            aux.x -= Manager::COLUMS;
        }
        break;
    }
    return aux;
}

//--------------------------------------------------------------------------------

/**
 * Gets the distance in cells from an origin to a destination.
 */
int MazeGraph::getDistance(const sf::Vector2i & orig, const sf::Vector2i & dest) {
    return std::abs(orig.y - dest.y) + std::abs(orig.x - dest.x);
}

//--------------------------------------------------------------------------------

/**
 * Checks if there is a direct path.
 */
bool MazeGraph::findDirectPath(const sf::Vector2i & orig, const sf::Vector2i & dest,
    MovingDirectionEnum lastDir, int max) {
    // The first thing to check is if we are in the destination.
    if(orig != dest) {
        // If we aren't in the destination, we'll get the current distance to it.
        int currentDistance = getDistance(orig, dest);
        if(max >= currentDistance) {
            // If we go in the right direction, we'll lock this cell to test it.
            bool result = false;
            lockCell(orig);

            // We'll check if the current cell is an intersection or not.
            if(IsOnIntersection(orig.y, orig.x)) {
                // If we're on an intersection, we'll first get the possible directions.
                MovingDirections dirs = GetDirections(orig, dest);
                ForEach(dirs, [&] (MovingDirectionEnum dir) {
                    // Then for each possible direction, we'll check if we have a direct
                    // path with it. When a direction returns true, the algorithm stops.
                    if(!result) {
                        result = findDirectPath(GetNextPoint(orig, dir), dest, dir, currentDistance);
                    }
                });
            } else {
                // If we're not on an intersection, we'll check the next cell.
                result = findDirectPath(GetNextPoint(orig, lastDir), dest, lastDir, currentDistance);
            }

            // Finally, we'll unlock the cell and return the result of the algorithm.
            unlockCell(orig);
            return result;
        } else {
            // If we don't go in the right direction, we don't have a direct path.
            return false;
        }
    } else {
        // If we are in the destination, we find a direct path.
        return true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets the opposite direction from another one.
 */
MovingDirectionEnum MazeGraph::GetOppositeDirection(MovingDirectionEnum dir) {
    switch(dir) {
    case MovingDirection::Up:    return MovingDirection::Down;
    case MovingDirection::Down:  return MovingDirection::Up;
    case MovingDirection::Left:  return MovingDirection::Right;
    case MovingDirection::Right: return MovingDirection::Left;
    }
    return MovingDirection::None;
}

//--------------------------------------------------------------------------------

/**
 * Erase a direction from an array.
 */
MazeGraph::MovingDirections MazeGraph::eraseDirection(const MovingDirections & dirs,
    MovingDirectionEnum dir) {
    MovingDirections list;
    ForEach(dirs, [&] (MovingDirectionEnum d) {
        if(d != dir) {
            list.push_back(d);
        }
    });
    return list;
}

//--------------------------------------------------------------------------------

/**
 * Gets a random direction from a list.
 */
MovingDirectionEnum MazeGraph::getRandomDirection(MovingEntity & entity,
    const MovingDirections & dirs) {
    if(dirs.size() > 0) {
        // If the list isn't empty, we'll erase the opposite direction from the list.
        MovingDirectionEnum opdir = GetOppositeDirection(entity.Direction());
        MovingDirections options = eraseDirection(dirs, opdir);
        // Then, we'll check the new list is empty.
        if(options.size() == 0) {
            // If the new list is empty, we'll return the unique direction in the old list.
            return dirs[0];
        } else if(options.size() == 1) {
            // If the new list isn't empty, but only have one element, we'll return that one.
            return options[0];
        } else {
            // If the new list isn't empty and have more than one element, we'll check if
            // in the new list is the last direction of the entity to follow that one.
            auto it = options.begin(), end = options.end();
            for (; it != end; ++it) {
                MovingDirectionEnum d = *it;
                if(d == entity.Direction()) {
                    return d;
                }
            }

            // If not, we'll get a random element from the new list.
            return options[core_->Random(options.size())];
        }
    } else {
        // If the list is empty, we have none direction to return.
        return MovingDirection::None;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets a random direction from a list.
 */
MovingDirectionEnum MazeGraph::getRandomDirection(const MovingDirections & dirs) {
    return dirs[core_->Random(dirs.size())];
}

//--------------------------------------------------------------------------------

/**
 * Checks if a direction exists inside the array.
 */
MovingDirectionEnum MazeGraph::checkDirectionInList(const MovingDirections & dirs,
    MovingDirectionEnum dir) {
    auto it = dirs.begin(), end = dirs.end();
    for (; it != end; ++it) {
        MovingDirectionEnum d = *it;
        if(d == dir) return dir;
    }
    return MovingDirection::None;
}

//--------------------------------------------------------------------------------

/**
 * Gets the direct direction if exists in the list.
 */
MovingDirectionEnum MazeGraph::getDirectDirection(const sf::Vector2i & orig, const sf::Vector2i & dest, const MovingDirections & dirs) {
    if(orig.y == dest.y && orig.x < dest.x) { // W
        return checkDirectionInList(dirs, MovingDirection::Right);
    } else if(orig.y == dest.y && orig.x > dest.x) { // E
        return checkDirectionInList(dirs, MovingDirection::Left);
    } else if(orig.x == dest.x && orig.y < dest.y) { // N
        return checkDirectionInList(dirs, MovingDirection::Down);
    } else if(orig.x == dest.x && orig.y > dest.y) { // S
        return checkDirectionInList(dirs, MovingDirection::Up);
    } else {
        return MovingDirection::None;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets the best direction to go to a destination.
 */
MovingDirectionEnum MazeGraph::GetDirection(MovingEntity & entity, const sf::Vector2i & dest) {
    // First, we'll get the origin cell of the entity.
    sf::Vector2i orig = Maze::SpriteCoordsToMaze(entity.X, entity.Y);
    // And then we'll check if we're not in the destination.
    if(orig != dest) {
        // If we aren't in the destination, we'll get the possible directions.
        MovingDirections dirs = GetDirections(orig, dest);
        MovingDirectionEnum result = MovingDirection::None;
        // We also need the distance from the entity to the destination cell.
        int maxDistance = getDistance(orig, dest);

        // Then, we'll lock the origin cell and try to find the first possible
        // direction with a direct path available. After the test we'll unlock
        // the cell to reset the maze map.
        lockCell(orig);
        ForEach(dirs, [&] (MovingDirectionEnum dir) {
            if(result == MovingDirection::None &&
                findDirectPath(GetNextPoint(orig, dir), dest, dir, maxDistance)) {
                result = dir;
            }
        });
        unlockCell(orig);

        // When there isn't a direct path, we'll try to get the direct direction.
        if(result == MovingDirection::None) {
            result = getDirectDirection(orig, dest, dirs);
        }

        // When there isn't a direct direction, we'll get a random direction,
        // but we wont take the opposite to the current entity's direction.
        if(result == MovingDirection::None) {
            result = getRandomDirection(entity, dirs);
        }

        // If we still haven't a direction, we finally get a random one.
        if(result == MovingDirection::None) {
            result = getRandomDirection(dirs);
        }

        // Finally, we'll return the result.
        return result;
    } else {
        // If we are in the destination, we don't have to move anywhere.
        return MovingDirection::None;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets a random direction to escape from the puckman.
 */
MovingDirectionEnum MazeGraph::GetEscapeDirection(Ghost & ghost, PuckmanEntity & puckman) {
    // First, we'll get the cell of the ghost.
    sf::Vector2i orig = Maze::SpriteCoordsToMaze(ghost.X, ghost.Y);
    // Second, we'll get the possible directions.
    MovingDirections dirs = GetDirections(orig.y, orig.x);
    if(dirs.size() > 0) {
        MovingDirectionEnum opdir;
        // Third, we'll check if puckman is in the same line.
        sf::Vector2i orig2 = Maze::SpriteCoordsToMaze(puckman.X, puckman.Y);
        if(orig.x == orig2.x || orig.y == orig2.y) {
            // When puckman is in the same line, we'll get the opposite direction
            // of puckman, to erase it from the possible directions. We do this
            // to avoid the ghost to run to a truly death.
            opdir = GetOppositeDirection(puckman.Direction());
        } else {
            // When puckman isn't in the same line, we'll get the opposite direction
            // of the ghost, to erase it from the possible directions. We do this
            // to avoid the ghost to repeat a loop between two intersections.
            opdir = GetOppositeDirection(ghost.Direction());
        }
        // Here, we'll erase the selected opposite direction from the list.
        MovingDirections options = eraseDirection(dirs, opdir);
        if(options.size() > 0) {
            // And finally, we'll return a random direction.
            return options[core_->Random(options.size())];
        } else {
            return dirs[0];
        }
    } else {
        return MovingDirection::None;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets a random direction for an entity.
 */
MovingDirectionEnum MazeGraph::GetRandomDirection(MovingEntity & entity) {
    // First, we'll get the origin cell of the entity.
    sf::Vector2i orig = Maze::SpriteCoordsToMaze(entity.X, entity.Y);
    // Second, we'll the possible directions of the cell.
    MovingDirections dirs = GetDirections(orig.y, orig.x);
    // And, finally we'll return a random direction.
    return getRandomDirection(dirs); ;
}

//--------------------------------------------------------------------------------

/**
 * Gets a node of the graph if the entity is inside the correct cell.
 */
const MazeGraph::Node & MazeGraph::GetNode(MovingEntity & entity) {
    sf::Vector2i cell = Maze::SpriteCoordsToMaze(entity.X, entity.Y);
    // First, we'll seek the current row.
    for(int i = 0; i < ROWS; i++) {
        if(rows_[i].Row == cell.y) {
            // Second, we'll seek the current column.
            for(int j = 0; j < COLS[i]; j++) {
                if(rows_[i].Cols[j].Col == cell.x) {
                    // Finally, if we find a node with the same row and column
                    // we'll return that node to the user.
                    return rows_[i].Cols[j];
                }
            }
        }
    }
    // But when no node have the same coordinates we'll return the invalid node.
    return NOT_FOUND;
}

//--------------------------------------------------------------------------------

/**
 * Gets a node of the graph.
 */
const MazeGraph::Node & MazeGraph::GetNode(int row, int col) {
    // First, we'll seek the current row.
    for(int i = 0; i < ROWS; i++) {
        if(rows_[i].Row == row) {
            // Second, we'll seek the current column.
            for(int j = 0; j < COLS[i]; j++) {
                if(rows_[i].Cols[j].Col == col) {
                    // Finally, if we find a node with the same row and column
                    // we'll return that node to the user.
                    return rows_[i].Cols[j];
                }
            }
        }
    }
    // But when no node have the same coordinates we'll return the invalid node.
    return NOT_FOUND;
}

//--------------------------------------------------------------------------------

/**
 * Checks if an entity is on an intersection.
 */
bool MazeGraph::IsOnIntersection(MovingEntity & entity) {
    return GetNode(entity).IsValid();
}

//--------------------------------------------------------------------------------

/**
 * Checks if a coordinates are an intersection.
 */
bool MazeGraph::IsOnIntersection(int row, int col) {
    return GetNode(row, col).IsValid();
}

//--------------------------------------------------------------------------------

/**
 * Checks if an entity is near to an intersection.
 */
bool MazeGraph::IsNearToIntersection(MovingEntity & entity) {
    sf::Vector2i cell = Maze::SpriteCoordsToMaze(entity.X, entity.Y);
    return GetNode(cell.y - 1, cell.x).IsValid() || GetNode(cell.y + 1, cell.x).IsValid() ||
           GetNode(cell.y, cell.x - 1).IsValid() || GetNode(cell.y, cell.x + 1).IsValid();
}

//--------------------------------------------------------------------------------

/**
 * Checks if a coordinates are near to an intersection.
 */
bool MazeGraph::IsNearToIntersection(int row, int col) {
    return GetNode(row - 1, col).IsValid() || GetNode(row + 1, col).IsValid() ||
           GetNode(row, col - 1).IsValid() || GetNode(row, col + 1).IsValid();
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
MazeGraph::MazeGraph() {
    const int ROW_VALUES[] = { 4, 8, 11, 14, 17, 20, 23, 26, 29, 32 };
    const int COL_STARTS[] = { 0, 6, 14, 22, 26, 30, 32, 40, 50, 60 };
    const int COL_VALUES[] = {
        /*  0 Row  4 */ 1, 6, 12, 15, 21, 26,
        /*  6 Row  8 */ 1, 6, 9, 12, 15, 18, 21, 26,
        /* 14 Row 11 */ 1, 6, 9, 12, 15, 18, 21, 26,
        /* 22 Row 14 */ 9, 12, 15, 18,
        /* 26 Row 17 */ 6, 9, 18, 21,
        /* 30 Row 20 */ 9, 18,
        /* 32 Row 23 */ 1, 6, 9, 12, 15, 18, 21, 26,
        /* 40 Row 26 */ 1, 3, 6, 9, 12, 15, 18, 21, 24, 26,
        /* 50 Row 29 */ 1, 3, 6, 9, 12, 15, 18, 21, 24, 26,
        /* 60 Row 32 */ 1, 12, 15, 26
    };
    const int DU = 0x01, DD = 0x02, DL = 0x04, DR = 0x08;
    const int DIR_VALUES[] = {
        /*  0 Row  4 */ DD|DR, DD|DL|DR, DD|DL, DD|DR, DD|DL|DR, DD|DL,
        /*  6 Row  8 */ DD|DR|DU, DD|DL|DR|DU, DD|DL|DR, DL|DR|DU,
                        DL|DR|DU, DD|DL|DR, DD|DL|DR|DU, DD|DL|DU,
        /* 14 Row 11 */ DR|DU, DD|DL|DU, DR|DU, DD|DL,
                        DD|DR, DL|DU, DD|DR|DU, DL|DU,
        /* 22 Row 14 */ DD|DR, DL|DR|DU, DL|DR|DU, DD|DL,
        /* 26 Row 17 */ DD|DL|DR|DU, DD|DL|DU, DD|DR|DU, DD|DL|DR|DU,
        /* 30 Row 20 */ DD|DR|DU, DD|DL|DU,
        /* 32 Row 23 */ DD|DR, DD|DL|DR|DU, DL|DR|DU, DD|DL,
                        DD|DR, DL|DR|DU, DD|DL|DR|DU, DD|DL,
        /* 40 Row 26 */ DR|DU, DD|DL, DD|DR|DU, DD|DL|DR, DL|DR|DU,
                        DL|DR|DU, DD|DL|DR, DD|DL|DU, DD|DR, DL|DU,
        /* 50 Row 29 */ DD|DR, DL|DR|DU, DL|DU, DR|DU, DD|DL,
                        DD|DR, DL|DU, DR|DU, DL|DR|DU, DD|DL,
        /* 60 Row 32 */ DR|DU, DL|DR|DU, DL|DR|DU, DL|DU
    };

    rows_.assign(ROWS, RowNode());

    for(int i = 0; i < ROWS; i++) {
        rows_[i].Row = ROW_VALUES[i];
        rows_[i].Y = ROW_VALUES[i] * Manager::CELL_HEIGHT - Maze::SPRITE_SEP_Y;
        rows_[i].Cols.assign(COLS[i], Node());

        for(int j = 0; j < COLS[i]; j++) {
            auto col = COL_VALUES[COL_STARTS[i] + j];
            auto dirsMask = DIR_VALUES[COL_STARTS[i] + j];
            rows_[i].Cols[j].Row = rows_[i].Row;
            rows_[i].Cols[j].Col = COL_VALUES[COL_STARTS[i] + j];
            rows_[i].Cols[j].X = COL_VALUES[COL_STARTS[i] + j] * Manager::CELL_WIDTH - Maze::SPRITE_SEP_X;
            rows_[i].Cols[j].Y = rows_[i].Y;

            MovingDirections dirs;
            if (dirsMask | DU) dirs.push_back(MovingDirection::Up);
            if (dirsMask | DD) dirs.push_back(MovingDirection::Down);
            if (dirsMask | DL) dirs.push_back(MovingDirection::Left);
            if (dirsMask | DR) dirs.push_back(MovingDirection::Right);
            rows_[i].Cols[j].Dirs = dirs;
        }
    }

    resetMap();
}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
MazeGraph::MazeGraph(const MazeGraph & source) {
    rows_ = source.rows_;
    for (int i = 0; i < Maze::ROWS; ++i) {
        for (int j = 0; j < Maze::COLUMS; ++j) {
            map_[i][j] = source.map_[i][j];
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
MazeGraph::~MazeGraph() {}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
MazeGraph & MazeGraph::operator =(const MazeGraph & source) {
    rows_ = source.rows_;
    for (int i = 0; i < Maze::ROWS; ++i) {
        for (int j = 0; j < Maze::COLUMS; ++j) {
            map_[i][j] = source.map_[i][j];
        }
    }
    return *this;
}
