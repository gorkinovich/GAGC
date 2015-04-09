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

#ifndef __PUCKMAN_MAZE_GRAPH_HEADER__
#define __PUCKMAN_MAZE_GRAPH_HEADER__

#include <vector>
#include <Games/Puckman/PuckmanEnums.h>
#include <Games/Puckman/PuckmanMaze.h>

class CoreManager;

namespace Puckman {
    class MovingEntity;
    class PuckmanEntity;
    class Ghost;
    /**
     * This class represents the maze graph.
     */
    class MazeGraph {
    public:
        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        typedef std::vector<MovingDirectionEnum> MovingDirections;

        /** This structure represents a node in the graph. */
        struct Node {
            /** The row of the node. */
            int Row;
            /** The column of the node. */
            int Col;
            /** The x coordinate of the intersection. */
            int X;
            /** The y coordinate of the intersection. */
            int Y;
            /** The list of directions of the node. */
            MovingDirections Dirs;

            /** Constructs a new structure. */
            Node(int row = 0, int col = 0);
            /** Checks if the node is valid or not. */
            bool IsValid() const;
        };

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        MovingDirections GetDirections(int row, int col);
        MovingDirections GetDirections(const sf::Vector2i & orig,
            const sf::Vector2i & dest);
        sf::Vector2i GetNextPoint(const sf::Vector2i & orig, MovingDirectionEnum dir);
        MovingDirectionEnum GetOppositeDirection(MovingDirectionEnum dir);
        MovingDirectionEnum GetDirection(MovingEntity & entity, const sf::Vector2i & dest);
        MovingDirectionEnum GetEscapeDirection(Ghost & ghost, PuckmanEntity & puckman);
        MovingDirectionEnum GetRandomDirection(MovingEntity & entity);
        const Node & GetNode(MovingEntity & entity);
        const Node & GetNode(int row, int col);
        bool IsOnIntersection(MovingEntity & entity);
        bool IsOnIntersection(int row, int col);
        bool IsNearToIntersection(MovingEntity & entity);
        bool IsNearToIntersection(int row, int col);

        //--------------------------------------------------------------------------------
        // Constructors, destructor and operators
        //--------------------------------------------------------------------------------

        MazeGraph();
        MazeGraph(const MazeGraph & source);
        virtual ~MazeGraph();
        MazeGraph & operator =(const MazeGraph & source);

    private:
        //--------------------------------------------------------------------------------
        // Constants
        //--------------------------------------------------------------------------------

        // This is the number of rows of the graph.
        static const int ROWS = 10;

        // This is the number of columns in each row of the graph.
        static const int COLS[];

        // This is the not found node.
        static const Node NOT_FOUND;

        //--------------------------------------------------------------------------------
        // Types
        //--------------------------------------------------------------------------------

        /** This structure represents a row node. */
        struct RowNode {
            /** The row of the node. */
            int Row;
            /** The y coordinate of the intersection. */
            int Y;
            /** The list of columns of the node. */
            std::vector<Node> Cols;

            /** Constructs a new structure. */
            RowNode() : Row(0), Y(0), Cols() {}
        };

        //--------------------------------------------------------------------------------
        // Fields
        //--------------------------------------------------------------------------------

        // The list of rows in the graph.
        std::vector<RowNode> rows_;

        // The map of the maze.
        bool map_[Maze::ROWS][Maze::COLUMS];

        // Used to call the random number generator.
        CoreManager * core_;

        //--------------------------------------------------------------------------------
        // Methods
        //--------------------------------------------------------------------------------

        void resetMap();
        void setMapValue(int row, int col, bool value);
        bool getMapValue(int row, int col);
        void lockCell(const sf::Vector2i & cell);
        void unlockCell(const sf::Vector2i & cell);
        int getDistance(const sf::Vector2i & orig, const sf::Vector2i & dest);
        bool findDirectPath(const sf::Vector2i & orig, const sf::Vector2i & dest,
            MovingDirectionEnum lastDir, int max);
        MovingDirections eraseDirection(const MovingDirections & dirs,
            MovingDirectionEnum dir);
        MovingDirectionEnum getRandomDirection(MovingEntity & entity,
            const MovingDirections & dirs);
        MovingDirectionEnum getRandomDirection(const MovingDirections & dirs);
        MovingDirectionEnum checkDirectionInList(const MovingDirections & dirs,
            MovingDirectionEnum dir);
        MovingDirectionEnum getDirectDirection(const sf::Vector2i & orig,
            const sf::Vector2i & dest, const MovingDirections & dirs);
    };
}

#endif
