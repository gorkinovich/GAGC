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

#ifndef __SOUND_HEADER__
#define __SOUND_HEADER__

#include <string>
#include <System/SharedTypes.h>

/**
 * This class represents a sound.
 */
class Sound {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    float Volume();
    void Volume(float value);

    bool IsPlaying();
    bool IsPaused();
    bool IsStoped();

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    bool Load(const std::string & path);
    bool Load(SharedSoundBuffer & buffer);
    void Unload();
    void Play(bool loop = false);
    void Pause();
    void Stop();
    void WaitUntilStop();

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    Sound();
    Sound(const Sound & source);
    ~Sound();
    Sound & operator =(const Sound & source);

    //--------------------------------------------------------------------------------
    // Static
    //--------------------------------------------------------------------------------

    static float GlobalVolume() { return globalVolume_; }
    static void GlobalVolume(float value) { globalVolume_ = value; }
    static void GlobalVolumeMute() { globalVolume_ = 0.0f; }
    static void GlobalVolumeFull() { globalVolume_ = 100.0f; }

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    SharedSoundBuffer buffer_; // The sound buffer.
    SharedSound sound_;        // The sound player.

    //--------------------------------------------------------------------------------
    // Static
    //--------------------------------------------------------------------------------

    static float globalVolume_;
};

#endif
