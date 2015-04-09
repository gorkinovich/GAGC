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

#include "MusicManager.h"
#include <SFML/System/Time.hpp>
#include <System/ForEach.h>

//********************************************************************************
// Constants
//********************************************************************************

const int STATUS_READY    = 0;
const int STATUS_PLAYLIST = 1;
const int STATUS_WAIT     = 2;
const int STATUS_STOPPING = 3;

//********************************************************************************
// Methods
//********************************************************************************

void MusicManager::Initialize() {
    status_ = STATUS_READY;
    playlist_.clear();
    currentSong_ = -1;
    timer_.Enable(false);
}

//--------------------------------------------------------------------------------

void MusicManager::Update(const sf::Time & timeDelta) {
    switch (status_) {
    case STATUS_PLAYLIST:
        if (IsStopped()) {
            NextSong();
        }
        break;

    case STATUS_STOPPING:
        int milliseconds = timeDelta.asMilliseconds();
        float nextVolume = Volume() - (stopStep_ * milliseconds);
        Volume(nextVolume < 0 ? 0 : nextVolume);
        timer_.Update(milliseconds);
        break;
    }
}

//--------------------------------------------------------------------------------

void MusicManager::Play(const std::string & path, bool loop) {
    if (music_.openFromFile(path)) {
        music_.setLoop(loop);
        music_.play();
    }
}

//--------------------------------------------------------------------------------

void MusicManager::Play() {
    if (IsStopped() || IsPaused()) {
        if (music_.getDuration().asMilliseconds() > 0) {
            music_.play();
        }
    }
}

//--------------------------------------------------------------------------------

void MusicManager::Pause() {
    if (IsStopped() || IsPaused()) {
        if (music_.getDuration().asMilliseconds() > 0) {
            music_.play();
        }
    } else {
        music_.pause();
    }
}

//--------------------------------------------------------------------------------

void MusicManager::Stop() {
    if (IsPlaying() || IsPaused()) {
        music_.stop();
    }
}

//--------------------------------------------------------------------------------

void MusicManager::NextSong() {
    if (!playlist_.empty()) {
        currentSong_ = (currentSong_ + 1) % playlist_.size();
        Play(playlist_[currentSong_], false);
    }
}

//--------------------------------------------------------------------------------

void MusicManager::SetPlaylist(const std::vector<std::string> & list) {
    ForEach(list, [&] (const std::string & item) {
        playlist_.push_back(item);
    });
    status_ = STATUS_PLAYLIST;
}

//--------------------------------------------------------------------------------

void MusicManager::ClearPlaylist() {
    playlist_.clear();
    currentSong_ = -1;
    status_ = STATUS_READY;
}

//--------------------------------------------------------------------------------

void MusicManager::SlowStop() {
    if (IsPlaying()) {
        const unsigned int MAX_INTERVAL = 1000;
        int savedStatus = status_;
        float savedVolume = Volume();
        timer_.SetAndEnable(MAX_INTERVAL, [=] (Timer &) {
            Volume(0);
            Stop();
            Volume(savedVolume);
            status_ = savedStatus;
        });
        stopStep_ = savedVolume / MAX_INTERVAL;
        status_ = STATUS_STOPPING;
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
MusicManager * MusicManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
MusicManager::MusicManager() {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
MusicManager::~MusicManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
MusicManager * MusicManager::Instance() {
    if (!instance_) {
        instance_ = new MusicManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
MusicManager & MusicManager::Reference() {
    return *(Instance());
}
