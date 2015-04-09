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

#ifndef __CORE_MANAGER_HEADER__
#define __CORE_MANAGER_HEADER__

#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <System/SharedTypes.h>
#include <System/AbstractState.h>

namespace sf {
    class Drawable;
}

class BitmapFont;

enum TextLanguage {
    TEXT_LANGUAGE_ENGLISH,
    TEXT_LANGUAGE_SPANISH
};

/**
 * This singleton class represents the core manager.
 */
class CoreManager {
public:
    friend class Texture2D;
    friend class BitmapFont;

    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const std::string TITLE;

    static const bool CURSOR_VISIBLE;
    static const unsigned int MAX_FPS;

    static const int LOW_WIDTH;
    static const int LOW_HEIGHT;
    static const int HIGH_WIDTH;
    static const int HIGH_HEIGHT;

    static const int HALF_LOW_WIDTH;
    static const int HALF_LOW_HEIGHT;
    static const int HALF_HIGH_WIDTH;
    static const int HALF_HIGH_HEIGHT;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    const std::string & KeyboardText() const { return keyboardText_; }
    void KeyboardText(const std::string & value) { keyboardText_ = value; }

    TextLanguage Language() const { return language_; }
    void Language(TextLanguage value) { language_ = value; }

    BitmapFont * Retro70Font() { return retro70Font_.get(); }
    BitmapFont * Retro80Font() { return retro80Font_.get(); }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    // General
    bool Initialize();
    void Release();
    void Run();
    void Exit();

    // States
    void SetNextState(const SharedState & state);

    // Draw
    void Draw(const sf::Drawable & victim);
    void SetClearColor(const sf::Color & value = sf::Color::Black);
    SharedTexture LoadTexture(const std::string & path, bool smooth = false);
    SharedImage LoadImage(const std::string & path);

    // Sound
    SharedSoundBuffer LoadSoundBuffer(const std::string & path);

    // Input
    bool IsKeyPressed(sf::Keyboard::Key key);
    bool IsButtonPressed(sf::Mouse::Button button);
    const sf::Vector2i & GetMousePosition();
    void UpdateMousePosition();

    void EnableKeyboardTextInput();
    void DisableKeyboardTextInput();
    void ClearKeyboardTextInput();

    void ShowMouseCursor();
    void HideMouseCursor();

    // Rand
    void InitRndSeed();
    int Random();
    int Random(int top);

    // Util
    void OpenURL(const std::string & url);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static CoreManager * Instance();
    static CoreManager & Reference();
    ~CoreManager();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    SharedRenderWindow window_;     // The main window of the game.
    sf::Color clearColor_;          // The clear color of the screen.
    sf::Vector2i mousePosition_;    // The current mouse position in the window.
    bool focus_;                    // The focus flag of the game.
    bool keyboardTextInputEnable_;  // The enable keyboard text input flag.
    std::string keyboardText_;      // The current keyboard text string.
    SharedState nextState_;         // The next state of the game.
    SharedState currentState_;      // The current state of the game.
    TextLanguage language_;         // The current language of the game.

    std::unique_ptr<BitmapFont> retro70Font_;   // The retro 70 windows bitmap font.
    std::unique_ptr<BitmapFont> retro80Font_;   // The retro 80 windows bitmap font.

    bool musicPaused_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    // General
    void pollEvents();

    // States
    void changeState(SharedState & state);

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static CoreManager * instance_;
    CoreManager();
};

#endif
