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

#ifndef __PUCKMAN_FRUIT_HEADER__
#define __PUCKMAN_FRUIT_HEADER__

#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanEntity.h>

namespace Puckman {
    /**
     * This class represents a Fruit entity.
     */
    class Fruit : public Entity {
    public:
        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        FruitTypeEnum Type() { return type_; }
        void Type(FruitTypeEnum value) { type_ = value; }

        int Value();
        FontSymbolEnum Symbol();
        int Sprite();

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        virtual void Draw();
        virtual void Update(int time);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        Fruit(FruitTypeEnum type = FruitType::Cherry);
        virtual ~Fruit();

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        FruitTypeEnum type_; // The type of the fruit.
    };
}

#endif
