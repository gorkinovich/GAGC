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

#include "CoreManager.h"
#include <ctime>
#include <cstdlib>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <System/AtariPalette.h>
#include <System/BitmapFont.h>
#include <System/Keyboard.h>
#include <System/Sound.h>
#include <System/MusicManager.h>
#include <Menu/RetroStartState.h>

//********************************************************************************
// Constants
//********************************************************************************

const std::string CoreManager::TITLE  = "Gorkinovich Amateur Games Collection";

const bool CoreManager::CURSOR_VISIBLE = true;
const unsigned int CoreManager::MAX_FPS = 30;

const int CoreManager::LOW_WIDTH = 640;
const int CoreManager::LOW_HEIGHT = 360;
const int CoreManager::HIGH_WIDTH = LOW_WIDTH * 2;
const int CoreManager::HIGH_HEIGHT = LOW_HEIGHT * 2;

const int CoreManager::HALF_LOW_WIDTH = LOW_WIDTH / 2;
const int CoreManager::HALF_LOW_HEIGHT = LOW_HEIGHT / 2;
const int CoreManager::HALF_HIGH_WIDTH = HIGH_WIDTH / 2;
const int CoreManager::HALF_HIGH_HEIGHT = HIGH_HEIGHT / 2;

//********************************************************************************
// OS Methods
//********************************************************************************

void CoreInitializeOS();

//********************************************************************************
// General Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
bool CoreManager::Initialize() {
    // Some OS initialization stuff.
    CoreInitializeOS();

    // Configure the render window.
    auto flags = sf::Style::Titlebar | sf::Style::Close;
    auto videoMode = sf::VideoMode(HIGH_WIDTH, HIGH_HEIGHT);
    window_ = std::make_shared<sf::RenderWindow>(videoMode, TITLE, flags);
    if (!window_) return false;

    // Configure the view of the render window.
    sf::View view(sf::FloatRect(0, 0, (float)LOW_WIDTH, (float)LOW_HEIGHT));
    window_->setView(view);

    // Configure some stuff of the render window.
    clearColor_ = AtariPalette::Hue00Lum00;
    window_->setMouseCursorVisible(CURSOR_VISIBLE);
    window_->setFramerateLimit(MAX_FPS);
    DisableKeyboardTextInput();

    // Configure the fonts of the game.
    retro70Font_.reset(new BitmapFont());
    retro80Font_.reset(new BitmapFont());

    if (!retro70Font_->Load("Content/Fonts/Retro70.png")) return false;
    if (!retro80Font_->Load("Content/Fonts/Retro80.png")) return false;

    // Configure the music manager.
    MusicManager::Instance()->Initialize();
    musicPaused_ = false;

    // Set the current state of the game.
    nextState_ = nullptr;
    changeState(MakeSharedState<RetroStartState>());

    // Everything is right.
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void CoreManager::Release() {
    DisableKeyboardTextInput();
    window_ = nullptr;
}

//--------------------------------------------------------------------------------

/**
 * The main loop of the object.
 */
void CoreManager::Run() {
    // Set the time and sound controller variables.
    sf::Clock clock;
    sf::Time timeDelta;
    bool soundMute = false;
    auto * music = MusicManager::Instance();
    // Execute the main loop of the game.
    while (window_->isOpen()) {
        // Update the time delta.
        timeDelta = clock.getElapsedTime();
        clock.restart();
        // Update events.
        pollEvents();
        UpdateMousePosition();
        Keyboard::Update();
        // Control the sound volume.
        if (Keyboard::IsKeyUp(Keyboard::F12)) {
            if (soundMute) {
                Sound::GlobalVolumeFull();
                soundMute = false;
            } else {
                Sound::GlobalVolumeMute();
                soundMute = true;
            }
        } else if (Keyboard::IsKeyUp(Keyboard::F11)) {
            if (music->IsPaused()) {
                music->Play();
                musicPaused_ = false;
            } else {
                music->Pause();
                musicPaused_ = true;
            }
        } else if (Keyboard::IsKeyUp(Keyboard::F10)) {
            music->NextSong();
        }
        // If the window is opened, we'll draw and update the logic.
        if (window_->isOpen() && currentState_) {
            // Draw the current state.
            window_->clear(clearColor_);
            currentState_->Draw(timeDelta);
            window_->display();
            // If the window has the focus, we'll update the logic.
            if (focus_) {
                currentState_->Update(timeDelta);
                music->Update(timeDelta);
                if (nextState_) {
                    changeState(nextState_);
                    nextState_ = nullptr;
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Exits the application.
 */
void CoreManager::Exit() {
    if (window_) {
        window_->close();
    }
}

//--------------------------------------------------------------------------------

/**
 * Processes all the events of the window.
 */
void CoreManager::pollEvents() {
    sf::Event currentEvent;
    while (window_->pollEvent(currentEvent)) {
        if (currentEvent.type == sf::Event::Closed) {
            window_->close();

        } else if (currentEvent.type == sf::Event::LostFocus) {
            focus_ = false;
            if (!MusicManager::Instance()->IsPaused()) {
                MusicManager::Instance()->Pause();
            }

        } else if (currentEvent.type == sf::Event::GainedFocus) {
            focus_ = true;
            if (!musicPaused_) {
                MusicManager::Instance()->Play();
            }

        } else if (currentEvent.type == sf::Event::TextEntered && keyboardTextInputEnable_) {
            const unsigned int MIN_CHAR = 32, MAX_CHAR = 255;
            const unsigned int BACKSPACE = 8, TAB = 9, ENTER = 13;
            auto code = currentEvent.text.unicode;
            if (MIN_CHAR <= code && code <= MAX_CHAR) {
                keyboardText_ += static_cast<char>(code);
            } else if (code == BACKSPACE && !keyboardText_.empty()) {
                keyboardText_.erase(std::prev(keyboardText_.end()));
            } else if (code == TAB) {
                keyboardText_ += '\t';
            } else if (code == ENTER) {
                keyboardText_ += '\n';
            }
        }
    }
}

//********************************************************************************
// States Methods
//********************************************************************************

/**
 * Changes the next state of the game.
 */
void CoreManager::SetNextState(const SharedState & state) {
    nextState_ = state;
}

//--------------------------------------------------------------------------------

/**
 * Changes the current state of the game.
 */
void CoreManager::changeState(SharedState & state) {
    if (currentState_ != state) {
        if (currentState_) currentState_->Release();
        currentState_ = state;
        if (currentState_) currentState_->Initialize();
    }
}

//********************************************************************************
// Draw Methods
//********************************************************************************

/**
 * Draws something on the screen.
 */
void CoreManager::Draw(const sf::Drawable & victim) {
    window_->draw(victim);
}

//--------------------------------------------------------------------------------

/**
 * Sets the clear color of the screen.
 */
void CoreManager::SetClearColor(const sf::Color & value) {
    clearColor_ = value;
}

//--------------------------------------------------------------------------------

/**
 * Loads a texture from a file.
 */
SharedTexture CoreManager::LoadTexture(const std::string & path, bool smooth) {
    auto victim = std::make_shared<sf::Texture>();
    if (!victim->loadFromFile(path)) return nullptr;
    victim->setSmooth(smooth);
    return victim;
}

//--------------------------------------------------------------------------------

/**
 * Loads a image from a file.
 */
SharedImage CoreManager::LoadImage(const std::string & path) {
    auto victim = std::make_shared<sf::Image>();
    if (!victim->loadFromFile(path)) return nullptr;
    return victim;
}

//********************************************************************************
// Sound Methods
//********************************************************************************

/**
 * Loads a sound buffer from a file.
 */
SharedSoundBuffer CoreManager::LoadSoundBuffer(const std::string & path) {
    auto victim = std::make_shared<sf::SoundBuffer>();
    if (!victim->loadFromFile(path)) return nullptr;
    return victim;
}

//********************************************************************************
// Input Methods
//********************************************************************************

/**
 * Checks if a key of the keyboard is pressed or not.
 */
bool CoreManager::IsKeyPressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key);
}

//--------------------------------------------------------------------------------

/**
 * Checks if a button of the mouse is pressed or not.
 */
bool CoreManager::IsButtonPressed(sf::Mouse::Button button) {
    return sf::Mouse::isButtonPressed(button);
}

//--------------------------------------------------------------------------------

/**
 * Gets the mouse position inside the window.
 */
const sf::Vector2i & CoreManager::GetMousePosition() {
    return mousePosition_;
}

//--------------------------------------------------------------------------------

/**
 * Updates the mouse position from the device.
 */
void CoreManager::UpdateMousePosition() {
    auto realPos = sf::Mouse::getPosition(*window_);
    auto viewPos = window_->mapPixelToCoords(realPos);
    mousePosition_.x = static_cast<int>(viewPos.x);
    mousePosition_.y = static_cast<int>(viewPos.y);
}

//--------------------------------------------------------------------------------

/**
 * Enables the keyboard text input.
 */
void CoreManager::EnableKeyboardTextInput() {
    window_->setKeyRepeatEnabled(true);
    keyboardTextInputEnable_ = true;
}

//--------------------------------------------------------------------------------

/**
 * Disables the keyboard text input.
 */
void CoreManager::DisableKeyboardTextInput() {
    window_->setKeyRepeatEnabled(false);
    keyboardTextInputEnable_ = false;
}

//--------------------------------------------------------------------------------

/**
 * Clears the keyboard text input.
 */
void CoreManager::ClearKeyboardTextInput() {
    keyboardText_ = "";
}

//--------------------------------------------------------------------------------

/**
 * Shows the mouse cursor.
 */
void CoreManager::ShowMouseCursor() {
    window_->setMouseCursorVisible(true);
}

//--------------------------------------------------------------------------------

/**
 * Hides the mouse cursor.
 */
void CoreManager::HideMouseCursor() {
    window_->setMouseCursorVisible(false);
}

//********************************************************************************
// Rand Methods
//********************************************************************************

/**
 * Initializes the random seed.
 */
void CoreManager::InitRndSeed() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

//--------------------------------------------------------------------------------

/**
 * Gets a random number.
 */
int CoreManager::Random() {
    return std::rand();
}

//--------------------------------------------------------------------------------

/**
 * Gets a random number.
 */
int CoreManager::Random(int top) {
    return std::rand() % top;
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
CoreManager * CoreManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
CoreManager::CoreManager() : window_(nullptr), clearColor_(sf::Color::Black),
    mousePosition_(0, 0), focus_(true), keyboardTextInputEnable_(false),
    keyboardText_(""), nextState_(nullptr), currentState_(nullptr),
    language_(TEXT_LANGUAGE_ENGLISH), retro70Font_(nullptr),
    retro80Font_(nullptr), musicPaused_(false) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
CoreManager::~CoreManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
CoreManager * CoreManager::Instance() {
    if (!instance_) {
        instance_ = new CoreManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
CoreManager & CoreManager::Reference() {
    return *(Instance());
}
