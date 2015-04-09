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

#include "Generator.h"
#include "Puzzle.h"

#include <random>
#include <ctime>

namespace NPuzzle {
    //********************************************************************************
    // Functions
    //********************************************************************************

    /**
     * Generates a random n-puzzle candidate.
     */
    std::vector<int> GenerateRandomCandidate() {
        const int MAX_ELEMS = Puzzle::RANGE * Puzzle::RANGE;

        std::default_random_engine generator(static_cast<unsigned long>(time(NULL)));
        std::uniform_int_distribution<int> distribution(0, MAX_ELEMS - 1);

        std::vector<int> result;
        std::vector<int> victims;
        for(int i = 0; i < MAX_ELEMS; ++i) {
            victims.push_back(i);
        }

        while(!victims.empty()) {
            int i = distribution(generator) % victims.size();
            result.push_back(victims[i]);
            victims.erase(victims.begin() + i);
        }

        return result;
    }
}
