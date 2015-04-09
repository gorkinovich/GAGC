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

#ifndef __PUCKMAN_ENUMS_HEADER__
#define __PUCKMAN_ENUMS_HEADER__

namespace Puckman {
    /**
     * This enumeration is a list of the font symbols in the game.
     */
    class FontSymbol {
    private:
        FontSymbol() {}
        ~FontSymbol() {}

    public:
        enum Type {
            Copyright,  // The id of the (c) symbol.
            Pts,        // The id of the points symbol.
            N100,       // The id of the 100 symbol.
            N300,       // The id of the 300 symbol.
            N500,       // The id of the 500 symbol.
            N700,       // The id of the 700 symbol.
            N1000,      // The id of the 1000 symbol.
            N2000,      // The id of the 2000 symbol.
            N3000,      // The id of the 3000 symbol.
            N5000,      // The id of the 5000 symbol.
            Namco,      // The id of the Namco symbol.
            UpRight,    // The id of the up-right corner symbol.
            DownRight,  // The id of the down-right corner symbol.
            UpLeft,     // The id of the up-left corner symbol.
            DownLeft    // The id of the down-left corner symbol.
        };
    };

    typedef FontSymbol::Type FontSymbolEnum;

    /**
     * This enumeration is a list of the allowed directions of the entities.
     */
    class MovingDirection {
    private:
        MovingDirection() {}
        ~MovingDirection() {}

    public:
        enum Type {
            Up    = 3, // The id of the up direction.
            Down  = 1, // The id of the down direction.
            Left  = 2, // The id of the left direction.
            Right = 0, // The id of the right direction.
            None  = 5  // The id of the none direction.
        };
    };

    typedef MovingDirection::Type MovingDirectionEnum;

    /**
     * This enumeration represents the different types of ghosts.
     */
    class GhostType {
    private:
        GhostType() {}
        ~GhostType() {}

    public:
        enum Type {
            Red   = 0, // The id of the red ghost.
            Pink  = 1, // The id of the pink ghost.
            Blue  = 2, // The id of the blue ghost.
            Brown = 3  // The id of the brown ghost.
        };
    };

    typedef GhostType::Type GhostTypeEnum;

    /**
     * This enumeration represents the different states of ghosts.
     */
    class GhostState {
    private:
        GhostState() {}
        ~GhostState() {}

    public:
        enum Type {
            Vulnerable, // The id of the vulnerable mode.
            Ending,     // The id of the ending mode.
            Normal      // The id of the normal mode.
        };
    };

    typedef GhostState::Type GhostStateEnum;

    /**
     * This enumeration represents the different types of ghost's states.
     */
    class GhostControlState {
    private:
        GhostControlState() {}
        ~GhostControlState() {}

    public:
        enum Type {
            House,      // The id of the house state.
            Scatter,    // The id of the scatter state.
            Chase,      // The id of the chase state.
            Frightened, // The id of the frightened state.
            Dead        // The id of the dead state.
        };
    };

    typedef GhostControlState::Type GhostControlStateEnum;

    /**
     * This enumeration represents the different types of fruits.
     */
    class FruitType {
    private:
        FruitType() {}
        ~FruitType() {}

    public:
        enum Type {
            Cherry,     // The id of the cherry fruit.
            Strawberry, // The id of the strawberry fruit.
            Peach,      // The id of the peach fruit.
            Bell,       // The id of the bell fruit.
            Apple,      // The id of the apple fruit.
            Grape,      // The id of the grape fruit.
            Flower,     // The id of the flower fruit.
            Key         // The id of the key fruit.
        };
    };

    typedef FruitType::Type FruitTypeEnum;
}

#endif
