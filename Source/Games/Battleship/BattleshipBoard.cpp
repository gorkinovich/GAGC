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

#include "BattleshipBoard.h"
#include <vector>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/Mouse.h>
#include <System/MathUtil.h>
#include <Games/Battleship/BattleshipManager.h>

//********************************************************************************
// Constants
//********************************************************************************

const int INIT_MODE   = 0;
const int PUT_MODE    = 1;
const int PLAYER_MODE = 2;
const int ENEMY_MODE  = 3;

const int MAX_SHIP_TEXS    = 32;
const int MAX_SHIP_TYPES   =  4;
const int MAX_SHIP_DIRS    =  4;
const int MAX_NEAR_OFFSETS =  8 ;

const int DIR_N = 0;
const int DIR_E = 1;
const int DIR_S = 2;
const int DIR_W = 3;

const int BOARD_OFFSETX = 12;
const int BOARD_OFFSETY = 13;

const int CELL_WIDTH  = 24;
const int CELL_HEIGHT = 24;

const int MASK_TEX1 = 0x000F; // 0000 0000 0000 1111
const int MASK_TEX2 = 0x001F; // 0000 0000 0001 1111

const int SHIP1N_TEX = 0x0000;
const int SHIP1E_TEX = 0x0001;
const int SHIP1S_TEX = 0x0002;
const int SHIP1W_TEX = 0x0003;

const int SHIPFN_TEX = 0x0004;
const int SHIPFE_TEX = 0x0005;
const int SHIPFS_TEX = 0x0006;
const int SHIPFW_TEX = 0x0007;

const int SHIPMN_TEX = 0x0008;
const int SHIPME_TEX = 0x0009;
const int SHIPMS_TEX = 0x000A;
const int SHIPMW_TEX = 0x000B;

const int SHIPBN_TEX = 0x000C;
const int SHIPBE_TEX = 0x000D;
const int SHIPBS_TEX = 0x000E;
const int SHIPBW_TEX = 0x000F;

const int CELL_HIT   = 0x0010; // 0000 0000 0001 XXXX
const int SHIP_SUNK  = 0x0020; // 0000 0000 0010 XXXX

const int WATER_CELL = 0x0100; // 0000 0001 0000 XXXX
const int SHIP1_CELL = 0x1000; // 0001 0000 0000 XXXX
const int SHIP2_CELL = 0x2000; // 0010 0000 0000 XXXX
const int SHIP3_CELL = 0x4000; // 0100 0000 0000 XXXX
const int SHIP4_CELL = 0x8000; // 1000 0000 0000 XXXX

const int AI_STATE_RANDOM  = 0;
const int AI_STATE_FINDING = 1;
const int AI_STATE_FOUND   = 2;

//********************************************************************************
// Static
//********************************************************************************

BattleshipBoard::Action BattleshipBoard::emptyAction_ = [] () {};

//********************************************************************************
// InnerData
//********************************************************************************

struct BattleshipBoard::InnerData {
    CoreManager * core;
    BattleshipManager * manager;

    sf::IntRect boardArea;
    sf::Vector2i boardPosition;
    Texture2D boardTexture;

    Texture2D shipsTextures[MAX_SHIP_TYPES][MAX_SHIP_DIRS];
    Texture2D shipsFragTexs[MAX_SHIP_TEXS];
    Texture2D cellGrayTexture;
    Texture2D cellHoverTexture;
    Texture2D cellClickTexture;
    Texture2D playerWaterHitTexture;
    Texture2D enemyShipHitTexture;

    BattleshipBoard::FleetBoard fleet;
    BattleshipBoard::EmptyBoard empty;

    sf::Vector2i cellCoords;
    int shipsCount[MAX_SHIP_TYPES];
    BattleshipBoard::Action onUpdate;
    BattleshipBoard::Action onFinish;

    int currentShipType;
    int currentShipDir;
    int currentMode;
    bool playSounds;
    bool fleetPutFinished;

    int aiState;
    int aiNextDir;
    sf::Vector2i aiLastCell;

    sf::Vector2i dirOffsets[MAX_SHIP_DIRS];
    sf::Vector2i nearOffsets[MAX_NEAR_OFFSETS];

    void MarkCell(int x, int y);
    void PutShip(int len);

    void DrawPutFleet();
    void DrawPlayer();
    void DrawEnemy();

    void FireOver(int row, int col);

    sf::Vector2i GetBoardCoords();
    sf::Vector2i GetPosition(const sf::Vector2i & pos);

    int GetTypeLength(int type);
    void GetEmptyCoords(std::vector<sf::Vector2i> & emptyCoords);

    int GetCurrentTypeLength() {
        return GetTypeLength(currentShipType);
    }

    bool IsInside(int r, int c) { return 0 <= r && r < MAX_SIZE && 0 <= c && c < MAX_SIZE; }
    bool IsInside(const sf::Vector2i & coords) { return IsInside(coords.y, coords.x); }

    bool IsShip1(int r, int c) { return (fleet[r][c] & SHIP1_CELL) != 0; }
    bool IsShip1(const sf::Vector2i & coords) { return IsShip1(coords.y, coords.x); }
    bool IsShip2(int r, int c) { return (fleet[r][c] & SHIP1_CELL) != 0; }
    bool IsShip2(const sf::Vector2i & coords) { return IsShip2(coords.y, coords.x); }
    bool IsShip3(int r, int c) { return (fleet[r][c] & SHIP1_CELL) != 0; }
    bool IsShip3(const sf::Vector2i & coords) { return IsShip3(coords.y, coords.x); }
    bool IsShip4(int r, int c) { return (fleet[r][c] & SHIP1_CELL) != 0; }
    bool IsShip4(const sf::Vector2i & coords) { return IsShip4(coords.y, coords.x); }

    bool IsWater(int r, int c) { return (fleet[r][c] & WATER_CELL) != 0; }
    bool IsWater(const sf::Vector2i & coords) { return IsWater(coords.y, coords.x); }
    bool IsShip(int r, int c) { return (fleet[r][c] & WATER_CELL) == 0; }
    bool IsShip(const sf::Vector2i & coords) { return IsShip(coords.y, coords.x); }
    bool IsHit(int r, int c) { return (fleet[r][c] & CELL_HIT) != 0; }
    bool IsHit(const sf::Vector2i & coords) { return IsHit(coords.y, coords.x); }
    bool IsSunk(int r, int c) { return (fleet[r][c] & SHIP_SUNK) != 0; }
    bool IsSunk(const sf::Vector2i & coords) { return IsSunk(coords.y, coords.x); }
    bool IsShipWithHit(int r, int c) { return IsHit(r, c) && IsShip(r, c); }
    bool IsShipWithHit(const sf::Vector2i & coords) { return IsShipWithHit(coords.y, coords.x); }

    bool IsWaterSafe(int r, int c) { return IsInside(r, c) && IsWater(r, c); }
    bool IsWaterSafe(const sf::Vector2i & coords) { return IsWaterSafe(coords.y, coords.x); }
    bool IsShipSafe(int r, int c) { return IsInside(r, c) && IsShip(r, c); }
    bool IsShipSafe(const sf::Vector2i & coords) { return IsShipSafe(coords.y, coords.x); }
    bool IsHitSafe(int r, int c) { return IsInside(r, c) && IsHit(r, c); }
    bool IsHitSafe(const sf::Vector2i & coords) { return IsHitSafe(coords.y, coords.x); }
    bool IsSunkSafe(int r, int c) { return IsInside(r, c) && IsSunk(r, c); }
    bool IsSunkSafe(const sf::Vector2i & coords) { return IsSunkSafe(coords.y, coords.x); }
    bool IsShipWithHitSafe(int r, int c) { return IsInside(r, c) && IsHit(r, c) && IsShip(r, c); }
    bool IsShipWithHitSafe(const sf::Vector2i & coords) { return IsShipWithHitSafe(coords.y, coords.x); }
};

//--------------------------------------------------------------------------------

/**
 * Marks a cell as not empty.
 */
void BattleshipBoard::InnerData::MarkCell(int x, int y) {
    if (IsInside(y, x)) {
        empty[y][x] = false;
    }
}

//--------------------------------------------------------------------------------

/**
 * Puts a ship on the board.
 */
void BattleshipBoard::InnerData::PutShip(int len) {
    // This flag is used to mark that there is nothing to avoid the
    // algorithm to put the ship inside the board.
    bool nothing = true;
    if (currentShipDir == DIR_N || currentShipDir == DIR_S) {
        // So we have a ship to put in vertical mode, so we'll check that
        // there is an empty place inside the board to put that ship.
        for (int y = cellCoords.y, end = cellCoords.y + len; y < end; ++y) {
            if (!empty[y][cellCoords.x]) {
                nothing = false;
                break;
            }
        }
        if (nothing) {
            // So we can place the ship. We'll mark the nearest cells as
            // not empty to avoid a ship close to another one.
            if (playSounds) manager->ClickSound().Play();
            for (int i = -1; i <= len; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    MarkCell(cellCoords.x + j, cellCoords.y + i);
                }
            }
            // And here an example of the "beauty" of the code of this game. This is used
            // to set inside the board the type and the textures used by the placed ship.
            if (currentShipDir == DIR_N) {
                if (len == 1) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP1_CELL | SHIP1N_TEX;
                } else if (len == 2) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP2_CELL | SHIPFN_TEX;
                    fleet[cellCoords.y + 1][cellCoords.x] = SHIP2_CELL | SHIPBN_TEX;
                } else if (len == 3) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP3_CELL | SHIPFN_TEX;
                    fleet[cellCoords.y + 1][cellCoords.x] = SHIP3_CELL | SHIPMN_TEX;
                    fleet[cellCoords.y + 2][cellCoords.x] = SHIP3_CELL | SHIPBN_TEX;
                } else {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP4_CELL | SHIPFN_TEX;
                    fleet[cellCoords.y + 1][cellCoords.x] = SHIP4_CELL | SHIPMN_TEX;
                    fleet[cellCoords.y + 2][cellCoords.x] = SHIP4_CELL | SHIPMN_TEX;
                    fleet[cellCoords.y + 3][cellCoords.x] = SHIP4_CELL | SHIPBN_TEX;
                }
            } else {
                if (len == 1) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP1_CELL | SHIP1S_TEX;
                } else if (len == 2) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP2_CELL | SHIPBS_TEX;
                    fleet[cellCoords.y + 1][cellCoords.x] = SHIP2_CELL | SHIPFS_TEX;
                } else if (len == 3) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP3_CELL | SHIPBS_TEX;
                    fleet[cellCoords.y + 1][cellCoords.x] = SHIP3_CELL | SHIPMS_TEX;
                    fleet[cellCoords.y + 2][cellCoords.x] = SHIP3_CELL | SHIPFS_TEX;
                } else {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP4_CELL | SHIPBS_TEX;
                    fleet[cellCoords.y + 1][cellCoords.x] = SHIP4_CELL | SHIPMS_TEX;
                    fleet[cellCoords.y + 2][cellCoords.x] = SHIP4_CELL | SHIPMS_TEX;
                    fleet[cellCoords.y + 3][cellCoords.x] = SHIP4_CELL | SHIPFS_TEX;
                }
            }
        }
    } else if (currentShipDir == DIR_E || currentShipDir == DIR_W) {
        // So we have a ship to put in horizontal mode, so we'll check that
        // there is an empty place inside the board to put that ship.
        for (int x = cellCoords.x, end = cellCoords.x + len; x < end; ++x) {
            if (!empty[cellCoords.y][x]) {
                nothing = false;
                break;
            }
        }
        if (nothing) {
            // So we can place the ship. We'll mark the nearest cells as
            // not empty to avoid a ship close to another one.
            if (playSounds) manager->ClickSound().Play();
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= len; ++j) {
                    MarkCell(cellCoords.x + j, cellCoords.y + i);
                }
            }
            // And here an example of the "beauty" of the code of this game. This is used
            // to set inside the board the type and the textures used by the placed ship.
            if (currentShipDir == DIR_E) {
                if (len == 1) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP1_CELL | SHIP1E_TEX;
                } else if (len == 2) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP2_CELL | SHIPBE_TEX;
                    fleet[cellCoords.y][cellCoords.x + 1] = SHIP2_CELL | SHIPFE_TEX;
                } else if (len == 3) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP3_CELL | SHIPBE_TEX;
                    fleet[cellCoords.y][cellCoords.x + 1] = SHIP3_CELL | SHIPME_TEX;
                    fleet[cellCoords.y][cellCoords.x + 2] = SHIP3_CELL | SHIPFE_TEX;
                } else {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP4_CELL | SHIPBE_TEX;
                    fleet[cellCoords.y][cellCoords.x + 1] = SHIP4_CELL | SHIPME_TEX;
                    fleet[cellCoords.y][cellCoords.x + 2] = SHIP4_CELL | SHIPME_TEX;
                    fleet[cellCoords.y][cellCoords.x + 3] = SHIP4_CELL | SHIPFE_TEX;
                }
            } else {
                if (len == 1) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP1_CELL | SHIP1W_TEX;
                } else if (len == 2) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP2_CELL | SHIPFW_TEX;
                    fleet[cellCoords.y][cellCoords.x + 1] = SHIP2_CELL | SHIPBW_TEX;
                } else if (len == 3) {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP3_CELL | SHIPFW_TEX;
                    fleet[cellCoords.y][cellCoords.x + 1] = SHIP3_CELL | SHIPMW_TEX;
                    fleet[cellCoords.y][cellCoords.x + 2] = SHIP3_CELL | SHIPBW_TEX;
                } else {
                    fleet[cellCoords.y][cellCoords.x]     = SHIP4_CELL | SHIPFW_TEX;
                    fleet[cellCoords.y][cellCoords.x + 1] = SHIP4_CELL | SHIPMW_TEX;
                    fleet[cellCoords.y][cellCoords.x + 2] = SHIP4_CELL | SHIPMW_TEX;
                    fleet[cellCoords.y][cellCoords.x + 3] = SHIP4_CELL | SHIPBW_TEX;
                }
            }
        }
    }
    // If the ship is placed inside the board, we'll remove from the count a ship
    // of the current type. Then we'll call the "on update" delegate.
    if (nothing) {
        shipsCount[currentShipType]--;
        onUpdate();
        // If we don't have more ships of this type, we'll select the next type.
        if (shipsCount[currentShipType] <= 0) {
            int newType = -1;
            for (int i = MAX_SHIP_TYPES - 1; i >= 0; --i) {
                if (shipsCount[i] > 0) {
                    newType = i;
                    break;
                }
            }
            // If there is another type left, we'll select it. If not, we'll
            // set the flag that marks the put mode finished and call the
            // "on finish" delegate to end the put mode.
            if (newType != -1) {
                currentShipType = newType;
            } else {
                fleetPutFinished = true;
                onFinish();
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the current put ship board.
 */
void BattleshipBoard::InnerData::DrawPutFleet() {
    // Draw the board elements.
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            auto coords = GetPosition(sf::Vector2i(j, i));
            if (IsShip(i, j)) {
                int index = fleet[i][j] & MASK_TEX2;
                shipsFragTexs[index].Draw(coords);
            } else if (!empty[i][j]) {
                cellGrayTexture.Draw(coords);
            }
        }
    }
    // Draw the ship to place.
    if (cellCoords.x != -1 && cellCoords.y != -1) {
        sf::Vector2i pos = GetPosition(cellCoords);
        shipsTextures[currentShipType][currentShipDir].Draw(pos);
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the current player board.
 */
void BattleshipBoard::InnerData::DrawPlayer() {
    // Draw the board elements.
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            auto coords = GetPosition(sf::Vector2i(j, i));
            if (IsShip(i, j)) {
                int index = fleet[i][j] & MASK_TEX2;
                shipsFragTexs[index].Draw(coords);
            } else if (IsHit(i, j)) {
                playerWaterHitTexture.Draw(coords);
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the current enemy board.
 */
void BattleshipBoard::InnerData::DrawEnemy() {
    // Draw the board elements.
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (IsHit(i, j)) {
                auto coords = GetPosition(sf::Vector2i(j, i));
                if (IsShip(i, j)) {
                    if (IsSunk(i, j)) {
                        int index = fleet[i][j] & MASK_TEX2;
                        shipsFragTexs[index].Draw(coords);
                    } else {
                        enemyShipHitTexture.Draw(coords);
                    }
                } else {
                    playerWaterHitTexture.Draw(coords);
                }
            }
        }
    }
    // Draw the coordinate to be fired.
    if (cellCoords.x != -1 && cellCoords.y != -1 && !IsHit(cellCoords)) {
        sf::Vector2i pos = GetPosition(cellCoords);
        if (Mouse::isButtonPressed(Mouse::Left)) {
            cellClickTexture.Draw(pos);
        } else {
            cellHoverTexture.Draw(pos);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Executes a fire over a position.
 */
void BattleshipBoard::InnerData::FireOver(int row, int col) {
    // First, we'll have to check that the coordinates are inside
    // the board and that cell have not been hit previously.
    if (IsInside(row, col) && !IsHit(row, col)) {
        // So we'll mark the cell as hit.
        fleet[row][col] |= CELL_HIT;
        if (IsShip(row, col)) {
            // If the hit cell is a ship, we'll have to check some stuff and we'll
            // use the index of the texture to get some information about the ship.
            int subTexIdx = fleet[row][col] & MASK_TEX1;
            if (subTexIdx == SHIP1N_TEX || subTexIdx == SHIP1E_TEX ||
                subTexIdx == SHIP1S_TEX || subTexIdx == SHIP1W_TEX) {
                // This was a ship of length 1, so we'll mark the sunk flag in the
                // current fired cell and update the ships count.
                fleet[row][col] |= SHIP_SUNK;
                shipsCount[SHIP_TYPE1]--;
                manager->SunkSound().Play();

            } else {
                // Whe the length is more than 1 unit we'll need to get the type of the ship.
                int shipType = -1;
                if (fleet[row][col] & SHIP2_CELL) {
                    shipType = SHIP_TYPE2;
                } else if (fleet[row][col] & SHIP3_CELL) {
                    shipType = SHIP_TYPE3;
                } else if (fleet[row][col] & SHIP4_CELL) {
                    shipType = SHIP_TYPE4;
                }
                // It's supposed to alway enter inside this if, but I put it just as a
                // precaution because sometimes you are so tired when programming that
                // you don't know what the hell are you doing. But the tragic of that
                // cases is that you still will hope that this ugly code will works.
                if (shipType != -1) {
                    const int VERTICAL = 1, HORIZONTAL = 2;
                    // So we know the type and that give us the length. Now we have to
                    // obtain the "start" coordinates of the ship and the orientation.
                    int orientation = 0;
                    int shipLen = GetTypeLength(shipType), r = row, c = col;
                    if (subTexIdx == SHIPFN_TEX || subTexIdx == SHIPBS_TEX) {
                        orientation = VERTICAL;

                    } else if (subTexIdx == SHIPFS_TEX || subTexIdx == SHIPBN_TEX) {
                        // When you're at the end of the ship a formula will work
                        // to get the start coordinate of the ship.
                        orientation = VERTICAL;
                        r = row + 1 - shipLen;

                    } else if (subTexIdx == SHIPMN_TEX || subTexIdx == SHIPMS_TEX) {
                        // When you're at the middle, you have to move to the start
                        // just to find the coordinate (checking the texture).
                        orientation = VERTICAL;
                        do {
                            --r;
                            subTexIdx = fleet[r][c] & MASK_TEX1;
                        } while (subTexIdx != SHIPFN_TEX && subTexIdx != SHIPBS_TEX);

                    } else if (subTexIdx == SHIPFW_TEX || subTexIdx == SHIPBE_TEX) {
                        orientation = HORIZONTAL;

                    } else if (subTexIdx == SHIPFE_TEX || subTexIdx == SHIPBW_TEX) {
                        // When you're at the end of the ship a formula will work
                        // to get the start coordinate of the ship.
                        orientation = HORIZONTAL;
                        c = col + 1 - shipLen;

                    } else if (subTexIdx == SHIPME_TEX || subTexIdx == SHIPMW_TEX) {
                        // When you're at the middle, you have to move to the start
                        // just to find the coordinate (checking the texture).
                        orientation = HORIZONTAL;
                        do {
                            --c;
                            subTexIdx = fleet[r][c] & MASK_TEX1;
                        } while (subTexIdx != SHIPFW_TEX && subTexIdx != SHIPBE_TEX);
                    }
                    // After obtain the start coordinates and the orientation, we'll
                    // check if the current hit ship is sunk or not. If it's sunk we'll
                    // update the ships count the sunk flag in each cell.
                    if (orientation == VERTICAL) {
                        bool sunk = true;
                        for (int i = 0; i < shipLen; ++i) {
                            if (!IsHit(r + i, c)) {
                                sunk = false;
                                break;
                            }
                        }
                        if (sunk) {
                            shipsCount[shipType]--;
                            for (int i = 0; i < shipLen; ++i) {
                                fleet[r + i][c] |= SHIP_SUNK;
                            }
                            manager->SunkSound().Play();
                        } else {
                            manager->HitSound().Play();
                        }
                    } else if (orientation == HORIZONTAL) {
                        bool sunk = true;
                        for (int i = 0; i < shipLen; ++i) {
                            if (!IsHit(r, c + i)) {
                                sunk = false;
                                break;
                            }
                        }
                        if (sunk) {
                            shipsCount[shipType]--;
                            for (int i = 0; i < shipLen; ++i) {
                                fleet[r][c + i] |= SHIP_SUNK;
                            }
                            manager->SunkSound().Play();
                        } else {
                            manager->HitSound().Play();
                        }
                    }
                }
            }
        } else {
            manager->WaterSound().Play();
        }
        // We have to call the "on update" delegate to end the
        // turn of the player and check the victory condition.
        onUpdate();
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets current cell position inside the board of the mouse coordinates.
 */
sf::Vector2i BattleshipBoard::InnerData::GetBoardCoords() {
    auto & mouseCoords = CoreManager::Instance()->GetMousePosition();
    if (MathUtil::PointInside(mouseCoords, boardArea)) {
        return sf::Vector2i(
            (mouseCoords.x - boardArea.left) / CELL_WIDTH,
            (mouseCoords.y - boardArea.top) / CELL_HEIGHT
        );
    } else {
        return sf::Vector2i(-1, -1);
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets the screen coordinates of a cell position inside the board.
 */
sf::Vector2i BattleshipBoard::InnerData::GetPosition(const sf::Vector2i & coords) {
    if (coords.x != -1 && coords.y != -1) {
        return sf::Vector2i(
            coords.x * CELL_WIDTH + boardArea.left,
            coords.y * CELL_HEIGHT + boardArea.top
        );
    } else {
        return sf::Vector2i();
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets the legth of a type of ship.
 */
int BattleshipBoard::InnerData::GetTypeLength(int type) {
    if (type == SHIP_TYPE1) {
        return 1;
    } else if (type == SHIP_TYPE2) {
        return 2;
    } else if (type == SHIP_TYPE3) {
        return 3;
    } else if (type == SHIP_TYPE4) {
        return 4;
    } else {
        return 0;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets all the empty coordinates for the current type of ship.
 */
void BattleshipBoard::InnerData::GetEmptyCoords(std::vector<sf::Vector2i> & emptyCoords) {
    // This method is used in the automatic mode to put a fleet inside the board.
    int typeLen = GetCurrentTypeLength();
    int maxSize = MAX_SIZE - typeLen;
    auto isEmpty = [&] (int r, int c) -> bool {
        return IsInside(r, c) ? empty[r][c] : false;
    };
    for (int i = 0; i <= maxSize; ++i) {
        for (int j = 0; j <= maxSize; ++j) {
            if (isEmpty(i, j)) {
                // When a cell is empty we'll check that a ship can be placed there.
                bool nothing = true;
                if (currentShipDir == DIR_N || currentShipDir == DIR_S) {
                    for (int k = 0; k < typeLen; ++k) {
                        if (!isEmpty(i + k, j)) {
                            nothing = false;
                            break;
                        }
                    }
                } else {
                    for (int k = 0; k < typeLen; ++k) {
                        if (!isEmpty(i, j + k)) {
                            nothing = false;
                            break;
                        }
                    }
                }
                // If we can place a ship, we'll add the coordinates as a candidate.
                if (nothing) {
                    emptyCoords.push_back(sf::Vector2i(j, i));
                }
            }
        }
    }
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the board.
 */
void BattleshipBoard::Initialize() {
    data_.reset(new InnerData());
    data_->core = CoreManager::Instance();
    data_->manager = BattleshipManager::Instance();
    auto & tileset = data_->manager->Tileset();

    // Delegate data.
    data_->onUpdate = emptyAction_;
    data_->onFinish = emptyAction_;

    // Texture data.
    data_->shipsTextures[SHIP_TYPE1][DIR_N].Load(tileset, sf::IntRect(121, 104, 24, 24));
    data_->shipsTextures[SHIP_TYPE1][DIR_E].Load(tileset, sf::IntRect(97, 152, 24, 24));
    data_->shipsTextures[SHIP_TYPE1][DIR_S].Load(tileset, sf::IntRect(193, 200, 24, 24));
    data_->shipsTextures[SHIP_TYPE1][DIR_W].Load(tileset, sf::IntRect(1, 224, 24, 24));

    data_->shipsTextures[SHIP_TYPE2][DIR_N].Load(tileset, sf::IntRect(145, 104, 24, 48));
    data_->shipsTextures[SHIP_TYPE2][DIR_E].Load(tileset, sf::IntRect(73, 176, 48, 24));
    data_->shipsTextures[SHIP_TYPE2][DIR_S].Load(tileset, sf::IntRect(169, 176, 24, 48));
    data_->shipsTextures[SHIP_TYPE2][DIR_W].Load(tileset, sf::IntRect(1, 200, 48, 24));

    data_->shipsTextures[SHIP_TYPE3][DIR_N].Load(tileset, sf::IntRect(169, 104, 24, 72));
    data_->shipsTextures[SHIP_TYPE3][DIR_E].Load(tileset, sf::IntRect(49, 200, 72, 24));
    data_->shipsTextures[SHIP_TYPE3][DIR_S].Load(tileset, sf::IntRect(145, 152, 24, 72));
    data_->shipsTextures[SHIP_TYPE3][DIR_W].Load(tileset, sf::IntRect(1, 176, 72, 24));

    data_->shipsTextures[SHIP_TYPE4][DIR_N].Load(tileset, sf::IntRect(193, 104, 24, 96));
    data_->shipsTextures[SHIP_TYPE4][DIR_E].Load(tileset, sf::IntRect(25, 224, 96, 24));
    data_->shipsTextures[SHIP_TYPE4][DIR_S].Load(tileset, sf::IntRect(121, 128, 24, 96));
    data_->shipsTextures[SHIP_TYPE4][DIR_W].Load(tileset, sf::IntRect(1, 152, 96, 24));

    const int CELL_TEX_START_X = 96, CELL_TEX_Y = 344;
    auto rect = sf::IntRect(CELL_TEX_START_X, CELL_TEX_Y, CELL_WIDTH, CELL_HEIGHT);
    for (int i = 0; i < MAX_SHIP_TEXS; ++i) {
        data_->shipsFragTexs[i].Load(tileset, rect);
        rect.left += CELL_WIDTH;
        if (rect.left >= 470) {
            rect.left = CELL_TEX_START_X;
            rect.top += CELL_HEIGHT;
        }
    }

    data_->cellGrayTexture.Load(tileset, sf::IntRect(24, CELL_TEX_Y, CELL_WIDTH, CELL_HEIGHT));
    data_->cellHoverTexture.Load(tileset, sf::IntRect(48, CELL_TEX_Y, CELL_WIDTH, CELL_HEIGHT));
    data_->cellClickTexture.Load(tileset, sf::IntRect(72, CELL_TEX_Y, CELL_WIDTH, CELL_HEIGHT));

    data_->playerWaterHitTexture.Load(tileset, sf::IntRect(480, 344, CELL_WIDTH, CELL_HEIGHT));
    data_->enemyShipHitTexture.Load(tileset, sf::IntRect(480, 368, CELL_WIDTH, CELL_HEIGHT));

    data_->boardTexture.Load(tileset, sf::IntRect(217, 77, 263, 267));

    // Logic data.
    Reset();
    SetBoardPosition(28, 23);
    ChangeShipType(SHIP_TYPE4);
    data_->currentShipDir = DIR_N;
    data_->currentMode = INIT_MODE;
    data_->playSounds = true;

    // AI data.
    data_->aiState = AI_STATE_RANDOM;

    data_->dirOffsets[DIR_N] = sf::Vector2i( 0, -1);
    data_->dirOffsets[DIR_E] = sf::Vector2i( 1,  0);
    data_->dirOffsets[DIR_S] = sf::Vector2i( 0,  1);
    data_->dirOffsets[DIR_W] = sf::Vector2i(-1,  0);

    data_->nearOffsets[0] = sf::Vector2i(-1, -1);
    data_->nearOffsets[1] = sf::Vector2i( 0, -1);
    data_->nearOffsets[2] = sf::Vector2i( 1, -1);
    data_->nearOffsets[3] = sf::Vector2i(-1,  0);
    data_->nearOffsets[4] = sf::Vector2i( 1,  0);
    data_->nearOffsets[5] = sf::Vector2i(-1,  1);
    data_->nearOffsets[6] = sf::Vector2i( 0,  1);
    data_->nearOffsets[7] = sf::Vector2i( 1,  1);
}

//--------------------------------------------------------------------------------

/**
 * Draws the board.
 */
void BattleshipBoard::Draw() {
    data_->boardTexture.Draw(data_->boardPosition);

    if (data_->currentMode == PUT_MODE) {
        data_->DrawPutFleet();

    } else if (data_->currentMode == PLAYER_MODE) {
        data_->DrawPlayer();

    } else if (data_->currentMode == ENEMY_MODE) {
        data_->DrawEnemy();
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the board.
 */
void BattleshipBoard::Update() {
    // This update method will change the field that store the current cell
    // under the mouse that can be an interactive element.
    auto coords = data_->GetBoardCoords();
    if (coords.x != -1 && coords.y != -1) {
        if (data_->currentMode == PUT_MODE) {
            // In the put mode we'll set the coordinates check the current
            // type of ship and the current direction, just to avoid that
            // case where you can put part of a ship beyond the border.
            auto setCoords = [&] (int len) {
                if (data_->currentShipDir == DIR_N || data_->currentShipDir == DIR_S) {
                    if (coords.y <= MAX_SIZE - len) {
                        data_->cellCoords = coords;
                    } else {
                        data_->cellCoords.x = coords.x;
                        data_->cellCoords.y = MAX_SIZE - len;
                    }
                } else if (data_->currentShipDir == DIR_E || data_->currentShipDir == DIR_W) {
                    if (coords.x <= MAX_SIZE - len) {
                        data_->cellCoords = coords;
                    } else {
                        data_->cellCoords.y = coords.y;
                        data_->cellCoords.x = MAX_SIZE - len;
                    }
                }
            };
            // We check the current type and calculate the final cell coordinate.
            if (data_->currentShipType == SHIP_TYPE1) {
                data_->cellCoords = coords;
            } else if (data_->currentShipType == SHIP_TYPE2) {
                setCoords(2);
            } else if (data_->currentShipType == SHIP_TYPE3) {
                setCoords(3);
            } else if (data_->currentShipType == SHIP_TYPE4) {
                setCoords(4);
            }

        } else if (data_->currentMode == PLAYER_MODE) {
            data_->cellCoords = sf::Vector2i(-1, -1);

        } else if (data_->currentMode == ENEMY_MODE) {
            // In the enemy board mode, we'll check that the current
            // coordinates can be hit by the current player.
            if (!data_->IsHit(coords)) {
                data_->cellCoords = coords;
            } else {
                data_->cellCoords = sf::Vector2i(-1, -1);
            }
        }
    } else {
        data_->cellCoords = sf::Vector2i(-1, -1);
    }
}

//--------------------------------------------------------------------------------

/**
 * Executes a click on the board.
 */
void BattleshipBoard::Click() {
    if (data_->cellCoords.x != -1 && data_->cellCoords.y != -1) {
        if (data_->currentMode == PUT_MODE) {
            data_->PutShip(data_->GetCurrentTypeLength());
        } else if (data_->currentMode == ENEMY_MODE) {
            data_->FireOver(data_->cellCoords.y, data_->cellCoords.x);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Sets the board on put mode.
 */
void BattleshipBoard::SetModeAIPutFleet() {
    data_->currentMode = PUT_MODE;
    data_->onUpdate = emptyAction_;
    data_->onFinish = emptyAction_;
    SetBoardPosition(28, 23);
}

//--------------------------------------------------------------------------------

/**
 * Sets the board on put mode.
 */
void BattleshipBoard::SetModePutFleet(Action onUpdate, Action onFinish) {
    data_->currentMode = PUT_MODE;
    data_->onUpdate = onUpdate;
    data_->onFinish = onFinish;
    SetBoardPosition(28, 23);
}

//--------------------------------------------------------------------------------

/**
 * Sets the board on player mode.
 */
void BattleshipBoard::SetModePlayer() {
    data_->currentMode = PLAYER_MODE;
    data_->onUpdate = emptyAction_;
    SetBoardPosition(28, 23);
}

//--------------------------------------------------------------------------------

/**
 * Sets the board on enemy mode.
 */
void BattleshipBoard::SetModeEnemy(Action onUpdate) {
    data_->currentMode = ENEMY_MODE;
    data_->onUpdate = onUpdate;
    SetBoardPosition(348, 23);
}

//--------------------------------------------------------------------------------

/**
 * Resets the board.
 */
void BattleshipBoard::Reset() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            data_->fleet[i][j] = WATER_CELL;
            data_->empty[i][j] = true;
        }
    }

    ResetCount();
    data_->fleetPutFinished = false;
    data_->onUpdate();
}

//--------------------------------------------------------------------------------

/**
 * Resets the count.
 */
void BattleshipBoard::ResetCount() {
    data_->shipsCount[SHIP_TYPE1] = 4;
    data_->shipsCount[SHIP_TYPE2] = 3;
    data_->shipsCount[SHIP_TYPE3] = 2;
    data_->shipsCount[SHIP_TYPE4] = 1;
}

//--------------------------------------------------------------------------------

/**
 * Sets the position of the board.
 */
void BattleshipBoard::SetBoardPosition(int x, int y) {
    data_->boardPosition = sf::Vector2i(x, y);
    data_->boardArea = sf::IntRect(x + BOARD_OFFSETX, y + BOARD_OFFSETY, 240, 240);
}

//--------------------------------------------------------------------------------

/**
 * Checks if the mouse is over the board.
 */
bool BattleshipBoard::MouseOverBoard() {
    auto boardCoords = data_->GetBoardCoords();
    return boardCoords.x != -1 && boardCoords.y != -1;
}

//--------------------------------------------------------------------------------

/**
 * Changes the current ship direction.
 */
void BattleshipBoard::NextShipDirection() {
    data_->currentShipDir = (data_->currentShipDir + 1) % MAX_SHIP_DIRS;
}

//--------------------------------------------------------------------------------

/**
 * Changes the current ship type.
 */
void BattleshipBoard::ChangeShipType(int type) {
    if (0 <= type && type < MAX_SHIP_TYPES) {
        if (data_->shipsCount[type] > 0) {
            data_->currentShipType = type;
            data_->cellCoords = sf::Vector2i(-1, -1);
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets a ship type count.
 */
int BattleshipBoard::ShipTypeCount(int type) {
    if (0 <= type && type < MAX_SHIP_TYPES) {
        return data_->shipsCount[type];
    } else {
        return 0;
    }
}

//--------------------------------------------------------------------------------

/**
 * Gets if there is any ship available.
 */
bool BattleshipBoard::AnyShipAvailable() {
    for (int i = 0; i < MAX_SHIP_TYPES; ++i) {
        if (data_->shipsCount[i] > 0) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------------

/**
 * Sets a random fleet.
 */
void BattleshipBoard::SetRandomFleet() {
    // This method must be used only in put mode.
    if (data_->currentMode == PUT_MODE) {
        // Initialize some logic fields.
        Reset();
        data_->playSounds = false;
        ChangeShipType(SHIP_TYPE4);
        // And until the algorithm finish, we'll get a random orientation,
        // then we'll get the empty coordinates inside the board.
        while (!data_->fleetPutFinished) {
            data_->currentShipDir = data_->core->Random(MAX_SHIP_DIRS);
            std::vector<sf::Vector2i> emptyCoords;
            data_->GetEmptyCoords(emptyCoords);
            // If there is no empty coordinates, we'll reset the logic field used
            // here. Otherwise we'll get a random coordinate and put the ship.
            if (emptyCoords.empty()) {
                Reset();
            } else {
                data_->cellCoords = emptyCoords[data_->core->Random(emptyCoords.size())];
                data_->PutShip(data_->GetCurrentTypeLength());
            }
        }
        data_->playSounds = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Executes a fire over a position.
 */
void BattleshipBoard::MachineFires() {
    if (data_->currentMode == PLAYER_MODE) {
        if (data_->aiState == AI_STATE_RANDOM) {
            // Lets find all the candidate cells.
            std::vector<sf::Vector2i> candidates;
            for (int i = 0; i < MAX_SIZE; ++i) {
                for (int j = 0; j < MAX_SIZE; ++j) {
                    // The candidate cell must be a not hit one.
                    if (!data_->IsHit(i, j)) {
                        // The cell cannot be near to a hit ship.
                        auto currentCoord = sf::Vector2i(j, i);
                        bool shipNotNear = true;
                        for (int k = 0; k < MAX_NEAR_OFFSETS; ++k) {
                            auto nearCoord = currentCoord + data_->nearOffsets[k];
                            if (data_->IsShipWithHitSafe(nearCoord)) {
                                shipNotNear = false;
                                break;
                            }
                        }
                        // Add the cell that is not near to a hit ship.
                        if (shipNotNear) {
                            candidates.push_back(currentCoord);
                        }
                    }
                }
            }


            // Select a random candidate and fire over that selected cell.
            data_->aiLastCell = candidates[data_->core->Random(candidates.size())];
            data_->FireOver(data_->aiLastCell.y, data_->aiLastCell.x);
            // If the cell has a ship change the state if not sunk.
            if (data_->IsShip(data_->aiLastCell) && !data_->IsSunk(data_->aiLastCell)) {
                data_->aiState = AI_STATE_FINDING;
                data_->aiNextDir = data_->core->Random(MAX_SHIP_DIRS);
            }

        } else if (data_->aiState == AI_STATE_FINDING) {
            // Get the next coordinates to shoot, and check that this coordinates
            // are inside the board and are not hit previously.
            auto nextCoords = data_->aiLastCell + data_->dirOffsets[data_->aiNextDir];
            while (!data_->IsInside(nextCoords) || data_->IsHit(nextCoords)) {
                data_->aiNextDir = (data_->aiNextDir + 1) % MAX_SHIP_DIRS;
                nextCoords = data_->aiLastCell + data_->dirOffsets[data_->aiNextDir];
            }

            // Fire over the next coordinates.
            data_->FireOver(nextCoords.y, nextCoords.x);
            if (data_->IsWater(nextCoords)) {
                // When hit on water select the next direction.
                data_->aiNextDir = (data_->aiNextDir + 1) % MAX_SHIP_DIRS;

            } else if (data_->IsSunk(nextCoords)) {
                // When the ship is sunk return to the random state.
                data_->aiState = AI_STATE_RANDOM;

            } else if (data_->IsShip(nextCoords)) {
                // When the ship is not sunk but another fragment have
                // been hit, we'll change the state of the AI.
                data_->aiLastCell = nextCoords;
                data_->aiState = AI_STATE_FOUND;
            }

        } else if (data_->aiState == AI_STATE_FOUND) {
            auto oppositeDir = [] (int dir) -> int {
                if (dir == DIR_N) {
                    return DIR_S;
                } else if (dir == DIR_E) {
                    return DIR_W;
                } else if (dir == DIR_S) {
                    return DIR_N;
                } else if (dir == DIR_W) {
                    return DIR_E;
                } else {
                    return 0;
                }
            };

            // Get the next coordinates to shoot, and check that this coordinates
            // are inside the board and are not hit previously.
            auto nextCoords = data_->aiLastCell + data_->dirOffsets[data_->aiNextDir];
            if (!data_->IsInside(nextCoords) || data_->IsHit(nextCoords)) {
                data_->aiNextDir = oppositeDir(data_->aiNextDir);
                nextCoords = data_->aiLastCell + data_->dirOffsets[data_->aiNextDir];
                while (data_->IsHitSafe(nextCoords)) {
                    nextCoords += data_->dirOffsets[data_->aiNextDir];
                }
            }

            // Fire over the next coordinates.
            data_->FireOver(nextCoords.y, nextCoords.x);
            if (data_->IsWater(nextCoords)) {
                // When hit on water select the opposite direction.
                data_->aiNextDir = oppositeDir(data_->aiNextDir);
                nextCoords = data_->aiLastCell + data_->dirOffsets[data_->aiNextDir];
                while (data_->IsHitSafe(nextCoords)) {
                    nextCoords += data_->dirOffsets[data_->aiNextDir];
                }
                data_->aiLastCell = nextCoords - data_->dirOffsets[data_->aiNextDir];

            } else if (data_->IsSunk(nextCoords)) {
                // When the ship is sunk return to the random state.
                data_->aiState = AI_STATE_RANDOM;

            } else if (data_->IsShip(nextCoords)) {
                // When the ship is not sunk but another fragment have
                // been hit, we'll continue with the current state.
                data_->aiLastCell = nextCoords;
            }
        }
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
BattleshipBoard::BattleshipBoard() : data_(nullptr) {
}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
BattleshipBoard::BattleshipBoard(const BattleshipBoard & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
BattleshipBoard::~BattleshipBoard() {
    data_ = nullptr;
}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
BattleshipBoard & BattleshipBoard::operator =(const BattleshipBoard & source) {
    data_ = source.data_;
    return *this;
}
