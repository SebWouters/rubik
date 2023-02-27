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
    constexpr uint32_t size = 65536 * 256;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint8_t> dist(0, rubik::action::G3.size() - 1);

    rubik::compact_state test;
    uint32_t key = rubik::thistlethwaite::key23(test);

    if (key != 0)
    {
        cout << "error: key = " << key << endl;
        return 255;
    }

    for (uint32_t count = 0; count < size; ++count)
    {
        test.twist(rubik::action::G3[dist(gen)]);
        key = rubik::thistlethwaite::key23(test);

        if (key != 0)
        {
            cout << "error: key = " << key << endl;
            return 255;
        }
    }

    cout << test.print() << endl;

    cout << "success" << endl;
    return 0;
}

