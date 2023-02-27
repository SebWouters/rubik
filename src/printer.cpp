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
#include <cstdio>

#include "actions.h"
#include "compact_state.h"

using namespace std;

namespace rubik
{
    string printer(const array<uint8_t, 48>& data)
    {
        array<char, 582> result;

        sprintf(result.data(),

            "             +--------+\n"
            "             |%2u %2u %2u|\n"
            "             |        |\n"
            "             |%2u  U %2u|\n"
            "             |        |\n"
            "             |%2u %2u %2u|\n"
            "    +--------+--------+--------+--------+\n"
            "    |%2u %2u %2u|%2u %2u %2u|%2u %2u %2u|%2u %2u %2u|\n"
            "    |        |        |        |        |\n"
            "    |%2u  L %2u|%2u  F %2u|%2u  R %2u|%2u  B %2u|\n"
            "    |        |        |        |        |\n"
            "    |%2u %2u %2u|%2u %2u %2u|%2u %2u %2u|%2u %2u %2u|\n"
            "    +--------+--------+--------+--------+\n"
            "             |%2u %2u %2u|\n"
            "             |        |\n"
            "             |%2u  D %2u|\n"
            "             |        |\n"
            "             |%2u %2u %2u|\n"
            "             +--------+",

                                          data[ 0], data[ 1], data[ 2],
                                          data[ 3],           data[ 4],
                                          data[ 5], data[ 6], data[ 7],
            data[ 8], data[ 9], data[10], data[16], data[17], data[18], data[24], data[25], data[26], data[32], data[33], data[34],
            data[11],           data[12], data[19],           data[20], data[27],           data[28], data[35],           data[36],
            data[13], data[14], data[15], data[21], data[22], data[23], data[29], data[30], data[31], data[37], data[38], data[39],
                                          data[40], data[41], data[42],
                                          data[43],           data[44],
                                          data[45], data[46], data[47]);

        return string(result.data(), result.size() - 1);
    }

    string printer(const compact_state& state)
    {
        array<char, 534> result;

        sprintf(result.data(),

            "    +----------+-----------------+    +----------+-------------------------------------+\n"
            "    | corners  | 0 1 2 3 4 5 6 7 |    | edges    |  0  1  2  3  4  5  6  7  8  9 10 11 |\n"
            "    +----------+-----------------+    +----------+-------------------------------------+\n"
            "    | occupant | %u %u %u %u %u %u %u %u |    | occupant | %2u %2u %2u %2u %2u %2u %2u %2u %2u %2u %2u %2u |\n"
            "    | parity   | %u %u %u %u %u %u %u %u |    | parity   |  %u  %u  %u  %u  %u  %u  %u  %u  %u  %u  %u  %u |\n"
            "    +----------+-----------------+    +----------+-------------------------------------+",

            state.corner_occupant(0), state.corner_occupant(1), state.corner_occupant(2), state.corner_occupant(3),
            state.corner_occupant(4), state.corner_occupant(5), state.corner_occupant(6), state.corner_occupant(7),
            state.edge_occupant(0), state.edge_occupant(1), state.edge_occupant( 2), state.edge_occupant( 3),
            state.edge_occupant(4), state.edge_occupant(5), state.edge_occupant( 6), state.edge_occupant( 7),
            state.edge_occupant(8), state.edge_occupant(9), state.edge_occupant(10), state.edge_occupant(11),
            state.corner_parity(0), state.corner_parity(1), state.corner_parity(2), state.corner_parity(3),
            state.corner_parity(4), state.corner_parity(5), state.corner_parity(6), state.corner_parity(7),
            state.edge_parity(0), state.edge_parity(1), state.edge_parity( 2), state.edge_parity( 3),
            state.edge_parity(4), state.edge_parity(5), state.edge_parity( 6), state.edge_parity( 7),
            state.edge_parity(8), state.edge_parity(9), state.edge_parity(10), state.edge_parity(11));

        return string(result.data(), result.size() - 1);
    }
}

