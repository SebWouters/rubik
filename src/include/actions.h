/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <string>
#include <array>

#pragma once

using namespace std;

namespace rubik
{
    enum class face : uint8_t
    {
        up = 0,
        down = 1,
        front = 2,
        back = 3,
        left = 4,
        right = 5
    };

    struct action
    {
        public:

            face f;
            uint8_t n;

            constexpr action(const face f_, const uint8_t n_) : f(f_), n(n_ % 4) { }

            action inverse() const { return action(f, 4 - n); }

            static const array<action, 18> G0;
            static const array<action, 14> G1;
            static const array<action, 10> G2;
            static const array<action, 6>  G3;

            string print() const;
    };
}

