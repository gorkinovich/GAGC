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

#include "ChessEnterNameState.h"
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Sound.h>
#include <System/Texture2D.h>
#include <System/SimpleLabel.h>
#include <System/GUIUtil.h>
#include <System/Keyboard.h>
#include <System/ForEach.h>
#include <Games/SaveManager.h>
#include <Games/Chess/ChessManager.h>
#include <Games/Chess/ChessSaveGames.h>
#include <Games/Chess/ChessSaveState.h>

//********************************************************************************
// Methods
//********************************************************************************

void ChessEnterNameState::Initialize() {
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    core->HideMouseCursor();
    core->SetClearColor(AtariPalette::Hue00Lum00);

    auto * manager = ChessManager::Instance();
    auto & savegames = manager->Saves().GetData();
    auto savename = savegames[ChessSaveState::LastSelecteIndex()].used ?
        savegames[ChessSaveState::LastSelecteIndex()].name : "";

    const int ROW0 = 156, ROW1 = 192;

    lineLabel_.reset(new SimpleLabel("Enter the savegame name (max. 16 characters):",
        0, ROW0, AtariPalette::Hue00Lum14, core->Retro70Font()));

    nameLabel_.reset(new SimpleLabel(savename + "_", 0, ROW1,
        AtariPalette::Hue00Lum14, core->Retro70Font()));

    if (language == TEXT_LANGUAGE_SPANISH) {
        lineLabel_->Text("Introduce el nombre de la partida (max. 16 caracteres):");
    }

    GUIUtil::CenterLabel(lineLabel_, CoreManager::HALF_LOW_WIDTH);
    GUIUtil::CenterLabel(nameLabel_, CoreManager::HALF_LOW_WIDTH);

    core->KeyboardText(savename);
    core->EnableKeyboardTextInput();
}

//--------------------------------------------------------------------------------

void ChessEnterNameState::Release() {
    auto * core = CoreManager::Instance();
    core->DisableKeyboardTextInput();
    lineLabel_.reset(nullptr);
    nameLabel_.reset(nullptr);
}

//--------------------------------------------------------------------------------

void ChessEnterNameState::Draw(const sf::Time & timeDelta) {
    lineLabel_->Draw();
    nameLabel_->Draw();
    ChessManager::Instance()->DrawMouseCursor();
}

//--------------------------------------------------------------------------------

void ChessEnterNameState::Update(const sf::Time & timeDelta) {
    auto * core = CoreManager::Instance();
    auto * manager = ChessManager::Instance();
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        manager->KeyboardSound().Play();
        core->SetNextState(MakeSharedState<ChessSaveState>());

    } else if (Keyboard::IsKeyUp(Keyboard::Return)) {
        manager->KeyboardSound().Play();
        std::string name = nameLabel_->Text().substr(0, nameLabel_->Text().length() - 1);
        manager->SaveCurrentGame(ChessSaveState::LastSelecteIndex(), name);

    } else {
        const int MAX_CHARS = 16;
        std::string name = "";
        std::string text = core->KeyboardText().substr(0, MAX_CHARS);
        ForEach(text, [&] (char item) {
            if (item != '\n') {
                name += item;
            }
        });
        core->KeyboardText(name);
        nameLabel_->Text(name + "_");
        GUIUtil::CenterLabel(nameLabel_, CoreManager::HALF_LOW_WIDTH);
    }
}

//********************************************************************************
// Constructors and destructor
//********************************************************************************

ChessEnterNameState::ChessEnterNameState() {}

//--------------------------------------------------------------------------------

ChessEnterNameState::~ChessEnterNameState() {}
