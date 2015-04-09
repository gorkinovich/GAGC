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

#ifndef __PUCKMAN_DEMO_LOGIC_HEADER__
#define __PUCKMAN_DEMO_LOGIC_HEADER__

#include <Games/Puckman/PuckmanGenericLogic.h>

namespace Puckman {
    class SequencedPuckman;

    /**
     * This class represents a demo logic.
     */
    class DemoLogic : public GenericLogic {
    public:
        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Initialize();
        void Release();
        void Draw();
        void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        DemoLogic();
        virtual ~DemoLogic();

    private:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // The amount of time when the demo begins.
        static const int BEGIN_DELAY_TIME = 1800;

        // The amount of time when the puckman is killed.
        static const int KILL_DELAY_TIME = 1000;

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // This is the puckman used in the demo.
        std::shared_ptr<SequencedPuckman> puckman_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void drawDemoMaze();
        void puckmanUpdate();
        virtual void killPuckman();
        virtual void checkFruitCollision(PuckmanEntity & puckman);
    };
}

#endif
