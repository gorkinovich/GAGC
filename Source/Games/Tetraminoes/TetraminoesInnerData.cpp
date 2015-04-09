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

#include "TetraminoesManager.h"
#include "TetraminoesInnerData.h"
#include <sstream>
#include <System/CoreManager.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>

//********************************************************************************
// Constants
//********************************************************************************

const int BLOCK_01   =  0; // 1.DarkBlue/1.0.1.png
const int BLOCK_02   =  1; // 1.DarkBlue/1.1.1.png
const int BLOCK_03   =  2; // 1.DarkBlue/1.1.2.png
const int BLOCK_04   =  3; // 1.DarkBlue/1.2.1.png
const int BLOCK_05   =  4; // 1.DarkBlue/1.2.2.png
const int BLOCK_06   =  5; // 1.DarkBlue/1.2.3.png
const int BLOCK_07   =  6; // 1.DarkBlue/1.2.4.png

const int BLOCK_09   =  7; // 2.Yellow/2.0.1.png
const int BLOCK_10   =  8; // 2.Yellow/2.1.1.png
const int BLOCK_11   =  9; // 2.Yellow/2.1.2.png
const int BLOCK_12   = 10; // 2.Yellow/2.1.3.png
const int BLOCK_13   = 11; // 2.Yellow/2.1.4.png
const int BLOCK_14   = 12; // 2.Yellow/2.2.1.png
const int BLOCK_15   = 13; // 2.Yellow/2.2.2.png
const int BLOCK_16   = 14; // 2.Yellow/2.2.3.png
const int BLOCK_17   = 15; // 2.Yellow/2.2.4.png
const int BLOCK_18   = 16; // 2.Yellow/2.2.5.png
const int BLOCK_19   = 17; // 2.Yellow/2.2.6.png

const int BLOCK_21   = 18; // 3.Murrey/3.0.1.png
const int BLOCK_22   = 19; // 3.Murrey/3.1.1.png
const int BLOCK_23   = 20; // 3.Murrey/3.1.2.png
const int BLOCK_24   = 21; // 3.Murrey/3.1.3.png
const int BLOCK_25   = 22; // 3.Murrey/3.1.4.png
const int BLOCK_26   = 23; // 3.Murrey/3.2.1.png
const int BLOCK_27   = 24; // 3.Murrey/3.2.2.png
const int BLOCK_28   = 25; // 3.Murrey/3.2.3.png
const int BLOCK_29   = 26; // 3.Murrey/3.2.4.png
const int BLOCK_30   = 27; // 3.Murrey/3.2.5.png
const int BLOCK_31   = 28; // 3.Murrey/3.2.6.png

const int BLOCK_33   = 29; // 4.Orange/4.0.1.png
const int BLOCK_34   = 30; // 4.Orange/4.1.1.png
const int BLOCK_35   = 31; // 4.Orange/4.1.2.png
const int BLOCK_36   = 32; // 4.Orange/4.1.3.png
const int BLOCK_37   = 33; // 4.Orange/4.1.4.png
const int BLOCK_38   = 34; // 4.Orange/4.2.1.png
const int BLOCK_39   = 35; // 4.Orange/4.2.2.png
const int BLOCK_40   = 36; // 4.Orange/4.2.3.png
const int BLOCK_41   = 37; // 4.Orange/4.2.4.png

const int BLOCK_43   = 38; // 5.LightBlue/5.0.1.png
const int BLOCK_44   = 39; // 5.LightBlue/5.1.1.png
const int BLOCK_45   = 40; // 5.LightBlue/5.1.2.png
const int BLOCK_46   = 41; // 5.LightBlue/5.1.3.png
const int BLOCK_47   = 42; // 5.LightBlue/5.1.4.png
const int BLOCK_48   = 43; // 5.LightBlue/5.2.1.png
const int BLOCK_49   = 44; // 5.LightBlue/5.2.2.png
const int BLOCK_50   = 45; // 5.LightBlue/5.2.3.png
const int BLOCK_51   = 46; // 5.LightBlue/5.2.4.png

const int BLOCK_53   = 47; // 6.Green/6.0.1.png
const int BLOCK_54   = 48; // 6.Green/6.1.1.png
const int BLOCK_55   = 49; // 6.Green/6.1.2.png
const int BLOCK_56   = 50; // 6.Green/6.1.3.png
const int BLOCK_57   = 51; // 6.Green/6.1.4.png
const int BLOCK_58   = 52; // 6.Green/6.2.1.png
const int BLOCK_59   = 53; // 6.Green/6.2.2.png
const int BLOCK_60   = 54; // 6.Green/6.2.3.png
const int BLOCK_73   = 55; // 6.Green/6.2.4.png
const int BLOCK_74   = 56; // 6.Green/6.2.5.png
const int BLOCK_61   = 57; // 6.Green/6.3.1.png
const int BLOCK_62   = 58; // 6.Green/6.3.2.png
const int BLOCK_63   = 59; // 6.Green/6.3.3.png
const int BLOCK_64   = 60; // 6.Green/6.3.4.png

const int BLOCK_66   = 61; // 7.Red/7.0.1.png
const int BLOCK_67   = 62; // 7.Red/7.1.1.png
const int BLOCK_68   = 63; // 7.Red/7.1.2.png
const int BLOCK_69   = 64; // 7.Red/7.1.3.png
const int BLOCK_70   = 65; // 7.Red/7.1.4.png
const int BLOCK_71   = 66; // 7.Red/7.2.1.png
const int BLOCK_72   = 67; // 7.Red/7.2.2.png

//--------------------------------------------------------------------------------

const int TYPE_BLUE   = 0;
const int TYPE_YELLOW = 1;
const int TYPE_MURREY = 2;
const int TYPE_ORANGE = 3;
const int TYPE_CYAN   = 4;
const int TYPE_GREEN  = 5;
const int TYPE_RED    = 6;

//--------------------------------------------------------------------------------

const int TYPE_WIDTHS[]  = { 32, 32, 32, 32, 32, 32, 16 };
const int TYPE_HEIGHTS[] = { 32, 48, 48, 48, 48, 48, 64 };

const int BLOCK_WIDTH  = 16;
const int BLOCK_HEIGHT = 16;

const int WORLD_START_X = 207;
const int WORLD_START_Y =  16;

const int EMPTY_CELL = -1;

// The maximum intervals of time allowed.
const int MAX_INTERVALS[] = {
    1000, 950, 900, 850, 800, 750, 700, 650, 600,
    550, 500, 450, 400, 350, 300, 250, 200, 150, 100, 50
};

// The numbers of lines to pass to the next level.
const int MAX_LINES[] = {
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 22, 24, 26, 28, 30, 32, 37, 42, 101
};

// The numbers of points the player receives when makes a line.
const int LINE_POINTS[] = {
    1, 2, 3, 4, 5, 6, 8, 10, 12, 14,
    16, 32, 48, 64, 128, 192, 256, 512, 768, 1024
};

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void TetraminoesManager::InnerData::Initialize() {
        core = CoreManager::Instance();

        // Load the textures of the game.
        tileset = core->LoadTexture("Content/Textures/Tetraminoes01.png");
        menu1Background = core->LoadTexture("Content/Textures/Tetraminoes02.png");
        menu2Background = core->LoadTexture("Content/Textures/Tetraminoes03.png");
        gameBackground = core->LoadTexture("Content/Textures/Tetraminoes04.png");

        int x = 96, y = 32;
        for (int i = 0; i < MAX_TYPES; ++i) {
            typesTextures[i].Load(tileset,
                sf::IntRect(x, y, TYPE_WIDTHS[i], TYPE_HEIGHTS[i]));
            x += TYPE_WIDTHS[i];
        }
        x = 0, y = 0;
        int textureLimit = tileset->getSize().x;
        for (int i = 0; i < MAX_BLOCKS; ++i) {
            blocksTextures[i].Load(tileset,
                sf::IntRect(x, y, BLOCK_WIDTH, BLOCK_HEIGHT));
            x += BLOCK_WIDTH;
            if (x >= textureLimit) {
                x = 0; y += BLOCK_HEIGHT;
            }
        }

        // Set the HUD data of the game.
        levelLabel.reset(new SimpleLabel("1", 0, 188,
            sf::Color::White, core->Retro70Font()));

        linesLabel.reset(new SimpleLabel("0", 392, 238,
            sf::Color::White, core->Retro70Font()));

        scoreLabel.reset(new SimpleLabel("0", 392, 288,
            sf::Color::White, core->Retro70Font()));

        gameOverLabel.reset(new SimpleLabel("Game Over",
            0, 176, sf::Color::White, core->Retro70Font()));

        gamePaused1Label.reset(new SimpleLabel("Game",
            392, 126, sf::Color::White, core->Retro70Font()));

        gamePaused2Label.reset(new SimpleLabel("Paused",
            384, 138, sf::Color::White, core->Retro70Font()));

        auto language = core->Language();
        if (language == TEXT_LANGUAGE_SPANISH) {
            gameOverLabel->Text("Partida Terminada");
            gamePaused1Label->Text("Juego");
            gamePaused2Label->Text("Pausado");
        }

        GUIUtil::CenterLabel(levelLabel, 404);
        GUIUtil::CenterLabel(gameOverLabel, 288);
        UpdateHud();

        // Load the sounds of the game.
        keyboardSound.Load("Content/Sounds/TetraminoesKey.wav");
        hitSound.Load("Content/Sounds/TetraminoesHit.wav");
        fallSound.Load("Content/Sounds/TetraminoesFall.wav");
        lineSound.Load("Content/Sounds/TetraminoesLine.wav");

        // Initialize the logical fields.
        StartGame(TetraminoesManager::DIFFICULTY_EASY);
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void TetraminoesManager::InnerData::Release() {
    // Wait to the sound to end.
    keyboardSound.WaitUntilStop();

    // Unload the sounds of the game.
    keyboardSound.Unload();
    hitSound.Unload();
    fallSound.Unload();
    lineSound.Unload();

    // Unload the textures of the game.
    tileset = nullptr;
    menu1Background = nullptr;
    menu2Background = nullptr;
    gameBackground = nullptr;
}

//--------------------------------------------------------------------------------

/**
 * Starts a new game.
 */
void TetraminoesManager::InnerData::StartGame(int difficulty) {
    level = difficulty;
    lines = 0;
    score = 0;
    target = MAX_LINES[level - 1];

    time = 0;
    timeMark = 0;
    maxInterval = MAX_INTERVALS[level - 1];

    SetKeyPressed();
    InitWorld();

    SelectNextPiece();
    GetNextPiece();

    gameOver = false;
    gamePaused = false;

    UpdateHud();
}

//--------------------------------------------------------------------------------

/**
 * Initializes the game's world.
 */
void TetraminoesManager::InnerData::InitWorld() {
    for(int i = 0; i < WORLD_HEIGHT; i++) {
        for(int j = 0; j < WORLD_WIDTH; j++) {
            world[i][j] = EMPTY_CELL;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Renders the game's world.
 */
void TetraminoesManager::InnerData::RenderWorld() {
    for(int i = 0; i < WORLD_HEIGHT; i++) {
        for(int j = 0; j < WORLD_WIDTH; j++) {
            if(world[i][j] != EMPTY_CELL) {
                blocksTextures[world[i][j]].Draw(
                    j * BLOCK_WIDTH + WORLD_START_X,
                    i * BLOCK_HEIGHT + WORLD_START_Y
                );
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Selects the next piece in the game.
 */
void TetraminoesManager::InnerData::SelectNextPiece() {
    nextPiece = core->Random(MAX_TYPES);
}

//--------------------------------------------------------------------------------

/**
 * Renders the next piece.
 */
void TetraminoesManager::InnerData::RenderNextPiece() {
    const sf::Vector2i COORDS[] = {
        sf::Vector2i(391, 62), sf::Vector2i(391, 54), sf::Vector2i(391, 54),
        sf::Vector2i(391, 54), sf::Vector2i(391, 54), sf::Vector2i(391, 54),
        sf::Vector2i(399, 46)
    };
    typesTextures[nextPiece].Draw(COORDS[nextPiece]);
}

//--------------------------------------------------------------------------------

/**
 * Gets the next piece.
 */
void TetraminoesManager::InnerData::GetNextPiece() {
    const int START_X[] = { 4, 4, 4, 4, 4, 4, 5 };
    const int START_Y[] = { 1, 1, 1, 0, 0, 1, 1 };
    const int ORIENTATION[] = { 1, 2, 4, 2, 2, 4, 2 };
    piece = nextPiece;

    pieceX = START_X[piece];
    pieceY = START_Y[piece];
    pieceOrientation = ORIENTATION[piece];

    SelectNextPiece();

    if(CheckCollision()) {
        gameOver = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Renders the piece.
 */
void TetraminoesManager::InnerData::RenderPiece() {
    auto ColToX = [] (int c) { return c * BLOCK_WIDTH + WORLD_START_X;  };
    auto RowToY = [] (int r) { return r * BLOCK_HEIGHT + WORLD_START_Y; };

    switch (piece) {
    case TYPE_BLUE:
        blocksTextures[BLOCK_04].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
        blocksTextures[BLOCK_05].Draw(ColToX(pieceX + 1), RowToY(pieceY - 1));
        blocksTextures[BLOCK_06].Draw(ColToX(pieceX),     RowToY(pieceY));
        blocksTextures[BLOCK_07].Draw(ColToX(pieceX + 1), RowToY(pieceY));
        break;

    case TYPE_YELLOW:
        switch (pieceOrientation) {
        case 1:
            blocksTextures[BLOCK_10].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            blocksTextures[BLOCK_18].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_17].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_12].Draw(ColToX(pieceX - 1), RowToY(pieceY + 1));
            break;
        case 2:
            blocksTextures[BLOCK_13].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_19].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_16].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_10].Draw(ColToX(pieceX - 1), RowToY(pieceY - 1));
            break;
        case 3:
            blocksTextures[BLOCK_11].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_18].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_14].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            blocksTextures[BLOCK_13].Draw(ColToX(pieceX + 1), RowToY(pieceY - 1));
            break;
        case 4:
            blocksTextures[BLOCK_12].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_19].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_15].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_11].Draw(ColToX(pieceX + 1), RowToY(pieceY + 1));
            break;
        }
        break;

    case TYPE_MURREY:
        switch (pieceOrientation) {
        case 1:
            blocksTextures[BLOCK_22].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            blocksTextures[BLOCK_30].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_28].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_25].Draw(ColToX(pieceX + 1), RowToY(pieceY + 1));
            break;
        case 2:
            blocksTextures[BLOCK_25].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_31].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_26].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_23].Draw(ColToX(pieceX - 1), RowToY(pieceY + 1));
            break;
        case 3:
            blocksTextures[BLOCK_23].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_30].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_27].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            blocksTextures[BLOCK_24].Draw(ColToX(pieceX - 1), RowToY(pieceY - 1));
            break;
        case 4:
            blocksTextures[BLOCK_24].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_31].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_29].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_22].Draw(ColToX(pieceX + 1), RowToY(pieceY - 1));
            break;
        }
        break;

    case TYPE_ORANGE:
        switch (pieceOrientation) {
        case 1:
            blocksTextures[BLOCK_34].Draw(ColToX(pieceX + 1), RowToY(pieceY - 1));
            blocksTextures[BLOCK_41].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_38].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_35].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            break;
        case 2:
            blocksTextures[BLOCK_37].Draw(ColToX(pieceX + 1), RowToY(pieceY + 1));
            blocksTextures[BLOCK_40].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_39].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_36].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            break;
        }
        break;

    case TYPE_CYAN:
        switch (pieceOrientation) {
        case 1:
            blocksTextures[BLOCK_45].Draw(ColToX(pieceX + 1), RowToY(pieceY + 1));
            blocksTextures[BLOCK_49].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_50].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_44].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            break;
        case 2:
            blocksTextures[BLOCK_46].Draw(ColToX(pieceX - 1), RowToY(pieceY + 1));
            blocksTextures[BLOCK_51].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_48].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_47].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            break;
        }
        break;

    case TYPE_GREEN:
        switch (pieceOrientation) {
        case 1:
            blocksTextures[BLOCK_54].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            blocksTextures[BLOCK_61].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_55].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_57].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            break;
        case 2:
            blocksTextures[BLOCK_56].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_62].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_57].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_55].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            break;
        case 3:
            blocksTextures[BLOCK_54].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            blocksTextures[BLOCK_63].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_55].Draw(ColToX(pieceX),     RowToY(pieceY + 1));
            blocksTextures[BLOCK_56].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            break;
        case 4:
            blocksTextures[BLOCK_56].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_64].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_57].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_54].Draw(ColToX(pieceX),     RowToY(pieceY - 1));
            break;
        }
        break;

    case TYPE_RED:
        switch (pieceOrientation) {
        case 1:
            blocksTextures[BLOCK_67].Draw(ColToX(pieceX), RowToY(pieceY - 1));
            blocksTextures[BLOCK_71].Draw(ColToX(pieceX), RowToY(pieceY));
            blocksTextures[BLOCK_71].Draw(ColToX(pieceX), RowToY(pieceY + 1));
            blocksTextures[BLOCK_68].Draw(ColToX(pieceX), RowToY(pieceY + 2));
            break;
        case 2:
            blocksTextures[BLOCK_69].Draw(ColToX(pieceX - 1), RowToY(pieceY));
            blocksTextures[BLOCK_72].Draw(ColToX(pieceX),     RowToY(pieceY));
            blocksTextures[BLOCK_72].Draw(ColToX(pieceX + 1), RowToY(pieceY));
            blocksTextures[BLOCK_70].Draw(ColToX(pieceX + 2), RowToY(pieceY));
            break;
        }
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the current cell collides with something.
 */
bool TetraminoesManager::InnerData::CheckCollision(int x, int y) {
    return x < 0 || x >= WORLD_WIDTH || y < 0 || y >= WORLD_HEIGHT ||
           world[y][x] != EMPTY_CELL;
}

//--------------------------------------------------------------------------------

/**
 * Checks if the current piece collides with something.
 */
bool TetraminoesManager::InnerData::CheckCollision() {
    switch (piece) {
    case TYPE_BLUE:
        return CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX + 1, pieceY - 1) ||
               CheckCollision(pieceX, pieceY)     || CheckCollision(pieceX + 1, pieceY);

    case TYPE_YELLOW:
        switch (pieceOrientation) {
        case 1:
            return CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX - 1, pieceY + 1);

        case 2:
            return CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX - 1, pieceY - 1);

        case 3:
            return CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX + 1, pieceY - 1);

        case 4:
            return CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX + 1, pieceY + 1);
        }
        break;

    case TYPE_MURREY:
        switch (pieceOrientation) {
        case 1:
            return CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX + 1, pieceY + 1);

        case 2:
            return CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX - 1, pieceY + 1);

        case 3:
            return CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX - 1, pieceY - 1);

        case 4:
            return CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX + 1, pieceY - 1);
        }
        break;

    case TYPE_ORANGE:
        switch (pieceOrientation) {
        case 1:
            return CheckCollision(pieceX + 1, pieceY - 1) || CheckCollision(pieceX + 1, pieceY) ||
                   CheckCollision(pieceX, pieceY)         || CheckCollision(pieceX, pieceY + 1);

        case 2:
            return CheckCollision(pieceX + 1, pieceY + 1) || CheckCollision(pieceX, pieceY + 1) ||
                   CheckCollision(pieceX, pieceY)         || CheckCollision(pieceX - 1, pieceY);
        }
        break;

    case TYPE_CYAN:
        switch (pieceOrientation) {
        case 1:
            return CheckCollision(pieceX + 1, pieceY + 1) || CheckCollision(pieceX + 1, pieceY) ||
                   CheckCollision(pieceX, pieceY)         || CheckCollision(pieceX, pieceY - 1);

        case 2:
            return CheckCollision(pieceX - 1, pieceY + 1) || CheckCollision(pieceX, pieceY + 1) ||
                   CheckCollision(pieceX, pieceY)         || CheckCollision(pieceX + 1, pieceY);
        }
        break;

    case TYPE_GREEN:
        switch (pieceOrientation) {
        case 1:
            return CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX + 1, pieceY);

        case 2:
            return CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX, pieceY + 1);

        case 3:
            return CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX - 1, pieceY);

        case 4:
            return CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX, pieceY - 1);
        }
        break;

    case TYPE_RED:
        switch (pieceOrientation) {
        case 1:
            return CheckCollision(pieceX, pieceY - 1) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX, pieceY + 1) || CheckCollision(pieceX, pieceY + 2);

        case 2:
            return CheckCollision(pieceX - 1, pieceY) || CheckCollision(pieceX, pieceY) ||
                   CheckCollision(pieceX + 1, pieceY) || CheckCollision(pieceX + 2, pieceY);
        }
    }

    return false;
}

//--------------------------------------------------------------------------------

/**
 * Turns the current piece to the right.
 */
void TetraminoesManager::InnerData::TurnRight() {
    if(piece != TYPE_BLUE) {
        int aux = pieceOrientation;
        pieceOrientation++;

        switch (piece) {
        case TYPE_YELLOW: if(pieceOrientation > 4) { pieceOrientation = 1; } break;
        case TYPE_MURREY: if(pieceOrientation > 4) { pieceOrientation = 1; } break;
        case TYPE_ORANGE: if(pieceOrientation > 2) { pieceOrientation = 1; } break;
        case TYPE_CYAN:   if(pieceOrientation > 2) { pieceOrientation = 1; } break;
        case TYPE_GREEN:  if(pieceOrientation > 4) { pieceOrientation = 1; } break;
        case TYPE_RED:    if(pieceOrientation > 2) { pieceOrientation = 1; } break;
        }

        if(CheckCollision()) {
            pieceOrientation = aux;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Turns the current piece to the left.
 */
void TetraminoesManager::InnerData::TurnLeft() {
    if(piece != TYPE_BLUE) {
        int aux = pieceOrientation;
        pieceOrientation--;

        switch (piece) {
        case TYPE_YELLOW: if(pieceOrientation < 1) { pieceOrientation = 4; } break;
        case TYPE_MURREY: if(pieceOrientation < 1) { pieceOrientation = 4; } break;
        case TYPE_ORANGE: if(pieceOrientation < 1) { pieceOrientation = 2; } break;
        case TYPE_CYAN:   if(pieceOrientation < 1) { pieceOrientation = 2; } break;
        case TYPE_GREEN:  if(pieceOrientation < 1) { pieceOrientation = 4; } break;
        case TYPE_RED:    if(pieceOrientation < 1) { pieceOrientation = 2; } break;
        }

        if(CheckCollision()) {
            pieceOrientation = aux;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Moves the current piece to the right.
 */
void TetraminoesManager::InnerData::MoveRight() {
    int aux = pieceX;
    pieceX++;

    if(CheckCollision()) {
        pieceX = aux;
    }
}

//--------------------------------------------------------------------------------

/**
 * Moves the current piece to the left.
 */
void TetraminoesManager::InnerData::MoveLeft() {
    int aux = pieceX;
    pieceX--;

    if(CheckCollision()) {
        pieceX = aux;
    }
}

//--------------------------------------------------------------------------------

/**
 * Puts the current piece inside the world.
 */
void TetraminoesManager::InnerData::PutPieceIntoWorld() {
    switch (piece) {
    case TYPE_BLUE:
        world[pieceY - 1][pieceX]     = BLOCK_04;
        world[pieceY - 1][pieceX + 1] = BLOCK_05;
        world[pieceY][pieceX]         = BLOCK_06;
        world[pieceY][pieceX + 1]     = BLOCK_07;
        break;

    case TYPE_YELLOW:
        switch (pieceOrientation) {
        case 1:
            world[pieceY - 1][pieceX]     = BLOCK_10;
            world[pieceY][pieceX]         = BLOCK_18;
            world[pieceY + 1][pieceX]     = BLOCK_17;
            world[pieceY + 1][pieceX - 1] = BLOCK_12;
            break;
        case 2:
            world[pieceY][pieceX + 1]     = BLOCK_13;
            world[pieceY][pieceX]         = BLOCK_19;
            world[pieceY][pieceX - 1]     = BLOCK_16;
            world[pieceY - 1][pieceX - 1] = BLOCK_10;
            break;
        case 3:
            world[pieceY + 1][pieceX]     = BLOCK_11;
            world[pieceY][pieceX]         = BLOCK_18;
            world[pieceY - 1][pieceX]     = BLOCK_14;
            world[pieceY - 1][pieceX + 1] = BLOCK_13;
            break;
        case 4:
            world[pieceY][pieceX - 1]     = BLOCK_12;
            world[pieceY][pieceX]         = BLOCK_19;
            world[pieceY][pieceX + 1]     = BLOCK_15;
            world[pieceY + 1][pieceX + 1] = BLOCK_11;
            break;
        }
        break;

    case TYPE_MURREY:
        switch (pieceOrientation) {
        case 1:
            world[pieceY - 1][pieceX]     = BLOCK_22;
            world[pieceY][pieceX]         = BLOCK_30;
            world[pieceY + 1][pieceX]     = BLOCK_28;
            world[pieceY + 1][pieceX + 1] = BLOCK_25;
            break;
        case 2:
            world[pieceY][pieceX + 1]     = BLOCK_25;
            world[pieceY][pieceX]         = BLOCK_31;
            world[pieceY][pieceX - 1]     = BLOCK_26;
            world[pieceY + 1][pieceX - 1] = BLOCK_23;
            break;
        case 3:
            world[pieceY + 1][pieceX]     = BLOCK_23;
            world[pieceY][pieceX]         = BLOCK_30;
            world[pieceY - 1][pieceX]     = BLOCK_27;
            world[pieceY - 1][pieceX - 1] = BLOCK_24;
            break;
        case 4:
            world[pieceY][pieceX - 1]     = BLOCK_24;
            world[pieceY][pieceX]         = BLOCK_31;
            world[pieceY][pieceX + 1]     = BLOCK_29;
            world[pieceY - 1][pieceX + 1] = BLOCK_22;
            break;
        }
        break;

    case TYPE_ORANGE:
        switch (pieceOrientation) {
        case 1:
            world[pieceY - 1][pieceX + 1] = BLOCK_34;
            world[pieceY][pieceX + 1]     = BLOCK_41;
            world[pieceY][pieceX]         = BLOCK_38;
            world[pieceY + 1][pieceX]     = BLOCK_35;
            break;
        case 2:
            world[pieceY + 1][pieceX + 1] = BLOCK_37;
            world[pieceY + 1][pieceX]     = BLOCK_40;
            world[pieceY][pieceX]         = BLOCK_39;
            world[pieceY][pieceX - 1]     = BLOCK_36;
            break;
        }
        break;

    case TYPE_CYAN:
        switch (pieceOrientation) {
        case 1:
            world[pieceY + 1][pieceX + 1] = BLOCK_45;
            world[pieceY][pieceX + 1]     = BLOCK_49;
            world[pieceY][pieceX]         = BLOCK_50;
            world[pieceY - 1][pieceX]     = BLOCK_44;
            break;
        case 2:
            world[pieceY + 1][pieceX - 1] = BLOCK_46;
            world[pieceY + 1][pieceX]     = BLOCK_51;
            world[pieceY][pieceX]         = BLOCK_48;
            world[pieceY][pieceX + 1]     = BLOCK_47;
            break;
        }
        break;

    case TYPE_GREEN:
        switch (pieceOrientation) {
        case 1:
            world[pieceY - 1][pieceX] = BLOCK_54;
            world[pieceY][pieceX]     = BLOCK_61;
            world[pieceY + 1][pieceX] = BLOCK_55;
            world[pieceY][pieceX + 1] = BLOCK_57;
            break;
        case 2:
            world[pieceY][pieceX - 1] = BLOCK_56;
            world[pieceY][pieceX]     = BLOCK_62;
            world[pieceY][pieceX + 1] = BLOCK_57;
            world[pieceY + 1][pieceX] = BLOCK_55;
            break;
        case 3:
            world[pieceY - 1][pieceX] = BLOCK_54;
            world[pieceY][pieceX]     = BLOCK_63;
            world[pieceY + 1][pieceX] = BLOCK_55;
            world[pieceY][pieceX - 1] = BLOCK_56;
            break;
        case 4:
            world[pieceY][pieceX - 1] = BLOCK_56;
            world[pieceY][pieceX]     = BLOCK_64;
            world[pieceY][pieceX + 1] = BLOCK_57;
            world[pieceY - 1][pieceX] = BLOCK_54;
            break;
        }
        break;

    case TYPE_RED:
        switch (pieceOrientation) {
        case 1:
            world[pieceY - 1][pieceX] = BLOCK_67;
            world[pieceY][pieceX]     = BLOCK_71;
            world[pieceY + 1][pieceX] = BLOCK_71;
            world[pieceY + 2][pieceX] = BLOCK_68;
            break;
        case 2:
            world[pieceY][pieceX - 1] = BLOCK_69;
            world[pieceY][pieceX]     = BLOCK_72;
            world[pieceY][pieceX + 1] = BLOCK_72;
            world[pieceY][pieceX + 2] = BLOCK_70;
            break;
        }
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if a line have been made.
 */
bool TetraminoesManager::InnerData::CheckLine(int line) {
    for (int i = 0; i < WORLD_WIDTH; i++) {
        if (world[line][i] == EMPTY_CELL) {
            return false;
        }
    }
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Checks the upper cell to a given one.
 */
void TetraminoesManager::InnerData::CheckUpperCell(int i, int j) {
    switch (world[i - 1][j]) {
    // Blue
    case BLOCK_04: world[i - 1][j] = BLOCK_02; break;
    case BLOCK_05: world[i - 1][j] = BLOCK_03; break;

    // Yellow
    case BLOCK_10: world[i - 1][j] = BLOCK_09; break;
    case BLOCK_14: world[i - 1][j] = BLOCK_12; break;
    case BLOCK_15: world[i - 1][j] = BLOCK_13; break;
    case BLOCK_18: world[i - 1][j] = BLOCK_11; break;

    // Murrey
    case BLOCK_22: world[i - 1][j] = BLOCK_21; break;
    case BLOCK_26: world[i - 1][j] = BLOCK_24; break;
    case BLOCK_27: world[i - 1][j] = BLOCK_25; break;
    case BLOCK_30: world[i - 1][j] = BLOCK_23; break;

    // Orange
    case BLOCK_34: world[i - 1][j] = BLOCK_33; break;
    case BLOCK_38: world[i - 1][j] = BLOCK_36; break;
    case BLOCK_39: world[i - 1][j] = BLOCK_37; break;

    // Cyan
    case BLOCK_44: world[i - 1][j] = BLOCK_43; break;
    case BLOCK_48: world[i - 1][j] = BLOCK_46; break;
    case BLOCK_49: world[i - 1][j] = BLOCK_47; break;

    // Green
    case BLOCK_54: world[i - 1][j] = BLOCK_53; break;
    case BLOCK_61: world[i - 1][j] = BLOCK_58; break;
    case BLOCK_62: world[i - 1][j] = BLOCK_60; break;
    case BLOCK_63: world[i - 1][j] = BLOCK_59; break;
    case BLOCK_73: world[i - 1][j] = BLOCK_56; break;
    case BLOCK_74: world[i - 1][j] = BLOCK_57; break;

    // Red
    case BLOCK_67: world[i - 1][j] = BLOCK_66; break;
    case BLOCK_71: world[i - 1][j] = BLOCK_68; break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks the bottom cell to a given one.
 */
void TetraminoesManager::InnerData::CheckBottomCell(int i, int j) {
    switch (world[i + 1][j]) {
    // Blue
    case BLOCK_06: world[i + 1][j] = BLOCK_02; break;
    case BLOCK_07: world[i + 1][j] = BLOCK_03; break;

    // Yellow
    case BLOCK_11: world[i + 1][j] = BLOCK_09; break;
    case BLOCK_16: world[i + 1][j] = BLOCK_12; break;
    case BLOCK_17: world[i + 1][j] = BLOCK_13; break;
    case BLOCK_18: world[i + 1][j] = BLOCK_10; break;

    // Murrey
    case BLOCK_23: world[i + 1][j] = BLOCK_21; break;
    case BLOCK_28: world[i + 1][j] = BLOCK_24; break;
    case BLOCK_29: world[i + 1][j] = BLOCK_25; break;
    case BLOCK_30: world[i + 1][j] = BLOCK_22; break;

    // Orange
    case BLOCK_35: world[i + 1][j] = BLOCK_33; break;
    case BLOCK_40: world[i + 1][j] = BLOCK_36; break;
    case BLOCK_41: world[i + 1][j] = BLOCK_37; break;

    // Cyan
    case BLOCK_45: world[i + 1][j] = BLOCK_43; break;
    case BLOCK_50: world[i + 1][j] = BLOCK_46; break;
    case BLOCK_51: world[i + 1][j] = BLOCK_47; break;

    // Green
    case BLOCK_55: world[i + 1][j] = BLOCK_53; break;
    case BLOCK_58: world[i + 1][j] = BLOCK_56; break;
    case BLOCK_59: world[i + 1][j] = BLOCK_57; break;
    case BLOCK_61: world[i + 1][j] = BLOCK_73; break;
    case BLOCK_63: world[i + 1][j] = BLOCK_74; break;
    case BLOCK_64: world[i + 1][j] = BLOCK_60; break;

    // Red
    case BLOCK_68: world[i + 1][j] = BLOCK_66; break;
    case BLOCK_71: world[i + 1][j] = BLOCK_67; break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Erases a line of the world.
 */
void TetraminoesManager::InnerData::EraseLine(int line) {
    if (line < 1) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            CheckBottomCell(line, i);
        }
    } else if (line > 18) {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            CheckUpperCell(line, i);
        }
    } else {
        for (int i = 0; i < WORLD_WIDTH; i++) {
            CheckUpperCell(line, i);
            CheckBottomCell(line, i);
        }
    }

    for (int i = line; i > 0; i--) {
        for (int j = 0; j < WORLD_WIDTH; j++) {
            world[i][j] = world[i - 1][j];
        }
    }

    for (int j = 0; j < WORLD_WIDTH; j++) {
        world[0][j] = EMPTY_CELL;
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if the player have made some lines.
 */
void TetraminoesManager::InnerData::CheckLines() {
    int numLinesErased = 0;
    int accumPoints = 0;

    for (int i = 0; i < WORLD_HEIGHT; i++) {
        if (CheckLine(i)) {
            EraseLine(i);
            numLinesErased++;
            accumPoints += LINE_POINTS[level - 1];
        }
    }

    if (numLinesErased >= 4) {
        accumPoints *= 2;
    }

    lines += numLinesErased;
    score += accumPoints;

    if (lines > target) {
        level++;
        maxInterval = MAX_INTERVALS[level - 1];
        target += MAX_LINES[level - 1];
    }

    if (accumPoints > 0) {
        lineSound.Play();
        UpdateHud();
    } else if (Keyboard::IsKeyUp(Keyboard::X)) {
        fallSound.Play();
    } else {
        hitSound.Play();
    }
}

//--------------------------------------------------------------------------------

/**
 * Moves down the current piece.
 */
void TetraminoesManager::InnerData::MoveDown() {
    int aux = pieceY;
    pieceY++;

    if (CheckCollision()) {
        pieceY = aux;
        PutPieceIntoWorld();
        CheckLines();
        GetNextPiece();
    }
}

//--------------------------------------------------------------------------------

/**
 * Moves the current piece to the bottom.
 */
void TetraminoesManager::InnerData::FallDown() {
    int aux;
    do {
        aux = pieceY;
        pieceY++;
    } while (!CheckCollision());

    pieceY = aux;
    PutPieceIntoWorld();
    CheckLines();
    GetNextPiece();
}

//--------------------------------------------------------------------------------


/**
 * Sets to true the key pressed controller.
 */
void TetraminoesManager::InnerData::SetKeyPressed() {
    keyPressed = true;
    keyTimeMark = 0;
}

//--------------------------------------------------------------------------------

/**
 * Updates the hud of the game.
 */
void TetraminoesManager::InnerData::UpdateHud() {
    auto ToString = [] (int victim) -> std::string {
        std::stringstream strconv;
        strconv << victim;
        return strconv.str();
    };
    levelLabel->Text(ToString(level));
    linesLabel->Text(ToString(lines));
    scoreLabel->Text(ToString(score));
    GUIUtil::CenterLabel(levelLabel, 404);
}
