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

#include "CoreManager.h"

#if defined(WIN32)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ObjBase.h>
#include <ShellAPI.h>

#else

#include <unistd.h>
#include <stdlib.h>

#endif

//********************************************************************************
// OS Methods
//********************************************************************************

/**
 * Initializes the OS.
 */
void CoreInitializeOS() {
#if defined(WIN32)
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
#endif
}

//********************************************************************************
// Util Methods
//********************************************************************************

/**
 * Opens an URL in the web browser.
 */
void CoreManager::OpenURL(const std::string & url) {
    try {
#if defined(WIN32)
        ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#else
        std::string browser = getenv("BROWSER");
        if(browser == "") return;

        char * args[3];
        args[0] = (char*)browser.c_str();
        args[1] = (char*)url.c_str();
        args[2] = 0;

        pid_t pid = fork();
        if(!pid) {
            execvp(browser.c_str(), args);
        }
#endif
    } catch(...) {
    }
}
