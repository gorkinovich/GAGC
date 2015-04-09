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

/******************************************************************************

                         -------------------------------
                         | Project configuration notes |
                         -------------------------------

 I use Visual Studio 2010, using this configuration values:

 Debugging, Working Directory:
  -> $(SolutionDir)Bin\

 C/C++, General, Additional Include Directories:
  -> $(SolutionDir)SFML\include;$(SolutionDir)Source

 Linker General, Additional Library Directories:
  -> $(SolutionDir)SFML\lib

 Linker, Debugging, Generate Program Database File:
  -> $(OutDir)$(TargetName).pdb

 Linker, Input, Additional Dependencies:
  -> sfml-system-d.lib sfml-graphics-d.lib sfml-window-d.lib sfml-audio-d.lib
  -> sfml-system.lib sfml-graphics.lib sfml-window.lib sfml-audio.lib

******************************************************************************/

#if defined(WIN32) && defined(NDEBUG)
#pragma comment(lib, "sfml-system.lib")
#pragma comment(lib, "sfml-graphics.lib")
#pragma comment(lib, "sfml-window.lib")
#pragma comment(lib, "sfml-audio.lib")
#elif defined(WIN32)
#pragma comment(lib, "sfml-system-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "sfml-window-d.lib")
#pragma comment(lib, "sfml-audio-d.lib")
#endif

#include <stdlib.h>
#include <System/CoreManager.h>
#include <Games/SaveManager.h>

#if defined(WIN32) && defined(NDEBUG)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

int main(int argc, char ** argv) {
#if defined(WIN32) && defined(NDEBUG)
    FreeConsole();
#endif
    auto & core = CoreManager::Reference();
    auto & save = SaveManager::Reference();
    if (core.Initialize()) {
        save.Initialize();
        core.Run();
    }
    save.Release();
    core.Release();
    return EXIT_SUCCESS;
}
