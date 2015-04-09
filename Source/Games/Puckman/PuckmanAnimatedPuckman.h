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

#ifndef __PUCKMAN_ANIMATED_PUCKMAN_HEADER__
#define __PUCKMAN_ANIMATED_PUCKMAN_HEADER__

#include <Games/Puckman/PuckmanPuckmanEntity.h>

namespace Puckman {
    /**
     * This class represents an animated Puckman entity.
     */
    class AnimatedPuckman : public PuckmanEntity {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        MovingDirectionEnum NeededDirection() { return neededDirection_; }
        void NeededDirection(MovingDirectionEnum value) { neededDirection_ = value; }

        bool Alive() { return alive_; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Respawn();
        void Kill();
        virtual void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        AnimatedPuckman();
        virtual ~AnimatedPuckman();

    protected:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // This is the needed direction by the demo and it will be checked in the future,
        // to see if the entity can move in that direction.
        MovingDirectionEnum neededDirection_;

        // This tells the program if the puckman is alive or not.
        bool alive_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void testNeededDirection();
    };
}

#endif
