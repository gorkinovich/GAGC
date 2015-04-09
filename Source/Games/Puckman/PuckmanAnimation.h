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

#ifndef __PUCKMAN_ANIMATION_HEADER__
#define __PUCKMAN_ANIMATION_HEADER__

#include <vector>

#define ARRAY_TO_FRAMES(x) std::vector<int>(std::begin(x), std::end(x))

namespace Puckman {
    class Manager;

    /**
     * This class is used to make an animation using sprites.
     */
    class Animation {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        int CurrentFrame() const { return currentFrame_; }

        const std::vector<int> & Frames() const { return frames_; }
        void Frames(const std::vector<int> & value) { frames_ = value; }

        bool Loop() const { return loop_; }
        void Loop(bool value) { loop_ = value; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Draw(int x, int y);
        void Update(int milliseconds);
        void Reset();
        void ResetTime();
        void SetFps(int fps);
        bool IsOnFinalFrame();

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        Animation(const std::vector<int> & frames, int fps, bool loop = true);

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        Manager * manager_;       // The manager of the game.
        int maxTimeInterval_;     // The maximum amount of time to change the current frame.
        int currentTime_;         // The current passed time.
        int currentFrame_;        // The current frame of the animation.
        std::vector<int> frames_; // The frames of the animation.
        bool loop_;               // This tells if the animation is a loop one or not.
    };
}

#endif
