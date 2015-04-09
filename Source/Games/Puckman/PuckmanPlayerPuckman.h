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

#ifndef __PUCKMAN_PLAYER_PUCKMAN_HEADER__
#define __PUCKMAN_PLAYER_PUCKMAN_HEADER__

#include <Games/Puckman/PuckmanPuckmanEntity.h>

namespace Puckman {
    class GameSound;

    /**
     * This class represents a Puckman entity controlled by the player.
     */
    class PlayerPuckman : public PuckmanEntity {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

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

        PlayerPuckman();
        virtual ~PlayerPuckman();

    protected:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // This is the asked direction by the user and it will be checked in the future,
        // to see if the entity can move in that direction.
        MovingDirectionEnum askedDirection_;

        // This tells the program if the puckman is alive or not.
        bool alive_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void testUserInput();
    };
}

#endif
