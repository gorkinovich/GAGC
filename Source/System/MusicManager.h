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

#ifndef __MUSIC_MANAGER_HEADER__
#define __MUSIC_MANAGER_HEADER__

#include <string>
#include <vector>
#include <SFML/Audio/Music.hpp>
#include <System/Timer.h>

namespace sf {
    class Time;
}

/**
 * This singleton class represents the music manager.
 */
class MusicManager {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    float Volume() const { return music_.getVolume(); }
    void Volume(float value) { music_.setVolume(value); }
    void VolumeMute() { music_.setVolume(0); }
    void VolumeFull() { music_.setVolume(100); }

    bool IsPlaying() const { return music_.getStatus() == sf::Music::Playing; }
    bool IsPaused()  const { return music_.getStatus() == sf::Music::Paused;  }
    bool IsStopped() const { return music_.getStatus() == sf::Music::Stopped; }

    bool IsSamePlaylist(const std::vector<std::string> & list) {
        return playlist_ == list;
    }

    unsigned int CurrentSong() const { return currentSong_; }
    void CurrentSong(unsigned int value) { currentSong_ = value; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Update(const sf::Time & timeDelta);

    void Play(const std::string & path, bool loop = false);
    void Play();
    void Pause();
    void Stop();

    void NextSong();
    void SetPlaylist(const std::vector<std::string> & list);
    void ClearPlaylist();
    void SlowStop();

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static MusicManager * Instance();
    static MusicManager & Reference();
    ~MusicManager();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int status_;
    sf::Music music_;
    std::vector<std::string> playlist_;
    unsigned int currentSong_;
    Timer timer_;
    float stopStep_;

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static MusicManager * instance_;
    MusicManager();
};

#endif
