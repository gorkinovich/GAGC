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

#include "PuckmanInsertCoinState.h"
#include <SFML/System/Time.hpp>
#include <System/Sound.h>
#include <System/Keyboard.h>
#include <Games/Puckman/PuckmanManager.h>
#include <Games/Puckman/PuckmanPalette.h>
#include <Games/Puckman/PuckmanGameData.h>
#include <Games/Puckman/PuckmanGameTimer.h>
#include <Games/Puckman/PuckmanSprites.h>
#include <Games/Puckman/PuckmanSelectPlayerState.h>

using namespace Puckman;

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Changes the inner substate of the class.
 */
void InsertCoinState::changeInnerState(Substates newState) {
    // We set the new state.
    innerState_ = newState;
    // And set a delay if we need it.
    if(newState != ANIMATION_STATE && newState != DEMO_STATE) {
        GameTimer::Set(DELAY_TIME);
    }
}

//--------------------------------------------------------------------------------

/**
 * Initializes the state.
 */
void InsertCoinState::Initialize() {
    // General data.
    changeInnerState(INITIAL_STATE);

    // Animation data.
    bigDotEated_ = false;
    bigDotsLogic_.ResetTime();

    puckman_.SetCoordinates(224, 156);
    puckman_.SetSpeedInterval(PUCKMAN_SLOW_INTERVAL);

    red_.SetCoordinates(253, 156);
    pink_.SetCoordinates(253 + 16, 156);
    blue_.SetCoordinates(253 + 32, 156);
    brown_.SetCoordinates(253 + 48, 156);

    puckman_.NeededDirection(MovingDirection::Left);
    red_.NeededDirection(MovingDirection::Left);
    pink_.NeededDirection(MovingDirection::Left);
    blue_.NeededDirection(MovingDirection::Left);
    brown_.NeededDirection(MovingDirection::Left);

    red_.BackToLife();
    pink_.BackToLife();
    blue_.BackToLife();
    brown_.BackToLife();

    red_.ChangeState(GhostState::Normal);
    pink_.ChangeState(GhostState::Normal);
    blue_.ChangeState(GhostState::Normal);
    brown_.ChangeState(GhostState::Normal);

    aniState_ = NORMAL;
}

//--------------------------------------------------------------------------------

/**
 * Releases the state.
 */
void InsertCoinState::Release() {
}

//--------------------------------------------------------------------------------

/**
 * Draws the text of the state.
 */
void InsertCoinState::drawText() {
    switch(innerState_) {
    case ANIMATION_STATE:
    case SHOW_FINAL_TEXT_STATE:
        if(bigDotEated_ == false) {
            manager_->DrawTile(20, 4, Maze::BIG_DOT);
        }
        manager_->DrawText(29, 5, "PAXNAMAN BY GORKIN", Palette::Pink);
        manager_->DrawText(32, 3, "PRESS C TO INSERT COIN", Palette::Blue);

    case SHOW_DOTS_STATE:
        manager_->DrawTile(24, 10, Maze::DOT);
        manager_->DrawText(24, 12, "10");
        manager_->DrawSymbol(24, 15, FontSymbol::Pts);

        manager_->DrawTile(26, 10, Maze::BIG_DOT);
        manager_->DrawText(26, 12, "50");
        manager_->DrawSymbol(26, 15, FontSymbol::Pts);

    case SHOW_BROWN_NICK_STATE:
        manager_->DrawText(16, 18, "\"GUZUTA\"", Palette::Brown);

    case SHOW_BROWN_NAME_STATE:
        manager_->DrawText(16, 8, "OTOBOKE---", Palette::Brown);

    case SHOW_BROWN_GHOST_STATE:
        manager_->DrawSprite(32, 124, Sprites::BrownGhostR1); // r15.5 c4 Brown ghost

    case SHOW_BLUE_NICK_STATE:
        manager_->DrawText(13, 18, "\"AOSUKE\"", Palette::Cyan);

    case SHOW_BLUE_NAME_STATE:
        manager_->DrawText(13, 8, "KIMAGURE--", Palette::Cyan);

    case SHOW_BLUE_GHOST_STATE:
        manager_->DrawSprite(32, 100, Sprites::BlueGhostR1); // r12.5 c4 Blue ghost

    case SHOW_PINK_NICK_STATE:
        manager_->DrawText(10, 19, "\"PINKY\"", Palette::Pink);

    case SHOW_PINK_NAME_STATE:
        manager_->DrawText(10, 8, "MACHIBUSE--", Palette::Pink);

    case SHOW_PINK_GHOST_STATE:
        manager_->DrawSprite(32, 76, Sprites::PinkGhostR1); // r9.5 c4 Pink ghost

    case SHOW_RED_NICK_STATE:
        manager_->DrawText(7, 18, "\"AKABEI\"", Palette::Red);

    case SHOW_RED_NAME_STATE:
        manager_->DrawText(7, 8, "OIKAKE----", Palette::Red);

    case SHOW_RED_GHOST_STATE:
        manager_->DrawSprite(32, 52, Sprites::RedGhostR1); // r6.5 c4 Red ghost

    case INITIAL_STATE:
        drawMenuHud();
        manager_->DrawText(5, 7, "CHARACTER / NICKNAME");
        break;
    }
}

//--------------------------------------------------------------------------------

/**
 * Draws the content of the state.
 */
void InsertCoinState::Draw(const sf::Time & timeDelta) {
    // First, we have to check in which state we are.
    if(innerState_ == DEMO_STATE) {
        // In the demo substate we'll call the draw demo method.
        demo_.Draw();

        // We'll clear the wormhole areas.
        manager_->ClearSprite(Manager::WORMHOLE_LX - Sprites::Width, Manager::WORMHOLE_Y);
        manager_->ClearSprite(Manager::WORMHOLE_LX, Manager::WORMHOLE_Y);
        manager_->ClearSprite(Manager::WORMHOLE_RX, Manager::WORMHOLE_Y);
        manager_->ClearSprite(Manager::WORMHOLE_RX + Sprites::Width, Manager::WORMHOLE_Y);

        // And after that we'll draw the menu HUD.
        drawMenuHud();

    } else if(innerState_ == ANIMATION_STATE) {
        // In the animation substate the first thing to do is to hide or not the
        // big dots of the screen, to make them blink.
        drawText();
        bigDotsLogic_.Draw();

        // Second, we'll draw the ghosts if they are alive. When the ghost are in
        // normal mode, they are drawn over the puckman sprite, but when they are
        // in vulnerable mode, they are drawn under the puckman sprite. In this
        // case the ghosts never touch puckman when they are in normal mode, so
        // we'll always paint then in first place, under the puckman sprite.
        if(red_.Alive()) { red_.Draw(); }
        if(pink_.Alive()) { pink_.Draw(); }
        if(blue_.Alive()) { blue_.Draw(); }
        if(brown_.Alive()) { brown_.Draw(); }

        // Third, we'll draw puckman if the animation isn't paused.
        if(GameTimer::IsStopped()) {
            puckman_.Draw();
        } else {
            // If the animation is paused, we'll check the state of the animation.
            // That will allow us to know, which points sprite we'll have to draw.
            switch(aniState_) {
            case NORMAL:
                puckman_.Draw();
                break;
            case RED_DIE:
                manager_->DrawSprite(red_.X, red_.Y, Sprites::N200);
                break;
            case PINK_DIE:
                manager_->DrawSprite(pink_.X, pink_.Y, Sprites::N400);
                break;
            case BLUE_DIE:
                manager_->DrawSprite(blue_.X, blue_.Y, Sprites::N800);
                break;
            case BROWN_DIE:
                manager_->DrawSprite(brown_.X, brown_.Y, Sprites::N1600);
                break;
            }
        }
    } else {
        // Here we'll draw the text of the manager_->
        drawText();
    }
}

//--------------------------------------------------------------------------------

/**
 * Try to change to the next substate of this class.
 */
void InsertCoinState::changeToNextState(Substates nextState) {
    if(GameTimer::IsStopped()) {
        changeInnerState(nextState);
    }
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner logic of the state.
 */
void InsertCoinState::Update(const sf::Time & timeDelta) {
    auto time = timeDelta.asMilliseconds();

    // Here we check if the user wants to insert a coin, if so we'll
    // change to the next state in the game, the select player state.
    if(Keyboard::IsKeyUp(Keyboard::C)) {
        data_->AddCoin();
        manager_->FruitEat().Play();
        GameTimer::Stop();
        manager_->SetNextState(Manager::Instance()->SelectPlayerStateInstance());
        return;
    }

    // Because we're doing some animations and effects, we'll need to use the
    // game timer and here we're going to update it.
    GameTimer::Update(time);

    // And here we'll control the logic of the substates. I'm sorry of this
    // spaghetti code, but make a cool animation system is very hard ;)
    switch(innerState_) {
    case ANIMATION_STATE:
        // In this substate we're going to make an animation, where puckman is going
        // to eat some ghosts. But first we'll update the big dots blinking.
        bigDotsLogic_.Update(time);

        // After that, we have to check if the game timer is stopped, because we're
        // going to use it to stop the animation when puckman eats a ghost.
        if(GameTimer::IsStopped()) {
            // If the brown ghost, the last one, dies and the timer have stopped,
            // we'll change to the next substate, where we'll see a demo of this game.
            if(aniState_ == BROWN_DIE) {
                demo_.Initialize();
                changeInnerState(DEMO_STATE);

            } else {
                // Here the animation isn't ended, so we'll update the entities of
                // the animation. This will make them move.
                puckman_.Update(time);
                red_.Update(time);
                pink_.Update(time);
                blue_.Update(time);
                brown_.Update(time);

                // The first part of the animation is puckman escaping from the ghosts,
                // but when puckman gets the big dot everything changes, because the
                // ghosts are vulnerable now and puckman can eat them. Here is where
                // we check if puckman have eated the big dot.
                if(puckman_.X <= 32) {
                    // If the big dot is eated, we have to update this flag, to
                    // avoid that the big dot eated is painted again.
                    bigDotEated_ = true;

                    // And we set puckman to fast mode.
                    puckman_.SetSpeedInterval(PUCKMAN_FAST_INTERVAL);

                    // Then we'll change the direction of all the entities.
                    puckman_.NeededDirection(MovingDirection::Right);
                    red_.NeededDirection(MovingDirection::Right);
                    pink_.NeededDirection(MovingDirection::Right);
                    blue_.NeededDirection(MovingDirection::Right);
                    brown_.NeededDirection(MovingDirection::Right);

                    // And we'll set all the ghosts to vulnerable mode.
                    red_.ChangeState(GhostState::Vulnerable);
                    pink_.ChangeState(GhostState::Vulnerable);
                    blue_.ChangeState(GhostState::Vulnerable);
                    brown_.ChangeState(GhostState::Vulnerable);
                }

                // If the big dot is eated, we'll check if puckman have caught any ghost.
                if(bigDotEated_) {
                    // If puckman is over the red ghost, we'll kill it, set the animation
                    // to the next state and pause it some time, to show the points gained
                    // by eating that ghost.
                    if(red_.Alive() && (puckman_.X + (Sprites::Width / 2) >= red_.X)) {
                        red_.Kill();
                        aniState_ = RED_DIE;
                        GameTimer::Set(POINTS_DELAY_TIME);
                    }

                    // If puckman is over the pink ghost, we'll kill it, set the animation
                    // to the next state and pause it some time, to show the points gained
                    // by eating that ghost.
                    if(pink_.Alive() && (puckman_.X + (Sprites::Width / 2) >= pink_.X)) {
                        pink_.Kill();
                        aniState_ = PINK_DIE;
                        GameTimer::Set(POINTS_DELAY_TIME);
                    }

                    // If puckman is over the blue ghost, we'll kill it, set the animation
                    // to the next state and pause it some time, to show the points gained
                    // by eating that ghost.
                    if(blue_.Alive() && (puckman_.X + (Sprites::Width / 2) >= blue_.X)) {
                        blue_.Kill();
                        aniState_ = BLUE_DIE;
                        GameTimer::Set(POINTS_DELAY_TIME);
                    }

                    // If puckman is over the brown ghost, we'll kill it, set the animation
                    // to the next state and pause it some time, to show the points gained
                    // by eating that ghost.
                    if(brown_.Alive() && (puckman_.X + (Sprites::Width / 2) >= brown_.X)) {
                        brown_.Kill();
                        aniState_ = BROWN_DIE;
                        GameTimer::Set(POINTS_DELAY_TIME);
                    }
                }
            }
        }
        break;

    case DEMO_STATE:
        // In this substate we're going to show a demo of the game, that's
        // why all the big stuff is going to be made inside the demo object.
        demo_.Update(time);
        // And when the demo is ended, we'll reset the state to start the
        // puckman and text animation one more time.
        if(demo_.Ended()) {
            Initialize();
        }
        break;

    case INITIAL_STATE:
        changeToNextState(SHOW_RED_GHOST_STATE);
        break;

    case SHOW_RED_GHOST_STATE:
        changeToNextState(SHOW_RED_NAME_STATE);
        break;

    case SHOW_RED_NAME_STATE:
        changeToNextState(SHOW_RED_NICK_STATE);
        break;

    case SHOW_RED_NICK_STATE:
        changeToNextState(SHOW_PINK_GHOST_STATE);
        break;

    case SHOW_PINK_GHOST_STATE:
        changeToNextState(SHOW_PINK_NAME_STATE);
        break;

    case SHOW_PINK_NAME_STATE:
        changeToNextState(SHOW_PINK_NICK_STATE);
        break;

    case SHOW_PINK_NICK_STATE:
        changeToNextState(SHOW_BLUE_GHOST_STATE);
        break;

    case SHOW_BLUE_GHOST_STATE:
        changeToNextState(SHOW_BLUE_NAME_STATE);
        break;

    case SHOW_BLUE_NAME_STATE:
        changeToNextState(SHOW_BLUE_NICK_STATE);
        break;

    case SHOW_BLUE_NICK_STATE:
        changeToNextState(SHOW_BROWN_GHOST_STATE);
        break;

    case SHOW_BROWN_GHOST_STATE:
        changeToNextState(SHOW_BROWN_NAME_STATE);
        break;

    case SHOW_BROWN_NAME_STATE:
        changeToNextState(SHOW_BROWN_NICK_STATE);
        break;

    case SHOW_BROWN_NICK_STATE:
        changeToNextState(SHOW_DOTS_STATE);
        break;

    case SHOW_DOTS_STATE:
        changeToNextState(SHOW_FINAL_TEXT_STATE);
        break;

    case SHOW_FINAL_TEXT_STATE:
        changeToNextState(ANIMATION_STATE);
        break;
    }
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
InsertCoinState::InsertCoinState() : GenericState(), innerState_(INITIAL_STATE),
    bigDotsLogic_(), bigDotEated_(false), puckman_(), red_(GhostType::Red),
    pink_(GhostType::Pink), blue_(GhostType::Blue), brown_(GhostType::Brown),
    aniState_(NORMAL), demo_() {
    bigDotsLogic_.Register(26, 10);
    bigDotsLogic_.Register(20, 4);
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
InsertCoinState::~InsertCoinState() {}
