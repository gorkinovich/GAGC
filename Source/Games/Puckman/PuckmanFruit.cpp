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

#include "PuckmanFruit.h"
#include <Games/Puckman/PuckmanManager.h>

using namespace Puckman;

//********************************************************************************
// Properties
//********************************************************************************

/**
 * The value of the fruit.
 */
int Fruit::Value() {
    switch(type_) {
    case FruitType::Cherry:     return 100;
    case FruitType::Strawberry: return 300;
    case FruitType::Peach:      return 500;
    case FruitType::Apple:      return 700;
    case FruitType::Grape:      return 1000;
    case FruitType::Flower:     return 2000;
    case FruitType::Bell:       return 3000;
    case FruitType::Key:        return 5000;
    }
    return 0;
}

//--------------------------------------------------------------------------------

/**
 * The symbol of the fruit's value.
 */
FontSymbolEnum Fruit::Symbol() {
    switch(type_) {
    case FruitType::Cherry:     return FontSymbol::N100;
    case FruitType::Strawberry: return FontSymbol::N300;
    case FruitType::Peach:      return FontSymbol::N500;
    case FruitType::Apple:      return FontSymbol::N700;
    case FruitType::Grape:      return FontSymbol::N1000;
    case FruitType::Flower:     return FontSymbol::N2000;
    case FruitType::Bell:       return FontSymbol::N3000;
    case FruitType::Key:        return FontSymbol::N5000;
    }
    return FontSymbol::Namco;
}

//--------------------------------------------------------------------------------

/**
 * The sprite of the fruit.
 */
int Fruit::Sprite() {
    switch(type_) {
    case FruitType::Cherry:     return 1;
    case FruitType::Strawberry: return 2;
    case FruitType::Peach:      return 3;
    case FruitType::Apple:      return 5;
    case FruitType::Grape:      return 6;
    case FruitType::Flower:     return 7;
    case FruitType::Bell:       return 4;
    case FruitType::Key:        return 8;
    }
    return 0;
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Draws the entity inside the screen.
 */
void Fruit::Draw() {
    manager_->DrawSprite(X, Y, Sprite());
}

//--------------------------------------------------------------------------------

/**
 * Updates the inner state of the entity.
 */
void Fruit::Update(int time) {
}

//********************************************************************************
// Constructors, destructor and operators
//********************************************************************************

/**
 * Constructs a new object.
 */
Fruit::Fruit(FruitTypeEnum type) : Entity(), type_(type) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
Fruit::~Fruit() {}
