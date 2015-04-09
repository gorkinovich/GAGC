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

#include "TicTacToeGameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <Games/TicTacToe/TicTacToeManager.h>

//********************************************************************************
// Methods
//********************************************************************************

void TicTacToeGameState::Initialize() {
    auto * core = CoreManager::Instance();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);
    manager = TicTacToeManager::Instance();
}

//--------------------------------------------------------------------------------

void TicTacToeGameState::Release() {
    manager = nullptr;
}

//--------------------------------------------------------------------------------

void TicTacToeGameState::Draw(const sf::Time & timeDelta) {
    manager->DrawGame();
}

//--------------------------------------------------------------------------------

void TicTacToeGameState::Update(const sf::Time & timeDelta) {
    manager->UpdateGame(timeDelta);
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

TicTacToeGameState::TicTacToeGameState() : manager(nullptr) {}

//--------------------------------------------------------------------------------

TicTacToeGameState::~TicTacToeGameState() {}
