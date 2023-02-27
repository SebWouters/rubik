/*
    rubik

    Copyright (c) 2023, Sebastian Wouters
    All rights reserved.

    This file is part of rubik, licensed under the BSD 3-Clause License.
    A copy of the License can be found in the file LICENSE in the root
    folder of this project.
*/

#include <vector>
#include <random>
#include <iostream>

#include "rubik.h"

using namespace std;

int main()
{
    rubik::compact_state state;

    // scramble
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint8_t> dist(0, rubik::action::G0.size() - 1);

    for (uint32_t count = 0; count < 2048; ++count)
        state.twist(rubik::action::G0[dist(gen)]);

    rubik::thistlethwaite tt;

    cout << "Start:" << endl;
    cout << state.print_symmetry() << endl;

    // G0 to G1
    uint32_t key = tt.key01(state);

    string steps;

    while (key != 0)
    {
        rubik::step s = tt.lut_g0_g1[key];
        state.twist(s.a);
        steps += " " + s.a.print();
        key = s.key;
    }

    cout << "Steps G0 to G1:" << steps << endl;
    cout << state.print_symmetry() << endl;

    for (uint8_t i = 0; i < 12; ++i)
    {
        if (state.edge_parity(i) != 0)
        {
            cout << "error: edge parity != 0" << endl;
            return 255;
        }
    }

    // G1 to G2
    key = tt.key12(state);

    steps.clear();

    while (key != 0)
    {
        rubik::step s = tt.lut_g1_g2[key];
        state.twist(s.a);
        steps += " " + s.a.print();
        key = s.key;
    }

    cout << "Steps G1 to G2:" << steps << endl;
    cout << state.print_symmetry() << endl;

    for (uint8_t i = 0; i < 8; ++i)
    {
        if (state.corner_parity(i) != 0)
        {
            cout << "error: corner parity != 0" << endl;
            return 255;
        }
    }

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (state.edge_occupant(i) > 3)
        {
            cout << "error: edge occupant > 3" << endl;
            return 255;
        }
    }

    // G2 to G3
    key = tt.key23(state);

    steps.clear();

    while (key != 0)
    {
        rubik::step s = tt.lut_g2_g3[key];
        state.twist(s.a);
        steps += " " + s.a.print();
        key = s.key;
    }

    cout << "Steps G2 to G3:" << steps << endl;
    cout << state.print_symmetry() << endl;

    // tetrad groups and orbits
    {
        const array<uint8_t, 4> shift = {
            static_cast<uint8_t>(state.corner_occupant(0) + 4),
            static_cast<uint8_t>(state.corner_occupant(1) + 4),
            static_cast<uint8_t>(state.corner_occupant(2) + 4),
            static_cast<uint8_t>(state.corner_occupant(3) + 4)
        };

        array<uint8_t, 4> actual = {
            state.corner_occupant(4),
            state.corner_occupant(5),
            state.corner_occupant(6),
            state.corner_occupant(7)
        };

        if (shift[0] == actual[1])
            actual = { actual[1], actual[0], actual[3], actual[2] };
        else if (shift[0] == actual[2])
            actual = { actual[2], actual[3], actual[0], actual[1] };
        else if (shift[0] == actual[3])
            actual = { actual[3], actual[2], actual[1], actual[0] };

        if (shift != actual)
        {
            cout << "error: tetrad group and/or orbit mismatch" << endl;
            return 255;
        }
    }

    // remaining two edge slices : suffices to check one
    for (uint8_t i = 8; i < 12; ++i)
    {
        if (state.edge_occupant(i) < 8)
        {
            cout << "error: edge occupant < 8" << endl;
            return 255;
        }
    }

    // G3 to G4
    key = tt.key34(state);

    steps.clear();

    while (key != 0)
    {
        rubik::step s = tt.lut_g3_g4[key];
        state.twist(s.a);
        steps += " " + s.a.print();
        key = s.key;
    }

    cout << "Steps G3 to G4:" << steps << endl;
    cout << state.print_symmetry() << endl;

    for (uint8_t i = 0; i < 8; ++i)
    {
        if (state.corner_parity(i) != 0)
        {
            cout << "error: corner parity != 0" << endl;
            return 255;
        }
        if (state.corner_occupant(i) != i)
        {
            cout << "error: corner occupant != i" << endl;
            return 255;
        }
    }

    for (uint8_t i = 0; i < 12; ++i)
    {
        if (state.edge_parity(i) != 0)
        {
            cout << "error: edge parity != 0" << endl;
            return 255;
        }
        if (state.edge_occupant(i) != i)
        {
            cout << "error: edge occupant != i" << endl;
            return 255;
        }
    }

    cout << "success" << endl;
    return 0;
}

