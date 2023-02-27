/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <array>
#include <string>

#include "actions.h"
#include "permutation_state.h"

using namespace std;

namespace
{
    constexpr array<array<array<uint8_t, 4>, 5>, 6> generators =
    {{
        {{ {  0,  2,  7,  5 }, {  1,  4,  6,  3 }, {  8, 32, 24, 16 }, {  9, 33, 25, 17 }, { 10, 34, 26, 18 } }}, // up = 0
        {{ { 40, 42, 47, 45 }, { 41, 44, 46, 43 }, { 13, 21, 29, 37 }, { 14, 22, 30, 38 }, { 15, 23, 31, 39 } }}, // down = 1
        {{ { 16, 18, 23, 21 }, { 17, 20, 22, 19 }, {  5, 24, 42, 15 }, {  6, 27, 41, 12 }, {  7, 29, 40, 10 } }}, // front = 2
        {{ { 32, 34, 39, 37 }, { 33, 36, 38, 35 }, {  2,  8, 45, 31 }, {  1, 11, 46, 28 }, {  0, 13, 47, 26 } }}, // back = 3
        {{ {  8, 10, 15, 13 }, {  9, 12, 14, 11 }, {  0, 16, 40, 39 }, {  3, 19, 43, 36 }, {  5, 21, 45, 34 } }}, // left = 4
        {{ { 24, 26, 31, 29 }, { 25, 28, 30, 27 }, {  2, 37, 42, 18 }, {  4, 35, 44, 20 }, {  7, 32, 47, 23 } }}  // right = 5
    }};
}

namespace rubik
{
    permutation_state::permutation_state()
    {
        for (uint8_t i = 0; i < 48; ++i)
            _data[i] = i;
    }

    void permutation_state::twist(const action what)
    {
        const uint8_t row = static_cast<uint8_t>(what.f);

        switch (what.n)
        {
            case 1:
                for (uint8_t j = 0; j < 5; ++j)
                {
                    const array<uint8_t, 4> permute = generators[row][j];

                    uint8_t temp = _data[permute[0]];
                    _data[permute[0]] = _data[permute[1]];
                    _data[permute[1]] = _data[permute[2]];
                    _data[permute[2]] = _data[permute[3]];
                    _data[permute[3]] = temp;
                }
                break;
            case 2:
                for (uint8_t j = 0; j < 5; ++j)
                {
                    const array<uint8_t, 4> permute = generators[row][j];

                    uint8_t temp = _data[permute[0]];
                    _data[permute[0]] = _data[permute[2]];
                    _data[permute[2]] = temp;
                    temp = _data[permute[1]];
                    _data[permute[1]] = _data[permute[3]];
                    _data[permute[3]] = temp;
                }
                break;
            case 3:
                for (uint8_t j = 0; j < 5; ++j)
                {
                    const array<uint8_t, 4> permute = generators[row][j];

                    uint8_t temp = _data[permute[3]];
                    _data[permute[3]] = _data[permute[2]];
                    _data[permute[2]] = _data[permute[1]];
                    _data[permute[1]] = _data[permute[0]];
                    _data[permute[0]] = temp;
                }
                break;
            case 0:
            default:
                break;
        }
    }

    string printer(const array<uint8_t, 48>& data);

    string permutation_state::print() const
    {
        return printer(_data);
    }
}

