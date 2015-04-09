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

#include "PuckmanDemoLogic.h"
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanSequencedPuckman.h>
#include <Games/Puckman/PuckmanSequencedGhost.h>
#include <Games/Puckman/PuckmanGameTimer.h>

using namespace Puckman;

//********************************************************************************
// Constants
//********************************************************************************

const Sequence::Node PUCKMAN_SEQUENCE[] = {
	Sequence::Node(26, 9, MovingDirection::Down),
    Sequence::Node(29, 9, MovingDirection::Right),
    Sequence::Node(29, 12, MovingDirection::Down),
    Sequence::Node(32, 12, MovingDirection::Right),
    Sequence::Node(32, 26, MovingDirection::Up),
    Sequence::Node(29, 26, MovingDirection::Left),
    Sequence::Node(29, 24, MovingDirection::Up),
    Sequence::Node(26, 24, MovingDirection::Right),
    Sequence::Node(26, 26, MovingDirection::Up),
    Sequence::Node(23, 26, MovingDirection::Left), //10
    Sequence::Node(23, 18, MovingDirection::Up),
    Sequence::Node(14, 18, MovingDirection::Left),
    Sequence::Node(14, 9, MovingDirection::Down),
    Sequence::Node(17, 9, MovingDirection::Left),
    Sequence::Node(17, 6, MovingDirection::Up),
    Sequence::Node(4, 6, MovingDirection::Left),
    Sequence::Node(4, 1, MovingDirection::Down),
    Sequence::Node(8, 1, MovingDirection::Right),
    Sequence::Node(8, 12, MovingDirection::Up),
    Sequence::Node(4, 12, MovingDirection::Left), //20
    Sequence::Node(4, 6, MovingDirection::Down),
    Sequence::Node(11, 6, MovingDirection::Left),
    Sequence::Node(11, 1, MovingDirection::Up),
    Sequence::Node(8, 1, MovingDirection::Right),
    Sequence::Node(8, 9, MovingDirection::Down),
    Sequence::Node(11, 9, MovingDirection::Right),
    Sequence::Node(11, 12, MovingDirection::Down),
    Sequence::Node(14, 12, MovingDirection::Left),
    Sequence::Node(14, 9, MovingDirection::Down),
    Sequence::Node(17, 9, MovingDirection::Left), //30
    Sequence::Node(17, 21, MovingDirection::Down),
    Sequence::Node(29, 21, MovingDirection::Right),
    Sequence::Node(29, 26, MovingDirection::Down),
    Sequence::Node(32, 26, MovingDirection::Left),
    Sequence::Node(32, 1, MovingDirection::Up),
    Sequence::Node(29, 1, MovingDirection::Right),
    Sequence::Node(29, 3, MovingDirection::Up),
    Sequence::Node(26, 3, MovingDirection::Left),
    Sequence::Node(26, 1, MovingDirection::Up),
    Sequence::Node(23, 1, MovingDirection::Right), //40
    Sequence::Node(23, 12, MovingDirection::Down),
    Sequence::Node(26, 12, MovingDirection::Right),
    Sequence::Node(26, 15, MovingDirection::Up),
    Sequence::Node(23, 15, MovingDirection::Right),
    Sequence::Node(23, 26, MovingDirection::Down),
    Sequence::Node(26, 26, MovingDirection::Left),
    Sequence::Node(26, 24, MovingDirection::Down),
    Sequence::Node(29, 24, MovingDirection::Right),
    Sequence::Node(29, 26, MovingDirection::Down),
    Sequence::Node(32, 26, MovingDirection::Left), //50
    Sequence::Node(32, 1, MovingDirection::Up),
    Sequence::Node(29, 1, MovingDirection::Right),
    Sequence::Node(29, 3, MovingDirection::Up),
    Sequence::Node(26, 3, MovingDirection::Left),
    Sequence::Node(26, 1, MovingDirection::Up),
    Sequence::Node(23, 1, MovingDirection::Right)
};

const Sequence::Node RED_SEQUENCE[] = {
	Sequence::Node(14, 9, MovingDirection::Down),
	Sequence::Node(17, 9, MovingDirection::Left),
	Sequence::Node(17, 6, MovingDirection::Up),
	Sequence::Node(8, 6, MovingDirection::Right),
	Sequence::Node(8, 21, MovingDirection::Up),
	Sequence::Node(4, 21, MovingDirection::Right),
	Sequence::Node(4, 23, MovingDirection::Left),
	Sequence::Node(4, 21, MovingDirection::Down),
	Sequence::Node(11, 21, MovingDirection::Right), //10
	Sequence::Node(11, 26, MovingDirection::Up),
	Sequence::Node(8, 26, MovingDirection::Left),
	Sequence::Node(8, 21, MovingDirection::Down),
	Sequence::Node(11, 21, MovingDirection::Right),
	Sequence::Node(11, 26, MovingDirection::Up),
	Sequence::Node(28, 204, MovingDirection::Down, true),
	Sequence::Node(11, 26, MovingDirection::Left),
	Sequence::Node(11, 21, MovingDirection::Down),
	Sequence::Node(17, 21, MovingDirection::Right),
	Sequence::Node(17, 1, MovingDirection::Left), //20
	Sequence::Node(17, 18, MovingDirection::Up),
	Sequence::Node(14, 18, MovingDirection::Left),
	Sequence::Node(14, 9, MovingDirection::Down),
	Sequence::Node(20, 9, MovingDirection::Right),
	Sequence::Node(20, 18, MovingDirection::Down),
	Sequence::Node(23, 18, MovingDirection::Right),
	Sequence::Node(23, 21, MovingDirection::Down),
	Sequence::Node(29, 21, MovingDirection::Right),
	Sequence::Node(29, 24, MovingDirection::Up), //30
	Sequence::Node(26, 24, MovingDirection::Right),
	Sequence::Node(26, 26, MovingDirection::Up),
	Sequence::Node(23, 26, MovingDirection::Left),
	Sequence::Node(23, 15, MovingDirection::Down),
	Sequence::Node(26, 15, MovingDirection::Left),
	Sequence::Node(26, 8, MovingDirection::Right),
	Sequence::Node(26, 12, MovingDirection::Up),
	Sequence::Node(23, 12, MovingDirection::Left),
	Sequence::Node(23, 6, MovingDirection::Up), //40
	Sequence::Node(17, 6, MovingDirection::Right),
	Sequence::Node(17, 9, MovingDirection::Up),
	Sequence::Node(14, 9, MovingDirection::Right),
	Sequence::Node(14, 13, MovingDirection::Down),
	Sequence::Node(136, 104, MovingDirection::Up, true),
	Sequence::Node(14, 13, MovingDirection::Left),
	Sequence::Node(14, 9, MovingDirection::Down),
	Sequence::Node(20, 9, MovingDirection::Right),
	Sequence::Node(20, 18, MovingDirection::Down),
	Sequence::Node(23, 18, MovingDirection::Left), //50
	Sequence::Node(23, 15, MovingDirection::Down),
	Sequence::Node(26, 15, MovingDirection::Left),
	Sequence::Node(26, 9, MovingDirection::Down),
	Sequence::Node(29, 9, MovingDirection::Right),
	Sequence::Node(29, 12, MovingDirection::Down),
	Sequence::Node(32, 12, MovingDirection::Left),
	Sequence::Node(32, 1, MovingDirection::Up),
	Sequence::Node(29, 1, MovingDirection::Right),
	Sequence::Node(29, 3, MovingDirection::Up)
};

const Sequence::Node PINK_SEQUENCE[] = {
	Sequence::Node(17, 13, MovingDirection::Up),
	Sequence::Node(14, 13, MovingDirection::Left),
	Sequence::Node(14, 9, MovingDirection::Down),
	Sequence::Node(17, 9, MovingDirection::Left),
	Sequence::Node(17, 6, MovingDirection::Up),
	Sequence::Node(4, 6, MovingDirection::Left),
	Sequence::Node(4, 1, MovingDirection::Down),
	Sequence::Node(8, 1, MovingDirection::Right),
	Sequence::Node(8, 3, MovingDirection::Left),
	Sequence::Node(8, 1, MovingDirection::Up), //10
	Sequence::Node(4, 1, MovingDirection::Right),
	Sequence::Node(4, 6, MovingDirection::Down),
	Sequence::Node(8, 6, MovingDirection::Left),
	Sequence::Node(8, 1, MovingDirection::Up),
	Sequence::Node(4, 1, MovingDirection::Right),
	Sequence::Node(4, 12, MovingDirection::Down), //15a
	Sequence::Node(7, 12, MovingDirection::Up),
	Sequence::Node(4, 12, MovingDirection::Left),
	Sequence::Node(4, 6, MovingDirection::Down),
	Sequence::Node(11, 6, MovingDirection::Left),
	Sequence::Node(11, 1, MovingDirection::Up),
	Sequence::Node(8, 1, MovingDirection::Right),
	Sequence::Node(8, 9, MovingDirection::Down), //20
	Sequence::Node(11, 9, MovingDirection::Right),
	Sequence::Node(11, 12, MovingDirection::Down),
	Sequence::Node(14, 12, MovingDirection::Right),
	Sequence::Node(14, 13, MovingDirection::Down),
	Sequence::Node(136, 104, MovingDirection::Up, true),
	Sequence::Node(14, 13, MovingDirection::Left),
	Sequence::Node(14, 12, MovingDirection::Right),
	Sequence::Node(14, 18, MovingDirection::Down),
	Sequence::Node(20, 18, MovingDirection::Left),
	Sequence::Node(20, 9, MovingDirection::Up), //30
	Sequence::Node(14, 9, MovingDirection::Right),
	Sequence::Node(14, 18, MovingDirection::Down),
	Sequence::Node(23, 18, MovingDirection::Right),
	Sequence::Node(23, 21, MovingDirection::Down),
	Sequence::Node(29, 21, MovingDirection::Right),
	Sequence::Node(29, 24, MovingDirection::Up),
	Sequence::Node(26, 24, MovingDirection::Right),
	Sequence::Node(26, 26, MovingDirection::Up),
	Sequence::Node(23, 26, MovingDirection::Left),
	Sequence::Node(23, 15, MovingDirection::Down), //40
	Sequence::Node(26, 15, MovingDirection::Left),
	Sequence::Node(26, 13, MovingDirection::Right),
	Sequence::Node(26, 15, MovingDirection::Up),
	Sequence::Node(23, 15, MovingDirection::Right),
	Sequence::Node(23, 18, MovingDirection::Up),
	Sequence::Node(20, 18, MovingDirection::Left),
	Sequence::Node(20, 9, MovingDirection::Up),
	Sequence::Node(17, 9, MovingDirection::Left),
	Sequence::Node(17, 6, MovingDirection::Down),
	Sequence::Node(23, 6, MovingDirection::Right), //50
	Sequence::Node(23, 12, MovingDirection::Down),
	Sequence::Node(26, 12, MovingDirection::Left),
	Sequence::Node(26, 9, MovingDirection::Down),
	Sequence::Node(29, 9, MovingDirection::Right),
	Sequence::Node(29, 12, MovingDirection::Down),
	Sequence::Node(32, 12, MovingDirection::Left),
	Sequence::Node(32, 1, MovingDirection::Up),
	Sequence::Node(29, 1, MovingDirection::Right),
	Sequence::Node(29, 6, MovingDirection::Up)
};

const Sequence::Node BLUE_SEQUENCE[] = {
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(136, 88, MovingDirection::Up, true),
	Sequence::Node(128, 88, MovingDirection::Down, true),
	Sequence::Node(132, 88, MovingDirection::Right, true),
	Sequence::Node(132, 104, MovingDirection::Up, true),
	Sequence::Node(14, 13, MovingDirection::Left),

	Sequence::Node(14, 12, MovingDirection::Right),
	Sequence::Node(14, 15, MovingDirection::Up),
	Sequence::Node(11, 15, MovingDirection::Right),
	Sequence::Node(11, 18, MovingDirection::Up),
	Sequence::Node(8, 18, MovingDirection::Left),
	Sequence::Node(8, 1, MovingDirection::Down),
	Sequence::Node(11, 1, MovingDirection::Right),
	Sequence::Node(11, 5, MovingDirection::Left),
	Sequence::Node(11, 1, MovingDirection::Up),
	Sequence::Node(8, 1, MovingDirection::Right), //10
	Sequence::Node(8, 6, MovingDirection::Up),
	Sequence::Node(4, 6, MovingDirection::Right),
	Sequence::Node(4, 12, MovingDirection::Down),
	Sequence::Node(8, 12, MovingDirection::Right),
	Sequence::Node(8, 18, MovingDirection::Down),
	Sequence::Node(11, 18, MovingDirection::Left),
	Sequence::Node(11, 15, MovingDirection::Down),
	Sequence::Node(14, 15, MovingDirection::Left),
	Sequence::Node(14, 13, MovingDirection::Down),
	Sequence::Node(136, 104, MovingDirection::Left, true), //20
	Sequence::Node(136, 88, MovingDirection::Right, true),
	Sequence::Node(136, 104, MovingDirection::Up, true),
	Sequence::Node(14, 13, MovingDirection::Left),
	Sequence::Node(14, 12, MovingDirection::Right),
	Sequence::Node(14, 18, MovingDirection::Down),
	Sequence::Node(20, 18, MovingDirection::Left),
	Sequence::Node(20, 9, MovingDirection::Up),
	Sequence::Node(17, 9, MovingDirection::Left),
	Sequence::Node(17, 6, MovingDirection::Down),
	Sequence::Node(23, 6, MovingDirection::Right), //30
	Sequence::Node(23, 12, MovingDirection::Down),
	Sequence::Node(26, 12, MovingDirection::Right),
	Sequence::Node(26, 21, MovingDirection::Down),
	Sequence::Node(29, 21, MovingDirection::Right),
	Sequence::Node(29, 26, MovingDirection::Down),
	Sequence::Node(32, 26, MovingDirection::Left),
	Sequence::Node(32, 2, MovingDirection::Right),
	Sequence::Node(32, 12, MovingDirection::Up),
	Sequence::Node(29, 12, MovingDirection::Left),
	Sequence::Node(29, 9, MovingDirection::Up), //40
	Sequence::Node(26, 9, MovingDirection::Right),
	Sequence::Node(26, 12, MovingDirection::Up),
	Sequence::Node(23, 12, MovingDirection::Left),
	Sequence::Node(23, 9, MovingDirection::Up),
	Sequence::Node(20, 9, MovingDirection::Right),
	Sequence::Node(20, 18, MovingDirection::Down),
	Sequence::Node(23, 18, MovingDirection::Left),
	Sequence::Node(23, 15, MovingDirection::Down),
	Sequence::Node(26, 15, MovingDirection::Left),
	Sequence::Node(26, 6, MovingDirection::Up), //50
	Sequence::Node(23, 6, MovingDirection::Left),
	Sequence::Node(23, 1, MovingDirection::Down),
	Sequence::Node(26, 1, MovingDirection::Right),
	Sequence::Node(26, 3, MovingDirection::Down)
};

const Sequence::Node BROWN_SEQUENCE[] = {
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),
	Sequence::Node(128, 120, MovingDirection::Down, true),
	Sequence::Node(136, 120, MovingDirection::Up, true),        
    Sequence::Node(132, 120, MovingDirection::Left, true),
    Sequence::Node(132, 104, MovingDirection::Up, true),
    Sequence::Node(14, 13, MovingDirection::Left),

	Sequence::Node(14, 12, MovingDirection::Right),
	Sequence::Node(14, 18, MovingDirection::Down),
	Sequence::Node(17, 18, MovingDirection::Right),
	Sequence::Node(17, 22, MovingDirection::Left),
	Sequence::Node(17, 18, MovingDirection::Up),
	Sequence::Node(14, 18, MovingDirection::Left),
	Sequence::Node(14, 9, MovingDirection::Down),
	Sequence::Node(23, 9, MovingDirection::Left),
	Sequence::Node(23, 1, MovingDirection::Down),
	Sequence::Node(26, 1, MovingDirection::Right), //10
	Sequence::Node(26, 3, MovingDirection::Down),
	Sequence::Node(29, 3, MovingDirection::Right),
	Sequence::Node(29, 6, MovingDirection::Up),
	Sequence::Node(26, 6, MovingDirection::Right),
	Sequence::Node(26, 18, MovingDirection::Down),
	Sequence::Node(29, 18, MovingDirection::Left),
	Sequence::Node(29, 15, MovingDirection::Down),
	Sequence::Node(32, 15, MovingDirection::Left),
	Sequence::Node(32, 1, MovingDirection::Up),
	Sequence::Node(29, 1, MovingDirection::Right), //20
	Sequence::Node(29, 6, MovingDirection::Up),
	Sequence::Node(28, 6, MovingDirection::Down),
	Sequence::Node(29, 6, MovingDirection::Left),
	Sequence::Node(29, 1, MovingDirection::Down),
	Sequence::Node(32, 1, MovingDirection::Right),
	Sequence::Node(32, 12, MovingDirection::Up),
	Sequence::Node(29, 12, MovingDirection::Left),
	Sequence::Node(29, 9, MovingDirection::Up),
	Sequence::Node(26, 9, MovingDirection::Right),
	Sequence::Node(26, 18, MovingDirection::Down), //30
	Sequence::Node(29, 18, MovingDirection::Left),
	Sequence::Node(29, 15, MovingDirection::Down),
	Sequence::Node(32, 15, MovingDirection::Left),
	Sequence::Node(32, 1, MovingDirection::Up),
	Sequence::Node(29, 1, MovingDirection::Right),
	Sequence::Node(29, 3, MovingDirection::Up),
	Sequence::Node(26, 3, MovingDirection::Left),
	Sequence::Node(26, 1, MovingDirection::Up)
};

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the demo logic object.
 */
void DemoLogic::Initialize() {
    // First, we'll initialize the maze data.
    initializeMaze(true);

    // Second, we'll initialize the ghost data.
    initializeGhosts();

    // Third, we'll initialize the fruit data.
    initializeFruit(FruitType::Cherry);

    // Fourth, we'll initialize the puckman data.
    initializePuckman(*puckman_);
    puckman_->NeededDirection(MovingDirection::Left);

    // Fifth, we'll initialize the sequences data.
    puckman_->MoveSequence.Reset();
    ((SequencedGhost *)red_.get())->MoveSequence.Reset();
    ((SequencedGhost *)pink_.get())->MoveSequence.Reset();
    ((SequencedGhost *)blue_.get())->MoveSequence.Reset();
    ((SequencedGhost *)brown_.get())->MoveSequence.Reset();

    // And finally, we'll initialize the logic data.
    ended_ = false;
    state_ = DemoLogic::INITIAL_STATE;
    GameTimer::Set(BEGIN_DELAY_TIME);
}

//--------------------------------------------------------------------------------

/**
 * Releases the demo logic object.
 */
void DemoLogic::Release() {
}

//--------------------------------------------------------------------------------

/**
 * Draws the maze of the demo.
 */
void DemoLogic::drawDemoMaze() {
    // Here, we'll draw the maze and the big dots blink controller will hide
    // or not the registered dots. The we'll draw a text for the demo.
    drawMaze();
    manager_->DrawText(20, 9, "GAME  OVER", Palette::Red);
}

//--------------------------------------------------------------------------------

/**
 * Draws the content of the demo.
 */
void DemoLogic::Draw() {
    switch(state_) {
    case INITIAL_STATE:
    case NORMAL_STATE:
        // First, we'll draw the maze and the fruit.
        drawDemoMaze();
        drawFruit();

        // Second, we'll check if the ghost are normal or not, because we have
        // to draw ghost when they are normal over puckman and when they are
        // vulnerable or dead under puckman.
        if(ghostsState_ == GhostState::Normal) {
            drawDeadGhosts();
            puckman_->Draw();
            drawAliveGhosts();

        } else {
            // If the ghosts are in vulnerable state and puckman eats one, there
            // will be a pause to show the points of the last killed ghost, that's
            // the reason why the timer is used in this state. When the timer is
            // stopped there isn't any pause and everybody is moving.
            if(GameTimer::IsStopped()) {
                drawVulnerableGhosts();
                puckman_->Draw();
                drawNormalGhosts();

            } else {
                // But if there is a pause the puckman and one ghost can't be drawn.
                // So we'll check one by one each ghost to find the eat one, to draw
                // the point over it.
                drawGhostsWhenPuckmanEats();
            }
        }
        break;

    case KILL_STATE:
    case DEATH_STATE:
        drawDemoMaze();
        puckman_->Draw();
        break;

    case END_STATE:
        drawDemoMaze();
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner logic of the demo.
 */
void DemoLogic::Update(int time) {
    switch(state_) {
    case DemoLogic::INITIAL_STATE:
        // In this state we'll take a pause in the game.
        if(GameTimer::IsStopped()) {
            state_ = NORMAL_STATE;
        }
        break;

    case NORMAL_STATE:
        // In this state we'll update the big dots blink controller and we'll
        // check if any dead ghost is in the house, to bring it back to life.
        bigDotsLogic_.Update(time);
        checksDeadGhostInHouse();

        // And if the game isn't paused, we'll update the ghost state, the
        // fruit logic, the collisions and all the entities.
        if(GameTimer::IsStopped()) {
            updateGhostState(time);
            updateFruit(time);
            checkCollisions(*puckman_);

            puckman_->Update(time);
            red_->Update(time);
            pink_->Update(time);
            blue_->Update(time);
            brown_->Update(time);

        } else {
            // If the game is paused, we'll only update the ghosts that are
            // not alive and are not the last one killed.
            updateGhostsWhenPuckmanEats(time);
        }
        break;

    case KILL_STATE:
        // Here the puckman have been caught and we're waiting some time.
        if(GameTimer::IsStopped()) {
            state_ = DEATH_STATE;
            puckman_->Kill();
        } else {
            bigDotsLogic_.Update(time);
            red_->Update(time);
            pink_->Update(time);
            blue_->Update(time);
            brown_->Update(time);
        }
        break;

    case DEATH_STATE:
        // After wait, we'll see the puckman death animation.
        if(puckman_->IsDeathSpriteEnded()) {
            state_ = END_STATE;
            GameTimer::Set(END_DELAY_TIME);
        } else {
            bigDotsLogic_.Update(time);
            puckman_->Update(time);
        }
        break;

    case END_STATE:
        // And finally we'll have a pause and end the demo.
        if(GameTimer::IsStopped()) {
            ended_ = true;
            puckman_->Respawn();
        }
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Update the state of the logic to kill the puckman.
 */
void DemoLogic::killPuckman() {
    puckman_->StopSpeed();
    genericKillPuckman();
    GameTimer::Set(KILL_DELAY_TIME);
}

//--------------------------------------------------------------------------------

/**
 * Updates the puckman entity.
 */
void DemoLogic::puckmanUpdate() {
    // First, we'll get the current cell of the puckman entity.
    sf::Vector2i cell = Maze::SpriteCoordsToMaze(puckman_->X, puckman_->Y);

    // Second, if the coordinates are inside the maze.
    if(0 <= cell.x && cell.x < Manager::COLUMS) {
        // We'll get the value of that cell.
        int cellValue = maze_->Data()[cell.y][cell.x];

        // And then we'll check that value.
        if(cellValue == Maze::CLEAN) {
            // If the cell is empty and isn't the last cell cleared, the
            // puckman will go in fast mode.
            if(cell != lastCellCleared_) {
                puckman_->SetFast();
            }

        } else if(cellValue == Maze::DOT) {
            // If the cell have a dot, we'll clear the cell and set
            // puckman in slow mode.
            clearCell(*puckman_, cell);

        } else if(cellValue == Maze::BIG_DOT) {
            // If the cell have a big dot, we'll clear the cell, set
            // puckman in slow mode and set the ghosts in vulnerable mode.
            clearCell(*puckman_, cell);
            setGhostsToVulnerable();
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Check the collision between the puckman and a fruit in the maze.
 */
void DemoLogic::checkFruitCollision(PuckmanEntity & puckman) {
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
DemoLogic::DemoLogic() : GenericLogic() {
    puckman_ = std::make_shared<SequencedPuckman>(ARRAY_TO_NODES(PUCKMAN_SEQUENCE));
    red_ = std::make_shared<SequencedGhost>(GhostType::Red, ARRAY_TO_NODES(RED_SEQUENCE));
    pink_ = std::make_shared<SequencedGhost>(GhostType::Pink, ARRAY_TO_NODES(PINK_SEQUENCE));
    blue_ = std::make_shared<SequencedGhost>(GhostType::Blue, ARRAY_TO_NODES(BLUE_SEQUENCE));
    brown_ = std::make_shared<SequencedGhost>(GhostType::Brown, ARRAY_TO_NODES(BROWN_SEQUENCE));

    fruit_ = Fruit(FruitType::Cherry);

    puckman_->CheckCoordinates = std::bind(&Maze::CheckSpriteCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    red_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    pink_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    blue_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);
    brown_->CheckCoordinates = std::bind(&Maze::CheckGhostCoords, maze_,
        std::placeholders::_1, std::placeholders::_2);

    puckman_->OnMove.push_back([this] (MovingEntity &) { puckmanUpdate(); });
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
DemoLogic::~DemoLogic() {}
