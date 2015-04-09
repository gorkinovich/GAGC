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

#include "Sound.h"
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <System/CoreManager.h>

//********************************************************************************
// Static
//********************************************************************************

float Sound::globalVolume_ = 100.0f;

//********************************************************************************
// Properties
//********************************************************************************

float Sound::Volume() {
    return sound_ ? sound_->getVolume() : 0.0f;
}

//--------------------------------------------------------------------------------

void Sound::Volume(float value) {
    if (sound_) {
        sound_->setVolume(value);
    }
}

//--------------------------------------------------------------------------------

bool Sound::IsPlaying() {
    return sound_ ? sound_->getStatus() == sf::Sound::Playing : false;
}

//--------------------------------------------------------------------------------

bool Sound::IsPaused() {
    return sound_ ? sound_->getStatus() == sf::Sound::Paused : false;
}

//--------------------------------------------------------------------------------

bool Sound::IsStoped() {
    return sound_ ? sound_->getStatus() == sf::Sound::Stopped : true;
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Loads a sound from a file.
 */
bool Sound::Load(const std::string & path) {
    auto * core = CoreManager::Instance();
    Unload();
    buffer_ = core->LoadSoundBuffer(path);
    if (!buffer_) return false;
    sound_ = std::make_shared<sf::Sound>();
    sound_->setBuffer(*buffer_);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Loads a sound from the memory.
 */
bool Sound::Load(SharedSoundBuffer & buffer) {
    Unload();
    buffer_ = buffer;
    sound_ = std::make_shared<sf::Sound>();
    sound_->setBuffer(*buffer_);
    return true;
}

//--------------------------------------------------------------------------------

/**
 * Unloads the current surface.
 */
void Sound::Unload() {
    Stop();
    buffer_ = nullptr;
    sound_ = nullptr;
}

//--------------------------------------------------------------------------------

/**
 * Plays the current sound.
 */
void Sound::Play(bool loop) {
    if (sound_ && !IsPlaying()) {
        sound_->setVolume(globalVolume_);
        sound_->setLoop(loop);
        sound_->play();
    }
}

//--------------------------------------------------------------------------------

/**
 * Pauses the current sound.
 */
void Sound::Pause() {
    if (sound_ && IsPlaying()) {
        sound_->pause();
    }
}

//--------------------------------------------------------------------------------

/**
 * Stops the current sound.
 */
void Sound::Stop() {
    if (sound_ && IsPlaying()) {
        sound_->stop();
    }
}

//--------------------------------------------------------------------------------

/**
 * Waits until the stop of the sound.
 */
void Sound::WaitUntilStop() {
    if (sound_ && !sound_->getLoop()) {
        while (sound_->getStatus() == sf::Sound::Playing);
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Sound::Sound() : buffer_(nullptr), sound_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The copy constructor of the object.
 */
Sound::Sound(const Sound & source) {
    (*this) = source;
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Sound::~Sound() {
    Unload();
}

//--------------------------------------------------------------------------------

/**
 * The assign operator of the object.
 */
Sound & Sound::operator =(const Sound & source) {
    buffer_ = source.buffer_;
    sound_ = source.sound_;
    return *this;
}
