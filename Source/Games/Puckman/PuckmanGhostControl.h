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

#ifndef __PUCKMAN_GHOST_CONTROL_HEADER__
#define __PUCKMAN_GHOST_CONTROL_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanMazeGraph.h>

namespace Puckman {
    class Maze;
    class Ghost;
    class PuckmanEntity;

    /**
     * This class is used to control the ghost direction in the maze.
     */
    class GhostControl {
    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        GhostControl(int maxScatterInterval, int maxChaseInterval, int maxHouseInterval,
            int minDotsToExit, int maxNumberOfChanges, const sf::Vector2i & corner,
            Ghost * ghost, GhostControlStateEnum state, PuckmanEntity * puckman, Maze * maze);
        GhostControl(const GhostControl & source);
        virtual ~GhostControl();
        GhostControl & operator =(const GhostControl & source);

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The amount of time the ghost is in scatter mode.
        int maxScatterInterval_;

        // The amount of time the ghost is in chase mode.
        int maxChaseInterval_;

        // The amount of time the ghost is waiting in the house.
        int maxHouseInterval_;

        // The minimum number of dots to exit from the house.
        int minDotsToExit_;

        // The number of state changes.
        int maxNumberOfChanges_;

        // The coordinates of the ghost corner.
        sf::Vector2i corner_;

        // This is the ghost reference.
        Ghost * ghost_;

        // This is the current state of the ghost.
        GhostControlStateEnum state_;

        // This is a chasing mark to control the change of state.
        bool isChasing_;

        // This is an "in house" mark to control the change of state.
        bool isInHouse_;

        // This is the time counter for the ghost.
        int timeCounter_;

        // This is the number of state changes of the ghost.
        int changes_;

        // This is the puckman reference.
        PuckmanEntity * puckman_;

        // This is the maze reference.
        Maze * maze_;

        // This tells to the program if a direction is getted on an intersection.
        bool isDirectionGetted_;

        // This is the maze graph.
        static MazeGraph graph_;

        // This is the maze graph.
        static sf::Vector2i houseEnter_;

        // This is the red ghost entity.
        static Ghost * red_;

        // This is the pink ghost entity.
        static Ghost * pink_;

        // This is the blue ghost entity.
        static Ghost * blue_;

        // This is the brown ghost entity.
        static Ghost * brown_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void setTimeCounter();
        void changeState(GhostControlStateEnum state);
        bool isGhostOnCorner();
        void checkState(int time);
        void houseUpdate();
        void scatterUpdate();
        void chaseUpdate();
        void frightenedUpdate();
        void deadUpdate();
    };
}

#endif
