/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <vector>
#include <chrono>
#include <random>
#include <iostream>

#include "rubik.h"

using namespace std;

int main()
{
    constexpr uint32_t size = 65536 * 256;

    vector<rubik::action> sequence;
    sequence.reserve(size);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint8_t> dist(0, rubik::action::G0.size() - 1);

    for (uint32_t count = 0; count < size; ++count)
        sequence.push_back(rubik::action::G0[dist(gen)]);

    rubik::permutation_state test1;
    auto start = chrono::system_clock::now();
    for (uint32_t count = 0; count < size; ++count)
        test1.twist(sequence[count]);
    auto stop = chrono::system_clock::now();
    auto time = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    cout << "Permutation state : " << 1e-3 * static_cast<double>(time) << " s." << endl;

    rubik::compact_state test2;
    start = chrono::system_clock::now();
    for (uint32_t count = 0; count < size; ++count)
        test2.twist(sequence[count]);
    stop = chrono::system_clock::now();
    time = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
    cout << "Compact state : " << 1e-3 * static_cast<double>(time) << " s." << endl;

    if (test1.data() == test2.data())
    {
        cout << "success" << endl;
        return 0;
    }
    else
    {
        cout << "error" << endl;
        return 255;
    }
}

