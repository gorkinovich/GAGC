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

#ifndef __RETRO_START_STATE_HEADER__
#define __RETRO_START_STATE_HEADER__

#include <string>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>
#include <System/Texture2D.h>
#include <System/Timer.h>

class TextConsole;

/**
 * This class represents the retro start state of the game.
 */
class RetroStartState : public AbstractState {
public:
    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    virtual void Initialize();
    virtual void Release();
    virtual void Draw(const sf::Time & timeDelta);
    virtual void Update(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    RetroStartState();
    virtual ~RetroStartState();

private:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int BGR_POS_X;
    static const int BGR_POS_Y;
    static const int BGR_WIDTH;
    static const int BGR_HEIGHT;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct ActionStepData {
        int Row;
        int Column;
        unsigned int NextInterval;
        std::string TextLine;
        ActionStepData() : Row(0), Column(0), NextInterval(0), TextLine() {}
        ActionStepData(int r, int c, unsigned int ni, const std::string & tl)
            : Row(r), Column(c), NextInterval(ni), TextLine(tl) {}
    };

    typedef std::vector<ActionStepData> ActionStepVector;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    TextConsole * console_;         // The text console.
    Timer timer_;                   // The main timer.
    Texture2D background_;          // The text console background.
    unsigned int step_;             // The current step.
    ActionStepVector actionsData_;  // The action steps data.
};

#endif
