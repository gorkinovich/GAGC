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

#ifndef __CHECKERS_DIALOG_STATE_HEADER__
#define __CHECKERS_DIALOG_STATE_HEADER__

#include <SFML/Graphics/Rect.hpp>
#include <System/AbstractState.h>

class Texture2D;
class SimpleLabel;
class TexturedButton;
class CoreManager;
class CheckersManager;

/**
 * This class represents the checkers exit state of the game.
 */
class CheckersDialogState : public AbstractState {
public:
    //--------------------------------------------------------------------------------
    // Constants
    //--------------------------------------------------------------------------------

    static const int EXIT_DLG           = 0;
    static const int EXIT_GAME_DLG      = 1;
    static const int LOAD_FROM_MENU_DLG = 2;
    static const int LOAD_FROM_GAME_DLG = 3;
    static const int RESET_DLG          = 4;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    virtual void Initialize();
    virtual void Release();
    virtual void Draw(const sf::Time & timeDelta);
    virtual void Update(const sf::Time & timeDelta);

    //--------------------------------------------------------------------------------
    // Constructors and destructor
    //--------------------------------------------------------------------------------

    CheckersDialogState(int dialogType);
    virtual ~CheckersDialogState();

    static SharedState MakeExitDialog() {
        return MakeSharedState<CheckersDialogState>(CheckersDialogState::EXIT_DLG);
    }

    static SharedState MakeExitGameDialog() {
        return MakeSharedState<CheckersDialogState>(CheckersDialogState::EXIT_GAME_DLG);
    }

    static SharedState MakeLoadFromMenuDialog() {
        return MakeSharedState<CheckersDialogState>(CheckersDialogState::LOAD_FROM_MENU_DLG);
    }

    static SharedState MakeLoadFromGameDialog() {
        return MakeSharedState<CheckersDialogState>(CheckersDialogState::LOAD_FROM_GAME_DLG);
    }

    static SharedState MakeResetDialog() {
        return MakeSharedState<CheckersDialogState>(CheckersDialogState::RESET_DLG);
    }

private:
    //--------------------------------------------------------------------------------
    // Fields
    //--------------------------------------------------------------------------------

    int dialogType_;
    CoreManager * core_;
    CheckersManager * manager_;

    sf::Vector2i titlePosition_;
    std::unique_ptr<Texture2D> titleTexture_;

    std::unique_ptr<SimpleLabel> questionLabel_;
    std::unique_ptr<SimpleLabel> yesLabel_;
    std::unique_ptr<SimpleLabel> noLabel_;

    std::unique_ptr<TexturedButton> yesButton_;
    std::unique_ptr<TexturedButton> noButton_;

    //--------------------------------------------------------------------------------
    // Methods
    //--------------------------------------------------------------------------------

    void executeYes();
    void executeNo();
};

#endif
