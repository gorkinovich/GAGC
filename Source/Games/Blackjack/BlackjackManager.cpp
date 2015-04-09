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

#include "BlackjackManager.h"
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <System/CoreManager.h>
#include <System/AtariPalette.h>
#include <System/Texture2D.h>
#include <System/Sound.h>
#include <System/SimpleLabel.h>
#include <System/TexturedButton.h>
#include <System/GUIUtil.h>
#include <System/Mouse.h>
#include <System/Keyboard.h>
#include <System/MathUtil.h>
#include <System/ForEach.h>
#include <Menu/DesktopState.h>
#include <Games/Blackjack/BlackjackGameState.h>
#include <Games/Blackjack/BlackjackGameMenuState.h>

//********************************************************************************
// Defines
//********************************************************************************

#define PLAYER_STATE   0
#define MACHINE_STATE  1
#define FINAL_STATE    2

#define MAX_TYPES_OF_CARDS   4
#define MAX_CARDS_BY_TYPE   13
#define MAX_PLAYER_CARDS    12

#define MAX_PAUSE_INTERVAL  200

#define MAX_FINAL_VALUE  21

//********************************************************************************
// Types
//********************************************************************************

enum BlackjackCardType {
    SPADE_CARD = 0,
    HEART_CARD = 1,
    CLUB_CARD = 2,
    DIAMOND_CARD = 3
};

struct BlackjackCard {
    BlackjackCardType type;
    unsigned int value; // 0..12
};

//********************************************************************************
// InnerData
//********************************************************************************

struct BlackjackManager::InnerData {
    SharedTexture tileset;
    Texture2D cardsTextures[MAX_TYPES_OF_CARDS][MAX_CARDS_BY_TYPE];

    int winner;
    int currentTime;
    std::vector<BlackjackCard> currentCards;
    std::vector<BlackjackCard> playerCards;
    std::vector<BlackjackCard> machineCards;
    unsigned int playerValue;

    int substate;
    std::unique_ptr<SimpleLabel> messageLabel;
    std::unique_ptr<SimpleLabel> machineLabel;
    std::unique_ptr<SimpleLabel> playerLabel;
    std::unique_ptr<SimpleLabel> newGameLabel;
    std::unique_ptr<SimpleLabel> getCardLabel;
    std::unique_ptr<SimpleLabel> standLabel;
    std::unique_ptr<SimpleLabel> menuLabel;
    std::unique_ptr<TexturedButton> newGameButton;
    std::unique_ptr<TexturedButton> getCardButton;
    std::unique_ptr<TexturedButton> standButton;
    std::unique_ptr<TexturedButton> menuButton;

    Sound keyboardSound;
    Sound clickSound;

    void GetRandomCard(std::vector<BlackjackCard> & victim) {
        if (!currentCards.empty()) {
            int index = CoreManager::Instance()->Random(currentCards.size());
            victim.push_back(currentCards[index]);
            currentCards.erase(currentCards.begin() + index);
        }
    }

    unsigned int GetFinalValue(std::vector<BlackjackCard> & victim) {
        unsigned int min = 0, max = 0;
        ForEach(victim, [&] (BlackjackCard & item) {
            unsigned int value = item.value + 1;
            const unsigned int MIN_VALUE = 1, MAX_VALUE = 10;
            if (value <= MIN_VALUE) {
                min += MIN_VALUE, max += (MAX_VALUE + MIN_VALUE);
            } else if (MIN_VALUE < value && value <= MAX_VALUE) {
                min += value, max += value;
            } else {
                min += MAX_VALUE, max += MAX_VALUE;
            }
        });
        return max <= MAX_FINAL_VALUE ? max : min;
    }
};

//********************************************************************************
// Properties
//********************************************************************************

SharedTexture & BlackjackManager::Tileset() { return data_->tileset; }

//--------------------------------------------------------------------------------

Sound & BlackjackManager::KeyboardSound() { return data_->keyboardSound; }
Sound & BlackjackManager::ClickSound()    { return data_->clickSound;    }

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void BlackjackManager::Initialize() {
    if (!initialized_) {
        auto * core = CoreManager::Instance();
        auto language = core->Language();

        // Get memory for the internal data.
        data_.reset(new InnerData());

        // Load the textures of the game.
        data_->tileset = core->LoadTexture("Content/Textures/Blackjack.png");

        const int CARD_WIDTH = 42, CARD_HEIGHT = 66, X_LIMIT = 504;
        for (int i = 0, x = 0, y = 0; i < MAX_TYPES_OF_CARDS; ++i) {
            for (int j = 0; j < MAX_CARDS_BY_TYPE; ++j) {
                auto rect = sf::IntRect(x, y, CARD_WIDTH, CARD_HEIGHT);
                data_->cardsTextures[i][j].Load(data_->tileset, rect);
                x += CARD_WIDTH;
                if (x >= X_LIMIT) {
                    x = 0;
                    y += CARD_HEIGHT;
                }
            }
        }

        // Set the HUD data of the game.
        data_->messageLabel.reset(new SimpleLabel("", 0, 312,
            AtariPalette::Hue01Lum14, core->Retro70Font()));

        data_->machineLabel.reset(new SimpleLabel("Machine:", 8, 6,
            AtariPalette::Hue00Lum00, core->Retro70Font()));

        data_->playerLabel.reset(new SimpleLabel("Player:", 8, 168,
            AtariPalette::Hue00Lum00, core->Retro70Font()));

        data_->newGameLabel.reset(new SimpleLabel("New Game", 0, 336,
            AtariPalette::Hue00Lum14, core->Retro70Font()));

        data_->getCardLabel.reset(new SimpleLabel("Get Card", 0, 336,
            AtariPalette::Hue00Lum14, core->Retro70Font()));

        data_->standLabel.reset(new SimpleLabel("Stand", 0, 336,
            AtariPalette::Hue00Lum14, core->Retro70Font()));

        data_->menuLabel.reset(new SimpleLabel("Menu", 0, 336,
            AtariPalette::Hue00Lum14, core->Retro70Font()));

        if (language == TEXT_LANGUAGE_SPANISH) {
            data_->machineLabel->Text("Máquina:");
            data_->playerLabel->Text("Jugador:");
            data_->newGameLabel->Text("Nueva Partida");
            data_->getCardLabel->Text("Pedir Carta");
            data_->standLabel->Text("Plantarse");
            data_->menuLabel->Text("Menú");
        }

        GUIUtil::CenterLabel(data_->messageLabel, CoreManager::HALF_LOW_WIDTH);
        GUIUtil::CenterLabel(data_->newGameLabel, 120);
        GUIUtil::CenterLabel(data_->getCardLabel, 120);
        GUIUtil::CenterLabel(data_->standLabel, 320);
        GUIUtil::CenterLabel(data_->menuLabel, 520);

        data_->newGameButton.reset(new TexturedButton());
        data_->newGameButton->Initialize(40, 330, data_->tileset,
            sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 408, 160, 24),
            sf::IntRect(352, 408, 160, 24));
        data_->newGameButton->OnClick([this] (TexturedButton &) {
            onNewGameClick();
        });

        data_->getCardButton.reset(new TexturedButton());
        data_->getCardButton->Initialize(40, 330, data_->tileset,
            sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 408, 160, 24),
            sf::IntRect(352, 408, 160, 24));
        data_->getCardButton->OnClick([this] (TexturedButton &) {
            onGetCardClick();
        });

        data_->standButton.reset(new TexturedButton());
        data_->standButton->Initialize(240, 330, data_->tileset,
            sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 408, 160, 24),
            sf::IntRect(352, 408, 160, 24));
        data_->standButton->OnClick([this] (TexturedButton &) {
            onStandClick();
        });

        data_->menuButton.reset(new TexturedButton());
        data_->menuButton->Initialize(440, 330, data_->tileset,
            sf::IntRect(32, 408, 160, 24), sf::IntRect(192, 264, 160, 24),
            sf::IntRect(352, 264, 160, 24));
        data_->menuButton->OnClick([this, core] (TexturedButton &) {
            ClickSound().Play();
            core->SetNextState(MakeSharedState<BlackjackGameMenuState>());
        });

        // Load the sounds of the game.
        data_->keyboardSound.Load("Content/Sounds/SharedKey.wav");
        data_->clickSound.Load("Content/Sounds/SharedClick.wav");

        // Set the initialized flag.
        initialized_ = true;
    }
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void BlackjackManager::Release() {
    if (initialized_ && data_) {
        // Wait to the sound to end.
        data_->keyboardSound.WaitUntilStop();
        data_->clickSound.WaitUntilStop();

        // Unload the sounds of the game.
        data_->keyboardSound.Unload();
        data_->clickSound.Unload();

        // Unload the textures of the game.
        data_->tileset = nullptr;

        // Remove the inner data of the game.
        data_.reset(nullptr);
    }

    // Change the initialized flag and set the next state.
    initialized_ = false;
    CoreManager::Instance()->SetNextState(MakeSharedState<DesktopState>());
}

//********************************************************************************
// Game methods
//********************************************************************************

/**
 * Starts a new game.
 */
void BlackjackManager::StartGame() {
    onNewGameClick();
    CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackGameState>());
}

//--------------------------------------------------------------------------------

/**
 * Draws the game.
 */
void BlackjackManager::DrawGame() {
    const int CARD_START_X = 8, CARD_MACHINE_Y = 24, CARD_PLAYER_Y = 186;
    int x = CARD_START_X, y = CARD_MACHINE_Y;

    auto paintCard = [this, &x, &y] (BlackjackCard & item) {
        data_->cardsTextures[item.type][item.value].Draw(x, y);
        const int CARD_OFFSET = 52;
        x += CARD_OFFSET;
    };

    data_->machineLabel->Draw();
    ForEach(data_->machineCards, paintCard);

    x = CARD_START_X, y = CARD_PLAYER_Y;
    data_->playerLabel->Draw();
    ForEach(data_->playerCards, paintCard);

    switch (data_->substate) {
    case PLAYER_STATE:
        data_->getCardButton->Draw();
        data_->getCardLabel->Draw();
        data_->standButton->Draw();
        data_->standLabel->Draw();
        break;

    case MACHINE_STATE:
        break;

    case FINAL_STATE:
        data_->messageLabel->Draw();
        data_->newGameButton->Draw();
        data_->newGameLabel->Draw();
        break;
    }
    data_->menuButton->Draw();
    data_->menuLabel->Draw();
}

//--------------------------------------------------------------------------------

/**
 * Updates the game.
 */
void BlackjackManager::UpdateGame(const sf::Time & timeDelta) {
    if (Keyboard::IsKeyUp(Keyboard::Escape)) {
        KeyboardSound().Play();
        CoreManager::Instance()->SetNextState(MakeSharedState<BlackjackGameMenuState>());

    } else {
        switch (data_->substate) {
        case PLAYER_STATE:
            data_->getCardButton->Update();
            data_->standButton->Update();
            break;

        case MACHINE_STATE:
            data_->currentTime += timeDelta.asMilliseconds();
            if (data_->currentTime >= MAX_PAUSE_INTERVAL) {
                data_->currentTime = 0;
                aiNextMove();
            }
            break;

        case FINAL_STATE:
            data_->newGameButton->Update();
            break;
        }
        data_->menuButton->Update();
    }
}

//--------------------------------------------------------------------------------

/**
 * The on new game button click.
 */
void BlackjackManager::onNewGameClick() {
    data_->currentCards.clear();
    data_->playerCards.clear();
    data_->machineCards.clear();

    for (int i = 0; i < MAX_TYPES_OF_CARDS; ++i) {
        for (int j = 0; j < MAX_CARDS_BY_TYPE; ++j) {
            BlackjackCard victim;
            victim.type = static_cast<BlackjackCardType>(i);
            victim.value = j;
            data_->currentCards.push_back(victim);
        }
    }

    data_->GetRandomCard(data_->playerCards);
    data_->GetRandomCard(data_->machineCards);

    data_->winner = 0;
    data_->currentTime = 0;
    changeSubstate(PLAYER_STATE);
}

//--------------------------------------------------------------------------------

/**
 * The on get card button click.
 */
void BlackjackManager::onGetCardClick() {
    data_->GetRandomCard(data_->playerCards);
    auto value = data_->GetFinalValue(data_->playerCards);
    if (value > MAX_FINAL_VALUE) {
        data_->winner = 2;
        changeSubstate(FINAL_STATE);
    }
}

//--------------------------------------------------------------------------------

/**
 * The on stand button click.
 */
void BlackjackManager::onStandClick() {
    data_->playerValue = data_->GetFinalValue(data_->playerCards);
    changeSubstate(MACHINE_STATE);
}

//--------------------------------------------------------------------------------

/**
 * The AI next move.
 */
void BlackjackManager::aiNextMove() {
    auto value = data_->GetFinalValue(data_->machineCards);
    if (value > data_->playerValue) {
        // The machine wins.
        data_->winner = 2;
        changeSubstate(FINAL_STATE);

    } else {
        auto takeCard = [this, &value] () {
            data_->GetRandomCard(data_->machineCards);
            value = data_->GetFinalValue(data_->machineCards);
            if (value > MAX_FINAL_VALUE) {
                // The machine loses.
                data_->winner = 1;
                changeSubstate(FINAL_STATE);
            }
        };
        if (value < data_->playerValue) {
            // The machine tries to win.
            takeCard();

        } else {
            const unsigned int MAX_THRESHOLD = 10;
            if ((value + MAX_THRESHOLD) <= MAX_FINAL_VALUE) {
                // The machine still can win.
                takeCard();

            } else {
                // The machine will "decide" if to risk or not to win.
                int chaos = CoreManager::Instance()->Random(20);
                if (chaos >= 10) {
                    // Let's take a chance.
                    takeCard();

                } else {
                    // Nobody wins.
                    data_->winner = 0;
                    changeSubstate(FINAL_STATE);
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Changes the substate of the game.
 */
void BlackjackManager::changeSubstate(int value) {
    // Get some data and change the substate.
    auto * core = CoreManager::Instance();
    auto language = core->Language();
    data_->substate = value;

    // Set the HUD values of the substate.
    switch (data_->substate) {
    case PLAYER_STATE:
    case MACHINE_STATE:
        data_->currentTime = 0;
        data_->messageLabel->Text("");
        break;

    case FINAL_STATE:
        if (language == TEXT_LANGUAGE_SPANISH) {
            if (data_->winner == 1) {
                data_->messageLabel->Text("¡Has ganado!");
            } else if (data_->winner == 2) {
                data_->messageLabel->Text("¡Has perdido!");
            } else {
                data_->messageLabel->Text("No ha ganado nadie...");
            }
        } else {
            if (data_->winner == 1) {
                data_->messageLabel->Text("You win!");
            } else if (data_->winner == 2) {
                data_->messageLabel->Text("You lose!");
            } else {
                data_->messageLabel->Text("Nobody wins...");
            }
        }
        break;
    }

    // Set the right position of some elements of the HUD.
    GUIUtil::CenterLabel(data_->messageLabel, CoreManager::HALF_LOW_WIDTH);
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
BlackjackManager * BlackjackManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
BlackjackManager::BlackjackManager() : initialized_(false), data_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
BlackjackManager::~BlackjackManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
BlackjackManager * BlackjackManager::Instance() {
    if (!instance_) {
        instance_ = new BlackjackManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
BlackjackManager & BlackjackManager::Reference() {
    return *(Instance());
}
