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

using namespace std;

namespace
{
    constexpr array<char, 6> face_to_char = { 'U', 'D', 'F', 'B', 'L', 'R' };
}

namespace rubik
{
    const array<action, 18> action::G0 =
    {
        action(face::up,    1),
        action(face::up,    2),
        action(face::up,    3),
        action(face::down,  1),
        action(face::down,  2),
        action(face::down,  3),
        action(face::front, 1),
        action(face::front, 2),
        action(face::front, 3),
        action(face::back,  1),
        action(face::back,  2),
        action(face::back,  3),
        action(face::left,  1),
        action(face::left,  2),
        action(face::left,  3),
        action(face::right, 1),
        action(face::right, 2),
        action(face::right, 3)
    };

    const array<action, 14> action::G1 =
    {
        action(face::up,    2),
        action(face::down,  2),
        action(face::front, 1),
        action(face::front, 2),
        action(face::front, 3),
        action(face::back,  1),
        action(face::back,  2),
        action(face::back,  3),
        action(face::left,  1),
        action(face::left,  2),
        action(face::left,  3),
        action(face::right, 1),
        action(face::right, 2),
        action(face::right, 3)
    };

    const array<action, 10> action::G2 =
    {
        action(face::up,    2),
        action(face::down,  2),
        action(face::front, 2),
        action(face::back,  2),
        action(face::left,  1),
        action(face::left,  2),
        action(face::left,  3),
        action(face::right, 1),
        action(face::right, 2),
        action(face::right, 3)
    };

    const array<action, 6> action::G3 =
    {
        action(face::up,    2),
        action(face::down,  2),
        action(face::front, 2),
        action(face::back,  2),
        action(face::left,  2),
        action(face::right, 2)
    };

    string action::print() const
    {
        array<char, 2> concat =
        {
            face_to_char[static_cast<uint8_t>(f)],
            static_cast<char>('0' + n)
        };
        return string(concat.data(), concat.size());
    }
}

