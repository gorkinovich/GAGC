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

#include "TextConsole.h"
#include <SFML/Graphics.hpp>
#include <System/CoreManager.h>
#include <System/BitmapFont.h>
#include <System/MathUtil.h>

//********************************************************************************
// Properties
//********************************************************************************

void TextConsole::CursorPosition(const sf::Vector2i & value) {
    cursorPosition_ = value;
    cursorPosition_.x = MathUtil::Clamp(cursorPosition_.x, 0, columns_ - 1);
    cursorPosition_.y = MathUtil::Clamp(cursorPosition_.y, 0, rows_ - 1);
}

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Sets the font of the text console.
 */
void TextConsole::SetCurrentFont(BitmapFont * value) {
    if (value != nullptr) {
        currentFont_ = value;
    }
}

//--------------------------------------------------------------------------------

/**
 * Creates a new text buffer.
 */
void TextConsole::createTextBuffer(int columns, int rows) {
    columns_ = columns;
    rows_ = rows;
    textBuffer_.resize(rows_ * columns_);
    if (currentFont_ != nullptr) {
        width_ = columns_ * currentFont_->Width();
        height_ = rows_ * currentFont_->Height();
    } else {
        width_ = 0;
        height_ = 0;
    }
}

//--------------------------------------------------------------------------------

/**
 * Changes the size of the text buffer.
 */
void TextConsole::ChangeSize(int columns, int rows, bool clear) {
    if (clear || textBuffer_.size() <= 0) {
        createTextBuffer(columns, rows);
        ClearScreen();

    } else {
        int oldColumns = columns_;
        int oldRows = rows_;
        std::vector<BufferCell> oldTextBuffer = textBuffer_;

        createTextBuffer(columns, rows);
        ClearScreen();

        for (int i = 0; i < rows_ && i < oldRows; ++i) {
            for (int j = 0; j < columns_ && j < oldColumns; ++j) {
                textBuffer_[fromCoordsToIndex(i, j)] = oldTextBuffer[i * oldColumns + j];
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Clears the text buffer with the current clear color.
 */
void TextConsole::ClearScreen() {
    ClearScreen(clearColor_);
}

//--------------------------------------------------------------------------------

/**
 * Clears the text buffer with a clear color.
 */
void TextConsole::ClearScreen(const sf::Color & clearColor) {
    for (int i = 0, len = textBuffer_.size(); i < len; ++i) {
        BufferCell & cell = textBuffer_[i];
        cell.CharData = '\0';
        cell.BackgroundColor = clearColor;
    }
}

//--------------------------------------------------------------------------------

/**
 * Clears a line inside the text buffer with the current clear color.
 */
void TextConsole::ClearLine(int row) {
    ClearLine(row, clearColor_);
}

//--------------------------------------------------------------------------------

/**
 * Clears a line inside the text buffer with a clear color.
 */
void TextConsole::ClearLine(int row, const sf::Color & clearColor) {
    if (0 <= row && row < rows_) {
        for (int j = 0; j < columns_; ++j) {
            BufferCell & cell = textBuffer_[fromCoordsToIndex(row, j)];
            cell.CharData = '\0';
            cell.BackgroundColor = clearColor;
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Writes a value inside a cell of the text buffer.
 */
void TextConsole::writeCell(int i, int j, char item) {
    BufferCell & cell = textBuffer_[fromCoordsToIndex(i, j)];
    cell.CharData = item;
    cell.ForegroundColor = foregroundColor_;
    cell.BackgroundColor = backgroundColor_;
}

//--------------------------------------------------------------------------------

/**
 * Writes an string inside the text buffer.
 */
void TextConsole::DirectWrite(int row, int col, const std::string & text) {
    if (textBuffer_.size() <= 0) return;
    if (0 <= row && row < rows_ && 0 <= col && col < columns_) {
        int j = col;
        auto i = text.begin();
        auto end = text.end();
        for (; i != end; ++i) {
            char item = *i;
            if (item == '\n') {
                break;
            } else {
                writeCell(row, j++, item);
                if (j >= columns_) break;
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Resets the current position of the cursor.
 */
void TextConsole::ResetCursorPosition() {
    cursorPosition_.x = 0;
    cursorPosition_.y = 0;
}

//--------------------------------------------------------------------------------

/**
 * Adds a new line to the current cursor inside the text buffer.
 */
void TextConsole::NewLine() {
    cursorPosition_.x = 0;
    int nextY = cursorPosition_.y + 1;
    if (nextY >= rows_) {
        int finalRow = rows_ - 1;
        for (int i = 0; i < finalRow; ++i) {
            for (int j = 0; j < columns_; ++j) {
                textBuffer_[fromCoordsToIndex(i, j)] = textBuffer_[fromCoordsToIndex(i + 1, j)];
            }
        }
        for (int j = 0; j < columns_; ++j) {
            BufferCell & cell = textBuffer_[fromCoordsToIndex(finalRow, j)];
            cell.CharData = '\0';
            cell.BackgroundColor = clearColor_;
        }
    } else {
        cursorPosition_.y = nextY;
    }
}

//--------------------------------------------------------------------------------

/**
 * Writes an string inside the text buffer.
 */
void TextConsole::Write(const std::string & text) {
    if (textBuffer_.size() <= 0) return;
    auto i = text.begin();
    auto end = text.end();
    for (; i != end; ++i) {
        char item = *i;
        if (item == '\n') {
            NewLine();
        } else {
            writeCell(cursorPosition_.y, cursorPosition_.x, item);
            ++(cursorPosition_.x);
            if (cursorPosition_.x >= columns_) {
                NewLine();
            }
        }
    }
}

//--------------------------------------------------------------------------------

/**
 * Writes an string inside the text buffer.
 */
void TextConsole::WriteLine(const std::string & text) {
    Write(text);
    NewLine();
}

//--------------------------------------------------------------------------------

/**
 * Draws the current text buffer in the screen.
 */
void TextConsole::Draw() {
    if (currentFont_ != nullptr) {
        int width = currentFont_->Width();
        int height = currentFont_->Height();
        CoreManager & core = CoreManager::Reference();
        emptySprite_->setScale((float)width, (float)height);
        sf::Vector2i dest(position_.x, position_.y);
        for (int i = 0; i < rows_; ++i) {
            dest.x = position_.x;
            for (int j = 0; j < columns_; ++j) {
                BufferCell & cell = textBuffer_[fromCoordsToIndex(i, j)];
                if (opaque_) {
                    emptySprite_->setColor(cell.BackgroundColor);
                    emptySprite_->setPosition((float)dest.x, (float)dest.y);
                    core.Draw(*emptySprite_);
                }
                currentFont_->Draw(cell.CharData, dest.x, dest.y, cell.ForegroundColor);
                dest.x += width;
            }
            dest.y += height;
        }
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
TextConsole * TextConsole::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
TextConsole::TextConsole() : position_(0, 0), columns_(0), rows_(0),
    width_(0), height_(0), opaque_(true), clearColor_(sf::Color::Black),
    currentFont_(nullptr), textBuffer_(), cursorPosition_(0, 0),
    foregroundColor_(sf::Color::White), backgroundColor_(sf::Color::Black) {
    // Create the empty texture from the memory.
    sf::Image image;
    image.create(1, 1, sf::Color::White);
    emptyTexture_.reset(new sf::Texture());
    if (emptyTexture_->loadFromImage(image)) {
        // Create the sprite to draw the empty texture.
        emptySprite_.reset(new sf::Sprite(*emptyTexture_));
    } else {
        emptyTexture_.reset(nullptr);
    }
}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
TextConsole::~TextConsole() {
    textBuffer_.clear();
    emptySprite_.reset(nullptr);
    emptyTexture_.reset(nullptr);
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
TextConsole * TextConsole::Instance() {
    if (!instance_) {
        instance_ = new TextConsole();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
TextConsole & TextConsole::Reference() {
    return *(Instance());
}
