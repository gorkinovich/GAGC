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

#ifndef __ALIEN_ANIMATION_HEADER__
#define __ALIEN_ANIMATION_HEADER__

#include <vector>

/**
 * This class represents an animation sequence.
 */
class AlienAnimation {
public:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct Node {
        int texture;
        int maxTicks;
        Node() : texture(0), maxTicks(0) {}
        Node(int t, int mt) : texture(t), maxTicks(mt) {}
    };

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    std::vector<Node> Frames;
    int CurrentFrame;
    int CurrentTicks;
    bool Loop;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool Finished() const {
        return CurrentFrame >= static_cast<int>(Frames.size());
    }

    int GetIndex() const {
        int last = Frames.size() - 1;
        return Frames[CurrentFrame <= last ? CurrentFrame : last].texture;
    }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Reset(bool loop = false) {
        CurrentFrame = 0;
        CurrentTicks = 0;
        Loop = loop;
    }

    void Update() {
        if (!Finished()) {
            ++CurrentTicks;
            if (CurrentTicks >= Frames[CurrentFrame].maxTicks) {
                ++CurrentFrame;
                CurrentTicks = 0;
                if (Finished() && Loop) {
                    CurrentFrame = 0;
                }
            }
        }
    }

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    AlienAnimation() { Reset(); }
};

#endif
