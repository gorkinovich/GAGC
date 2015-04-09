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

#ifndef __CHESS_GAME_DATA_HEADER__
#define __CHESS_GAME_DATA_HEADER__

#include <vector>
#include <functional>
#include <SFML/Graphics/Rect.hpp>

/**
 * This class represents the chess board game data.
 */
class ChessGameData {
public:
    friend class SaveManager;
    friend class ChessMiniMax;

    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int BOARD_SIZE = 8;

    static const int CELL_WIDTH  = 40;
    static const int CELL_HEIGHT = 40;

    static const int EASY_LEVEL   = 0;
    static const int NORMAL_LEVEL = 1;
    static const int HARD_LEVEL   = 2;

    static const int NO_WINNER = 0;
    static const int P1_WINNER = 1;
    static const int P2_WINNER = 2;
    static const int AI_WINNER = 3;
    static const int GAME_DRAW = 4;

    static const int WHITE_SIDE =  0;
    static const int BLACK_SIDE =  1;
    static const int MAX_SIDES  =  2;
    static const int BOTH_SIDES = -1;

    static const int DEAD_PIECE   = -1;
    static const int PAWN_PIECE   =  0;
    static const int ROOK_PIECE   =  1;
    static const int KNIGHT_PIECE =  2;
    static const int BISHOP_PIECE =  3;
    static const int QUEEN_PIECE  =  4;
    static const int KING_PIECE   =  5;
    static const int PIECE_TYPES  =  6;

    static const int PIECE_NOT_FOUND    = -1;
    static const int MAX_PIECES         = 32;
    static const int MAX_PIECES_SIDE    = 16;
    static const int WHITE_PIECES_START =  0;
    static const int WHITE_PIECES_END   = 16;
    static const int BLACK_PIECES_START = 16;
    static const int BLACK_PIECES_END   = 32;

    static const sf::Vector2i NO_CELL;
    static const int MAX_OFFSETS[PIECE_TYPES];

    static const int MAX_PAWN_OFFSETS   =  4;
    static const int MAX_ROOK_OFFSETS   = 28;
    static const int MAX_KNIGHT_OFFSETS =  8;
    static const int MAX_BISHOP_OFFSETS = 28;
    static const int MAX_QUEEN_OFFSETS  = 56;
    static const int MAX_KING_OFFSETS   = 10;

    static const sf::Vector2i WHITE_PAWN_OFFSETS[MAX_PAWN_OFFSETS];
    static const sf::Vector2i BLACK_PAWN_OFFSETS[MAX_PAWN_OFFSETS];
    static const sf::Vector2i ROOK_OFFSETS[MAX_ROOK_OFFSETS];
    static const sf::Vector2i KNIGHT_OFFSETS[MAX_KNIGHT_OFFSETS];
    static const sf::Vector2i BISHOP_OFFSETS[MAX_BISHOP_OFFSETS];
    static const sf::Vector2i QUEEN_OFFSETS[MAX_QUEEN_OFFSETS];
    static const sf::Vector2i KING_OFFSETS[MAX_KING_OFFSETS];

    static const int MARK_NONE          = 0;
    static const int MARK_PAWN_ATTACK   = 1;
    static const int MARK_PAWN_VICTIM   = 2;
    static const int MARK_CANT_CASTLING = 3;

    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    struct Piece {
        int side, type, mark;
        sf::Vector2i position;
        Piece() : side(WHITE_SIDE), type(DEAD_PIECE), mark(MARK_NONE),
            position(NO_CELL) {}
        Piece(int s, int t, const sf::Vector2i & p) : side(s), type(t),
            mark(MARK_NONE), position(p) {}
        bool Dead() const { return type == DEAD_PIECE; }
        bool NotDead() const { return type != DEAD_PIECE; }
    };

    typedef Piece BoardPieces[MAX_PIECES];
    typedef std::vector<sf::Vector2i> CoordsVector;

    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    bool SinglePlayer() const { return singlePlayer_; }
    int Difficulty() const { return difficulty_; }
    int PlayerSide() const { return playerSide_; }
    int Winner() const { return winner_; }
    int Turn() const { return turn_; }
    bool WhiteCheck() const { return whiteCheck_; }
    bool BlackCheck() const { return blackCheck_; }
    const BoardPieces & Pieces() const { return pieces_; }

    bool GameOver() const { return winner_ != NO_WINNER; }

    //--------------------------------------------------------------------------------
    // General Methods
    //--------------------------------------------------------------------------------

    template<class TFunc>
    void ForEachInPieces(TFunc operation) {
        for (int i = 0; i < MAX_PIECES; ++i) {
            operation(pieces_[i], i);
        }
    }

    static void ShowErrorMessages(bool value) {
        showErrorMessages_ = value;
    }

    void Start(bool singlePlayer, int difficulty, int playerSide);
    void Reset();

    void SetWinner(int side);
    void NextTurn();

    void ConvertPawnToRook();
    void ConvertPawnToKnight();
    void ConvertPawnToBishop();
    void ConvertPawnToQueen();

    bool MakeMove(const sf::Vector2i & orig, const sf::Vector2i & dest);
    bool MakeMove(int r1, int c1, int r2, int c2) {
        return MakeMove(sf::Vector2i(c1, r1), sf::Vector2i(c2, r2));
    }

    //--------------------------------------------------------------------------------
    // Query Methods
    //--------------------------------------------------------------------------------

    bool IsInside(int r, int c) const;
    bool IsInside(const sf::Vector2i & c) const { return IsInside(c.y, c.x); }

    bool AnyPawnToConvert() const;
    void GetCandidates(CoordsVector & victims) const;
    void GetPossibleMoves(CoordsVector & victims, const sf::Vector2i & coords) const;

    //--------------------------------------------------------------------------------
    // Constructors, destructor and operators
    //--------------------------------------------------------------------------------

    ChessGameData();
    ChessGameData(const ChessGameData & source);
    ~ChessGameData();
    ChessGameData & operator =(const ChessGameData & source);

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    bool singlePlayer_;
    int difficulty_;
    int playerSide_;
    int winner_;

    int turn_;
    bool whiteCheck_;
    bool blackCheck_;
    BoardPieces pieces_;

    static bool showErrorMessages_;

    //--------------------------------------------------------------------------------
    // General Methods
    //--------------------------------------------------------------------------------

    const sf::Vector2i * getMoveOffsets(int side, int type) const;
    const sf::Vector2i * getMoveOffsets(const Piece & piece) const {
        return getMoveOffsets(piece.side, piece.type);
    }

    int getPiece(int r, int c) const;
    int getPiece(const sf::Vector2i & coords) const;
    int getPieceByType(int side, int type) const;

    void convertPawnTo(int type);

    bool makeFutureMove(int side, const sf::Vector2i & orig,
        const sf::Vector2i & dest) const;

    bool isCheck() const;
    bool isCheckmate() const;

    void checkAndMarkPawn(Piece & current, int enemySide, int r, int c);
    bool collision(const sf::Vector2i & orig, const sf::Vector2i & dest);

    bool makeWhitePawnMove(Piece & current, const sf::Vector2i & dest);
    bool makeBlackPawnMove(Piece & current, const sf::Vector2i & dest);
    bool makePawnMove(Piece & current, const sf::Vector2i & dest);

    bool makeGenericMove(Piece & current, const sf::Vector2i & dest);
    bool makeRookMove(Piece & current, const sf::Vector2i & dest);
    bool makeKnightMove(Piece & current, const sf::Vector2i & dest);
    bool makeBishopMove(Piece & current, const sf::Vector2i & dest);
    bool makeQueenMove(Piece & current, const sf::Vector2i & dest);

    bool makeKingCastling(Piece & current, const sf::Vector2i & dest);
    bool makeKingMove(Piece & current, const sf::Vector2i & dest);

    void clearAllErrorMessages() const;
    void clearErrorMessage(int error) const;
    void setErrorMessage(int error) const;
};

#endif
