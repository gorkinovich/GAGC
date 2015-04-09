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

#ifndef __TEXT_CONSOLE_HEADER__
#define __TEXT_CONSOLE_HEADER__

#include <memory>
#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
    class Texture;
    class Sprite;
}

class BitmapFont;

/**
 * This singleton class represents a text console, used to simulate stuff like
 * the old text mode DOS games.
 */
class TextConsole {
public:
    //--------------------------------------------------------------------------------
    // Properties
    //--------------------------------------------------------------------------------

    const sf::Vector2i & Position() const { return position_; }
    void Position(const sf::Vector2i & value) { position_ = value; }

    int Columns() const { return columns_; }
    int Rows() const { return rows_; }

    int Width() const { return width_; }
    int Height() const { return height_; }

    bool Opaque() const { return opaque_; }
    void Opaque(bool value) { opaque_ = value; }

    const sf::Color & ClearColor() const { return clearColor_; }
    void ClearColor(const sf::Color & value) { clearColor_ = value; }

    const sf::Vector2i & CursorPosition() const { return cursorPosition_; }
    void CursorPosition(const sf::Vector2i & value);

    const sf::Color & ForegroundColor() const { return foregroundColor_; }
    void ForegroundColor(const sf::Color & value) { foregroundColor_ = value; }

    const sf::Color & BackgroundColor() const { return backgroundColor_; }
    void BackgroundColor(const sf::Color & value) { backgroundColor_ = value; }

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void SetCurrentFont(BitmapFont * value);

    void ChangeSize(int columns, int rows, bool clear = true);

    void ClearScreen();
    void ClearScreen(const sf::Color & clearColor);

    void ClearLine(int row);
    void ClearLine(int row, const sf::Color & clearColor);

    void DirectWrite(int row, int col, const std::string & text);

    void ResetCursorPosition();
    void NewLine();

    void Write(const std::string & text);
    void WriteLine(const std::string & text);

    void Draw();

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static TextConsole * Instance();
    static TextConsole & Reference();
    ~TextConsole();

private:
    //--------------------------------------------------------------------------------
    // Types
    //--------------------------------------------------------------------------------

    class BufferCell {
    public:
        char CharData;
        sf::Color ForegroundColor;
        sf::Color BackgroundColor;
        BufferCell(char cd = '\0') : CharData(cd), ForegroundColor(sf::Color::White),
            BackgroundColor(sf::Color::Black) {}
    };

    typedef std::vector<BufferCell> BufferCellVector;
    typedef std::unique_ptr<sf::Texture> UniqueTexture;
    typedef std::unique_ptr<sf::Sprite> UniqueSprite;

    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    sf::Vector2i position_;         // The start coordinates in the screen.
    int columns_;                   // The number of columns.
    int rows_;                      // The number of rows.
    int width_;                     // The width.
    int height_;                    // The height.
    bool opaque_;                   // The background opaque flag.
    sf::Color clearColor_;          // The clear color.
    BitmapFont * currentFont_;      // The font.
    BufferCellVector textBuffer_;   // The text buffer.
    UniqueTexture emptyTexture_;    // The empty texture used to clear the background.
    UniqueSprite emptySprite_;      // The sprite of the empty texture.
    sf::Vector2i cursorPosition_;   // The position of the cursor.
    sf::Color foregroundColor_;     // The foreground color of the text.
    sf::Color backgroundColor_;     // The background color of the text.

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void createTextBuffer(int columns, int rows);
    void writeCell(int i, int j, char item);

    int fromCoordsToIndex(int i, int j) { return i * columns_ + j; }

    //--------------------------------------------------------------------------------
    // Singleton pattern
    //--------------------------------------------------------------------------------

    static TextConsole * instance_;
    TextConsole();
};

#endif
