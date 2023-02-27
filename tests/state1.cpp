/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <iostream>

#include "rubik.h"

using namespace std;

int main()
{
    for (uint8_t i = 0; i < rubik::action::G0.size(); ++i)
    {
        rubik::permutation_state test;

        test.twist(rubik::action::G0[i]);

        rubik::compact_state test2;

        test2.twist(rubik::action::G0[i]);

        if (test.data() != test2.data())
        {
            cout << "error" << endl;
            return 255;
        }
    }

    cout << "success" << endl;
    return 0;
}

