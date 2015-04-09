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

#ifndef __MOUSE_HEADER__
#define __MOUSE_HEADER__

#include <array>
#include <SFML/Window/Mouse.hpp>

/**
 * This class represents a mouse inside the game.
 */
class Mouse : public sf::Mouse {
public:
    static void Update();
    static bool IsAnyButtonPressed();
    static bool IsAnyButtonDown();
    static bool IsAnyButtonUp();
    static bool IsButtonPressed(Button key);
    static bool IsButtonDown(Button key);
    static bool IsButtonUp(Button key);

private:
    Mouse() {}
    ~Mouse() {}

    static std::array<bool, ButtonCount> previous_;
    static std::array<bool, ButtonCount> current_;
};

#endif
