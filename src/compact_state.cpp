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
#include "compact_state.h"

using namespace std;

namespace
{
    constexpr array<array<uint8_t, 3>, 8> corner_map =
    {{
        { 31, 47, 37 }, // C0, starting at reference
        { 15, 40, 21 }, // C1
        { 24,  7, 18 }, // C2
        {  8,  0, 34 }, // C3
        { 13, 39, 45 }, // C4
        { 29, 23, 42 }, // C5
        { 10, 16,  5 }, // C6
        { 26, 32,  2 }  // C7
    }};

    constexpr array<array<uint8_t, 2>, 12> edge_map =
    {{
        { 41, 22 }, // E0, starting at reference
        { 46, 38 }, // E1
        {  6, 17 }, // E2
        {  1, 33 }, // E3
        { 12, 19 }, // E4
        { 27, 20 }, // E5
        { 11, 36 }, // E6
        { 28, 35 }, // E7
        { 14, 43 }, // E8
        { 30, 44 }, // E9
        {  9,  3 }, // E10
        { 25,  4 }  // E11
    }};

    constexpr array<array<array<uint8_t, 2>, 4>, 6> corner_generators =
    {{
        // first  number: corner position permutation
        // second number: parity addition for incoming corner
        // corner parity is the number of ccw in-place rotations needed to align the occupying corner's reference with the position reference (W)
        {{ { 2, 1 }, { 6, 2 }, { 3, 1 }, { 7, 2 } }}, // up = 0
        {{ { 0, 1 }, { 4, 2 }, { 1, 1 }, { 5, 2 } }}, // down = 1
        {{ { 1, 2 }, { 6, 1 }, { 2, 2 }, { 5, 1 } }}, // front = 2
        {{ { 0, 2 }, { 7, 1 }, { 3, 2 }, { 4, 1 } }}, // back = 3
        {{ { 3, 0 }, { 6, 0 }, { 1, 0 }, { 4, 0 } }}, // left = 4
        {{ { 2, 0 }, { 7, 0 }, { 0, 0 }, { 5, 0 } }}  // right = 5
    }};

    constexpr uint8_t corner_update(const uint8_t combined, const uint8_t parity)
    {
        uint8_t pos = combined & static_cast<uint8_t>(15);
        uint8_t par = combined >> 4;
        par = static_cast<uint8_t>((par + parity) % 3);
        return static_cast<uint8_t>(pos | (par << 4));
    }

    constexpr array<array<uint8_t, 4>, 6> edge_generators =
    {{
        // edge position permutations
        // for up and down: there's a parity toggle on cw and ccw turns
        {  2, 10,  3, 11 }, // up = 0
        {  0,  9,  1,  8 }, // down = 1
        {  0,  4,  2,  5 }, // front = 2
        {  1,  7,  3,  6 }, // back = 3
        {  4,  8,  6, 10 }, // left = 4
        {  5, 11,  7,  9 }  // right = 5
    }};
}

namespace rubik
{
    compact_state::compact_state()
    {
        // occupant of position i  = edge/corner[i] & static_cast<uint8_t>(15)
        // parity of occupant at i = edge/corner[i] >> 4

        for (uint8_t i = 0; i < _corners.size(); ++i)
            _corners[i] = i; // all corners start at parity 0

        for (uint8_t i = 0; i < _edges.size(); ++i)
            _edges[i] = i; // all edges start at parity 0
    }

    void compact_state::twist(const action what)
    {
        const uint8_t row = static_cast<uint8_t>(what.f);

        switch (what.n)
        {
            case 1:
                {
                    const array<array<uint8_t, 2>, 4> permute1 = corner_generators[row];

                    uint8_t temp = _corners[permute1[0][0]];
                    _corners[permute1[0][0]] = corner_update(_corners[permute1[1][0]], permute1[0][1]);
                    _corners[permute1[1][0]] = corner_update(_corners[permute1[2][0]], permute1[1][1]);
                    _corners[permute1[2][0]] = corner_update(_corners[permute1[3][0]], permute1[2][1]);
                    _corners[permute1[3][0]] = corner_update(                    temp, permute1[3][1]);

                    const array<uint8_t, 4> permute2 = edge_generators[row];

                    const uint8_t toggle = row < 2 ? 16 : 0;

                    temp = _edges[permute2[0]];
                    _edges[permute2[0]] = _edges[permute2[1]] ^ toggle;
                    _edges[permute2[1]] = _edges[permute2[2]] ^ toggle;
                    _edges[permute2[2]] = _edges[permute2[3]] ^ toggle;
                    _edges[permute2[3]] = temp ^ toggle;
                }
                break;
            case 2:
                {
                    const array<array<uint8_t, 2>, 4> permute1 = corner_generators[row];

                    uint8_t temp = _corners[permute1[0][0]];
                    _corners[permute1[0][0]] = _corners[permute1[2][0]];
                    _corners[permute1[2][0]] = temp;

                    temp = _corners[permute1[1][0]];
                    _corners[permute1[1][0]] = _corners[permute1[3][0]];
                    _corners[permute1[3][0]] = temp;

                    const array<uint8_t, 4> permute2 = edge_generators[row];

                    temp = _edges[permute2[0]];
                    _edges[permute2[0]] = _edges[permute2[2]];
                    _edges[permute2[2]] = temp;

                    temp = _edges[permute2[1]];
                    _edges[permute2[1]] = _edges[permute2[3]];
                    _edges[permute2[3]] = temp;
                }
                break;
            case 3:
                {
                    const array<array<uint8_t, 2>, 4> permute1 = corner_generators[row];

                    uint8_t temp = _corners[permute1[3][0]];
                    _corners[permute1[3][0]] = corner_update(_corners[permute1[2][0]], permute1[3][1]);
                    _corners[permute1[2][0]] = corner_update(_corners[permute1[1][0]], permute1[2][1]);
                    _corners[permute1[1][0]] = corner_update(_corners[permute1[0][0]], permute1[1][1]);
                    _corners[permute1[0][0]] = corner_update(                    temp, permute1[0][1]);

                    const array<uint8_t, 4> permute2 = edge_generators[row];

                    const uint8_t toggle = row < 2 ? 16 : 0;

                    temp = _edges[permute2[3]];
                    _edges[permute2[3]] = _edges[permute2[2]] ^ toggle;
                    _edges[permute2[2]] = _edges[permute2[1]] ^ toggle;
                    _edges[permute2[1]] = _edges[permute2[0]] ^ toggle;
                    _edges[permute2[0]] = temp ^ toggle;
                }
                break;
            case 0:
            default:
                break;
        }
    }

    const array<uint8_t, 48> compact_state::data() const
    {
        array<uint8_t, 48> data;

        for (uint8_t i = 0; i < 48; ++i)
            data[i] = 99;

        for (uint8_t i = 0; i < 8; ++i)
        {
            uint8_t combined = _corners[i];
            uint8_t pos = combined & static_cast<uint8_t>(15);
            uint8_t par = combined >> 4;

            array<uint8_t, 3> target = corner_map[i];
            array<uint8_t, 3> source = corner_map[pos];
            for (uint8_t j = 0; j < 3; ++j)
            {
                data[target[j]] = source[(j + par) % 3];
            }
        }

        for (uint8_t i = 0; i < 12; ++i)
        {
            uint8_t combined = _edges[i];
            uint8_t pos = combined & static_cast<uint8_t>(15);
            uint8_t par = combined >> 4;

            array<uint8_t, 2> target = edge_map[i];
            array<uint8_t, 2> source = edge_map[pos];
            for (uint8_t j = 0; j < 2; ++j)
            {
                data[target[j]] = source[j ^ par];
            }
        }

        return data;
    }

    string printer(const array<uint8_t, 48>& data);

    string compact_state::print() const
    {
        return printer(data());
    }

    string printer(const compact_state& state);

    string compact_state::print_symmetry() const
    {
        return printer(*this);
    }
}

