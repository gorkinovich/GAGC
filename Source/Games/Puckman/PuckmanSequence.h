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

#ifndef __PUCKMAN_SEQUENCE_HEADER__
#define __PUCKMAN_SEQUENCE_HEADER__

#define ARRAY_TO_NODES(x) std::vector<Sequence::Node>(std::begin(x), std::end(x))

#include <vector>
#include <Games/Puckman/PuckmanEnums.h>

namespace Puckman {
    class MovingEntity;
    class AnimatedPuckman;
    class Ghost;

    /**
     * This class is used to make a sequence using entities.
     */
    class Sequence {
    public:
        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        // This struct is used in the sequence array.
        struct Node {
            // The row.
            int Row;

            // The column.
            int Column;

            // The direction.
            MovingDirectionEnum Direction;

            // Tells to the program if the coordinates are in pixel values.
            bool PixelCoords;

            // Constructs a new Node object.
            Node(int row, int col, MovingDirectionEnum dir, bool pixelCoords = false)
                : Row(row), Column(col), Direction(dir), PixelCoords(pixelCoords) {}
        };

        //--------------------------------------------------------------------------------
        // Properties
        //--------------------------------------------------------------------------------

        const std::vector<Node> & Data() const { return data_; }

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void Reset();
        void Reset(const std::vector<Node> & data);
        bool IsEnded();
        void Check(Ghost & entity);
        void Check(AnimatedPuckman & entity);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        Sequence();
        Sequence(const std::vector<Node> & data);
        Sequence(const Sequence & source);
        virtual ~Sequence();
        Sequence & operator =(const Sequence & source);

    private:
        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The sequence of directions for the entities.
        std::vector<Node> data_;

        // The current node in the sequence.
        int currentNode_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        bool checkCoordinates(MovingEntity & entity);
        void nextNode();
    };
}

#endif
