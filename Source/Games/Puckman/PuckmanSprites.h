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

#ifndef __PUCKMAN_SPRITES_HEADER__
#define __PUCKMAN_SPRITES_HEADER__

namespace Puckman {
    /**
     * This static class is a list of sprite definitions.
     */
    class Sprites {
    private:
        Sprites() {}
        ~Sprites() {}

    public:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        static const int Width  = 16;
        static const int Height = 16;

        static const int LifeIcon        = 0;
        static const int FruitCherry     = 1;
        static const int FruitStrawberry = 2;
        static const int FruitPeach      = 3;
        static const int FruitBell       = 4;
        static const int FruitApple      = 5;
        static const int FruitGrape      = 6;
        static const int FruitFlower     = 7;
        static const int FruitKey        = 8;

        static const int N200  = 9;
        static const int N400  = 10;
        static const int N800  = 11;
        static const int N1600 = 12;

        static const int SuperPuckmanR1 = 13;
        static const int SuperPuckmanR2 = 14;
        static const int SuperPuckmanR3 = 15;

        static const int PuckmanAll = 16;
        static const int PuckmanR1  = 17;
        static const int PuckmanR2  = 18;
        static const int PuckmanD1  = 19;
        static const int PuckmanD2  = 20;
        static const int PuckmanL1  = 21;
        static const int PuckmanL2  = 22;
        static const int PuckmanU1  = 23;
        static const int PuckmanU2  = 24;

        static const int PuckmanDeath01 = 25;
        static const int PuckmanDeath02 = 26;
        static const int PuckmanDeath03 = 27;
        static const int PuckmanDeath04 = 28;
        static const int PuckmanDeath05 = 29;
        static const int PuckmanDeath06 = 30;
        static const int PuckmanDeath07 = 31;
        static const int PuckmanDeath08 = 32;
        static const int PuckmanDeath09 = 33;
        static const int PuckmanDeath10 = 34;
        static const int PuckmanDeath11 = 35;
        static const int PuckmanDeath12 = 36;

        static const int RedGhostR1 = 37;
        static const int RedGhostR2 = 38;
        static const int RedGhostD1 = 39;
        static const int RedGhostD2 = 40;
        static const int RedGhostL1 = 41;
        static const int RedGhostL2 = 42;
        static const int RedGhostU1 = 43;
        static const int RedGhostU2 = 44;

        static const int BlueGhostR1 = 45;
        static const int BlueGhostR2 = 46;
        static const int BlueGhostD1 = 47;
        static const int BlueGhostD2 = 48;
        static const int BlueGhostL1 = 49;
        static const int BlueGhostL2 = 50;
        static const int BlueGhostU1 = 51;
        static const int BlueGhostU2 = 52;

        static const int PinkGhostR1 = 53;
        static const int PinkGhostR2 = 54;
        static const int PinkGhostD1 = 55;
        static const int PinkGhostD2 = 56;
        static const int PinkGhostL1 = 57;
        static const int PinkGhostL2 = 58;
        static const int PinkGhostU1 = 59;
        static const int PinkGhostU2 = 60;

        static const int BrownGhostR1 = 61;
        static const int BrownGhostR2 = 62;
        static const int BrownGhostD1 = 63;
        static const int BrownGhostD2 = 64;
        static const int BrownGhostL1 = 65;
        static const int BrownGhostL2 = 66;
        static const int BrownGhostU1 = 67;
        static const int BrownGhostU2 = 68;

        static const int BlueGhost1     = 69;
        static const int BlueGhost2     = 70;
        static const int WhiteGhost1    = 71;
        static const int WhiteGhost2    = 72;
        static const int DeadGhostRight = 73;
        static const int DeadGhostDown  = 74;
        static const int DeadGhostLeft  = 75;
        static const int DeadGhostUp    = 76;

        static const int Intermission11   = 77;
        static const int Intermission12   = 78;
        static const int Intermission21   = 79;
        static const int Intermission22   = 80;
        static const int IntermissionBoom = 81;
        static const int Intermission31   = 82;
        static const int Intermission32   = 83;
        static const int Intermission41   = 84;
        static const int Intermission42   = 85;
    };
}

#endif
