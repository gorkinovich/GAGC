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

#ifndef __ABSTRACT_STATE_HEADER__
#define __ABSTRACT_STATE_HEADER__

#include <memory>

namespace sf {
    class Time;
}

class CoreManager;

/**
 * This class represents a generic state.
 */
class AbstractState {
public:
    virtual void Initialize();
    virtual void Release();
    virtual void Draw(const sf::Time & timeDelta);
    virtual void Update(const sf::Time & timeDelta);
    virtual ~AbstractState();
protected:
    CoreManager & core_;
    AbstractState();
};

typedef std::shared_ptr<AbstractState> SharedState;

template<class T>
SharedState MakeSharedState() {
    return SharedState(new T());
}

template<class T, class TP1>
SharedState MakeSharedState(TP1 p1) {
    return SharedState(new T(p1));
}

template<class T, class TP1, class TP2>
SharedState MakeSharedState(TP1 p1, TP2 p2) {
    return SharedState(new T(p1, p2));
}

template<class T, class TP1, class TP2, class TP3>
SharedState MakeSharedState(TP1 p1, TP2 p2, TP3 p3) {
    return SharedState(new T(p1, p2, p3));
}

template<class T, class TP1, class TP2, class TP3, class TP4>
SharedState MakeSharedState(TP1 p1, TP2 p2, TP3 p3, TP4 p4) {
    return SharedState(new T(p1, p2, p3, p4));
}

#endif
