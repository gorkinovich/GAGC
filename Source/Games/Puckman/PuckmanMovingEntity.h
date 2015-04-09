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

#ifndef __PUCKMAN_MOVING_ENTITY_HEADER__
#define __PUCKMAN_MOVING_ENTITY_HEADER__

#include <vector>
#include <functional>
#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanEntity.h>

namespace Puckman {
    /**
     * This class represents an entity that can move inside the game.
     */
    class MovingEntity : public Entity {
    public:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // This is the speed when the entity doesn't move.
        static const int NO_SPEED = 0;

        // This is the minimum speed for any entity.
        static const int MIN_SPEED = 1;

        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        // Checks if the entity coordinates are valid or not.
        typedef std::function<bool (int, int)> CheckSpriteCoordinates;

        // Checks if the entity coordinates are valid or not.
        typedef std::function<void (MovingEntity &)> OnMoveAction;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // This calls to a method that checks if a new coordinates are valid or not.
        CheckSpriteCoordinates CheckCoordinates;

        std::vector<OnMoveAction> OnMove;

        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        int Speed() { return speed_; }
        void Speed(int value);

        MovingDirectionEnum Direction() { return direction_; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void ResetTime();
        void ResetSpeed();
        void StopSpeed();
        void SetSpeedInterval(int interval);
        void Move();

        // Sets the direction of the entity.
        virtual void SetDirection(MovingDirectionEnum dir) = 0;

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        MovingEntity();
        virtual ~MovingEntity();

    protected:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // A normal amount of time that an interval can have.
        static const int MIN_INTERVAL = 15;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        int maxTimeInterval_;           // The maximum time interval to move.
        int currentTime_;               // The current time passed.
        int speed_;                     // The speed of the entity.
        MovingDirectionEnum direction_; // The direction of the speed.

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        bool checkSpriteCoords(int x, int y);
        bool testUpdateCoordinates(MovingDirectionEnum d);
        bool updateCoordinates();
    };
}

#endif
