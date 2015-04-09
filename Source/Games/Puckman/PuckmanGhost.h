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

#ifndef __PUCKMAN_GHOST_HEADER__
#define __PUCKMAN_GHOST_HEADER__

#include <Games/Puckman/PuckmanMovingEntity.h>
#include <Games/Puckman/PuckmanAnimation.h>

namespace Puckman {
    /**
     * This class represents a ghost entity.
     */
    class Ghost : public MovingEntity {
    public:
        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        // Controls that the speed interval tick of the ghost is right.
        typedef std::function<void (Ghost &)> GhostSpeedController;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The speed controller of the ghost.
        GhostSpeedController SpeedController;

        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        MovingDirectionEnum NeededDirection() { return neededDirection_; }
        void NeededDirection(MovingDirectionEnum value) { neededDirection_ = value; }

        GhostTypeEnum TypeOfGhost() { return typeOfGhost_; }
        bool Alive() { return alive_; }
        GhostStateEnum State() { return state_; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void BackToLife();
        void Kill();
        void ChangeState(GhostStateEnum state);
        virtual void SetDirection(MovingDirectionEnum dir);
        virtual void Draw();
        virtual void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        Ghost(GhostTypeEnum type);
        virtual ~Ghost();

    protected:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The number of different ghosts.
        static const int GHOSTS = 4;

        // The maximum number of frames per second of the animation.
        static const int MAX_FPS = 8;

        // The number of sprites of the entity.
        static const int NORMAL_SPRITES = 4;

        // The id of the sprite "right".
        static const int RIGHT_SPRITE = 0;

        // The id of the sprite "down".
        static const int DOWN_SPRITE = 1;

        // The id of the sprite "left".
        static const int LEFT_SPRITE = 2;

        // The id of the sprite "up".
        static const int UP_SPRITE = 3;

        // The number of sprites of the entity.
        static const int OTHER_SPRITES = 3;

        // The id of the sprite "blue".
        static const int BLUE_SPRITE = 0;

        // The id of the sprite "white 1".
        static const int WHITE1_SPRITE = 1;

        // The id of the sprite "white 2".
        static const int WHITE2_SPRITE = 2;

        // The speed interval tick when the ghost is dead.
        static const int DEAD_SPEED_INTERVAL = 13;

        // The speed interval tick when the ghost is normal.
        static const int NORMAL_SPEED_INTERVAL = 15;

        // The speed interval tick when the ghost is vulnerable.
        static const int VULNERABLE_SPEED_INTERVAL = 26;

        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        typedef std::vector<Animation> AnimationVector;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The sprites of the entity.
        AnimationVector normalSprites_;

        // The current type of ghost.
        GhostTypeEnum typeOfGhost_;

        // The current used normal sprite.
        int currentNormalSprite_;

        // The sprites of the entity.
        AnimationVector otherSprites_;

        // The current used other sprite.
        int currentOtherSprite_;

        // This is the needed direction by the demo and it will be checked in the future,
        // to see if the entity can move in that direction.
        MovingDirectionEnum neededDirection_;

        // This tells to the program if a ghost is alive or not.
        bool alive_;

        // This is the state of an alive ghost.
        GhostStateEnum state_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void speedController(Ghost & ghost);
        void resetSprites();
        void updateSprites(int time);
        void setCurrentSprite(int index);
        void testNeededDirection();
    };
}

#endif
