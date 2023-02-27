/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <array>
#include <vector>
#include <tuple>
#include <utility>
#include <functional>

#include "actions.h"
#include "compact_state.h"
#include "thistlethwaite.h"

using namespace std;

namespace
{
    template <typename unum_t, uint8_t n, unum_t size = static_cast<unum_t>(1) << (n - 1)>
    static array<unum_t, size> build_lut_nc4()
    {
        constexpr unum_t mask = size - 1;

        array<unum_t, size> table;

        constexpr unum_t maxval = static_cast<unum_t>((1U << (8 * sizeof(unum_t))) - 1);
        for (unum_t i = 0; i < size; ++i)
            table[i] = maxval;

        unum_t counter = 0;

        for (uint8_t bit1 = 3; bit1 < n; ++bit1)
        {
            // highest bit can be deduced from other bits, as exactly 4 bits are set
            const unum_t mask1 = mask & (static_cast<unum_t>(1) << bit1);

            for (uint8_t bit2 = 2; bit2 < bit1; ++bit2)
            {
                const unum_t mask2 = static_cast<unum_t>(mask1 | (static_cast<unum_t>(1) << bit2));

                for (uint8_t bit3 = 1; bit3 < bit2; ++bit3)
                {
                    const unum_t mask3 = static_cast<unum_t>(mask2 | (static_cast<unum_t>(1) << bit3));

                    for (uint8_t bit4 = 0; bit4 < bit3; ++bit4)
                    {
                        const unum_t mask4 = static_cast<unum_t>(mask3 | (static_cast<unum_t>(1) << bit4));
                        table[mask4] = counter++;
                    }
                }
            }
        }

        return table;
    }

    static array<uint8_t, 64> build_lut_4f()
    {
        array<uint8_t, 64> table;

        for (uint8_t i = 0; i < 64; ++i)
            table[i] = 255;

        for (uint8_t i0 = 0; i0 < 4; ++i0)
        {
            for (uint8_t i1 = 0; i1 < 4; ++i1)
            {
                if (i1 == i0)
                    continue;

                const uint8_t k1 = static_cast<uint8_t>(i1 - (i1 > i0 ? 1 : 0));

                for (uint8_t i2 = 0; i2 < 4; ++i2)
                {
                    if (i2 == i0 || i2 == i1)
                        continue;

                    const uint8_t k2 = static_cast<uint8_t>(i2 - (i2 > i0 ? 1 : 0) - (i2 > i1 ? 1 : 0));

                    table[i0 + 4 * (i1 + 4 * i2)] = static_cast<uint8_t>(i0 + 4 * (k1 + 3 * k2));
                }
            }
        }

        return table;
    }

    static const array<uint16_t, 2048> lut_12c4 = build_lut_nc4<uint16_t, 12>();
    static const array<uint8_t, 128> lut_8c4 = build_lut_nc4<uint8_t, 8>();
    static const array<uint8_t, 64> lut_4f = build_lut_4f();

    static uint16_t part01(const rubik::compact_state& state)
    {
        // restriction from G0 = { U, D, F, B, L, R } to G1 = { U2, D2, F, B, L, R }
        // all edges should have parity 0, because G1 cannot change their parity

        uint16_t key = 0;

        // sum of parities modulo 2 is zero => last one can be deduced
        for (uint8_t i = 0; i < 11; ++i)
        {
            const uint16_t p = state.edge_parity(i);
            key |= (p << i);
        }

        return key;
    }

    constexpr uint32_t size12a = 2187; // 3^7 : corner parities at references (W)
                    // size12b = 495;  // 12! / (4! * 8!) : edge slice { E0, E1, E2, E3 } occupied by 0, 1, 2 and 3
    constexpr uint32_t size23a = 70;   // 8! / (4! * 4!) : corners in correct tetrad groups
    constexpr uint32_t size23b = 6;    // reduction of 4! * 4! internal tetrad occupation permutations to 4! * 4 allowed tetrad occupation permutations
                    // size23c = 70;   // 8! / (4! * 4!) : remaining edge slices
    constexpr uint32_t size34a = 96;   // 4! * 4 allowed tetrad position permutations
                    // size34b = 6912; // (4!)^3 / 2 allowed (even) edge position permutations

    static uint16_t part12a(const rubik::compact_state& state)
    {
        // restriction from G1 = { U2, D2, F, B, L, R } to G2 = { U2, D2, F2, B2, L, R }
        // all corners should have parity 0, because G2 cannot change their parity

        uint16_t key = 0;
        uint16_t factor = 1;

        // sum of parities modulo 3 is zero => last one can be deduced
        for (uint8_t i = 0; i < 7; ++i)
        {
            const uint16_t p = state.corner_parity(i);
            key += static_cast<uint16_t>(factor * p);
            factor *= 3;
        }

        return key;
    }

    static uint16_t part12b(const rubik::compact_state& state)
    {
        // restriction from G1 = { U2, D2, F, B, L, R } to G2 = { U2, D2, F2, B2, L, R }
        // edges 0 to 3 should be in slice { E0, E1, E2, E3 }, because G2 cannot move them in or out

        uint16_t pos = 0;

        // number of set bits is four => last one can be deduced
        for (uint8_t i = 0; i < 11; ++i)
        {
            const uint8_t j = state.edge_occupant(i);
            const uint16_t bit = j < 4 ? 1 : 0;
            pos |= (bit << i);
        }

        return lut_12c4[pos];
    }

    static uint8_t part23a(const rubik::compact_state& state)
    {
        // restriction from G2 = { U2, D2, F2, B2, L, R } to G3 = { U2, D2, F2, B2, L2, R2 }
        // the corners should be:
        //     - part23a: in their correct tetrad groups { C0, C1, C2, C3 } and { C4, C5, C6, C7 }
        //     - part23b: in their correct tetrad orbit, i.e. one of the 4! * 4 allowed tetrad position permutations of the 4! * 4! internal tetrad position permutations
        // the edges should be:
        //     - part23c: in their correct slices { E4, E5, E6, E7 } and { E8, E9, E10, E11 }

        uint8_t pos = 0;

        // number of set bits is 4: last one can be deduced
        for (uint8_t i = 0; i < 7; ++i)
        {
            const uint8_t j = state.corner_occupant(i);
            const uint8_t bit = j < 4 ? 1 : 0; // first tetrad group
            pos |= (bit << i);
        }

        return lut_8c4[pos];
    }

    static uint8_t part23b(const rubik::compact_state& state)
    {
        array<uint8_t, 8> ghost;
        uint8_t t0 = 0;
        uint8_t t1 = 4;

        for (uint8_t i = 0; i < 8; ++i)
        {
            const uint8_t j = state.corner_occupant(i);

            if (j < 4)
                ghost[t0++] = j;
            else
                ghost[t1++] = j;
        }

        const uint8_t g4 = ghost[0] + 4;
        const uint8_t g5 = ghost[1] + 4;
        const uint8_t g6 = ghost[2] + 4;
        const uint8_t g7 = ghost[3] + 4;

        array<uint8_t, 3> expected567;

        if (ghost[4] == g4)
            expected567 = { g5, g6, g7 };
        else if (ghost[4] == g5)
            expected567 = { g4, g7, g6 };
        else if (ghost[4] == g6)
            expected567 = { g7, g4, g5 };
        else // ghost[4] == g7
            expected567 = { g6, g5, g4 };

        if (ghost[5] == expected567[0])
        {
            if (ghost[6] == expected567[1])
                return 0; // 5, 6, 7
            else
                return 1; // 5, 7, 6
        }
        else if (ghost[5] == expected567[1])
        {
            if (ghost[6] == expected567[2])
                return 2; // 6, 7, 5
            else
                return 3; // 6, 5, 7
        }
        else // ghost[5] == expected567[2]
        {
            if (ghost[6] == expected567[0])
                return 4; // 7, 5, 6
            else
                return 5; // 7, 6, 5
        }
    }

    static uint8_t part23c(const rubik::compact_state& state)
    {
        uint8_t pos = 0;

        // number of set bits is 4: last one can be deduced
        for (uint8_t i = 4; i < 11; ++i)
        {
            const uint8_t j = state.edge_occupant(i);

            const uint8_t shift = i - 4; // shift of first slice position (i) in [0-3], shift of second slice position (i) in [4-7]
            const uint8_t bit = j < 8 ? 1 : 0; // bit of first slice occupant (j) is 1, bit of second slice occupant (j) is 0

            pos |= (bit << shift);
        }

        return lut_8c4[pos];
    }

    static uint8_t part34a(const rubik::compact_state& state)
    {
        // C0 to C3 must be occupied by corners 0 to 3
        const uint8_t i0 = state.corner_occupant(0);
        const uint8_t i1 = state.corner_occupant(1);
        const uint8_t i2 = state.corner_occupant(2);
        uint8_t key = lut_4f[i0 + 4 * (i1 + 4 * i2)];

        // C4 must by occupied by corner 4
        key += static_cast<uint8_t>(24 * (state.corner_occupant(4) - 4));

        return key;
    }

    static uint16_t part34b(const rubik::compact_state& state)
    {
        array<uint8_t, 3> keys;

        for (uint8_t k = 0; k < 3; ++k)
        {
            // E[4 * k] to E[4 * k + 3] must be occupied by edges 4 * k to 4 * k + 3
            const uint8_t m = k * 4;
            const uint8_t i0 = state.edge_occupant(m + 0) - m;
            const uint8_t i1 = state.edge_occupant(m + 1) - m;
            const uint8_t i2 = state.edge_occupant(m + 2) - m;
            keys[k] = lut_4f[i0 + 4 * (i1 + 4 * i2)];
        }

        // global edge position permutation is even
        // for keys[2] : [ 0 - 3 ] + 4 * [ 0 - 2 ] + 12 * [ 0 - 1 ] + 24 * [ 0 ] is not needed, since the option [ 0 - 1 ] is determined by permutation even
        // note that this is tied to the way lut_4f is built up (!!)

        constexpr uint16_t size = 24;
        const uint16_t combined = static_cast<uint16_t>(keys[0] + size * (keys[1] + size * (keys[2] % 12)));

        return combined;
    }

    static uint32_t key01compute(const rubik::compact_state& state)
    {
        return part01(state);
    }

    static uint32_t key12compute(const rubik::compact_state& state)
    {
        const uint32_t p12a = part12a(state);
        const uint32_t p12b = part12b(state);
        return p12a + size12a * p12b;
    }

    static uint32_t key23compute(const rubik::compact_state& state)
    {
        const uint32_t p23a = part23a(state);
        const uint32_t p23b = part23b(state);
        const uint32_t p23c = part23c(state);
        return p23a + size23a * (p23b + size23b * p23c);
    }

    static uint32_t key34compute(const rubik::compact_state& state)
    {
        const uint32_t p34a = part34a(state);
        const uint32_t p34b = part34b(state);
        return p34a + size34a * p34b;
    }

    template <size_t n>
    static vector<rubik::step> build_lut_thistle(const uint32_t size, const function<uint32_t(const rubik::compact_state&)>& computer, const array<rubik::action, n>& GX)
    {
        const rubik::action none(rubik::face::up, 0);

        vector<rubik::step> table(size, rubik::step(UINT32_MAX, none));

        const rubik::compact_state sol;
        const uint32_t key_sol = computer(sol);
        table[key_sol] = rubik::step(0, none);
        uint32_t count = 1;

        vector<pair<rubik::compact_state, uint32_t>> last = { { sol, key_sol } };
        vector<pair<rubik::compact_state, uint32_t>> novel;

        while (count < size)
        {
            for (uint32_t j = 0; j < last.size(); ++j)
            {
                for (uint32_t k = 0; k < GX.size(); ++k)
                {
                    rubik::compact_state walker = last[j].first;

                    walker.twist(GX[k]);

                    const uint32_t key = computer(walker);

                    if (table[key].key == UINT32_MAX)
                    {
                        table[key] = rubik::step(last[j].second, GX[k].inverse());
                        ++count;
                        novel.push_back({ walker, key });
                    }
                }
            }

            last.clear();
            swap(last, novel);
        }

        return table;
    }
}

namespace rubik
{
    thistlethwaite::thistlethwaite() :
        lut_g0_g1(build_lut_thistle(size01, key01compute, action::G0)),
        lut_g1_g2(build_lut_thistle(size12, key12compute, action::G1)),
        lut_g2_g3(build_lut_thistle(size23, key23compute, action::G2)),
        lut_g3_g4(build_lut_thistle(size34, key34compute, action::G3)) { }

    uint32_t thistlethwaite::key01(const compact_state& state)
    {
        return key01compute(state);
    }

    uint32_t thistlethwaite::key12(const compact_state& state)
    {
        return key12compute(state);
    }

    uint32_t thistlethwaite::key23(const compact_state& state)
    {
        return key23compute(state);
    }

    uint32_t thistlethwaite::key34(const compact_state& state)
    {
        return key34compute(state);
    }
}

