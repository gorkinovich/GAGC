/******************************************************************************
 Copyright (c) 2014 Gorka Su�rez Garc�a

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

#ifndef __REVERSI_SAVE_STATE_HEADER__
#define __REVERSI_SAVE_STATE_HEADER__

#include <vector>
#include <System/AbstractState.h>

class SimpleLabel;
class TexturedButton;

/**
 * This class represents the reversi save state of the game.
 */
class ReversiSaveState : public AbstractState {
public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    virtual void Initialize();
    virtual void Release();
    virtual void Draw(const sf::Time & timeDelta);
    virtual void Update(const sf::Time & timeDelta);

    static int LastSelecteIndex() { return lastSelecteIndex_; }

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    ReversiSaveState();
    virtual ~ReversiSaveState();

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    static int lastSelecteIndex_;

    std::unique_ptr<SimpleLabel> titleLabel_;
    std::unique_ptr<SimpleLabel> exitLabel_;

    std::vector<std::unique_ptr<SimpleLabel>> savesLabels_;
    std::vector<std::unique_ptr<TexturedButton>> savesButtons_;
};

#endif
