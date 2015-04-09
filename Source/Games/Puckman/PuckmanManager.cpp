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
#include <System/ForEach.h>
#include <Menu/DesktopState.h>
#include <Games/SaveManager.h>
#include <Games/Puckman/PuckmanMainState.h>

using namespace Puckman;

//********************************************************************************
// Properties
//********************************************************************************

//--------------------------------------------------------------------------------
// Screen
//--------------------------------------------------------------------------------

SharedTexture & Manager::Tileset() { return data_->tileset_; }

//--------------------------------------------------------------------------------
// Sound
//--------------------------------------------------------------------------------

Sound & Manager::Beginning() { return data_->beginning_; }
Sound & Manager::Chomp()     { return data_->chomp_;     }
Sound & Manager::Death()     { return data_->death_;     }
Sound & Manager::ExtraPac()  { return data_->extrapac_;  }
Sound & Manager::FruitEat()  { return data_->fruiteat_;  }
Sound & Manager::GhostEat()  { return data_->ghosteat_;  }
Sound & Manager::Siren()     { return data_->siren_;     }

//--------------------------------------------------------------------------------
// Data
//--------------------------------------------------------------------------------

Puckman::GameData * Manager::DataInstance()   { return data_->data.get();   }

//--------------------------------------------------------------------------------
// States
//--------------------------------------------------------------------------------

Puckman::InsertCoinState * Manager::InsertCoinStateInstance() {
    return data_->insertCoinState.get();
}

Puckman::SelectPlayerState * Manager::SelectPlayerStateInstance() {
    return data_->selectPlayerState.get();
}

Puckman::GameState * Manager::GameStateInstance() {
    return data_->gameState.get();
}

//********************************************************************************
// Methods
//********************************************************************************

void Manager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();
        core->InitRndSeed();

        data_.reset(new InnerData());
        data_->LoadScreenContent();
        data_->LoadSoundContent();

        data_->data.reset(new Puckman::GameData());

        data_->insertCoinState.reset(new Puckman::InsertCoinState());
        data_->selectPlayerState.reset(new Puckman::SelectPlayerState());
        data_->gameState.reset(new Puckman::GameState());

        state_ = nullptr;
        nextState_ = nullptr;
        changeState(InsertCoinStateInstance());

        SaveManager::Instance()->PuckmanLoad();
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

void Manager::Release() {
    if (initialized_ && data_) {
        nextState_ = nullptr;
        changeState(nullptr);

        data_->gameState.reset(nullptr);
        data_->selectPlayerState.reset(nullptr);
        data_->insertCoinState.reset(nullptr);

        data_->data.reset(nullptr);

        data_->UnloadSoundContent();
        data_->UnloadScreenContent();
        data_.reset(nullptr);
    }

    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//--------------------------------------------------------------------------------

void Manager::ResetGame() {
    SetNextState(InsertCoinStateInstance());
    CoreManager::Instance()->SetNextState(MakeSharedState<PuckmanMainState>());
}

//********************************************************************************
// Sound methods
//********************************************************************************

void Manager::StopAll() {
    data_->beginning_.Stop();
    data_->chomp_.Stop();
    data_->death_.Stop();
    data_->extrapac_.Stop();
    data_->fruiteat_.Stop();
    data_->ghosteat_.Stop();
    data_->siren_.Stop();
}

//********************************************************************************
// States methods
//********************************************************************************

void Manager::changeState(AbstractState * victim) {
    // First, release the last state.
    if (state_ != nullptr) {
        state_->Release();
    }
    // Second, get the new state.
    state_ = victim;
    // And last, initialize the new state.
    if (state_ != nullptr) {
        state_->Initialize();
    }
}

//--------------------------------------------------------------------------------

void Manager::ChangeToNextState() {
    if (nextState_ && state_ != nextState_) {
        changeState(nextState_);
        nextState_ = nullptr;
    }
}

//--------------------------------------------------------------------------------

void Manager::SetNextState(AbstractState * state) {
    nextState_ = state;
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

Manager::Manager() : initialized_(false), nextState_(nullptr),
    state_(nullptr), data_(nullptr) {}

//--------------------------------------------------------------------------------

Manager::~Manager() {}

//--------------------------------------------------------------------------------

Manager * Manager::Instance() {
    return &(Reference());
}

//--------------------------------------------------------------------------------

Manager & Manager::Reference() {
    static Manager instance;
    return instance;
}
