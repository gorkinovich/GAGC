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

#include "AtariPalette.h"

//********************************************************************************
// Constants
//********************************************************************************

/*
 * Paleta Atari TIA (Atari 2600)
 * hue / luminance
 *
 *       00      02      04      06      08      10      12      14
 * 00: 000000, 404040, 6C6C6C, 909090, B0B0B0, C8C8C8, DCDCDC, ECECEC
 * 01: 444400, 646410, 848424, A0A034, B8B840, D0D050, E8E85C, FCFC68
 * 02: 702800, 844414, 985C28, AC783C, BC8C4C, CCA05C, DCB468, ECC878
 * 03: 841800, 983418, AC5030, C06848, D0805C, E09470, ECA880, FCBC94
 * 04: 880000, 9C2020, B03C3C, C05858, D07070, E08888, ECA0A0, FCB4B4
 * 05: 78005C, 8C2074, A03C88, B0589C, C070B0, D084C0, DC9CD0, ECB0E0
 * 06: 480078, 602090, 783CA4, 8C58B8, A070CC, B484DC, C49CEC, D4B0FC
 * 07: 140084, 302098, 4C3CAC, 6858C0, 7C70D0, 9488E0, A8A0EC, BCB4FC
 * 08: 000088, 1C209C, 3840B0, 505CC0, 6874D0, 7C8CE0, 90A4EC, A4B8FC
 * 09: 00187C, 1C3890, 3854A8, 5070BC, 6888CC, 7C9CDC, 90B4EC, A4C8FC
 * 10: 002C5C, 1C4C78, 386890, 5084AC, 689CC0, 7CB4D4, 90CCE8, A4E0FC
 * 11: 003C2C, 1C5C48, 387C64, 509C80, 68B494, 7CD0AC, 90E4C0, A4FCD4
 * 12: 003C00, 205C20, 407C40, 5C9C5C, 74B474, 8CD08C, A4E4A4, B8FCB8
 * 13: 143800, 345C1C, 507C38, 6C9850, 84B468, 9CCC7C, B4E490, C8FCA4
 * 14: 2C3000, 4C501C, 687034, 848C4C, 9CA864, B4C078, CCD488, E0EC9C
 * 15: 442800, 644818, 846830, A08444, B89C58, D0B46C, E8CC7C, FCE08C
 */

// 000000, 404040, 6C6C6C, 909090, B0B0B0, C8C8C8, DCDCDC, ECECEC
const sf::Color AtariPalette::Hue00Lum00 = sf::Color(0x00, 0x00, 0x00);
const sf::Color AtariPalette::Hue00Lum02 = sf::Color(0x40, 0x40, 0x40);
const sf::Color AtariPalette::Hue00Lum04 = sf::Color(0x6C, 0x6C, 0x6C);
const sf::Color AtariPalette::Hue00Lum06 = sf::Color(0x90, 0x90, 0x90);
const sf::Color AtariPalette::Hue00Lum08 = sf::Color(0xB0, 0xB0, 0xB0);
const sf::Color AtariPalette::Hue00Lum10 = sf::Color(0xC8, 0xC8, 0xC8);
const sf::Color AtariPalette::Hue00Lum12 = sf::Color(0xDC, 0xDC, 0xDC);
const sf::Color AtariPalette::Hue00Lum14 = sf::Color(0xEC, 0xEC, 0xEC);

// 444400, 646410, 848424, A0A034, B8B840, D0D050, E8E85C, FCFC68
const sf::Color AtariPalette::Hue01Lum00 = sf::Color(0x44, 0x44, 0x00);
const sf::Color AtariPalette::Hue01Lum02 = sf::Color(0x64, 0x64, 0x10);
const sf::Color AtariPalette::Hue01Lum04 = sf::Color(0x84, 0x84, 0x24);
const sf::Color AtariPalette::Hue01Lum06 = sf::Color(0xA0, 0xA0, 0x34);
const sf::Color AtariPalette::Hue01Lum08 = sf::Color(0xB8, 0xB8, 0x40);
const sf::Color AtariPalette::Hue01Lum10 = sf::Color(0xD0, 0xD0, 0x50);
const sf::Color AtariPalette::Hue01Lum12 = sf::Color(0xE8, 0xE8, 0x5C);
const sf::Color AtariPalette::Hue01Lum14 = sf::Color(0xFC, 0xFC, 0x68);

// 702800, 844414, 985C28, AC783C, BC8C4C, CCA05C, DCB468, ECC878
const sf::Color AtariPalette::Hue02Lum00 = sf::Color(0x70, 0x28, 0x00);
const sf::Color AtariPalette::Hue02Lum02 = sf::Color(0x84, 0x44, 0x14);
const sf::Color AtariPalette::Hue02Lum04 = sf::Color(0x98, 0x5C, 0x28);
const sf::Color AtariPalette::Hue02Lum06 = sf::Color(0xAC, 0x78, 0x3C);
const sf::Color AtariPalette::Hue02Lum08 = sf::Color(0xBC, 0x8C, 0x4C);
const sf::Color AtariPalette::Hue02Lum10 = sf::Color(0xCC, 0xA0, 0x5C);
const sf::Color AtariPalette::Hue02Lum12 = sf::Color(0xDC, 0xB4, 0x68);
const sf::Color AtariPalette::Hue02Lum14 = sf::Color(0xEC, 0xC8, 0x78);

// 841800, 983418, AC5030, C06848, D0805C, E09470, ECA880, FCBC94
const sf::Color AtariPalette::Hue03Lum00 = sf::Color(0x84, 0x18, 0x00);
const sf::Color AtariPalette::Hue03Lum02 = sf::Color(0x98, 0x34, 0x18);
const sf::Color AtariPalette::Hue03Lum04 = sf::Color(0xAC, 0x50, 0x30);
const sf::Color AtariPalette::Hue03Lum06 = sf::Color(0xC0, 0x68, 0x48);
const sf::Color AtariPalette::Hue03Lum08 = sf::Color(0xD0, 0x80, 0x5C);
const sf::Color AtariPalette::Hue03Lum10 = sf::Color(0xE0, 0x94, 0x70);
const sf::Color AtariPalette::Hue03Lum12 = sf::Color(0xEC, 0xA8, 0x80);
const sf::Color AtariPalette::Hue03Lum14 = sf::Color(0xFC, 0xBC, 0x94);

// 880000, 9C2020, B03C3C, C05858, D07070, E08888, ECA0A0, FCB4B4
const sf::Color AtariPalette::Hue04Lum00 = sf::Color(0x88, 0x00, 0x00);
const sf::Color AtariPalette::Hue04Lum02 = sf::Color(0x9C, 0x20, 0x20);
const sf::Color AtariPalette::Hue04Lum04 = sf::Color(0xB0, 0x3C, 0x3C);
const sf::Color AtariPalette::Hue04Lum06 = sf::Color(0xC0, 0x58, 0x58);
const sf::Color AtariPalette::Hue04Lum08 = sf::Color(0xD0, 0x70, 0x70);
const sf::Color AtariPalette::Hue04Lum10 = sf::Color(0xE0, 0x88, 0x88);
const sf::Color AtariPalette::Hue04Lum12 = sf::Color(0xEC, 0xA0, 0xA0);
const sf::Color AtariPalette::Hue04Lum14 = sf::Color(0xFC, 0xB4, 0xB4);

// 78005C, 8C2074, A03C88, B0589C, C070B0, D084C0, DC9CD0, ECB0E0
const sf::Color AtariPalette::Hue05Lum00 = sf::Color(0x78, 0x00, 0x5C);
const sf::Color AtariPalette::Hue05Lum02 = sf::Color(0x8C, 0x20, 0x74);
const sf::Color AtariPalette::Hue05Lum04 = sf::Color(0xA0, 0x3C, 0x88);
const sf::Color AtariPalette::Hue05Lum06 = sf::Color(0xB0, 0x58, 0x9C);
const sf::Color AtariPalette::Hue05Lum08 = sf::Color(0xC0, 0x70, 0xB0);
const sf::Color AtariPalette::Hue05Lum10 = sf::Color(0xD0, 0x84, 0xC0);
const sf::Color AtariPalette::Hue05Lum12 = sf::Color(0xDC, 0x9C, 0xD0);
const sf::Color AtariPalette::Hue05Lum14 = sf::Color(0xEC, 0xB0, 0xE0);

// 480078, 602090, 783CA4, 8C58B8, A070CC, B484DC, C49CEC, D4B0FC
const sf::Color AtariPalette::Hue06Lum00 = sf::Color(0x48, 0x00, 0x78);
const sf::Color AtariPalette::Hue06Lum02 = sf::Color(0x60, 0x20, 0x90);
const sf::Color AtariPalette::Hue06Lum04 = sf::Color(0x78, 0x3C, 0xA4);
const sf::Color AtariPalette::Hue06Lum06 = sf::Color(0x8C, 0x58, 0xB8);
const sf::Color AtariPalette::Hue06Lum08 = sf::Color(0xA0, 0x70, 0xCC);
const sf::Color AtariPalette::Hue06Lum10 = sf::Color(0xB4, 0x84, 0xDC);
const sf::Color AtariPalette::Hue06Lum12 = sf::Color(0xC4, 0x9C, 0xEC);
const sf::Color AtariPalette::Hue06Lum14 = sf::Color(0xD4, 0xB0, 0xFC);

// 140084, 302098, 4C3CAC, 6858C0, 7C70D0, 9488E0, A8A0EC, BCB4FC
const sf::Color AtariPalette::Hue07Lum00 = sf::Color(0x14, 0x00, 0x84);
const sf::Color AtariPalette::Hue07Lum02 = sf::Color(0x30, 0x20, 0x98);
const sf::Color AtariPalette::Hue07Lum04 = sf::Color(0x4C, 0x3C, 0xAC);
const sf::Color AtariPalette::Hue07Lum06 = sf::Color(0x68, 0x58, 0xC0);
const sf::Color AtariPalette::Hue07Lum08 = sf::Color(0x7C, 0x70, 0xD0);
const sf::Color AtariPalette::Hue07Lum10 = sf::Color(0x94, 0x88, 0xE0);
const sf::Color AtariPalette::Hue07Lum12 = sf::Color(0xA8, 0xA0, 0xEC);
const sf::Color AtariPalette::Hue07Lum14 = sf::Color(0xBC, 0xB4, 0xFC);

// 000088, 1C209C, 3840B0, 505CC0, 6874D0, 7C8CE0, 90A4EC, A4B8FC
const sf::Color AtariPalette::Hue08Lum00 = sf::Color(0x00, 0x00, 0x88);
const sf::Color AtariPalette::Hue08Lum02 = sf::Color(0x1C, 0x20, 0x9C);
const sf::Color AtariPalette::Hue08Lum04 = sf::Color(0x38, 0x40, 0xB0);
const sf::Color AtariPalette::Hue08Lum06 = sf::Color(0x50, 0x5C, 0xC0);
const sf::Color AtariPalette::Hue08Lum08 = sf::Color(0x68, 0x74, 0xD0);
const sf::Color AtariPalette::Hue08Lum10 = sf::Color(0x7C, 0x8C, 0xE0);
const sf::Color AtariPalette::Hue08Lum12 = sf::Color(0x90, 0xA4, 0xEC);
const sf::Color AtariPalette::Hue08Lum14 = sf::Color(0xA4, 0xB8, 0xFC);

// 00187C, 1C3890, 3854A8, 5070BC, 6888CC, 7C9CDC, 90B4EC, A4C8FC
const sf::Color AtariPalette::Hue09Lum00 = sf::Color(0x00, 0x18, 0x7C);
const sf::Color AtariPalette::Hue09Lum02 = sf::Color(0x1C, 0x38, 0x90);
const sf::Color AtariPalette::Hue09Lum04 = sf::Color(0x38, 0x54, 0xA8);
const sf::Color AtariPalette::Hue09Lum06 = sf::Color(0x50, 0x70, 0xBC);
const sf::Color AtariPalette::Hue09Lum08 = sf::Color(0x68, 0x88, 0xCC);
const sf::Color AtariPalette::Hue09Lum10 = sf::Color(0x7C, 0x9C, 0xDC);
const sf::Color AtariPalette::Hue09Lum12 = sf::Color(0x90, 0xB4, 0xEC);
const sf::Color AtariPalette::Hue09Lum14 = sf::Color(0xA4, 0xC8, 0xFC);

// 002C5C, 1C4C78, 386890, 5084AC, 689CC0, 7CB4D4, 90CCE8, A4E0FC
const sf::Color AtariPalette::Hue10Lum00 = sf::Color(0x00, 0x2C, 0x5C);
const sf::Color AtariPalette::Hue10Lum02 = sf::Color(0x1C, 0x4C, 0x78);
const sf::Color AtariPalette::Hue10Lum04 = sf::Color(0x38, 0x68, 0x90);
const sf::Color AtariPalette::Hue10Lum06 = sf::Color(0x50, 0x84, 0xAC);
const sf::Color AtariPalette::Hue10Lum08 = sf::Color(0x68, 0x9C, 0xC0);
const sf::Color AtariPalette::Hue10Lum10 = sf::Color(0x7C, 0xB4, 0xD4);
const sf::Color AtariPalette::Hue10Lum12 = sf::Color(0x90, 0xCC, 0xE8);
const sf::Color AtariPalette::Hue10Lum14 = sf::Color(0xA4, 0xE0, 0xFC);

// 003C2C, 1C5C48, 387C64, 509C80, 68B494, 7CD0AC, 90E4C0, A4FCD4
const sf::Color AtariPalette::Hue11Lum00 = sf::Color(0x00, 0x3C, 0x2C);
const sf::Color AtariPalette::Hue11Lum02 = sf::Color(0x1C, 0x5C, 0x48);
const sf::Color AtariPalette::Hue11Lum04 = sf::Color(0x38, 0x7C, 0x64);
const sf::Color AtariPalette::Hue11Lum06 = sf::Color(0x50, 0x9C, 0x80);
const sf::Color AtariPalette::Hue11Lum08 = sf::Color(0x68, 0xB4, 0x94);
const sf::Color AtariPalette::Hue11Lum10 = sf::Color(0x7C, 0xD0, 0xAC);
const sf::Color AtariPalette::Hue11Lum12 = sf::Color(0x90, 0xE4, 0xC0);
const sf::Color AtariPalette::Hue11Lum14 = sf::Color(0xA4, 0xFC, 0xD4);

// 003C00, 205C20, 407C40, 5C9C5C, 74B474, 8CD08C, A4E4A4, B8FCB8
const sf::Color AtariPalette::Hue12Lum00 = sf::Color(0x00, 0x3C, 0x00);
const sf::Color AtariPalette::Hue12Lum02 = sf::Color(0x20, 0x5C, 0x20);
const sf::Color AtariPalette::Hue12Lum04 = sf::Color(0x40, 0x7C, 0x40);
const sf::Color AtariPalette::Hue12Lum06 = sf::Color(0x5C, 0x9C, 0x5C);
const sf::Color AtariPalette::Hue12Lum08 = sf::Color(0x74, 0xB4, 0x74);
const sf::Color AtariPalette::Hue12Lum10 = sf::Color(0x8C, 0xD0, 0x8C);
const sf::Color AtariPalette::Hue12Lum12 = sf::Color(0xA4, 0xE4, 0xA4);
const sf::Color AtariPalette::Hue12Lum14 = sf::Color(0xB8, 0xFC, 0xB8);

// 143800, 345C1C, 507C38, 6C9850, 84B468, 9CCC7C, B4E490, C8FCA4
const sf::Color AtariPalette::Hue13Lum00 = sf::Color(0x14, 0x38, 0x00);
const sf::Color AtariPalette::Hue13Lum02 = sf::Color(0x34, 0x5C, 0x1C);
const sf::Color AtariPalette::Hue13Lum04 = sf::Color(0x50, 0x7C, 0x38);
const sf::Color AtariPalette::Hue13Lum06 = sf::Color(0x6C, 0x98, 0x50);
const sf::Color AtariPalette::Hue13Lum08 = sf::Color(0x84, 0xB4, 0x68);
const sf::Color AtariPalette::Hue13Lum10 = sf::Color(0x9C, 0xCC, 0x7C);
const sf::Color AtariPalette::Hue13Lum12 = sf::Color(0xB4, 0xE4, 0x90);
const sf::Color AtariPalette::Hue13Lum14 = sf::Color(0xC8, 0xFC, 0xA4);

// 2C3000, 4C501C, 687034, 848C4C, 9CA864, B4C078, CCD488, E0EC9C
const sf::Color AtariPalette::Hue14Lum00 = sf::Color(0x2C, 0x30, 0x00);
const sf::Color AtariPalette::Hue14Lum02 = sf::Color(0x4C, 0x50, 0x1C);
const sf::Color AtariPalette::Hue14Lum04 = sf::Color(0x68, 0x70, 0x34);
const sf::Color AtariPalette::Hue14Lum06 = sf::Color(0x84, 0x8C, 0x4C);
const sf::Color AtariPalette::Hue14Lum08 = sf::Color(0x9C, 0xA8, 0x64);
const sf::Color AtariPalette::Hue14Lum10 = sf::Color(0xB4, 0xC0, 0x78);
const sf::Color AtariPalette::Hue14Lum12 = sf::Color(0xCC, 0xD4, 0x88);
const sf::Color AtariPalette::Hue14Lum14 = sf::Color(0xE0, 0xEC, 0x9C);

// 442800, 644818, 846830, A08444, B89C58, D0B46C, E8CC7C, FCE08C
const sf::Color AtariPalette::Hue15Lum00 = sf::Color(0x44, 0x28, 0x00);
const sf::Color AtariPalette::Hue15Lum02 = sf::Color(0x64, 0x48, 0x18);
const sf::Color AtariPalette::Hue15Lum04 = sf::Color(0x84, 0x68, 0x30);
const sf::Color AtariPalette::Hue15Lum06 = sf::Color(0xA0, 0x84, 0x44);
const sf::Color AtariPalette::Hue15Lum08 = sf::Color(0xB8, 0x9C, 0x58);
const sf::Color AtariPalette::Hue15Lum10 = sf::Color(0xD0, 0xB4, 0x6C);
const sf::Color AtariPalette::Hue15Lum12 = sf::Color(0xE8, 0xCC, 0x7C);
const sf::Color AtariPalette::Hue15Lum14 = sf::Color(0xFC, 0xE0, 0x8C);

const sf::Color AtariPalette::Colors[] = {
    Hue00Lum00, Hue00Lum02, Hue00Lum04, Hue00Lum06, Hue00Lum08, Hue00Lum10, Hue00Lum12, Hue00Lum14,
    Hue01Lum00, Hue01Lum02, Hue01Lum04, Hue01Lum06, Hue01Lum08, Hue01Lum10, Hue01Lum12, Hue01Lum14,
    Hue02Lum00, Hue02Lum02, Hue02Lum04, Hue02Lum06, Hue02Lum08, Hue02Lum10, Hue02Lum12, Hue02Lum14,
    Hue03Lum00, Hue03Lum02, Hue03Lum04, Hue03Lum06, Hue03Lum08, Hue03Lum10, Hue03Lum12, Hue03Lum14,
    Hue04Lum00, Hue04Lum02, Hue04Lum04, Hue04Lum06, Hue04Lum08, Hue04Lum10, Hue04Lum12, Hue04Lum14,
    Hue05Lum00, Hue05Lum02, Hue05Lum04, Hue05Lum06, Hue05Lum08, Hue05Lum10, Hue05Lum12, Hue05Lum14,
    Hue06Lum00, Hue06Lum02, Hue06Lum04, Hue06Lum06, Hue06Lum08, Hue06Lum10, Hue06Lum12, Hue06Lum14,
    Hue07Lum00, Hue07Lum02, Hue07Lum04, Hue07Lum06, Hue07Lum08, Hue07Lum10, Hue07Lum12, Hue07Lum14,
    Hue08Lum00, Hue08Lum02, Hue08Lum04, Hue08Lum06, Hue08Lum08, Hue08Lum10, Hue08Lum12, Hue08Lum14,
    Hue09Lum00, Hue09Lum02, Hue09Lum04, Hue09Lum06, Hue09Lum08, Hue09Lum10, Hue09Lum12, Hue09Lum14,
    Hue10Lum00, Hue10Lum02, Hue10Lum04, Hue10Lum06, Hue10Lum08, Hue10Lum10, Hue10Lum12, Hue10Lum14,
    Hue11Lum00, Hue11Lum02, Hue11Lum04, Hue11Lum06, Hue11Lum08, Hue11Lum10, Hue11Lum12, Hue11Lum14,
    Hue12Lum00, Hue12Lum02, Hue12Lum04, Hue12Lum06, Hue12Lum08, Hue12Lum10, Hue12Lum12, Hue12Lum14,
    Hue13Lum00, Hue13Lum02, Hue13Lum04, Hue13Lum06, Hue13Lum08, Hue13Lum10, Hue13Lum12, Hue13Lum14,
    Hue14Lum00, Hue14Lum02, Hue14Lum04, Hue14Lum06, Hue14Lum08, Hue14Lum10, Hue14Lum12, Hue14Lum14,
    Hue15Lum00, Hue15Lum02, Hue15Lum04, Hue15Lum06, Hue15Lum08, Hue15Lum10, Hue15Lum12, Hue15Lum14
};
