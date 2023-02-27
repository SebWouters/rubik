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
    struct permutation_state
    {
        private:

            array<uint8_t, 48> _data;

        public:

            permutation_state();

            const array<uint8_t, 48>& data() const { return _data; }

            void twist(const action what);

            string print() const;
    };
}

