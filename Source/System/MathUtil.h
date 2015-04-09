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

#ifndef __MATH_UTIL_HEADER__
#define __MATH_UTIL_HEADER__

#include <cmath>
#include <SFML/Graphics/Rect.hpp>

/**
 * This static class is a collection of math utility functions.
 */
class MathUtil {
private:
    MathUtil() {}
    ~MathUtil() {}

public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const sf::Vector2i V2I_ZERO;
    static const sf::Vector2u V2U_ZERO;
    static const sf::Vector2f V2F_ZERO;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    /**
     * Checks if a number is odd.
     */
    template<typename T>
    static bool IsOdd(T victim) {
        return victim % 2 == 1;
    }

    /**
     * Checks if a number is even.
     */
    template<typename T>
    static bool IsEven(T victim) {
        return victim % 2 == 0;
    }

    /**
     * Calculates the length of the vector.
     */
    template<typename T>
    static float Length(const sf::Vector2<T> & victim) {
        auto x = static_cast<float>(victim.x);
        auto y = static_cast<float>(victim.y);
        return std::sqrt(x * x + y * y);
    }

    /**
     * Creates a unit vector from the specified vector.
     */
    static void Normalize(sf::Vector2f & victim) {
        auto norm = Length(victim);
        victim.x /= norm;
        victim.y /= norm;
    }

    /**
     * Creates a unit vector from the specified vector.
     */
    static sf::Vector2f GetNormalized(const sf::Vector2f & victim) {
        auto norm = Length(victim);
        return sf::Vector2f(victim.x / norm, victim.y / norm);
    }

    /**
     * Restricts a value to be within a specified range.
     */
    template<typename T>
    static T Clamp(T value, T min, T max) {
        return (value < min) ? min : ((value > max) ? max : value);
    }

    /**
     * Gets the right side of a rectangle.
     */
    template<typename T>
    static T GetRight(const sf::Rect<T> & victim) {
        return victim.left + victim.width;
    }

    /**
     * Gets the bottom side of a rectangle.
     */
    template<typename T>
    static T GetBottom(const sf::Rect<T> & victim) {
        return victim.top + victim.height;
    }

    /**
     * Checks if a rectangle is inside another one.
     */
    template<typename T>
    static bool RectangleInside(const sf::Rect<T> & outRect, const sf::Rect<T> & inRect) {
        return (outRect.left < inRect.left) && (GetRight(inRect) < GetRight(outRect)) &&
               (outRect.top < inRect.top) && (GetBottom(inRect) < GetBottom(outRect));
    }

    /**
     * Checks if a point is inside a rectangle.
     */
    template<typename T>
    static bool PointInside(const sf::Vector2<T> & point, const sf::Rect<T> & area) {
        return (area.left <= point.x) && (point.x < area.left + area.width) &&
               (area.top <= point.y) && (point.y < area.top + area.height);
    }

    /**
     * Checks if a point is inside a rectangle.
     */
    template<typename T>
    static bool PointInside(T x, T y, T areaX1, T areaY1, T areaX2, T areaY2) {
        return (areaX1 <= x) && (x < areaX2) && (areaY1 <= y) && (y < areaY2);
    }

    /**
     * Checks if a rectangle collides with another one.
     */
    template<typename T>
    static bool RectangleCollides(T ax1, T ay1, T ax2, T ay2, T bx1, T by1, T bx2, T by2) {
        return PointInside(ax1, ay1, bx1, by1, bx2, by2) ||
               PointInside(ax2, ay1, bx1, by1, bx2, by2) ||
               PointInside(ax1, ay2, bx1, by1, bx2, by2) ||
               PointInside(ax2, ay2, bx1, by1, bx2, by2) ||
               PointInside(bx1, by1, ax1, ay1, ax2, ay2) ||
               PointInside(bx2, by1, ax1, ay1, ax2, ay2) ||
               PointInside(bx1, by2, ax1, ay1, ax2, ay2) ||
               PointInside(bx2, by2, ax1, ay1, ax2, ay2);
    }

    /**
     * Checks if a rectangle collides with another one.
     */
    template<typename T>
    static bool RectangleCollides(const sf::Vector2<T> & a1, const sf::Vector2<T> & a2,
        const sf::Vector2<T> & b1, const sf::Vector2<T> & b2) {
        return PointInside(a1.x, a1.y, b1.x, b1.y, b2.x, b2.y) ||
               PointInside(a2.x, a1.y, b1.x, b1.y, b2.x, b2.y) ||
               PointInside(a1.x, a2.y, b1.x, b1.y, b2.x, b2.y) ||
               PointInside(a2.x, a2.y, b1.x, b1.y, b2.x, b2.y) ||
               PointInside(b1.x, b1.y, a1.x, a1.y, a2.x, a2.y) ||
               PointInside(b2.x, b1.y, a1.x, a1.y, a2.x, a2.y) ||
               PointInside(b1.x, b2.y, a1.x, a1.y, a2.x, a2.y) ||
               PointInside(b2.x, b2.y, a1.x, a1.y, a2.x, a2.y);
    }
};

/**
 * This class represents a linear equation.
 */
template<typename T>
class LinearEquation {
public:
    /** Gets the X value with an Y value input. */
    T GetX(T y) { return ((y - x0_) * b_) / a_; }

    /** Gets the Y value with an X value input. */
    T GetY(T x) { return x0_ + (x * a_) / b_; }

    LinearEquation(T x1, T y1, T x2, T y2) {
        if (x1 <= x2) {
            initialize(x1, y1, x2, y2);
        } else {
            initialize(x2, y2, x1, y1);
        }
    }

    LinearEquation(const sf::Vector2<T> & p1, const sf::Vector2<T> & p2) {
        if (p1.x <= p2.x) {
            initialize(p1.x, p1.y, p2.x, p2.y);
        } else {
            initialize(p2.x, p2.y, p1.x, p1.y);
        }
    }

private:
    // y = (a/b) * x + x0
    // a = y2-y1, b = x2-x1
    T a_, b_, x0_;

    void initialize(T x1, T y1, T x2, T y2) {
        a_ = y2 - y1, b_ = x2 - x1;
        x0_ = y1 - (x1 * a_) / b_;
    }
};

#endif
