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

#include "PuckmanMainState.h"
#include <SFML/System/Time.hpp>
#include <System/Mouse.h>
#include <Games/Puckman/PuckmanManager.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

void PuckmanMainState::Initialize() {
    manager = Manager::Instance();
    manager->Initialize();
    PuckmanSharedState::Initialize();
    Mouse::Update();
}

//--------------------------------------------------------------------------------

void PuckmanMainState::Release() {
    PuckmanSharedState::Release();
    manager = nullptr;
}

//--------------------------------------------------------------------------------

void PuckmanMainState::Draw(const sf::Time & timeDelta) {
    PuckmanSharedState::Draw(timeDelta);
    manager->State()->Draw(timeDelta);
}

//--------------------------------------------------------------------------------

void PuckmanMainState::Update(const sf::Time & timeDelta) {
    Mouse::Update();
    PuckmanSharedState::Update(timeDelta);
    manager->State()->Update(timeDelta);
    manager->ChangeToNextState();
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

PuckmanMainState::PuckmanMainState() : manager(nullptr) {}

//--------------------------------------------------------------------------------

PuckmanMainState::~PuckmanMainState() {}
