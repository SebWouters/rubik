/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <vector>
#include <random>
#include <iostream>

#include "rubik.h"

using namespace std;

int main()
{
    const rubik::compact_state solution;

    const uint32_t key_sol = rubik::thistlethwaite::key34(solution);
    if (key_sol != 0)
    {
        cout << "error: key(solution) = " << key_sol << endl;
        return 255;
    }

    vector<uint8_t> reached(rubik::thistlethwaite::size34, 0);
    reached[0] = 1;
    uint32_t count = 1;

    vector<rubik::compact_state> last = { solution };
    uint32_t iterations = 0;

    while (count < rubik::thistlethwaite::size34)
    {
        vector<rubik::compact_state> novel;

        for (uint32_t j = 0; j < last.size(); ++j)
        {
            for (uint32_t k = 0; k < rubik::action::G3.size(); ++k)
            {
                rubik::compact_state walker = last[j];
                walker.twist(rubik::action::G3[k]);

                const uint32_t key = rubik::thistlethwaite::key34(walker);

                if (reached[key] == 0)
                {
                    reached[key] = 1;
                    ++count;
                    novel.push_back(walker);
                }
            }
        }

        last = novel;
        ++iterations;
    }

    cout << "Finished building G3 in " << iterations << " iterations." << endl;

    if (iterations != 15)
    {
        cout << "error: iterations != 15" << endl;
        return 255;
    }

    cout << "success" << endl;
    return 0;
}

