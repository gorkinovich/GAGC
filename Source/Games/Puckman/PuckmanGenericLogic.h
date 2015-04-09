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

#ifndef __PUCKMAN_GENERIC_LOGIC_HEADER__
#define __PUCKMAN_GENERIC_LOGIC_HEADER__

#include <memory>
#include <SFML/Graphics/Rect.hpp>
#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanFruit.h>
#include <Games/Puckman/PuckmanMaze.h>
#include <Games/Puckman/PuckmanBigDotsLogic.h>

namespace Puckman {
    class Ghost;
    class Manager;
    class PuckmanEntity;

    /**
     * This class represents a generic game logic.
     */
    class GenericLogic {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        bool Ended() const { return ended_; }

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        GenericLogic();
        virtual ~GenericLogic();

    protected:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The amount of time that the kill pause interval must have.
        static const int KILL_PAUSE_INTERVAL = 1000;

        // The vulnerable amount of time that the interval must have.
        static const int VULNERABLE_INTERVAL = 3600;

        // The vulnerable ending amount of time that the interval must have.
        static const int VULNERABLE_ENDING_INTERVAL = 1800;

        // The amount of time when the logic ends.
        static const int END_DELAY_TIME = 2000;

        // The amount of time to show the fruit.
        static const int SHOW_FRUIT_DELAY_TIME = 18000;

        // The amount of time where the fruit is shown.
        static const int FRUIT_DELAY_TIME = 10000;

        // This is the state id when the logic is paused at the beginning.
        static const int INITIAL_STATE = 0;

        // This is the state id when the logic starts to work.
        static const int NORMAL_STATE = 1;

        // This is the state id when the puckman is going to die.
        static const int KILL_STATE = 2;

        // This is the state id when the puckman dies.
        static const int DEATH_STATE = 3;

        // This is the state id when the game ends.
        static const int END_STATE = 4;

        // This is the identification when the last ghost killed is the no one.
        static const int NONE_LAST_KILLED = 0;

        // This is the identification when the last ghost killed is the red one.
        static const int RED_LAST_KILLED = 1;

        // This is the identification when the last ghost killed is the pink one.
        static const int PINK_LAST_KILLED = 2;

        // This is the identification when the last ghost killed is the blue one.
        static const int BLUE_LAST_KILLED = 3;

        // This is the identification when the last ghost killed is the brown one.
        static const int BROWN_LAST_KILLED = 4;

        // The speed interval tick when the ghost is dead.
        static const int DEAD_SPEED_INTERVAL = 12;

        // The speed interval tick when the ghost is in hurry mode.
        static const int HURRY_SPEED_INTERVAL = 14;

        // The speed interval tick when the ghost is normal.
        static const int NORMAL_SPEED_INTERVAL = 15;

        // The speed interval tick when the ghost is in the wormhole.
        static const int WORMHOLE_SPEED_INTERVAL = 18;

        // The speed interval tick when the ghost is vulnerable.
        static const int VULNERABLE_SPEED_INTERVAL = 26;

        // The speed interval tick when the ghost is in the house.
        static const int HOUSE_SPEED_INTERVAL = 30;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The manager of the game.
        Manager * manager_;

        // Tells to the program if the logic have ended.
        bool ended_;

        // The inner state of the logic.
        int state_;

        // This is the state of all alive ghosts.
        GhostStateEnum ghostsState_;

        // The current time passed in the vulnerable ghost state.
        int ghostTime_;

        // This is the red ghost entity.
        std::shared_ptr<Ghost> red_;

        // This is the pink ghost entity.
        std::shared_ptr<Ghost> pink_;

        // This is the blue ghost entity.
        std::shared_ptr<Ghost> blue_;

        // This is the brown ghost entity.
        std::shared_ptr<Ghost> brown_;

        // Tells to the program if the ghosts are in hurry mode.
        bool hurryMode_;

        // The last ghost killed by the puckman.
        int lastGhostKilled_;

        // The number of ghosts killed by the puckman.
        int numberOfGhostsKilled_;

        // This is the fruit shown in the maze.
        Fruit fruit_;

        // This is the fruit time counter.
        int fruitTime_;

        // Tells to the program if the fruit must be shown or not.
        bool showFruit_;

        // This is the maze reference.
        Maze * maze_;

        // This is the controller of the big dots blinking.
        BigDotsLogic bigDotsLogic_;

        // The last cell cleared by the puckman.
        sf::Vector2i lastCellCleared_;

        // This is the default maze.
        static Maze defaultMaze_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void initializeMaze(bool clearMaze);
        void initializePuckman(PuckmanEntity & puckman);
        void ghostSpeedController(Ghost & ghost);
        void initializeGhost(Ghost & ghost, int x, int y, MovingDirectionEnum dir);
        void initializeGhosts();
        void updateGhostsState();
        void setGhostsToNormal();
        void setGhostsToVulnerable();
        void setGhostsToEnding();
        void resetLastKilledGhost(int ghost);
        void checksDeadGhostInHouse(Ghost & ghost);
        void checksDeadGhostInHouse();
        void drawDeadGhosts();
        void drawAliveGhosts();
        void drawVulnerableGhosts();
        void drawNormalGhosts();
        void drawGhostWhenPuckmanEats(Ghost & ghost, int lastKilledId);
        void drawGhostsWhenPuckmanEats();
        void updateGhostWhenPuckmanEats(int time, Ghost & ghost, int lastKilledId);
        void updateGhostsWhenPuckmanEats(int time);
        void drawMaze();
        void initializeFruit(FruitTypeEnum type);
        void drawFruit();
        void updateFruit(int time);
        void updateGhostState(int time);
        void clearCell(PuckmanEntity & puckman, const sf::Vector2i & cell);
        bool checkCollision(Entity & a, Entity & b);
        void genericKillPuckman();
        void checkGhostCollision(PuckmanEntity & puckman, Ghost & ghost, int lastKilledId);
        void checkCollisions(PuckmanEntity & puckman);
        void drawPoints(int x, int y);

        /** Update the state of the logic to kill the puckman. */
        virtual void killPuckman() = 0;

        /** Check the collision between the puckman and a fruit in the maze. */
        virtual void checkFruitCollision(PuckmanEntity & puckman) = 0;
    };
}

#endif
