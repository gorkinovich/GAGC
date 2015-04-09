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

#include "PuckmanManager.h"
#include "PuckmanManagerInner.h"
#include <System/ForEach.h>
#include <Games/Puckman/PuckmanPalette.h>

using namespace Puckman;

//********************************************************************************
// Constants
//********************************************************************************

const int START_X = 208;
const int START_Y =  46;

const int SPACE_INDEX = 58;

//********************************************************************************
// Tile Methods
//********************************************************************************

/**
 * Transforms a tile coordinates to a 2D vector.
 */
sf::Vector2i Manager::CellCoordsToVector2(int row, int col) {
    return sf::Vector2i(START_X + col * CELL_WIDTH, START_Y + row * CELL_WIDTH);
}

//--------------------------------------------------------------------------------

/**
 * Clear a tile from the screen.
 */
void Manager::ClearCell(int row, int col) {
    data_->blankCell_.Draw(CellCoordsToVector2(row, col));
}

//--------------------------------------------------------------------------------

/**
 * Draws a tile on the screen.
 */
void Manager::DrawTile(int row, int col, int index, const sf::Color & color) {
    //ClearCell(row, col);
    data_->tiles_[index].Draw(CellCoordsToVector2(row, col), color);
}

//--------------------------------------------------------------------------------

/**
 * Draws a tile on the screen.
 */
void Manager::DrawTile(int row, int col, int index) {
    DrawTile(row, col, index, sf::Color::White);
}

//********************************************************************************
// Text Methods
//********************************************************************************

/**
 * Transforms a character value to the index value used to draw text.
 */
int Manager::GetIndexFromChar(char c) {
    if((47 < c && c < 58) || (64 < c && c < 91)) {
        return c;
    } else if(96 < c && c < 123) { // a-z
        return c - 32;
    } else if(c == 33) { // !
        return 59;
    } else if(c == 34) { // "
        return 60;
    } else if(c == 45) { // -
        return 61;
    } else if(c == 46) { // .
        return 62;
    } else if(c == 47) { // /
        return 63;
    } else { // Anything else will be an space.
        return SPACE_INDEX;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws a text on the screen.
 */
void Manager::DrawText(int row, int col, const std::string & text, const sf::Color & color) {
    ForEach(text, [&] (char c) {
        DrawTile(row, col, GetIndexFromChar(c), color);
        ++col;
    });
}

//--------------------------------------------------------------------------------

/**
 * Draws a text on the screen.
 */
void Manager::DrawText(int row, int col, const std::string & text) {
    DrawText(row, col, text, Palette::White);
}

//--------------------------------------------------------------------------------

/**
 * Draws a text on the screen, aligned to the left.
 */
void Manager::DrawTextLeft(int row, int col, const std::string & text, const sf::Color & color) {
    DrawText(row, col - (text.size() - 1), text, color);
}

//--------------------------------------------------------------------------------

/**
 * Draws a text on the screen, aligned to the left.
 */
void Manager::DrawTextLeft(int row, int col, const std::string & text) {
    DrawTextLeft(row, col, text, Palette::White);
}

//--------------------------------------------------------------------------------

/**
 * Draws a symbol on the screen.
 */
void Manager::DrawSymbol(int row, int col, FontSymbolEnum symbol, const sf::Color & color) {
    switch(symbol) {
    case FontSymbol::Copyright:
        DrawTile(row, col, 64, color);
        break;

    case FontSymbol::Pts:
        //ClearCell(row, col + 1);
        DrawTile(row, col, 91, color);
        break;

    case FontSymbol::N100:
        DrawTile(row, col, 20, color);
        DrawTile(row, col + 1, 24, color);
        break;

    case FontSymbol::N300:
        DrawTile(row, col, 21, color);
        DrawTile(row, col + 1, 24, color);
        break;

    case FontSymbol::N500:
        DrawTile(row, col, 22, color);
        DrawTile(row, col + 1, 24, color);
        break;

    case FontSymbol::N700:
        DrawTile(row, col, 23, color);
        DrawTile(row, col + 1, 24, color);
        break;

    case FontSymbol::N1000:
        DrawTile(row, col, 25, color);
        DrawTile(row, col + 1, 32, color);
        DrawTile(row, col + 2, 33, color);
        break;

    case FontSymbol::N2000:
        DrawTile(row, col, 26, color);
        DrawTile(row, col + 1, 27, color);
        DrawTile(row, col + 2, 32, color);
        DrawTile(row, col + 3, 33, color);
        break;

    case FontSymbol::N3000:
        DrawTile(row, col, 28, color);
        DrawTile(row, col + 1, 29, color);
        DrawTile(row, col + 2, 32, color);
        DrawTile(row, col + 3, 33, color);
        break;

    case FontSymbol::N5000:
        DrawTile(row, col, 30, color);
        DrawTile(row, col + 1, 31, color);
        DrawTile(row, col + 2, 32, color);
        DrawTile(row, col + 3, 33, color);
        break;

    case FontSymbol::Namco:
        //for(int i = 1; i < 7; i++) {
        //    ClearCell(row, col + i);
        //}
        DrawTile(row, col, 92, color);
        break;

    case FontSymbol::UpRight:
        DrawTile(row, col, 16, color);
        break;

    case FontSymbol::DownRight:
        DrawTile(row, col, 17, color);
        break;

    case FontSymbol::UpLeft:
        DrawTile(row, col, 18, color);
        break;

    case FontSymbol::DownLeft:
        DrawTile(row, col, 19, color);
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws a symbol on the screen.
 */
void Manager::DrawSymbol(int row, int col, FontSymbolEnum symbol) {
    DrawSymbol(row, col, symbol, Palette::White);
}

//********************************************************************************
// Sprite Methods
//********************************************************************************

/**
 * Transforms a sprite coordinates to a 2D vector.
 */
sf::Vector2i Manager::SpriteCoordsToVector2(int x, int y) {
    return sf::Vector2i(START_X + x, START_Y + y);
}

//--------------------------------------------------------------------------------

/**
 * Clear a sprite from the screen.
 */
void Manager::ClearSprite(int x, int y) {
    data_->blankSprite_.Draw(SpriteCoordsToVector2(x, y));
}

//--------------------------------------------------------------------------------

/**
 * Draws a sprite on the screen.
 */
void Manager::DrawSprite(int x, int y, int index) {
    data_->sprites_[index].Draw(SpriteCoordsToVector2(x, y));
}
