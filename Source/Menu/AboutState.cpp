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

#include "AboutState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/BitmapFont.h>
#include <System/Texture2D.h>
#include <System/Keyboard.h>
#include <Menu/DesktopState.h>

//********************************************************************************
// Constants
//********************************************************************************

const int MAX_TEXTURES = 31;
const std::string TEXTURE_PATHS[MAX_TEXTURES] = {
    "Content/About/Snake-01.png",
    "Content/About/Snake-02.png",
    "Content/About/Snake-03.png",
    "Content/About/Snake-04.png",
    "Content/About/Minesweeper-01.png",
    "Content/About/Battleship-01.png",
    "Content/About/Battleship-02.png",
    "Content/About/Battleship-03.png",
    "Content/About/Battleship-04.png",
    "Content/About/Battleship-05.png",
    "Content/About/Blackjack-01.png",
    "Content/About/TicTacToe-01.png",
    "Content/About/Pong-01.png",
    "Content/About/Pong-02.png",
    "Content/About/Pong-03.png",
    "Content/About/Pong-04.png",
    "Content/About/Pong-05.png",
    "Content/About/Pong-06.png",
    "Content/About/Puzzle-01.png",
    "Content/About/AlienParty-01.png",
    "Content/About/Chess-01.png",
    "Content/About/Chess-02.png",
    "Content/About/Reversi-01.png",
    "Content/About/Reversi-02.png",
    "Content/About/Puckman-01.png",
    "Content/About/Tetraminoes-01.png",
    "Content/About/Tetraminoes-02.png",
    "Content/About/Tetraminoes-03.png",
    "Content/About/Tetraminoes-04.png",
    "Content/About/Tetraminoes-05.png",
    "Content/About/Tetraminoes-06.png"
};

const int SECTION_INDEX       = 0;
const int SECTION_CREDITS     = 1;
const int SECTION_SNAKE       = 2;
const int SECTION_MINESWEEPER = 3;
const int SECTION_BATTLESHIP  = 4;
const int SECTION_TICTACTOE   = 5;
const int SECTION_BLACKJACK   = 6;
const int SECTION_PONG        = 7;
const int SECTION_PUZZLE      = 8;
const int SECTION_ALIENPARTY  = 9;
const int SECTION_CHESS       = 10;
const int SECTION_REVERSI     = 11;
const int SECTION_CHECKERS    = 12;
const int SECTION_PUCKMAN     = 13;
const int SECTION_TETRAMINOES = 14;

const int MAX_SECTIONS = 15;
const int SECTION_PAGES[MAX_SECTIONS] = {
    1, 1, 5, 2, 6, 2, 2, 9,
    2, 2, 3, 3, 1, 3, 7
};

//********************************************************************************
// InnerData
//********************************************************************************

struct AboutState::InnerData {
    CoreManager * core;
    int section, page;
    Texture2D screenshots[MAX_TEXTURES];

    void Draw(const std::string text, int row, int col,
        const sf::Color & color = AtariPalette::Hue00Lum14) {
        auto * font = core->Retro70Font();
        auto fw = font->Width(), fh = font->Height();
        core->Retro70Font()->Draw(text, col * fw, row * fh, color);
    }

    void DrawScreenshot(int index) {
        int x = (CoreManager::LOW_WIDTH - screenshots[index].Width()) / 2;
        int y = (CoreManager::LOW_HEIGHT - screenshots[index].Height()) / 2;
        screenshots[index].Draw(x, y);
    }
};

//********************************************************************************
// Methods
//********************************************************************************

void AboutState::Initialize() {
    auto * core = CoreManager::Instance();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    data_.reset(new InnerData());
    data_->core = core;
    data_->section = SECTION_INDEX;
    data_->page = 0;
    for (int i = 0; i < MAX_TEXTURES; ++i) {
        data_->screenshots[i].Load(TEXTURE_PATHS[i]);
    }
}

//--------------------------------------------------------------------------------

void AboutState::Release() {
    for (int i = 0; i < MAX_TEXTURES; ++i) {
        data_->screenshots[i].Unload();
    }
    data_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void AboutState::Draw(const sf::Time & timeDelta) {
    if (data_->core->Language() == TEXT_LANGUAGE_ENGLISH) {
        switch (data_->section) {
        case SECTION_INDEX:
            data_->Draw("\"About\" Index", 1, 1, AtariPalette::Hue10Lum02);

            data_->Draw("[1] Credits", 3, 1);
            data_->Draw("[2] Snake (2001 C)", 4, 1);
            data_->Draw("[3] Minesweeper (2002 VB6)", 5, 1);
            data_->Draw("[4] Battleship (2002 Pascal)", 6, 1);
            data_->Draw("[5] Tic-Tac-Toe (2003 VB6)", 7, 1);
            data_->Draw("[6] Blackjack (2003 VB6)", 8, 1);
            data_->Draw("[7] Pong (2003 VB6, 2004 ASM, 2006 Java)", 9, 1);
            data_->Draw("[8] Puzzle (2004 Pascal)", 10, 1);
            data_->Draw("[9] Alien Party (2005 C)", 11, 1);
            data_->Draw("[0] Chess (2006 Pascal)", 12, 1);
            data_->Draw("[E] Reversi (2006 Pascal)", 13, 1);
            data_->Draw("[R] Checkers (2008 Haskell)", 14, 1);
            data_->Draw("[T] Puckman (2009 C# & XNA)", 15, 1);
            data_->Draw("[Y] Tetraminoes (2009 Java)", 16, 1);

            data_->Draw("[ESC] Return to desktop", 28, 1, AtariPalette::Hue01Lum14);
            break;

        case SECTION_CREDITS:
            data_->Draw("Credits", 1, 36, AtariPalette::Hue10Lum02);

            data_->Draw("Programming and graphic design by Gorka Suárez García.", 3, 1);

            data_->Draw("Sound effects made with the great tool sfxr created by Tomas Pettersson.", 5, 1);
            data_->Draw("URL: http://www.drpetter.se/project_sfxr.html", 6, 1);

            data_->Draw("Colorless Aura, Dance of the Sugar Plum Fairies, Despair and Triumph,", 8, 1);
            data_->Draw("Gymnopedie No. 1, Gymnopedie No. 2, Gymnopedie No. 3, Prelude in C (BWV 846),", 9, 1);
            data_->Draw("Shores of Avalon, Kevin MacLeod (incompetech.com)", 10, 1);
            data_->Draw("Licensed under Creative Commons: By Attribution 3.0", 11, 1);
            data_->Draw("http://creativecommons.org/licenses/by/3.0/", 12, 1);

            data_->Draw("If you want to contact with me you do it by:", 14, 1);
            data_->Draw("e-mail:   gorkasg@yahoo.es", 15, 1);
            data_->Draw("LinkedIn: http://www.linkedin.com/in/gorkasg/en", 16, 1);
            data_->Draw("Twitter:  https://twitter.com/gorkinovich/", 17, 1);

            data_->Draw("[F1] LinkedIn", 22, 1);
            data_->Draw("[F2] Twitter", 23, 1);
            data_->Draw("[F5] sfxr", 24, 1);
            data_->Draw("[F6] Kevin MacLeod", 25, 1);

            data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
            break;

        case SECTION_SNAKE:
            if (data_->page == 0) {
                data_->Draw("Snake (2001)", 1, 33, AtariPalette::Hue10Lum02);

                data_->Draw("The snake game was my first one finished, after finish the 1st course of a", 3, 1);
                data_->Draw("Technical Education in Programming & learn enough C & conio.h, to be able", 4, 1);
                data_->Draw("to develop the game.", 5, 1);

                data_->Draw("A curious detail is that the control only has 2 keys, the \"simple control\"", 7, 1);
                data_->Draw("in the remake. I did it that way because I never like the classic control with", 8, 1);
                data_->Draw("the 4 directional keys. To me is more intuitive to have only left & right to", 9, 1);
                data_->Draw("move the snake in the game.", 10, 1);

                data_->Draw("The game has several jokes about some characters of an old spanish TV show", 12, 1);
                data_->Draw("called \"Verano Azul\". And also has some inner jokes of my classmates.", 13, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(0);
                data_->Draw("(2/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(1);
                data_->Draw("(3/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->DrawScreenshot(2);
                data_->Draw("(4/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(3);
                data_->Draw("(5/5)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_MINESWEEPER:
            if (data_->page == 0) {
                data_->Draw("Minesweeper (2002)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("In the 2nd course of my Technical Education in Programming, I started to learn", 3, 1);
                data_->Draw("Visual Basic 6. Back then I knew enough programming to read a book of VB6 and", 4, 1);
                data_->Draw("learn the language in a week. So I decided to make some games, like this one.", 5, 1);

                data_->Draw("My classmates were impressed when I showed them the game. But for some reason", 7, 1);
                data_->Draw("they were more impressed with the snake game. They even spent a week playing", 8, 1);
                data_->Draw("with the snake game to beat each other scores.", 9, 1);

                data_->Draw("A year later, I made some changes in the minesweeper game to use graphics as", 11, 1);
                data_->Draw("cells of the game, instead the VB buttons. I was trying to make it feel more", 12, 1);
                data_->Draw("look alike to the Windows game I knew.", 13, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(4);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_BATTLESHIP:
            if (data_->page == 0) {
                data_->Draw("Battleship (2002)", 1, 31, AtariPalette::Hue10Lum02);

                data_->Draw("This game was a practice for a subject of programming in the 1st course at", 3, 1);
                data_->Draw("the university. The professor said: \"If you make the battleship game, you'll", 4, 1);
                data_->Draw("have the subject approved.\" Only a few number of people & me finished the", 5, 1);
                data_->Draw("game, but the promise wasn't real & we had to do the final exam.", 6, 1);

                data_->Draw("Anyway, this game taught me a lot, because I discovered the use of state", 8, 1);
                data_->Draw("machines inside the AI of the machine.", 9, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(5);
                data_->Draw("(2/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(6);
                data_->Draw("(3/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->DrawScreenshot(7);
                data_->Draw("(4/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(8);
                data_->Draw("(5/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 5) {
                data_->DrawScreenshot(9);
                data_->Draw("(6/6)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_TICTACTOE:
            if (data_->page == 0) {
                data_->Draw("Tic-Tac-Toe (2003)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("When I was at the university, I used to have a list of \"basic\" games to", 3, 1);
                data_->Draw("do to learn videogames programming. Tic-tac-toe was one in that list.", 4, 1);
                data_->Draw("Unfortunately I never completed that list, there were about 28 games in", 5, 1);
                data_->Draw("the list. With this game I learnt how to do an AI with a system of rules,", 6, 1);
                data_->Draw("a technique used, in a bigger dimension, in real time strategy games.", 7, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(11);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_BLACKJACK:
            if (data_->page == 0) {
                data_->Draw("Blackjack (2003)", 1, 31, AtariPalette::Hue10Lum02);

                data_->Draw("After made tic-tac-toe, I started a game called Sette e mezzo (7 & a half)", 3, 1);
                data_->Draw("with VB6. I'm not a graphic artist & in those days I was even worse than", 4, 1);
                data_->Draw("today in that area, so I made the game with a horrible look.", 5, 1);

                data_->Draw("Once the game was made, I started the blackjack game, because I only needed", 7, 1);
                data_->Draw("to change a few lines of code to finish that new game.", 8, 1);

                data_->Draw("The true is, I don't remember now which one, of those games, was made in", 10, 1);
                data_->Draw("first place... ^_^U", 11, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(10);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_PONG:
            if (data_->page == 0) {
                data_->Draw("Pong (2003)", 1, 34, AtariPalette::Hue10Lum02);

                data_->Draw("This project began in 2000 or 2001, but in 2003 was when I made some", 3, 1);
                data_->Draw("improve to the game & put the GPL license. This was my first game with", 4, 1);
                data_->Draw("something almost like a kind of \"AI\". At least I tried to put an AI", 5, 1);
                data_->Draw("in the game, but the result was quite poor.", 6, 1);

                data_->Draw("After finish the game I thought to learn MFC with \"Visual\" C++ & port", 8, 1);
                data_->Draw("the game to C++. In those days I used to have the absurd idea that real", 9, 1);
                data_->Draw("games were made only with C++. Youth stuff, you know. So I started to", 10, 1);
                data_->Draw("read a book about the MFC and... and I discarded the idea forever...", 11, 1);

                data_->Draw("Really, a horror of API... -_-U", 13, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(12);
                data_->Draw("(2/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(13);
                data_->Draw("(3/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->Draw("Pong (2004)", 1, 34, AtariPalette::Hue10Lum02);

                data_->Draw("Pong is something like the \"Hello world\" in videogames programming. So", 3, 1);
                data_->Draw("I was learning at the university a subject about microprocessors and I", 4, 1);
                data_->Draw("had to learn Intel 80x86's assembler. To prove myself I really learnt it,", 5, 1);
                data_->Draw("I decided to make Pong 100% in assembler. It was a whole experience to", 6, 1);
                data_->Draw("check that I really learnt the language making that game.", 7, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(4/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(14);
                data_->Draw("(5/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 5) {
                data_->DrawScreenshot(15);
                data_->Draw("(6/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 6) {
                data_->DrawScreenshot(16);
                data_->Draw("(7/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 7) {
                data_->Draw("Pong (2006)", 1, 34, AtariPalette::Hue10Lum02);

                data_->Draw("This was my first game in J2ME. I learnt a lot with it. Especially that", 3, 1);
                data_->Draw("Java is an awful platform... lol", 4, 1);

                data_->Draw("Even with that torment I had, it isn't my only game in Java, nor J2ME.", 6, 1);

                data_->Draw("One important thing I learnt was that in managed languages, like Java,", 8, 1);
                data_->Draw("the programmer must be very careful with some type of operations, like", 9, 1);
                data_->Draw("using strings, because it's easy to not see where a new is made.", 10, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(8/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 8) {
                data_->DrawScreenshot(17);
                data_->Draw("(9/9)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_PUZZLE:
            if (data_->page == 0) {
                data_->Draw("Puzzle (2004)", 1, 33, AtariPalette::Hue10Lum02);

                data_->Draw("After make the pong in assembler, I started to learn the famous Mode13h. So", 3, 1);
                data_->Draw("I made a library to handle that mode. This game was the result of having to", 4, 1);
                data_->Draw("do a test to the library I was making.", 5, 1);

                data_->Draw("The game was done with Turbo Pascal & I learnt that you can't get memory if", 7, 1);
                data_->Draw("the block isn't a multiply of 8. To me that was a kind of WTF, so I didn't", 8, 1);
                data_->Draw("like at all Pascal back then. And the funny thing was that teachers at the", 9, 1);
                data_->Draw("university were saying that Pascal was better than C to teach programming,", 10, 1);
                data_->Draw("because C let you to make \"bad\" things. Well, at least you can do things", 11, 1);
                data_->Draw("with C, not like Pascal that allows you to do nothing at all.", 12, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(18);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_ALIENPARTY:
            if (data_->page == 0) {
                data_->Draw("Alien Party (2005)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("I made this game to give a talk in a students' association at the university.", 3, 1);
                data_->Draw("The talk was about how to start to develop games with SDL under Linux.", 4, 1);

                data_->Draw("The idea started because I made a course of videogames programming at the", 6, 1);
                data_->Draw("Complutense University. I learnt some basic ideas of games architecture,", 7, 1);
                data_->Draw("so I decided to make a talk to teach that knowledge. I thought that make", 8, 1);
                data_->Draw("a clone of the Space Invaders would be a great idea. The final result was", 9, 1);
                data_->Draw("not good enough to me, compared to the original... :S", 10, 1);

                data_->Draw("I didn't spend too much time making the game. I made the graphics with paint.", 12, 1);
                data_->Draw("But at the end, there were a lot of details I didn't implement in the game", 15, 1);
                data_->Draw("that I would liked to include. For example, more enemies, weapons & other", 16, 1);
                data_->Draw("extras that was in the original game.", 17, 1);

                data_->Draw("I gave the talk twice, but I don't know if somebody really learnt something", 19, 1);
                data_->Draw("or not. One of the talks was at the Complutense University, here in Madrid.", 20, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(19);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_CHESS:
            if (data_->page == 0) {
                data_->Draw("Chess (2006)", 1, 33, AtariPalette::Hue10Lum02);

                data_->Draw("This game makes me feel proud of it & I made it at the university. It's a", 3, 1);
                data_->Draw("chess game with an AI to play against the machine. Obviously, there are", 4, 1);
                data_->Draw("dozens of free and much better chess games, but with my experience was", 5, 1);
                data_->Draw("a great achievement to make it.", 6, 1);

                data_->Draw("There are 2 important things that I learnt with this game. 1st, do not", 8, 1);
                data_->Draw("make another game for ms-dos, nevermore. Too much stupid limits. And", 9, 1);
                data_->Draw("I'm not a genius like the developers of Battle Chess. 2nd, things are", 10, 1);
                data_->Draw("always more complicated that what you can see at the beginning.", 11, 1);
                data_->Draw("Especially when you try to make theory works in the hard cold reality.", 12, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(20);
                data_->Draw("(2/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(21);
                data_->Draw("(3/3)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_REVERSI:
            if (data_->page == 0) {
                data_->Draw("Reversi (2006)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("I know what you may be thinking. Isn't this game identical to the chess game", 3, 1);
                data_->Draw("you made? Yes, it is, because I reused the UI code to make the game. And I", 4, 1);
                data_->Draw("made it even with my hate to Pascal, because it was a work to do for a", 5, 1);
                data_->Draw("subject in the 1st course at the university, at the programming laboratory.", 6, 1);

                data_->Draw("It wasn't part of the work to implement the 13h mode or the mouse. But I was", 8, 1);
                data_->Draw("doing the chess game in those days, so I decided to use the same interface", 9, 1);
                data_->Draw("and adapt the previous game I was making. It was good, because I had a", 10, 1);
                data_->Draw("problem with the mini-max algorithm I was learning from a tutorial at gamedev", 11, 1);
                data_->Draw("or flipcode that has some errors. Reversi was simple enough to test the code", 12, 1);
                data_->Draw("and finally I found the errors.", 13, 1);

                data_->Draw("Some years later I made an iPhone version of the reversi game, called Crystal", 15, 1);
                data_->Draw("Reversi. But unfortunately Apple decided to remove it from the store in one", 16, 1);
                data_->Draw("of those days that they quit \"clones\". :'(", 17, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(22);
                data_->Draw("(2/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(23);
                data_->Draw("(3/3)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_CHECKERS:
            if (data_->page == 0) {
                data_->Draw("Checkers (2008)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("This game was a work to the functional programming subject at the university.", 3, 1);
                data_->Draw("I made it with haskell, a language that I love. I really like functional", 4, 1);
                data_->Draw("programming & it seems that only few people like it, like the nerds of the", 5, 1);
                data_->Draw("programming languages.", 6, 1);

                data_->Draw("I had a partner in this task. He was trying to do an interface to the game", 8, 1);
                data_->Draw("in OpenGL, but he found too much problems & didn't implemented his part.", 9, 1);
                data_->Draw("At the end I made the whole game & developed a text input system to enter", 10, 1);
                data_->Draw("the actions and command of the game.", 11, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
            }
            break;

        case SECTION_PUCKMAN:
            if (data_->page == 0) {
                data_->Draw("Puckman (2009)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("This is a clone of the classic Pac-man of Namco. In 2007 I was in the .NET", 3, 1);
                data_->Draw("club of students, when I thought: \"Wouldn't be great to make a talk about", 4, 1);
                data_->Draw("XNA someday?\" But I was very busy with the university. Finally, in the", 5, 1);
                data_->Draw("summer of 2009 I decided to start the game.", 6, 1);

                data_->Draw("As an amateur videogames programmer, I had more than a few games back then.", 8, 1);
                data_->Draw("But I was not happy with the result of all of them. None of those games", 9, 1);
                data_->Draw("had good graphics, animations o sound. I was very disappointed with my work.", 10, 1);
                data_->Draw("And I wanted to change that situation.", 11, 1);

                data_->Draw("I started the project without take a minute to think about it, to analyze the", 13, 1);
                data_->Draw("situation. That fool decision was a mistake & few days later I had to stop &", 14, 1);
                data_->Draw("I started to study the original game. I was trying to find the rules. I did", 15, 1);
                data_->Draw("what I could, but I was without internet back then in the place I was that", 16, 1);
                data_->Draw("summer. One day I get back to Madrid & I found a good dossier about the game", 17, 1);
                data_->Draw("on the internet, so I saved it to use that information later.", 18, 1);

                data_->Draw("I realized that many of my decisions were wrong. Maybe I didn't have to", 20, 1);
                data_->Draw("change all the code of the game, but was a lot work to do. And my main", 21, 1);
                data_->Draw("objectives with the game didn't require to make a 100% exact clone of", 22, 1);
                data_->Draw("the original. So at the end I decided to make a simplified version.", 23, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->Draw("Puckman (2009)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("I made all the logic first, leaving the AI of the ghost to the end. Thanks", 3, 1);
                data_->Draw("to the dossier I found, I didn't spend too much time implementing it. The", 4, 1);
                data_->Draw("main goals of the ghost were really well explained in that document. But,", 5, 1);
                data_->Draw("one more time, I couldn't make it exactly to the original, because the", 6, 1);
                data_->Draw("path finding algorithm was very different.", 7, 1);

                data_->Draw("I finished the first version after 30 days of development. The result was", 9, 1);
                data_->Draw("very good if you compare it to the 18 month of the original development.", 10, 1);
                data_->Draw("Finally, I had a game to be proud of it.", 11, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(2/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(24);
                data_->Draw("(3/3)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_TETRAMINOES:
            if (data_->page == 0) {
                data_->Draw("Tetraminoes (2009)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("This was an old challenge I had, to make a clone of the famous Tetris game,", 3, 1);
                data_->Draw("of the famous russian videogames developer, Alexey Pajitnov. But the real", 4, 1);
                data_->Draw("reason to make it was that I promised, to the girlfriend I had back those", 5, 1);
                data_->Draw("days, that I would like to do for her phone a version of the game.", 6, 1);

                data_->Draw("I started making 1st an applet for web browsers. After that I ported the", 8, 1);
                data_->Draw("game to J2ME. After 3 days of work I finished that mobile version. It was", 9, 1);
                data_->Draw("an awful task to change a source code in OOP to structured programming.", 10, 1);
                data_->Draw("I also had to change the control of the game, because the people that", 11, 1);
                data_->Draw("tested the game were unhappy with the first one. At the end I decided to", 12, 1);
                data_->Draw("not make in the future any game in J2ME if I wasn't going to get paid for", 13, 1);
                data_->Draw("it. And by the way, years later came the smartphones & all that technology", 14, 1);
                data_->Draw("was deprecated. So all I learnt was useless. ¬_¬U", 15, 1);

                data_->Draw("[ESC] Return to index", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(25);
                data_->Draw("(2/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(26);
                data_->Draw("(3/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->DrawScreenshot(27);
                data_->Draw("(4/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(28);
                data_->Draw("(5/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 5) {
                data_->DrawScreenshot(29);
                data_->Draw("(6/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 6) {
                data_->DrawScreenshot(30);
                data_->Draw("(7/7)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;
        }
    } else {
        switch (data_->section) {
        case SECTION_INDEX:
            data_->Draw("Índice \"Sobre...\"", 1, 1, AtariPalette::Hue10Lum02);

            data_->Draw("[1] Créditos", 3, 1);
            data_->Draw("[2] Snake (2001 C)", 4, 1);
            data_->Draw("[3] Buscaminas (2002 VB6)", 5, 1);
            data_->Draw("[4] Battleship (2002 Pascal)", 6, 1);
            data_->Draw("[5] Tic-Tac-Toe (2003 VB6)", 7, 1);
            data_->Draw("[6] Blackjack (2003 VB6)", 8, 1);
            data_->Draw("[7] Pong (2003 VB6, 2004 ASM, 2006 Java)", 9, 1);
            data_->Draw("[8] Puzzle (2004 Pascal)", 10, 1);
            data_->Draw("[9] Alien Party (2005 C)", 11, 1);
            data_->Draw("[0] Ajedrez (2006 Pascal)", 12, 1);
            data_->Draw("[E] Reversi (2006 Pascal)", 13, 1);
            data_->Draw("[R] Damas (2008 Haskell)", 14, 1);
            data_->Draw("[T] Puckman (2009 C# y XNA)", 15, 1);
            data_->Draw("[Y] Tetraminoes (2009 Java)", 16, 1);

            data_->Draw("[ESC] Volver al escritorio", 28, 1, AtariPalette::Hue01Lum14);
            break;

        case SECTION_CREDITS:
            data_->Draw("Créditos", 1, 36, AtariPalette::Hue10Lum02);

            data_->Draw("Programación y diseño gráfico por Gorka Suárez García.", 3, 1);

            data_->Draw("Efectos de sonido creados con la gran herramienta sfxr creada por Tomas", 5, 1);
            data_->Draw("Pettersson. URL: http://www.drpetter.se/project_sfxr.html", 6, 1);

            data_->Draw("Colorless Aura, Dance of the Sugar Plum Fairies, Despair and Triumph,", 8, 1);
            data_->Draw("Gymnopedie No. 1, Gymnopedie No. 2, Gymnopedie No. 3, Prelude in C (BWV 846),", 9, 1);
            data_->Draw("Shores of Avalon, Kevin MacLeod (incompetech.com)", 10, 1);
            data_->Draw("Licenciadas bajo Creative Commons: By Attribution 3.0", 11, 1);
            data_->Draw("http://creativecommons.org/licenses/by/3.0/", 12, 1);

            data_->Draw("Si deseas contactar conmigo puedes hacerlo a través de:", 14, 1);
            data_->Draw("e-mail:   gorkasg@yahoo.es", 15, 1);
            data_->Draw("LinkedIn: http://www.linkedin.com/in/gorkasg/es", 16, 1);
            data_->Draw("Facebook: https://www.facebook.com/Gorkinovich", 17, 1);
            data_->Draw("Twitter:  https://twitter.com/gorkinovich/", 18, 1);

            data_->Draw("[F1] LinkedIn", 21, 1);
            data_->Draw("[F2] Facebook", 22, 1);
            data_->Draw("[F3] Twitter", 23, 1);
            data_->Draw("[F5] sfxr", 24, 1);
            data_->Draw("[F6] Kevin MacLeod", 25, 1);

            data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
            break;

        case SECTION_SNAKE:
            if (data_->page == 0) {
                data_->Draw("Snake (2001)", 1, 33, AtariPalette::Hue10Lum02);

                data_->Draw("La serpiente fue mi primer juego terminado, tras finalizar 1º de DAI", 3, 1);
                data_->Draw("y haber aprendido lo suficiente de C y de la conio.h, como para poder", 4, 1);
                data_->Draw("afrontar su desarrollo.", 5, 1);

                data_->Draw("Como detalle curioso el control solo tenía dos teclas, lo que en la versión", 7, 1);
                data_->Draw("nueva he llamado el control simplificado. Eso fue así porque nunca me gustó", 8, 1);
                data_->Draw("el control clásico de las 4 direcciones. A mí me sigue pareciendo más", 9, 1);
                data_->Draw("intuitivo el tener solo derecha e izquierda, para girar la serpiente.", 10, 1);

                data_->Draw("Además por aquella época, teníamos una especie de \"culto\" a Chanquete", 12, 1);
                data_->Draw("y Piraña, los personajes de una serie antigua llamada \"Verano Azul\".", 13, 1);
                data_->Draw("Por ello en el juego había una serie de comentarios que eran bromas", 14, 1);
                data_->Draw("recurrentes entre los compañeros de clase.", 15, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(0);
                data_->Draw("(2/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(1);
                data_->Draw("(3/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->DrawScreenshot(2);
                data_->Draw("(4/5)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(3);
                data_->Draw("(5/5)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_MINESWEEPER:
            if (data_->page == 0) {
                data_->Draw("Buscaminas (2002)", 1, 31, AtariPalette::Hue10Lum02);

                data_->Draw("En 2º de DAI empezamos a dar Visual Basic 6. Por entonces ya sabía programar", 3, 1);
                data_->Draw("y opté por leerme un libro cutre de Anaya que tenía. En una semana ya sabía", 4, 1);
                data_->Draw("VB6 y me puse a hacer un par de juegos, este y el Pong.", 5, 1);

                data_->Draw("El caso es que causó impresión en la clase, cuando mostré el juego a mis", 7, 1);
                data_->Draw("compañeros. Sin embargo el que más les impresionó al final fue el de la", 8, 1);
                data_->Draw("serpiente. Al punto que se pusieron durante una semana entera a retarse", 9, 1);
                data_->Draw("entre ellos a ver quién sacaba la mejor puntuación.", 10, 1);

                data_->Draw("Un año más tarde retoqué el buscaminas para usar gráficos en vez de los", 12, 1);
                data_->Draw("botones de VB como casillas del juego. Intentando que se pareciera más", 13, 1);
                data_->Draw("al que yo conocía de Windows.", 14, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(4);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_BATTLESHIP:
            if (data_->page == 0) {
                data_->Draw("Battleship (2002)", 1, 31, AtariPalette::Hue10Lum02);

                data_->Draw("El motivo para hacer este juego fue una asignatura de programación de 1º de", 3, 1);
                data_->Draw("carrera. El profesor decía que si lográbamos hacer el hundir la flota nos", 4, 1);
                data_->Draw("aprobaría la asignatura. Presentamos el juego yo y unas pocas personas más,", 5, 1);
                data_->Draw("pero ninguno vio ese milagroso aprobado y nos tuvimos que presentar al examen.", 6, 1);

                data_->Draw("Al final hacerlo me sirvió para mucho, pues descubrí el uso de las máquinas", 8, 1);
                data_->Draw("de estados para la IA de la máquina.", 9, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(5);
                data_->Draw("(2/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(6);
                data_->Draw("(3/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->DrawScreenshot(7);
                data_->Draw("(4/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(8);
                data_->Draw("(5/6)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 5) {
                data_->DrawScreenshot(9);
                data_->Draw("(6/6)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_TICTACTOE:
            if (data_->page == 0) {
                data_->Draw("Tic-Tac-Toe (2003)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("Estando ya en la universidad me puse a hacer el Tic-tac-toe, para avanzar", 3, 1);
                data_->Draw("en  mi lista de juegos \"básicos\" que debía hacer para aprender a programar", 4, 1);
                data_->Draw("videojuegos. Lo cierto es que no completé la lista, pues eran unos 28 en", 5, 1);
                data_->Draw("total. Sin embargo este cayó del listado y aprendí algo así como lo que es", 6, 1);
                data_->Draw("un sistema de reglas mínimo para una IA, algo que en mayor magnitud se", 7, 1);
                data_->Draw("emplea en los juegos de estrategia en tiempo real.", 8, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(11);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_BLACKJACK:
            if (data_->page == 0) {
                data_->Draw("Blackjack (2003)", 1, 31, AtariPalette::Hue10Lum02);

                data_->Draw("Estando en racha después de hacer el tic-tac-toe, me puse a hacer el juego,", 3, 1);
                data_->Draw("de las 7 y media. Como lo mío no es ser un artista gráfico, y mucho menos", 4, 1);
                data_->Draw("por entonces, lo hice de forma muy cutre en VB6.", 5, 1);

                data_->Draw("Entonces, al tener hecho uno, me puse a hacer el blackjack. Solo tenía que", 7, 1);
                data_->Draw("cambiar unas cuantas líneas de código y poco más.", 8, 1);

                data_->Draw("Lo cierto es que ya no recuerdo si fue este el primero uno u otro... ^_^U", 10, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(10);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_PONG:
            if (data_->page == 0) {
                data_->Draw("Pong (2003)", 1, 34, AtariPalette::Hue10Lum02);

                data_->Draw("A pesar de estar fechado como del 2003, el proyecto empezó en el 2000 o 2001.", 3, 1);
                data_->Draw("En el 2003 lo que ocurrió fue que debí realizar alguna pequeña mejora, además", 4, 1);
                data_->Draw("de ponerle la licencia GPL. Este fue mi primer juego con algo así como una", 5, 1);
                data_->Draw("\"inteligencia artificial\". Al menos lo intenté, aunque el resultado fue un", 6, 1);
                data_->Draw("poco lamentable.", 7, 1);

                data_->Draw("Después de terminarlo pensé en aprender a usar las MFC con el \"Visual\" C++", 9, 1);
                data_->Draw("para portarlo a C++. Por entonces tenía la absurda idea de pensar que para", 10, 1);
                data_->Draw("hacer un juego de verdad había que usar C++, cosas de la juventud. El caso es", 11, 1);
                data_->Draw("que me puse a leer un libro sobre las MFC... y acabé descartando la idea...", 12, 1);

                data_->Draw("En serio, menudo horror de API... -_-U", 14, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(12);
                data_->Draw("(2/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(13);
                data_->Draw("(3/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->Draw("Pong (2004)", 1, 34, AtariPalette::Hue10Lum02);

                data_->Draw("Se podría decir que el Pong es como el \"Hola mundo\" a la hora de aprender", 3, 1);
                data_->Draw("a programar videojuegos. Así que estaba estudiando una asignatura sobre", 4, 1);
                data_->Draw("microprocesadores y había que aprender ensamblador del Intel 80x86. Para", 5, 1);
                data_->Draw("convencerme de si realmente lo había logrado, opté por hacerlo íntegramente", 6, 1);
                data_->Draw("en ensamblador. Fue toda una experiencia \"comprobar\" que realmente había", 7, 1);
                data_->Draw("aprendido ensamblador, haciendo este juego ^_^U", 8, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(4/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(14);
                data_->Draw("(5/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 5) {
                data_->DrawScreenshot(15);
                data_->Draw("(6/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 6) {
                data_->DrawScreenshot(16);
                data_->Draw("(7/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 7) {
                data_->Draw("Pong (2006)", 1, 34, AtariPalette::Hue10Lum02);

                data_->Draw("Este fue mi primer juego para J2ME. Me hizo aprender bastante. Sobre todo", 3, 1);
                data_->Draw("que Java es una plataforma horrible... xD", 4, 1);

                data_->Draw("A pesar del tormento, no fue mi único juego en Java, ni en J2ME.", 6, 1);

                data_->Draw("Una de las cosas que aprendí entonces, era que había que tener mucho", 8, 1);
                data_->Draw("cuidado con operaciones aparentemente inocuas como lo era usar cadenas.", 9, 1);
                data_->Draw("Yo estaba acostumbrado a tener control relativamente total de la memoria,", 10, 1);
                data_->Draw("y en lenguajes como Java es fácil no darse cuenta de que se están haciendo", 11, 1);
                data_->Draw("news de forma \"involuntaria\" al manejar objetos como las cadenas.", 12, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(8/9)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 8) {
                data_->DrawScreenshot(17);
                data_->Draw("(9/9)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_PUZZLE:
            if (data_->page == 0) {
                data_->Draw("Puzzle (2004)", 1, 33, AtariPalette::Hue10Lum02);

                data_->Draw("Después de hacer el Pong en ensamblador, se me ocurrió trastear con el Modo13h", 3, 1);
                data_->Draw("y hacerme una librería para ello. Este juego fue el resultado de tener que", 4, 1);
                data_->Draw("comprobar que dicha librería funcionaba bien.", 5, 1);

                data_->Draw("Está hecho con Turbo Pascal y aprendí que no te deja pedir memoria que no sea", 7, 1);
                data_->Draw("múltiplo de 8. Ello fue un motivo más para llevarme las manos a la cabeza cada", 8, 1);
                data_->Draw("vez que un profesor me decía que Pascal era mejor lenguaje que C/C++. Uno de", 9, 1);
                data_->Draw("los motivos que me dieron era que C te deja hacer cosas \"malas\". Claro que", 10, 1);
                data_->Draw("Pascal no deja hacer cosas malas, no te deja hacer nada en general... ¬_¬'", 11, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(18);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_ALIENPARTY:
            if (data_->page == 0) {
                data_->Draw("Alien Party (2005)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("Este juego fue realizado con motivo de dar una charla en las Jornadas del", 3, 1);
                data_->Draw("Software Libre de la UPSAM, organizadas por la asociación Linups, sobre", 4, 1);
                data_->Draw("como introducirse en la programación de juegos con SDL bajo Linux.", 5, 1);

                data_->Draw("La historia completa fue que hice en la Complutense un curso de programación", 7, 1);
                data_->Draw("de videojuegos en la escuela de verano. De ahí saqué algunas ideas", 8, 1);
                data_->Draw("fundamentales sobre la arquitectura básica de todo videojuego. Así que", 9, 1);
                data_->Draw("decidí aprovechar para hacer un taller de programación, con el fin de pasar", 10, 1);
                data_->Draw("a otros esos conocimientos. Y qué mejor que intentar hacer un clon del Space", 11, 1);
                data_->Draw("Invaders, fue lo que me dije a mí mismo. Al final, el resultado fue bastante", 12, 1);
                data_->Draw("cutre al compararlo con el original... :S", 13, 1);

                data_->Draw("No me llevó mucho tiempo desarrollarlo, a pesar de tener que hacer yo los", 15, 1);
                data_->Draw("gráficos con el paint. Sin embargo se quedaron en el tintero algunas cosas", 16, 1);
                data_->Draw("que me hubiera gustado añadir. Por ejemplo, un poco más de diversidad de", 17, 1);
                data_->Draw("enemigos, más armas (algunas de hecho están en las texturas del juego), una", 18, 1);
                data_->Draw("diferenciación clara entre cada cambio de fase y algunos extras como la nave", 19, 1);
                data_->Draw("que pasaba deprisa por arriba en el original.", 20, 1);

                data_->Draw("La charla no estuvo mal, aunque no sé si alguien aprendió algo. El caso es", 22, 1);
                data_->Draw("que al año siguiente, el 13 de mayo del 2005, tuve oportunidad de repetirla", 23, 1);
                data_->Draw("en la UCM gracias a la asociación GUEIM.", 24, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/2)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(19);
                data_->Draw("(2/2)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_CHESS:
            if (data_->page == 0) {
                data_->Draw("Ajedrez (2006)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("Este es uno de los juegos que más orgulloso me siento de haber realizado en", 3, 1);
                data_->Draw("mis años de universidad. Consiste en un ajedrez dotado de una IA para poder", 4, 1);
                data_->Draw("jugar contra él. No es ninguna maravilla comparado con las docenas de", 5, 1);
                data_->Draw("ajedreces gratuitos que se pueden encontrar por internet, pero dada la", 6, 1);
                data_->Draw("experiencia que tenía en su momento fue todo un logro hacerlo.", 7, 1);

                data_->Draw("Dos cosas muy importantes aprendí con este juego. 1º que no volvería a", 9, 1);
                data_->Draw("programar nada para ms-dos nunca más, pues sus limitaciones son abusivas", 10, 1);
                data_->Draw("para alguien que no está acostumbrado a realizar optimizaciones épicas;", 11, 1);
                data_->Draw("como los grandes maestros que desarrollaron el Battle Chess por ejemplo.", 12, 1);
                data_->Draw("2º que las cosas siempre son más complicadas de lo que aparentemente", 13, 1);
                data_->Draw("parecían en un principio. Sobre todo cuando se trata de hacer funcionar", 14, 1);
                data_->Draw("la teoría, en la dura limitación de la realidad.", 15, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(20);
                data_->Draw("(2/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(21);
                data_->Draw("(3/3)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_REVERSI:
            if (data_->page == 0) {
                data_->Draw("Reversi (2006)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("Sé lo que posiblemente estén pensando. ¿Este juego no es calcado al ajedrez", 3, 1);
                data_->Draw("que hiciste para ms-dos? Pues sí, principalmente porque reutilicé el código", 4, 1);
                data_->Draw("de la interfaz, para desarrollarlo. Y lo hice, a pesar de mi extrema aversión", 5, 1);
                data_->Draw("a Pascal, porque era una práctica del laboratorio de programación en 1º.", 6, 1);

                data_->Draw("Obviamente no pedían que implementara el modo13h o el ratón. Pero cuando la", 8, 1);
                data_->Draw("mandaron, tenía ya hecha la interfaz del Ajedrez y no me supuso problema", 9, 1);
                data_->Draw("adaptar lo que ya tenía hecho. Lo bueno fue que este juego me sirvió como", 10, 1);
                data_->Draw("banco de pruebas para corregir el algoritmo de búsqueda de la IA (un mini-max", 11, 1);
                data_->Draw("con podas alfa-beta), pues el tutorial de internet que miré de flipcode o de", 12, 1);
                data_->Draw("gamedev tenía errores en su implementación.", 13, 1);

                data_->Draw("Tiempo más tarde haría una versión para iPhone de este juego que se llamó", 15, 1);
                data_->Draw("Crystal Reversi. Por desgracia Apple decidió quitarlo de la tienda en un", 16, 1);
                data_->Draw("arrebato de destrucción de \"clones\". :'(", 17, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(22);
                data_->Draw("(2/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(23);
                data_->Draw("(3/3)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_CHECKERS:
            if (data_->page == 0) {
                data_->Draw("Damas (2008)", 1, 33, AtariPalette::Hue10Lum02);

                data_->Draw("Este juego fue para una práctica de programación funcional en la universidad", 3, 1);
                data_->Draw("y lo programé en haskell, lenguaje de mis amores. Lo cierto es que me gusta", 4, 1);
                data_->Draw("la programación funcional bastante, tiene cierto encanto que parece ser que", 5, 1);
                data_->Draw("solo se lo veo yo y la gente friki de los lenguajes de programación.", 6, 1);

                data_->Draw("Eramos dos en la práctica y mi compañero iba a haber hecho una interfaz", 8, 1);
                data_->Draw("gráfica, pero tuvimos problemas con las APIs de OpenGL para haskell y al", 9, 1);
                data_->Draw("final no se pudo hacer esa parte. Así que el juego lo acabé haciendo yo", 10, 1);
                data_->Draw("solo y me hice una especie de entrada de texto para meter las acciones del", 11, 1);
                data_->Draw("jugador a la hora de echar una partida.", 12, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
            }
            break;

        case SECTION_PUCKMAN:
            if (data_->page == 0) {
                data_->Draw("Puckman (2009)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("Es un clon del clásico Pac-man de Namco. En el 2007, en una reunión del club", 3, 1);
                data_->Draw("de .NET de la universidad, se me ocurrió que podía ser interesante el hacerlo", 4, 1);
                data_->Draw("con XNA y dar luego charlas sobre ello. Por desgracia, los estudios me tenían", 5, 1);
                data_->Draw("ocupado. No fue hasta el verano del 2009 que me puse a ello.", 6, 1);

                data_->Draw("Como programador amateur de videojuegos había hecho, a estas alturas, unos", 8, 1);
                data_->Draw("cuantos juegos. Sin embargo todos carecían de gráficos vistosos, ninguno", 9, 1);
                data_->Draw("tenía sonido y no había ninguna animación. Así que tenía una especie de", 10, 1);
                data_->Draw("espina clavada con el tema y estaba insatisfecho conmigo mismo.", 11, 1);

                data_->Draw("Empecé el proyecto a lo loco, sin analizar el tema demasiado. Ello supuso un", 13, 1);
                data_->Draw("serio problema a los pocos días y tuve que ponerme a estudiar el original,", 14, 1);
                data_->Draw("para descubrir las reglas del juego. Fui haciendo lo que pude porque en aquel", 15, 1);
                data_->Draw("momento no tenía internet donde estaba veraneando, pero un día que volví a", 16, 1);
                data_->Draw("Madrid encontré un dosier bastante completo sobre el tema y me lo guardé.", 17, 1);

                data_->Draw("Descubrí que había enfocado muchas cosas mal. Podría haberlo corregido sin", 19, 1);
                data_->Draw("tener que cambiar todo el código, puede que ni siquiera la mitad, aun así", 20, 1);
                data_->Draw("era demasiado trabajo y para mi objetivo daba igual que fuera un clon 100%", 21, 1);
                data_->Draw("fiel al original. Así que opté por dejar algunas cosas simplificadas.", 22, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->Draw("Puckman (2009)", 1, 32, AtariPalette::Hue10Lum02);

                data_->Draw("Logré montar toda la lógica, dejando para lo último la IA de los fantasmas.", 3, 1);
                data_->Draw("Tampoco me llevó mucho tiempo hacerla gracias al dosier que encontré, donde", 4, 1);
                data_->Draw("venía bien explicado cuales son los objetivos que los fantasmas intentan", 5, 1);
                data_->Draw("alcanzar. Sin embargo tampoco logré que la IA fuera exactamente igual, ya", 6, 1);
                data_->Draw("que el algoritmo para buscar caminos no es igual, ni siquiera la elección", 7, 1);
                data_->Draw("al azar de una dirección.", 8, 1);

                data_->Draw("Al final, lo que parecía un desarrollo que no llegaba a su fin, terminó en", 10, 1);
                data_->Draw("una 1ª versión alfa tras aproximadamente 30 días de desarrollo. El resultado", 11, 1);
                data_->Draw("final fue bastante bueno, comparado con los 18 meses que se tardó en hacer", 12, 1);
                data_->Draw("el original. Y con ello tuve un juego programado por mi, del que sentirme lo", 13, 1);
                data_->Draw("suficientemente orgulloso.", 14, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(2/3)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(24);
                data_->Draw("(3/3)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;

        case SECTION_TETRAMINOES:
            if (data_->page == 0) {
                data_->Draw("Tetraminoes (2009)", 1, 30, AtariPalette::Hue10Lum02);

                data_->Draw("Fue un reto que tenía pendiente en su día, el crear un clon del mítico Tetris", 3, 1);
                data_->Draw("de Alexey Pajitnov, el famoso programador ruso de videojuegos. Pero la razón", 4, 1);
                data_->Draw("final por la que lo hice fue una promesa a quien era mi novia entonces. Le", 5, 1);
                data_->Draw("prometí hacer un tetris en condiciones para su móvil.", 6, 1);

                data_->Draw("Según me puse manos a la obra pensé que no estaría mal hacer 1º la versión", 8, 1);
                data_->Draw("applet del juego para navegadores web. Aunque como no tenía servidor web", 9, 1);
                data_->Draw("propio no la llegué a colgar online. Una vez terminado me puse con el", 10, 1);
                data_->Draw("proyecto para J2ME. Después de 3 días de copiar, pegar y modificar,", 11, 1);
                data_->Draw("conseguí portar la versión applet a móviles.", 12, 1);

                data_->Draw("La tarea fue pasar de la programación orientada a objetos a la estructurada,", 14, 1);
                data_->Draw("algo súper-divertido si te gusta flagelarte gratuitamente. También tuve que", 15, 1);
                data_->Draw("modificar el control de ambos, pues no tuvo mucho éxito el que había puesto.", 16, 1);
                data_->Draw("Después de terminar opté por no volver a hacer nada para J2ME si no era", 17, 1);
                data_->Draw("cobrando dinero por ello, claro que un par de años más tarde la tecnología", 18, 1);
                data_->Draw("cayó en desgracia con la llegada de los smartphones. Y todo lo que había", 19, 1);
                data_->Draw("aprendido no sirvió para nada. ¬_¬U", 20, 1);

                data_->Draw("[ESC] Volver al índice", 28, 1, AtariPalette::Hue01Lum14);
                data_->Draw("(1/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 1) {
                data_->DrawScreenshot(25);
                data_->Draw("(2/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 2) {
                data_->DrawScreenshot(26);
                data_->Draw("(3/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 3) {
                data_->DrawScreenshot(27);
                data_->Draw("(4/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 4) {
                data_->DrawScreenshot(28);
                data_->Draw("(5/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 5) {
                data_->DrawScreenshot(29);
                data_->Draw("(6/7)", 28, 74, AtariPalette::Hue12Lum02);

            } else if (data_->page == 6) {
                data_->DrawScreenshot(30);
                data_->Draw("(7/7)", 28, 74, AtariPalette::Hue12Lum02);
            }
            break;
        }
    }
}

//--------------------------------------------------------------------------------

void AboutState::Update(const sf::Time & timeDelta) {
    switch (data_->section) {
    case SECTION_INDEX:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            data_->core->SetNextState(MakeSharedState<DesktopState>());
        } else if (Keyboard::IsKeyUp(Keyboard::Num1)) {
            data_->section = SECTION_CREDITS;
        } else if (Keyboard::IsKeyUp(Keyboard::Num2)) {
            data_->section = SECTION_SNAKE;
        } else if (Keyboard::IsKeyUp(Keyboard::Num3)) {
            data_->section = SECTION_MINESWEEPER;
        } else if (Keyboard::IsKeyUp(Keyboard::Num4)) {
            data_->section = SECTION_BATTLESHIP;
        } else if (Keyboard::IsKeyUp(Keyboard::Num5)) {
            data_->section = SECTION_TICTACTOE;
        } else if (Keyboard::IsKeyUp(Keyboard::Num6)) {
            data_->section = SECTION_BLACKJACK;
        } else if (Keyboard::IsKeyUp(Keyboard::Num7)) {
            data_->section = SECTION_PONG;
        } else if (Keyboard::IsKeyUp(Keyboard::Num8)) {
            data_->section = SECTION_PUZZLE;
        } else if (Keyboard::IsKeyUp(Keyboard::Num9)) {
            data_->section = SECTION_ALIENPARTY;
        } else if (Keyboard::IsKeyUp(Keyboard::Num0)) {
            data_->section = SECTION_CHESS;
        } else if (Keyboard::IsKeyUp(Keyboard::E)) {
            data_->section = SECTION_REVERSI;
        } else if (Keyboard::IsKeyUp(Keyboard::R)) {
            data_->section = SECTION_CHECKERS;
        } else if (Keyboard::IsKeyUp(Keyboard::T)) {
            data_->section = SECTION_PUCKMAN;
        } else if (Keyboard::IsKeyUp(Keyboard::Y)) {
            data_->section = SECTION_TETRAMINOES;
        }
        break;

    case SECTION_CREDITS:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            data_->section = SECTION_INDEX;

        } else {
            auto * core = data_->core;
            auto language = core->Language();
            if (language == TEXT_LANGUAGE_ENGLISH) {
                if (Keyboard::IsKeyUp(Keyboard::F1)) {
                    core->OpenURL("http://www.linkedin.com/in/gorkasg/en");
                } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
                    core->OpenURL("https://twitter.com/gorkinovich/");
                } else if (Keyboard::IsKeyUp(Keyboard::F5)) {
                    core->OpenURL("http://www.drpetter.se/project_sfxr.html");
                } else if (Keyboard::IsKeyUp(Keyboard::F6)) {
                    core->OpenURL("http://incompetech.com");
                }
            } else {
                if (Keyboard::IsKeyUp(Keyboard::F1)) {
                    core->OpenURL("http://www.linkedin.com/in/gorkasg/es");
                } else if (Keyboard::IsKeyUp(Keyboard::F2)) {
                    core->OpenURL("https://www.facebook.com/Gorkinovich");
                } else if (Keyboard::IsKeyUp(Keyboard::F3)) {
                    core->OpenURL("https://twitter.com/gorkinovich/");
                } else if (Keyboard::IsKeyUp(Keyboard::F5)) {
                    core->OpenURL("http://www.drpetter.se/project_sfxr.html");
                } else if (Keyboard::IsKeyUp(Keyboard::F6)) {
                    core->OpenURL("http://incompetech.com");
                }
            }
        }
        break;

    default:
        if (Keyboard::IsKeyUp(Keyboard::Escape)) {
            data_->section = SECTION_INDEX;
            data_->page = 0;

        } else if (Keyboard::IsKeyUp(Keyboard::Left) || Keyboard::IsKeyUp(Keyboard::PageUp)) {
            if (data_->page > 0) {
                data_->page--;
            }

        } else if (Keyboard::IsKeyUp(Keyboard::Right) || Keyboard::IsKeyUp(Keyboard::PageDown)) {
            data_->page++;
            int maxPages = SECTION_PAGES[data_->section];
            if (data_->page >= maxPages) {
                data_->page = maxPages - 1;
            }
        }
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

AboutState::AboutState() {}

//--------------------------------------------------------------------------------

AboutState::~AboutState() {}
