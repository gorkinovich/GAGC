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

#include "BlackjackGameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Mouse.h>
#include <Games/Blackjack/BlackjackManager.h>

//********************************************************************************
// Methods
//********************************************************************************

void BlackjackGameState::Initialize() {
    auto * core = CoreManager::Instance();
    core->ShowMouseCursor();
    core->SetClearColor(AtariPalette::Hue12Lum00);
    manager = BlackjackManager::Instance();
    Mouse::Update();
}

//--------------------------------------------------------------------------------

void BlackjackGameState::Release() {
    manager = nullptr;
}

//--------------------------------------------------------------------------------

void BlackjackGameState::Draw(const sf::Time & timeDelta) {
    manager->DrawGame();
}

//--------------------------------------------------------------------------------

void BlackjackGameState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    manager->UpdateGame(timeDelta);
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

BlackjackGameState::BlackjackGameState() : manager(nullptr) {}

//--------------------------------------------------------------------------------

BlackjackGameState::~BlackjackGameState() {}
