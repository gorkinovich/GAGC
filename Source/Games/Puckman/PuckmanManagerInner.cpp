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
#include <System/CoreManager.h>
#include <Games/Puckman/PuckmanPalette.h>

using namespace Puckman;

//********************************************************************************
// Screen Methods
//********************************************************************************

void Manager::InnerData::LoadScreenContent() {
    auto * core = CoreManager::Instance();
    tileset_ = core->LoadTexture("Content/Textures/Puckman.png");


    tiles_[0].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[1].Load(tileset_, sf::IntRect(CELL_WIDTH * 1, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[2].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[3].Load(tileset_, sf::IntRect(CELL_WIDTH * 3, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[4].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[5].Load(tileset_, sf::IntRect(CELL_WIDTH * 5, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[6].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[7].Load(tileset_, sf::IntRect(CELL_WIDTH * 7, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[8].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[9].Load(tileset_, sf::IntRect(CELL_WIDTH * 9, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[10].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[11].Load(tileset_, sf::IntRect(CELL_WIDTH * 11, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[12].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[13].Load(tileset_, sf::IntRect(CELL_WIDTH * 13, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[14].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[15].Load(tileset_, sf::IntRect(CELL_WIDTH * 15, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));

    tiles_[16].Load(tileset_, sf::IntRect(CELL_WIDTH * 23, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT)); //Up-right
    tiles_[17].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT)); //Down-right
    tiles_[18].Load(tileset_, sf::IntRect(CELL_WIDTH * 25, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT)); //Up-left
    tiles_[19].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT)); //Down-left

    tiles_[20].Load(tileset_, sf::IntRect(CELL_WIDTH * 21, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //1xx
    tiles_[21].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //3xx
    tiles_[22].Load(tileset_, sf::IntRect(CELL_WIDTH * 23, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //5xx
    tiles_[23].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //7xx
    tiles_[24].Load(tileset_, sf::IntRect(CELL_WIDTH * 25, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //x00
    tiles_[25].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //10xx
    tiles_[26].Load(tileset_, sf::IntRect(CELL_WIDTH * 17, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //20xxa
    tiles_[27].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //20xxb
    tiles_[28].Load(tileset_, sf::IntRect(CELL_WIDTH * 19, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //30xxa
    tiles_[29].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //30xxb
    tiles_[30].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //50xxa
    tiles_[31].Load(tileset_, sf::IntRect(CELL_WIDTH * 27, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //50xxb
    tiles_[32].Load(tileset_, sf::IntRect(CELL_WIDTH * 28, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //xx00a
    tiles_[33].Load(tileset_, sf::IntRect(CELL_WIDTH * 29, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //xx00b

    tiles_[34].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[35].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));
    tiles_[36].Load(tileset_, sf::IntRect(CELL_WIDTH * 19, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[37].Load(tileset_, sf::IntRect(CELL_WIDTH * 19, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));
    tiles_[38].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[39].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));
    tiles_[40].Load(tileset_, sf::IntRect(CELL_WIDTH * 21, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[41].Load(tileset_, sf::IntRect(CELL_WIDTH * 21, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));
    tiles_[42].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[43].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));
    tiles_[44].Load(tileset_, sf::IntRect(CELL_WIDTH * 23, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[45].Load(tileset_, sf::IntRect(CELL_WIDTH * 23, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));
    tiles_[46].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 13, CELL_WIDTH, CELL_HEIGHT));
    tiles_[47].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 14, CELL_WIDTH, CELL_HEIGHT));

    tiles_[48].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //0 (48)
    tiles_[49].Load(tileset_, sf::IntRect(CELL_WIDTH * 7, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //1
    tiles_[50].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //2
    tiles_[51].Load(tileset_, sf::IntRect(CELL_WIDTH * 9, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //3
    tiles_[52].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //4
    tiles_[53].Load(tileset_, sf::IntRect(CELL_WIDTH * 11, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //5
    tiles_[54].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //6
    tiles_[55].Load(tileset_, sf::IntRect(CELL_WIDTH * 13, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //7
    tiles_[56].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //8
    tiles_[57].Load(tileset_, sf::IntRect(CELL_WIDTH * 15, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //9
    tiles_[58].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //Space
    tiles_[59].Load(tileset_, sf::IntRect(CELL_WIDTH * 5, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //!
    tiles_[60].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //"
    tiles_[61].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //-
    tiles_[62].Load(tileset_, sf::IntRect(CELL_WIDTH * 1, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); //.
    tiles_[63].Load(tileset_, sf::IntRect(CELL_WIDTH * 3, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT)); // /
    tiles_[64].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //(c)
    tiles_[65].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //A (65)
    tiles_[66].Load(tileset_, sf::IntRect(CELL_WIDTH * 1, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //B
    tiles_[67].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //C
    tiles_[68].Load(tileset_, sf::IntRect(CELL_WIDTH * 3, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //D
    tiles_[69].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //E
    tiles_[70].Load(tileset_, sf::IntRect(CELL_WIDTH * 5, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //F
    tiles_[71].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //G
    tiles_[72].Load(tileset_, sf::IntRect(CELL_WIDTH * 7, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //H
    tiles_[73].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //I
    tiles_[74].Load(tileset_, sf::IntRect(CELL_WIDTH * 9, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //J
    tiles_[75].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //K
    tiles_[76].Load(tileset_, sf::IntRect(CELL_WIDTH * 11, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //L
    tiles_[77].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //M
    tiles_[78].Load(tileset_, sf::IntRect(CELL_WIDTH * 13, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //N
    tiles_[79].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //O
    tiles_[80].Load(tileset_, sf::IntRect(CELL_WIDTH * 15, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //P
    tiles_[81].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //Q
    tiles_[82].Load(tileset_, sf::IntRect(CELL_WIDTH * 17, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //R
    tiles_[83].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //S
    tiles_[84].Load(tileset_, sf::IntRect(CELL_WIDTH * 19, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //T
    tiles_[85].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //U
    tiles_[86].Load(tileset_, sf::IntRect(CELL_WIDTH * 21, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //V
    tiles_[87].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //W
    tiles_[88].Load(tileset_, sf::IntRect(CELL_WIDTH * 23, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //X
    tiles_[89].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //Y
    tiles_[90].Load(tileset_, sf::IntRect(CELL_WIDTH * 25, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT)); //Z
    tiles_[91].Load(tileset_, sf::IntRect(CELL_WIDTH * 27, CELL_HEIGHT * 2, CELL_WIDTH * 3, CELL_HEIGHT)); //PTS
    tiles_[92].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 0, CELL_WIDTH * 7, CELL_HEIGHT)); //Namco

    tiles_[93].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[94].Load(tileset_, sf::IntRect(CELL_WIDTH * 31, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[95].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT));
    tiles_[96].Load(tileset_, sf::IntRect(CELL_WIDTH * 31, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT));

    tiles_[97].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[98].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[99].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[100].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 0, CELL_WIDTH, CELL_HEIGHT));
    tiles_[101].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT));
    tiles_[102].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT));
    tiles_[103].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT));
    tiles_[104].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 1, CELL_WIDTH, CELL_HEIGHT));
    tiles_[105].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT));
    tiles_[106].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT));
    tiles_[107].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT));
    tiles_[108].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT));
    tiles_[109].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT));
    tiles_[110].Load(tileset_, sf::IntRect(CELL_WIDTH * 31, CELL_HEIGHT * 2, CELL_WIDTH, CELL_HEIGHT));
    tiles_[111].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 3, CELL_WIDTH, CELL_HEIGHT));
    tiles_[112].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 3, CELL_WIDTH, CELL_HEIGHT));
    tiles_[113].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 3, CELL_WIDTH, CELL_HEIGHT));
    tiles_[114].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 3, CELL_WIDTH, CELL_HEIGHT));
    tiles_[115].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 3, CELL_WIDTH, CELL_HEIGHT));
    tiles_[116].Load(tileset_, sf::IntRect(CELL_WIDTH * 31, CELL_HEIGHT * 3, CELL_WIDTH, CELL_HEIGHT));
    tiles_[117].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 4, CELL_WIDTH, CELL_HEIGHT));
    tiles_[118].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 4, CELL_WIDTH, CELL_HEIGHT));
    tiles_[119].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 4, CELL_WIDTH, CELL_HEIGHT));
    tiles_[120].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 4, CELL_WIDTH, CELL_HEIGHT));
    tiles_[121].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 4, CELL_WIDTH, CELL_HEIGHT));
    tiles_[122].Load(tileset_, sf::IntRect(CELL_WIDTH * 31, CELL_HEIGHT * 4, CELL_WIDTH, CELL_HEIGHT));
    tiles_[123].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 5, CELL_WIDTH, CELL_HEIGHT));
    tiles_[124].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 5, CELL_WIDTH, CELL_HEIGHT));
    tiles_[125].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 5, CELL_WIDTH, CELL_HEIGHT));
    tiles_[126].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 5, CELL_WIDTH, CELL_HEIGHT));
    tiles_[127].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 6, CELL_WIDTH, CELL_HEIGHT));
    tiles_[128].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 6, CELL_WIDTH, CELL_HEIGHT));
    tiles_[129].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 6, CELL_WIDTH, CELL_HEIGHT));
    tiles_[130].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 6, CELL_WIDTH, CELL_HEIGHT));
    tiles_[131].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 7, CELL_WIDTH, CELL_HEIGHT));
    tiles_[132].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 7, CELL_WIDTH, CELL_HEIGHT));
    tiles_[133].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 7, CELL_WIDTH, CELL_HEIGHT));
    tiles_[134].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 7, CELL_WIDTH, CELL_HEIGHT));
    tiles_[135].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 8, CELL_WIDTH, CELL_HEIGHT));
    tiles_[136].Load(tileset_, sf::IntRect(CELL_WIDTH * 33, CELL_HEIGHT * 8, CELL_WIDTH, CELL_HEIGHT));
    tiles_[137].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 8, CELL_WIDTH, CELL_HEIGHT));
    tiles_[138].Load(tileset_, sf::IntRect(CELL_WIDTH * 35, CELL_HEIGHT * 8, CELL_WIDTH, CELL_HEIGHT));


    sprites_[0].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Puckman
    sprites_[1].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Cherry
    sprites_[2].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Strawberry
    sprites_[3].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Peach
    sprites_[4].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Bell
    sprites_[5].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Apple
    sprites_[6].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Grape
    sprites_[7].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //WTF???
    sprites_[8].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT)); //Key

    sprites_[9].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT)); //200
    sprites_[10].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT)); //400
    sprites_[11].Load(tileset_, sf::IntRect(CELL_WIDTH * 28, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT)); //800
    sprites_[12].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT)); //1600

    sprites_[13].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 3, SPRITE_WIDTH * 2, SPRITE_HEIGHT * 2));
    sprites_[14].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 3, SPRITE_WIDTH * 2, SPRITE_HEIGHT * 2));
    sprites_[15].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 3, SPRITE_WIDTH * 2, SPRITE_HEIGHT * 2));

    sprites_[16].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //All 0
    sprites_[17].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Right 1
    sprites_[18].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Right 2
    sprites_[19].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Down 1
    sprites_[20].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Down 2
    sprites_[21].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Left 1
    sprites_[22].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Left 2
    sprites_[23].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Up 1
    sprites_[24].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 5, SPRITE_WIDTH, SPRITE_HEIGHT)); //Up 2

    sprites_[25].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[26].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[27].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[28].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[29].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[30].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[31].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[32].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[33].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[34].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[35].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));
    sprites_[36].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 7, SPRITE_WIDTH, SPRITE_HEIGHT));

    sprites_[37].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //R1
    sprites_[38].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //R2
    sprites_[39].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //D1
    sprites_[40].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //D2
    sprites_[41].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //L1
    sprites_[42].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //L2
    sprites_[43].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //U1
    sprites_[44].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //U2

    sprites_[45].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //R1
    sprites_[46].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //R2
    sprites_[47].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //D1
    sprites_[48].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //D2
    sprites_[49].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //L1
    sprites_[50].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //L2
    sprites_[51].Load(tileset_, sf::IntRect(CELL_WIDTH * 28, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //U1
    sprites_[52].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //U2

    sprites_[53].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //R1
    sprites_[54].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //R2
    sprites_[55].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //D1
    sprites_[56].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //D2
    sprites_[57].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //L1
    sprites_[58].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //L2
    sprites_[59].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //U1
    sprites_[60].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //U2

    sprites_[61].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //R1
    sprites_[62].Load(tileset_, sf::IntRect(CELL_WIDTH * 18, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //R2
    sprites_[63].Load(tileset_, sf::IntRect(CELL_WIDTH * 20, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //D1
    sprites_[64].Load(tileset_, sf::IntRect(CELL_WIDTH * 22, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //D2
    sprites_[65].Load(tileset_, sf::IntRect(CELL_WIDTH * 24, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //L1
    sprites_[66].Load(tileset_, sf::IntRect(CELL_WIDTH * 26, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //L2
    sprites_[67].Load(tileset_, sf::IntRect(CELL_WIDTH * 28, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //U1
    sprites_[68].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //U2

    sprites_[69].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //Blue1
    sprites_[70].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 11, SPRITE_WIDTH, SPRITE_HEIGHT)); //Blue2
    sprites_[71].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //White1
    sprites_[72].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 9, SPRITE_WIDTH, SPRITE_HEIGHT)); //White1
    sprites_[73].Load(tileset_, sf::IntRect(CELL_WIDTH * 28, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //R
    sprites_[74].Load(tileset_, sf::IntRect(CELL_WIDTH * 30, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //D
    sprites_[75].Load(tileset_, sf::IntRect(CELL_WIDTH * 32, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //L
    sprites_[76].Load(tileset_, sf::IntRect(CELL_WIDTH * 34, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //U

    sprites_[77].Load(tileset_, sf::IntRect(CELL_WIDTH * 0, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I11
    sprites_[78].Load(tileset_, sf::IntRect(CELL_WIDTH * 2, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I12
    sprites_[79].Load(tileset_, sf::IntRect(CELL_WIDTH * 4, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I21
    sprites_[80].Load(tileset_, sf::IntRect(CELL_WIDTH * 6, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I21
    sprites_[81].Load(tileset_, sf::IntRect(CELL_WIDTH * 8, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //BOOM
    sprites_[82].Load(tileset_, sf::IntRect(CELL_WIDTH * 10, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I31
    sprites_[83].Load(tileset_, sf::IntRect(CELL_WIDTH * 12, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I32
    sprites_[84].Load(tileset_, sf::IntRect(CELL_WIDTH * 14, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I41
    sprites_[85].Load(tileset_, sf::IntRect(CELL_WIDTH * 16, CELL_HEIGHT * 13, SPRITE_WIDTH, SPRITE_HEIGHT)); //I42


    blankCell_.Load(CELL_WIDTH, CELL_HEIGHT, Palette::Black);
    blankSprite_.Load(SPRITE_WIDTH, SPRITE_HEIGHT, Palette::Black);
}

//--------------------------------------------------------------------------------

void Manager::InnerData::UnloadScreenContent() {
    tileset_ = nullptr;
    for (int i = 0; i < TILES; ++i) {
        tiles_[i].Unload();
    }
    blankCell_.Unload();
    blankSprite_.Unload();
}

//********************************************************************************
// Sound Methods
//********************************************************************************

void Manager::InnerData::LoadSoundContent() {
    beginning_.Load("Content/Sounds/PuckmanBeginning.wav");
    chomp_.Load("Content/Sounds/PuckmanChomp.wav");
    death_.Load("Content/Sounds/PuckmanDeath.wav");
    extrapac_.Load("Content/Sounds/PuckmanExtrapac.wav");
    fruiteat_.Load("Content/Sounds/PuckmanFruiteat.wav");
    ghosteat_.Load("Content/Sounds/PuckmanGhosteat.wav");
    siren_.Load("Content/Sounds/PuckmanSiren.wav");
}

//--------------------------------------------------------------------------------

void Manager::InnerData::UnloadSoundContent() {
    // Wait to the sound to end.
    death_.WaitUntilStop();

    // Unload the sounds of the game.
    beginning_.Unload();
    chomp_.Unload();
    death_.Unload();
    extrapac_.Unload();
    fruiteat_.Unload();
    ghosteat_.Unload();
    siren_.Unload();
}
