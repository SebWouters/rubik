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

#pragma once

#include "actions.h"

using namespace std;

namespace rubik
{
    struct compact_state
    {
        private:

            array<uint8_t, 8> _corners;

            array<uint8_t, 12> _edges;

        public:

            uint8_t corner_occupant(uint8_t i) const { return _corners[i] & static_cast<uint8_t>(15); }

            uint8_t corner_parity(uint8_t i) const { return _corners[i] >> 4; }

            uint8_t edge_occupant(uint8_t i) const { return _edges[i] & static_cast<uint8_t>(15); }

            uint8_t edge_parity(uint8_t i) const { return _edges[i] >> 4; }

            const array<uint8_t, 48> data() const;

            compact_state();

            void twist(const action what);

            string print() const;

            string print_symmetry() const;
    };
}

