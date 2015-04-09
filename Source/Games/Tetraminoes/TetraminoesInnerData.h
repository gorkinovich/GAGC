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

#ifndef __TETRAMINOES_INNER_DATA_STATE_HEADER__
#define __TETRAMINOES_INNER_DATA_STATE_HEADER__

#include <SFML/Graphics/Texture.hpp>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>

class CoreManager;

//********************************************************************************
// Constants
//********************************************************************************

const int MAX_BLOCKS = 68;
const int MAX_TYPES  =  7;

const int WORLD_HEIGHT = 20;
const int WORLD_WIDTH  = 10;

// The maximum intervals of time allowed to repeat a key stroke.
const int MAX_KEY_INTERVAL = 200;

//********************************************************************************
// InnerData
//********************************************************************************

struct TetraminoesManager::InnerData {
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    CoreManager * core;

    SharedTexture tileset;
    SharedTexture menu1Background;
    SharedTexture menu2Background;
    SharedTexture gameBackground;

    Texture2D typesTextures[MAX_TYPES];
    Texture2D blocksTextures[MAX_BLOCKS];
    TetraminoesManager::RankingArray ranking_;

    int time;        // The time controller.
    int timeMark;    // The time mark.
    int maxInterval; // The interval of time until move the piece.

    bool keyPressed; // The key pressed controller flag.
    int keyTimeMark; // The key pressed time mark.

    int level;  // The current level.
    int lines;  // The current lines.
    int score;  // The current points.
    int target; // The target number of lines to get to the next level.

    int world[WORLD_HEIGHT][WORLD_WIDTH]; // The game "world".

    int nextPiece;        // The next piece in the game.
    int piece;            // The current piece in the game.
    int pieceX;           // The x-coordinate of the current piece.
    int pieceY;           // The y-coordinate of the current piece.
    int pieceOrientation; // The orientation of the current piece.

    bool gameOver;   // Tells if the game have end.
    bool gamePaused; // Tells if the game is paused.

    std::unique_ptr<SimpleLabel> levelLabel;
    std::unique_ptr<SimpleLabel> linesLabel;
    std::unique_ptr<SimpleLabel> scoreLabel;
    std::unique_ptr<SimpleLabel> gameOverLabel;
    std::unique_ptr<SimpleLabel> gamePaused1Label;
    std::unique_ptr<SimpleLabel> gamePaused2Label;

    Sound keyboardSound;
    Sound hitSound;
    Sound fallSound;
    Sound lineSound;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Release();

    void StartGame(int difficulty);

    void InitWorld();
    void RenderWorld();

    void SelectNextPiece();
    void RenderNextPiece();
    void GetNextPiece();
    void RenderPiece();

    bool CheckCollision(int x, int y);
    bool CheckCollision();

    void TurnRight();
    void TurnLeft();
    void MoveRight();
    void MoveLeft();

    void PutPieceIntoWorld();
    bool CheckLine(int line);
    void CheckUpperCell(int i, int j);
    void CheckBottomCell(int i, int j);

    void EraseLine(int line);
    void CheckLines();
    void MoveDown();
    void FallDown();

    void SetKeyPressed();
    void UpdateHud();
};

#endif
