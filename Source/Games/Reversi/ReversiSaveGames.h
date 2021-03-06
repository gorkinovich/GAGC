/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#ifndef __REVERSI_SAVE_GAMES_HEADER__
#define __REVERSI_SAVE_GAMES_HEADER__

#include <array>
#include <string>
#include <Games/Reversi/ReversiGameData.h>

/**
 * This class represents the reversi save games.
 */
class ReversiSaveGames {
public:
    friend class SaveManager;

    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int MAX_ENTRIES = 10;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct Entry {
        bool used;
        std::string name;
        ReversiGameData data;
        Entry();
        Entry(const Entry & source);
        Entry & operator =(const Entry & source);
    };

    typedef std::array<Entry, MAX_ENTRIES> Array;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    Array & GetData() { return data_; }

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    ReversiSaveGames();
    ReversiSaveGames(const ReversiSaveGames & source);
    virtual ~ReversiSaveGames();
    ReversiSaveGames & operator =(const ReversiSaveGames & source);

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    Array data_;
};

#endif
