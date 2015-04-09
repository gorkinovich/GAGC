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

#ifndef __PUCKMAN_ENTITY_HEADER__
#define __PUCKMAN_ENTITY_HEADER__

namespace Puckman {
    class Manager;

    /**
     * This class represents an entity inside the game.
     */
    class Entity {
    public:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        int X; // The x coordinate.
        int Y; // The y coordinate.

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        /** Sets the new coordinates of the entity. */
        void SetCoordinates(int x, int y) { X = x; Y = y; }

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        /** Constructs a new object. */
        Entity();

        /** The destructor of the object. */
        virtual ~Entity() {}

    protected:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The manager of the game.
        Manager * manager_;
    };
}

#endif
