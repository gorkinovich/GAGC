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

#include "MenuManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <System/CoreManager.h>
#include <System/MusicManager.h>

//********************************************************************************
// Methods
//********************************************************************************

/**
 * Initializes the data of the object.
 */
void MenuManager::Initialize() {
    auto * core = CoreManager::Instance();
    if (tileset_ == nullptr) {
        tileset_ = core->LoadTexture("Content/Textures/Menu.png");
    }
    currentSong_ = -1;
}

//--------------------------------------------------------------------------------

/**
 * Releases the data of the object.
 */
void MenuManager::Release() {
    tileset_ = nullptr;
}

//--------------------------------------------------------------------------------

void MenuManager::LoadGeneralPlaylist() {
    std::vector<std::string> playlist;
    playlist.push_back("Content/Music/GymnopedieNo1.ogg");
    playlist.push_back("Content/Music/PreludeInC-BWV846.ogg");
    playlist.push_back("Content/Music/GymnopedieNo2.ogg");
    playlist.push_back("Content/Music/ColorlessAura.ogg");
    playlist.push_back("Content/Music/GymnopedieNo3.ogg");
    playlist.push_back("Content/Music/DespairAndTriumph.ogg");
    auto * music = MusicManager::Instance();
    if (!music->IsSamePlaylist(playlist)) {
        music->Volume(80);
        music->ClearPlaylist();
        music->SetPlaylist(playlist);
        music->SlowStop();
        music->CurrentSong(currentSong_);
    }
}

//--------------------------------------------------------------------------------

void MenuManager::LoadPuckmanPlaylist() {
    auto * music = MusicManager::Instance();
    currentSong_ = music->CurrentSong();
    music->ClearPlaylist();
    music->SlowStop();
}

//--------------------------------------------------------------------------------

void MenuManager::LoadTetraminoesPlaylist() {
    std::vector<std::string> playlist;
    playlist.push_back("Content/Music/DanceOfTheSugarPlumFairies.ogg");
    auto * music = MusicManager::Instance();
    if (!music->IsSamePlaylist(playlist)) {
        music->Volume(80);
        currentSong_ = music->CurrentSong();
        music->ClearPlaylist();
        music->SetPlaylist(playlist);
        music->SlowStop();
    }
}

//--------------------------------------------------------------------------------

void MenuManager::LoadAlienPartyPlaylist() {
    std::vector<std::string> playlist;
    playlist.push_back("Content/Music/ShoresOfAvalon.ogg");
    auto * music = MusicManager::Instance();
    if (!music->IsSamePlaylist(playlist)) {
        music->Volume(80);
        currentSong_ = music->CurrentSong();
        music->ClearPlaylist();
        music->SetPlaylist(playlist);
        music->SlowStop();
    }
}

//********************************************************************************
// Singleton pattern ( http://en.wikipedia.org/wiki/Singleton_pattern )
//********************************************************************************

/**
 * The main instance of the class.
 */
MenuManager * MenuManager::instance_ = nullptr;

//--------------------------------------------------------------------------------

/**
 * Constructs a new object.
 */
MenuManager::MenuManager() : tileset_(nullptr) {}

//--------------------------------------------------------------------------------

/**
 * The destructor of the object.
 */
MenuManager::~MenuManager() {}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
MenuManager * MenuManager::Instance() {
    if (!instance_) {
        instance_ = new MenuManager();
    }
    return instance_;
}

//--------------------------------------------------------------------------------

/**
 * Gets the main instance of the class.
 */
MenuManager & MenuManager::Reference() {
    return *(Instance());
}
