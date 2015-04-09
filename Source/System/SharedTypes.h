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

#ifndef __SHARED_TYPES_HEADER__
#define __SHARED_TYPES_HEADER__

#include <memory>

namespace sf {
    class RenderWindow;
    class Texture;
    class Sprite;
    class Image;
    class SoundBuffer;
    class Sound;
    class Music;
}

typedef std::shared_ptr<sf::RenderWindow> SharedRenderWindow;
typedef std::shared_ptr<sf::Texture> SharedTexture;
typedef std::shared_ptr<sf::Sprite> SharedSprite;
typedef std::shared_ptr<sf::Image> SharedImage;
typedef std::shared_ptr<sf::SoundBuffer> SharedSoundBuffer;
typedef std::shared_ptr<sf::Sound> SharedSound;
typedef std::shared_ptr<sf::Music> SharedMusic;

#endif
