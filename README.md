conventions
-----------

### permutation state

```
             +--------+
             | 0  1  2|
             | 3  U  4|
             | 5  6  7|
    +--------+--------+--------+--------+
    | 8  9 10|16 17 18|24 25 26|32 33 34|
    |11  L 12|19  F 20|27  R 28|35  B 36|
    |13 14 15|21 22 23|29 30 31|37 38 39|
    +--------+--------+--------+--------+
             |40 41 42|
             |43  D 44|
             |45 46 47|
             +--------+
```

### compact state

```
           C3--E3--C7
            |   Z   |
           E10  U  E11
            |   Z   |
   C3-E10--C6--E2--C2-E11--C7--E3--C3
    | W Z W |       | W Z W |       |
   E6 Z L Z E4  F  E5 Z R Z E7  B  E6
    | W Z W |       | W Z W |       |
   C4--E8--C1--E0--C5--E9--C0--E1--C4
            |   Z   |
           E8   D   E9
            |   Z   |
           C4--E1--C0
```

* ```W``` are the corner parity references (fixed in ```G2 = { U2, D2, F2, B2, L, R }```)
* ```Z``` are the edge parity references (fixed in ```G1 = { U2, D2, F, B, L, R }```)

subgroups
---------

### G0 = { U, D, F, B, L, R }

* sum of corner parities mod 3 is always zero (see compact_state.cpp : corner_generators):
    * corner occupants: 8! states
    * corner parities: 3^7 states
* sum of edge parities mod 2 is always zero (see compact_state.cpp : edge_generators):
    * edge occupants: 12! states
    * edge parities: 2^11 states
* corner and edge moves are not independent:
    * each edge and corner occupant permutation is odd
    * e.g. corner occupant permutation from the solution with ```U1```: ```{ C6 : 6, C3 : 3, C7 : 7, C2 : 2 }```  =>  ```{ C6 : 3, C3 : 7, C7 : 2, C2 : 6 }```: 3 swaps
    * the global corner occupant permutation and global edge occupant permutation are hence either both even or both odd
* the total number of states is hence upper bounded by (8! * 3^7 * 12! * 2^11) / 2 = 43252003274489856000
* it turns out that the upper bound is also the actual number of states

### G1 = { U2, D2, F, B, L, R }

* only half twists are allowed for up and down
* edge parities (```Z```) cannot change
* going from G0 to G1 reduces the number of allowed states by 2048:
    * edge parities: 2^11 states

### G2 = { U2, D2, F2, B2, L, R }

* only half twists are allowed for up, down, front and back
* corner parities (```W```) cannot change
* edge occupants in slice ```{ E0, E1, E2, E3 }``` remain within this slice
* going from G1 to G2 reduces the number of allowed states by 1082565:
    * corner parities: 3^7 states
    * edge slice occupants: 12! / (8! * 4!) states

### G3 = { U2, D2, F2, B2, L2, R2 }

* only half twists are allowed
* edge occupants in slices ```{ E4, E5, E6, E7 }``` and ```{ E8, E9, E10, E11 }``` remain within their slice
* corner occupants in tetrad groups ```{ C0, C1, C2, C3 }``` and ```{ C4, C5, C6, C7 }``` remain within their tetrad group
* the tetrad orbits are restricted to 4! * 4 states:
    * suppose the first tetrad occupation is ```{ C0 : a, C1 : b, C2 : c, C3 : d }``` with ```0 <= a, b, c, d <= 3```
    * the second tetrad occupation is any of:
        * ```{ C4 : a + 4, C5 : b + 4, C6 : c + 4, C7 : d + 4 }```
        * ```{ C4 : b + 4, C5 : a + 4, C6 : d + 4, C7 : c + 4 }```
        * ```{ C4 : c + 4, C5 : d + 4, C6 : a + 4, C7 : b + 4 }```
        * ```{ C4 : d + 4, C5 : c + 4, C6 : b + 4, C7 : a + 4 }```
    * the global corner occupant permutation is hence even, and therefore also the global edge occupant permutation
* going from G2 to G3 reduces the number of allowed states by 29400:
    * tetrad groups: 8! / (4! * 4!) states
    * tetrad orbits: 6 states (out of 4! * 4! permutations 4! * 4 are allowed)
    * remaining edge slice occupants: 8! / (4! * 4!) states

### G4 = { U0, D0, F0, B0, L0, R0 }

* the solution
* going from G3 to G4 reduces the allowed states by 663552:
    * tetrad orbits: 4! * 4 states
    * (even) edge occupant permutations within their slice: (4!)^3 / 2 states

