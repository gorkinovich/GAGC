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

#include "Keyboard.h"

//********************************************************************************
// Static
//********************************************************************************

std::array<bool, Keyboard::KeyCount> Keyboard::previous_;
std::array<bool, Keyboard::KeyCount> Keyboard::current_;

//--------------------------------------------------------------------------------

/**
 * Updates the current state of the object.
 */
void Keyboard::Update() {
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        previous_[i] = current_[i];
        current_[i] = sf::Keyboard::isKeyPressed(static_cast<Key>(i));
    }
}

//--------------------------------------------------------------------------------

/**
 * Checks if any key is pressed or not.
 */
bool Keyboard::IsAnyKeyPressed() {
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        if (current_[i]) return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

/**
 * Checks if any key have been pressed down.
 */
bool Keyboard::IsAnyKeyDown() {
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        if (!previous_[i] && current_[i]) return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

/**
 * Checks if any key have been pressed up.
 */
bool Keyboard::IsAnyKeyUp() {
    for (int i = 0; i < sf::Keyboard::KeyCount; ++i) {
        if (previous_[i] && !current_[i]) return true;
    }
    return false;
}

//--------------------------------------------------------------------------------

/**
 * Checks if a key is pressed or not.
 */
bool Keyboard::IsKeyPressed(Key key) {
    return current_[key];
}

//--------------------------------------------------------------------------------

/**
 * Checks if a key have been pressed down.
 */
bool Keyboard::IsKeyDown(Key key) {
    return !previous_[key] && current_[key];
}

//--------------------------------------------------------------------------------

/**
 * Checks if a key have been pressed up.
 */
bool Keyboard::IsKeyUp(Key key) {
    return previous_[key] && !current_[key];
}
