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

#ifndef __PUCKMAN_MANAGER_INNER_HEADER__
#define __PUCKMAN_MANAGER_INNER_HEADER__

#include <array>
#include <SFML/Graphics/Texture.hpp>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Puckman/PuckmanInsertCoinState.h>
#include <Games/Puckman/PuckmanSelectPlayerState.h>
#include <Games/Puckman/PuckmanGameState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int TILES   = 139;
const int SPRITES =  86;

//********************************************************************************
// InnerData
//********************************************************************************

namespace Puckman {
    struct Manager::InnerData {
        // Screen
        typedef std::array<Texture2D, TILES> ArrayOfTiles;
        typedef std::array<Texture2D, SPRITES> ArrayOfSprites;

        SharedTexture tileset_;  // The texture with the tiles of the game.
        ArrayOfTiles tiles_;     // The tiles of the game.
        ArrayOfSprites sprites_; // The sprites of the game.
        Texture2D blankCell_;    // The blank cell texture.
        Texture2D blankSprite_;  // The blank sprite texture.

        void LoadScreenContent();
        void UnloadScreenContent();

        // Sound
        Sound beginning_; // The sound when you begin a new game.
        Sound chomp_;     // The chomp sound.
        Sound death_;     // The sound when you die.
        Sound extrapac_;  // The sound when you get an extra life.
        Sound fruiteat_;  // The sound when you eat a fruit.
        Sound ghosteat_;  // The sound when you eat a ghost.
        Sound siren_;     // The siren sound.

        void LoadSoundContent();
        void UnloadSoundContent();

        // Data
        std::unique_ptr<Puckman::GameData> data;

        // States
        std::unique_ptr<Puckman::InsertCoinState> insertCoinState;
        std::unique_ptr<Puckman::SelectPlayerState> selectPlayerState;
        std::unique_ptr<Puckman::GameState> gameState;
    };
}

#endif
