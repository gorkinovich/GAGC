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

#ifndef __PUCKMAN_GAME_DATA_HEADER__
#define __PUCKMAN_GAME_DATA_HEADER__

#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanPlayer.h>

namespace Puckman {
    /**
     * This class represents the data of the game.
     */
    class GameData {
    public:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The minimum number of coins of the game.
        static const int MIN_COINS = 0;

        // The maximum number of coins of the game.
        static const int MAX_COINS = 99;

        // The maximum number of fruits in the array.
        static const int MAX_FRUITS = 6;

        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        int HighScore() { return highScore_; }
        void HighScore(int value) { highScore_ = value; }

        int Coins() { return coins_; }
        void Coins(int value) {
            if(MIN_COINS <= value && value <= MAX_COINS) {
                coins_ = value;
            }
        }

        Player & Player1() { return player1_; }
        Player & Player2() { return player2_; }

        int NumberOfPlayers() { return numberOfPlayers_; }
        void NumberOfPlayers(int value) { numberOfPlayers_ = value; }

        int PlayerTurn() { return playerTurn_; }
        void PlayerTurn(int value) {
            if(1 <= value && value <= 2) {
                playerTurn_ = value;
            }
        }

        const std::vector<FruitTypeEnum> & LastFruits() { return lastFruits_; }
        void LastFruits(const std::vector<FruitTypeEnum> & value) { lastFruits_ = value; }

        // Gets the current player.
        Player & PlayerData() { return playerTurn_ == 1 ? player1_ : player2_; }

        // Gets or sets the current score.
        int Score() {
            return playerTurn_ == 1 ? player1_.Score() : player2_.Score();
        }
        void Score(int value) {
            if (playerTurn_ == 1)
                player1_.Score(value);
            else
                player2_.Score(value);
        }

        // Gets or sets the current level.
        int Level() {
            return playerTurn_ == 1 ? player1_.Level() : player2_.Level();
        }
        void Level(int value) {
            if (playerTurn_ == 1)
                player1_.Level(value);
            else
                player2_.Level(value);
        }

        // Gets the current maze.
        Maze & MazeRef() {
            return playerTurn_ == 1 ? player1_.MazeRef() : player2_.MazeRef();
        }
        Maze * MazePtr() {
            return playerTurn_ == 1 ? player1_.MazePtr() : player2_.MazePtr();
        }

        // Gets the current avatar.
        PlayerPuckman & PuckmanRef() {
            return playerTurn_ == 1 ? player1_.PuckmanRef() : player2_.PuckmanRef();
        }

        // Gets or sets the current number of extra lives.
        int ExtraLives() {
            return playerTurn_ == 1 ? player1_.ExtraLives() : player2_.ExtraLives();
        }
        void ExtraLives(int value) {
            if (playerTurn_ == 1)
                player1_.ExtraLives(value);
            else
                player2_.ExtraLives(value);
        }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void AddCoin();
        void AddFruit(FruitTypeEnum fruit);
        void CheckHighScore();
        void ChangeTurn();

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        GameData();
        GameData(const GameData & source);
        virtual ~GameData();

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The highest score in the game.
        int highScore_;

        // The inserted coins in the game.
        int coins_;

        // The data of the player 1.
        Player player1_;

        // The data of the player 2.
        Player player2_;

        // The number of players in the game.
        int numberOfPlayers_;

        // The current player turn.
        int playerTurn_;

        // The list of fruits of the last or current game.
        std::vector<FruitTypeEnum> lastFruits_;
    };
}

#endif
