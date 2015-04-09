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

#ifndef __PUCKMAN_PUCKMAN_ENTITY_HEADER__
#define __PUCKMAN_PUCKMAN_ENTITY_HEADER__

#include <Games/Puckman/PuckmanMovingEntity.h>
#include <Games/Puckman/PuckmanAnimation.h>

namespace Puckman {
    /**
     * This class represents the Puckman entity.
     */
    class PuckmanEntity : public MovingEntity {
    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void SetFast();
        void SetSlow();
        virtual void SetDirection(MovingDirectionEnum dir);
        virtual void Draw();
        bool IsDeathSpriteEnded();

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        PuckmanEntity();
        virtual ~PuckmanEntity();

    protected:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The number of sprites of the entity.
        static const int SPRITES = 6;

        // The maximum number of frames per second of the animation.
        static const int MAX_FPS = 8;

        // The id of the sprite "right".
        static const int RIGHT_SPRITE = 0;

        // The id of the sprite "down".
        static const int DOWN_SPRITE = 1;

        // The id of the sprite "left".
        static const int LEFT_SPRITE = 2;

        // The id of the sprite "up".
        static const int UP_SPRITE = 3;

        // The id of the sprite "death".
        static const int DEATH_SPRITE = 4;

        // The id of the sprite "stop".
        static const int STOP_SPRITE = 5;

        // A fast amount of time that an interval can have.
        static const int FAST_INTERVAL = 14;

        // A slow amount of time that an interval can have.
        static const int SLOW_INTERVAL = 16;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        std::vector<Animation> sprites_; // The sprites of the entity.
        int currentSprite_;              // The current used sprite.

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void setCurrentSprite(int index);
    };
}

#endif
