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

#ifndef __TIMER_HEADER__
#define __TIMER_HEADER__

#include <functional>

namespace sf {
    class Time;
}

/**
 * This class represents a timer inside the game.
 */
class Timer {
public:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    typedef std::function<void(Timer &)> ActionEvent;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool Enable() const { return enable_; }
    void Enable(bool value) { enable_ = value; }

    unsigned int Interval() const { return interval_; }
    void Interval(unsigned int value) { interval_ = value; }

    ActionEvent Action() const { return action_; }
    void Action(ActionEvent value) { action_ = value; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void Update(const sf::Time & timeDelta);
    void Update(unsigned int elapsedMilliseconds);
    void ResetTime();
    void SetAndEnable(unsigned int interval, ActionEvent action, bool resetTime = true);

    //--------------------------------------------------------------------------------
    // Constructors, destructor and Operators
    //--------------------------------------------------------------------------------

    Timer();
    Timer(const Timer & source);
    virtual ~Timer();
    Timer & operator =(const Timer & source);

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    bool enable_;               // The is enable flag of the timer.
    unsigned int currentTime_;  // The current accumulated time of the timer.
    unsigned int interval_;     // The interval of the timer.
    ActionEvent action_;        // The timer action method.
};

#endif
