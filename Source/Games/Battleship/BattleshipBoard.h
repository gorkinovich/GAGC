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

#ifndef __BATTLESHIP_BOARD_HEADER__
#define __BATTLESHIP_BOARD_HEADER__

#include <memory>
#include <functional>

/**
 * This class represents a battleship board in the game.
 */
class BattleshipBoard {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int SHIP_TYPE1 = 0;
    static const int SHIP_TYPE2 = 1;
    static const int SHIP_TYPE3 = 2;
    static const int SHIP_TYPE4 = 3;

    static const int MAX_SIZE = 10;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    typedef int FleetBoard[MAX_SIZE][MAX_SIZE];
    typedef bool EmptyBoard[MAX_SIZE][MAX_SIZE];
    typedef std::function<void ()> Action;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Initialize();
    void Draw();
    void Update();
    void Click();

    void SetModeAIPutFleet();
    void SetModePutFleet(Action onUpdate, Action onFinish);
    void SetModePlayer();
    void SetModeEnemy(Action onUpdate);

    void Reset();
    void ResetCount();

    void SetBoardPosition(int x, int y);
    bool MouseOverBoard();

    void NextShipDirection();
    void ChangeShipType(int type);
    int ShipTypeCount(int type);
    bool AnyShipAvailable();
    void SetRandomFleet();

    void MachineFires();

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    BattleshipBoard();
    BattleshipBoard(const BattleshipBoard & source);
    virtual ~BattleshipBoard();
    BattleshipBoard & operator =(const BattleshipBoard & source);

private:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct InnerData;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    std::shared_ptr<InnerData> data_;
    static Action emptyAction_;
};

#endif
