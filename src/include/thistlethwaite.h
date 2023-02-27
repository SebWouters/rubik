/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <vector>

#pragma once

#include "actions.h"
#include "compact_state.h"

using namespace std;

namespace rubik
{
    struct step
    {
        uint32_t key;

        action a;

        step(uint32_t key_, action a_) : key(key_), a(a_) { }
    };

    struct thistlethwaite
    {
        public:

            thistlethwaite();

            static constexpr uint32_t size01 = 2048;
            static constexpr uint32_t size12 = 1082565;
            static constexpr uint32_t size23 = 29400;
            static constexpr uint32_t size34 = 663552;

            static uint32_t key01(const compact_state& state);

            static uint32_t key12(const compact_state& state);

            static uint32_t key23(const compact_state& state);

            static uint32_t key34(const compact_state& state);

            const vector<step> lut_g0_g1;

            const vector<step> lut_g1_g2;

            const vector<step> lut_g2_g3;

            const vector<step> lut_g3_g4;
    };
}

