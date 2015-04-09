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

#ifndef __ATARI_PALETTE_HEADER__
#define __ATARI_PALETTE_HEADER__

#include <SFML/Graphics/Color.hpp>

/**
 * This static class represents the Atari 2600 palette.
 */
class AtariPalette {
public:
    static const int NumberOfColors  = 128;

    static const int Hue00Lum00Index =   0;
    static const int Hue00Lum02Index =   1;
    static const int Hue00Lum04Index =   2;
    static const int Hue00Lum06Index =   3;
    static const int Hue00Lum08Index =   4;
    static const int Hue00Lum10Index =   5;
    static const int Hue00Lum12Index =   6;
    static const int Hue00Lum14Index =   7;

    static const int Hue01Lum00Index =   8;
    static const int Hue01Lum02Index =   9;
    static const int Hue01Lum04Index =  10;
    static const int Hue01Lum06Index =  11;
    static const int Hue01Lum08Index =  12;
    static const int Hue01Lum10Index =  13;
    static const int Hue01Lum12Index =  14;
    static const int Hue01Lum14Index =  15;

    static const int Hue02Lum00Index =  16;
    static const int Hue02Lum02Index =  17;
    static const int Hue02Lum04Index =  18;
    static const int Hue02Lum06Index =  19;
    static const int Hue02Lum08Index =  20;
    static const int Hue02Lum10Index =  21;
    static const int Hue02Lum12Index =  22;
    static const int Hue02Lum14Index =  23;

    static const int Hue03Lum00Index =  24;
    static const int Hue03Lum02Index =  25;
    static const int Hue03Lum04Index =  26;
    static const int Hue03Lum06Index =  27;
    static const int Hue03Lum08Index =  28;
    static const int Hue03Lum10Index =  29;
    static const int Hue03Lum12Index =  30;
    static const int Hue03Lum14Index =  31;

    static const int Hue04Lum00Index =  32;
    static const int Hue04Lum02Index =  33;
    static const int Hue04Lum04Index =  34;
    static const int Hue04Lum06Index =  35;
    static const int Hue04Lum08Index =  36;
    static const int Hue04Lum10Index =  37;
    static const int Hue04Lum12Index =  38;
    static const int Hue04Lum14Index =  39;

    static const int Hue05Lum00Index =  40;
    static const int Hue05Lum02Index =  41;
    static const int Hue05Lum04Index =  42;
    static const int Hue05Lum06Index =  43;
    static const int Hue05Lum08Index =  44;
    static const int Hue05Lum10Index =  45;
    static const int Hue05Lum12Index =  46;
    static const int Hue05Lum14Index =  47;

    static const int Hue06Lum00Index =  48;
    static const int Hue06Lum02Index =  49;
    static const int Hue06Lum04Index =  50;
    static const int Hue06Lum06Index =  51;
    static const int Hue06Lum08Index =  52;
    static const int Hue06Lum10Index =  53;
    static const int Hue06Lum12Index =  54;
    static const int Hue06Lum14Index =  55;

    static const int Hue07Lum00Index =  56;
    static const int Hue07Lum02Index =  57;
    static const int Hue07Lum04Index =  58;
    static const int Hue07Lum06Index =  59;
    static const int Hue07Lum08Index =  60;
    static const int Hue07Lum10Index =  61;
    static const int Hue07Lum12Index =  62;
    static const int Hue07Lum14Index =  63;

    static const int Hue08Lum00Index =  64;
    static const int Hue08Lum02Index =  65;
    static const int Hue08Lum04Index =  66;
    static const int Hue08Lum06Index =  67;
    static const int Hue08Lum08Index =  68;
    static const int Hue08Lum10Index =  69;
    static const int Hue08Lum12Index =  70;
    static const int Hue08Lum14Index =  71;

    static const int Hue09Lum00Index =  72;
    static const int Hue09Lum02Index =  73;
    static const int Hue09Lum04Index =  74;
    static const int Hue09Lum06Index =  75;
    static const int Hue09Lum08Index =  76;
    static const int Hue09Lum10Index =  77;
    static const int Hue09Lum12Index =  78;
    static const int Hue09Lum14Index =  79;

    static const int Hue10Lum00Index =  80;
    static const int Hue10Lum02Index =  81;
    static const int Hue10Lum04Index =  82;
    static const int Hue10Lum06Index =  83;
    static const int Hue10Lum08Index =  84;
    static const int Hue10Lum10Index =  85;
    static const int Hue10Lum12Index =  86;
    static const int Hue10Lum14Index =  87;

    static const int Hue11Lum00Index =  88;
    static const int Hue11Lum02Index =  89;
    static const int Hue11Lum04Index =  90;
    static const int Hue11Lum06Index =  91;
    static const int Hue11Lum08Index =  92;
    static const int Hue11Lum10Index =  93;
    static const int Hue11Lum12Index =  94;
    static const int Hue11Lum14Index =  95;

    static const int Hue12Lum00Index =  96;
    static const int Hue12Lum02Index =  97;
    static const int Hue12Lum04Index =  98;
    static const int Hue12Lum06Index =  99;
    static const int Hue12Lum08Index = 100;
    static const int Hue12Lum10Index = 101;
    static const int Hue12Lum12Index = 102;
    static const int Hue12Lum14Index = 103;

    static const int Hue13Lum00Index = 104;
    static const int Hue13Lum02Index = 105;
    static const int Hue13Lum04Index = 106;
    static const int Hue13Lum06Index = 107;
    static const int Hue13Lum08Index = 108;
    static const int Hue13Lum10Index = 109;
    static const int Hue13Lum12Index = 110;
    static const int Hue13Lum14Index = 111;

    static const int Hue14Lum00Index = 112;
    static const int Hue14Lum02Index = 113;
    static const int Hue14Lum04Index = 114;
    static const int Hue14Lum06Index = 115;
    static const int Hue14Lum08Index = 116;
    static const int Hue14Lum10Index = 117;
    static const int Hue14Lum12Index = 118;
    static const int Hue14Lum14Index = 119;

    static const int Hue15Lum00Index = 120;
    static const int Hue15Lum02Index = 121;
    static const int Hue15Lum04Index = 122;
    static const int Hue15Lum06Index = 123;
    static const int Hue15Lum08Index = 124;
    static const int Hue15Lum10Index = 125;
    static const int Hue15Lum12Index = 126;
    static const int Hue15Lum14Index = 127;

    static const sf::Color Hue00Lum00;
    static const sf::Color Hue00Lum02;
    static const sf::Color Hue00Lum04;
    static const sf::Color Hue00Lum06;
    static const sf::Color Hue00Lum08;
    static const sf::Color Hue00Lum10;
    static const sf::Color Hue00Lum12;
    static const sf::Color Hue00Lum14;

    static const sf::Color Hue01Lum00;
    static const sf::Color Hue01Lum02;
    static const sf::Color Hue01Lum04;
    static const sf::Color Hue01Lum06;
    static const sf::Color Hue01Lum08;
    static const sf::Color Hue01Lum10;
    static const sf::Color Hue01Lum12;
    static const sf::Color Hue01Lum14;

    static const sf::Color Hue02Lum00;
    static const sf::Color Hue02Lum02;
    static const sf::Color Hue02Lum04;
    static const sf::Color Hue02Lum06;
    static const sf::Color Hue02Lum08;
    static const sf::Color Hue02Lum10;
    static const sf::Color Hue02Lum12;
    static const sf::Color Hue02Lum14;

    static const sf::Color Hue03Lum00;
    static const sf::Color Hue03Lum02;
    static const sf::Color Hue03Lum04;
    static const sf::Color Hue03Lum06;
    static const sf::Color Hue03Lum08;
    static const sf::Color Hue03Lum10;
    static const sf::Color Hue03Lum12;
    static const sf::Color Hue03Lum14;

    static const sf::Color Hue04Lum00;
    static const sf::Color Hue04Lum02;
    static const sf::Color Hue04Lum04;
    static const sf::Color Hue04Lum06;
    static const sf::Color Hue04Lum08;
    static const sf::Color Hue04Lum10;
    static const sf::Color Hue04Lum12;
    static const sf::Color Hue04Lum14;

    static const sf::Color Hue05Lum00;
    static const sf::Color Hue05Lum02;
    static const sf::Color Hue05Lum04;
    static const sf::Color Hue05Lum06;
    static const sf::Color Hue05Lum08;
    static const sf::Color Hue05Lum10;
    static const sf::Color Hue05Lum12;
    static const sf::Color Hue05Lum14;

    static const sf::Color Hue06Lum00;
    static const sf::Color Hue06Lum02;
    static const sf::Color Hue06Lum04;
    static const sf::Color Hue06Lum06;
    static const sf::Color Hue06Lum08;
    static const sf::Color Hue06Lum10;
    static const sf::Color Hue06Lum12;
    static const sf::Color Hue06Lum14;

    static const sf::Color Hue07Lum00;
    static const sf::Color Hue07Lum02;
    static const sf::Color Hue07Lum04;
    static const sf::Color Hue07Lum06;
    static const sf::Color Hue07Lum08;
    static const sf::Color Hue07Lum10;
    static const sf::Color Hue07Lum12;
    static const sf::Color Hue07Lum14;

    static const sf::Color Hue08Lum00;
    static const sf::Color Hue08Lum02;
    static const sf::Color Hue08Lum04;
    static const sf::Color Hue08Lum06;
    static const sf::Color Hue08Lum08;
    static const sf::Color Hue08Lum10;
    static const sf::Color Hue08Lum12;
    static const sf::Color Hue08Lum14;

    static const sf::Color Hue09Lum00;
    static const sf::Color Hue09Lum02;
    static const sf::Color Hue09Lum04;
    static const sf::Color Hue09Lum06;
    static const sf::Color Hue09Lum08;
    static const sf::Color Hue09Lum10;
    static const sf::Color Hue09Lum12;
    static const sf::Color Hue09Lum14;

    static const sf::Color Hue10Lum00;
    static const sf::Color Hue10Lum02;
    static const sf::Color Hue10Lum04;
    static const sf::Color Hue10Lum06;
    static const sf::Color Hue10Lum08;
    static const sf::Color Hue10Lum10;
    static const sf::Color Hue10Lum12;
    static const sf::Color Hue10Lum14;

    static const sf::Color Hue11Lum00;
    static const sf::Color Hue11Lum02;
    static const sf::Color Hue11Lum04;
    static const sf::Color Hue11Lum06;
    static const sf::Color Hue11Lum08;
    static const sf::Color Hue11Lum10;
    static const sf::Color Hue11Lum12;
    static const sf::Color Hue11Lum14;

    static const sf::Color Hue12Lum00;
    static const sf::Color Hue12Lum02;
    static const sf::Color Hue12Lum04;
    static const sf::Color Hue12Lum06;
    static const sf::Color Hue12Lum08;
    static const sf::Color Hue12Lum10;
    static const sf::Color Hue12Lum12;
    static const sf::Color Hue12Lum14;

    static const sf::Color Hue13Lum00;
    static const sf::Color Hue13Lum02;
    static const sf::Color Hue13Lum04;
    static const sf::Color Hue13Lum06;
    static const sf::Color Hue13Lum08;
    static const sf::Color Hue13Lum10;
    static const sf::Color Hue13Lum12;
    static const sf::Color Hue13Lum14;

    static const sf::Color Hue14Lum00;
    static const sf::Color Hue14Lum02;
    static const sf::Color Hue14Lum04;
    static const sf::Color Hue14Lum06;
    static const sf::Color Hue14Lum08;
    static const sf::Color Hue14Lum10;
    static const sf::Color Hue14Lum12;
    static const sf::Color Hue14Lum14;

    static const sf::Color Hue15Lum00;
    static const sf::Color Hue15Lum02;
    static const sf::Color Hue15Lum04;
    static const sf::Color Hue15Lum06;
    static const sf::Color Hue15Lum08;
    static const sf::Color Hue15Lum10;
    static const sf::Color Hue15Lum12;
    static const sf::Color Hue15Lum14;

    static const sf::Color Colors[];
};

#endif
