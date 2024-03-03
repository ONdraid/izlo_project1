#include <stddef.h>
#include "cnf.h"

#define ENABLE_PODMINKA_1 1
#define ENABLE_PODMINKA_2 1
#define ENABLE_PODMINKA_3 1

//
// LOGIN: xvomaco00 (Ondřej Vomáčka)
//

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 1)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
// Pole streets ma velikost num_of_streets a obsahuje vsechny existujuci ulice
//    - pro 0 <= i < num_of_streets predstavuje streets[i] jednu existujici
//      ulici od krizovatky streets[i].crossroad_from ke krizovatce streets[i].crossroad_to
void at_least_one_valid_street_for_each_step(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets, const Street* streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    assert(streets != NULL);
#if ENABLE_PODMINKA_1
    for (unsigned step = 0; step < num_of_streets ; ++step) {
        // pro kazdy krok step
        Clause* cl = create_new_clause(formula);
        for (unsigned u = 0; u < num_of_streets; ++u)
        {
            // pro kazdou ulici u
            add_literal_to_clause(cl, true, step, streets[u].crossroad_from, streets[u].crossroad_to);
        }
    }
#endif
}


// Tato funkce by mela do formule pridat klauzule predstavujici podminku 2)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void at_most_one_street_for_each_step(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
#if ENABLE_PODMINKA_2
    for (unsigned step = 0; step < num_of_streets ; ++step) {
        // pro kazdy krok step
        for (unsigned z = 0; z < num_of_crossroads; ++z) {
            // pro kazdou krizovatku z
            for (unsigned k = 0; k < num_of_crossroads; ++k) {
                // pro kazdou krizovatku k
                for (unsigned zSecond = 0; zSecond < num_of_crossroads; ++zSecond) {
                    // pro kazdou druhou krizovatku zSecond
                    for (unsigned kSecond = 0; kSecond < num_of_crossroads; ++kSecond)
                    {
                        // pro kazdu dvojici krizovatek (z, k) a (zSecond, kSecond)
                        if (k == kSecond && z == zSecond) continue;

                        Clause* cl = create_new_clause(formula);
                        add_literal_to_clause(cl, false, step, z, k);
                        add_literal_to_clause(cl, false, step, zSecond, kSecond);
                    }
                }
            }
        }
    }
#endif
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 3)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_connected(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
#if ENABLE_PODMINKA_3
    for (unsigned step = 0; step < num_of_streets - 1; ++step) {
        // pro kazdy krok step
        for (unsigned z = 0; z < num_of_crossroads; ++z) {
            // pro kazdou krizovatku z
            for (unsigned k = 0; k < num_of_crossroads; ++k) {
                // pro kazdou krizovatku k
                // pro kazdu dvojici krizovatek (z, k)
                Clause* cl = create_new_clause(formula);
                add_literal_to_clause(cl, false, step, z, k);

                for (unsigned i = 0; i < num_of_crossroads; ++i)
                {
                    // pro kazdou krizovatku zacinajici v k a koncici v i
                    add_literal_to_clause(cl, true, step + 1, k, i);
                }
            }
        }
    }
#endif
}

// Tato funkce by mela do formule pridat klauzule predstavujici podminku 4)
// Křižovatky jsou reprezentovany cisly 0, 1, ..., num_of_crossroads-1
// Cislo num_of_streets predstavuje pocet ulic a proto i pocet kroku cesty
void streets_do_not_repeat(CNF* formula, unsigned num_of_crossroads, unsigned num_of_streets)
{
    assert(formula != NULL);
    assert(num_of_crossroads > 0);
    assert(num_of_streets > 0);
    
    for (unsigned i = 0; i < num_of_streets; ++i) {
        // pro kazdy krok i
        for (unsigned j = 0; j < num_of_streets; ++j) {
            if (i != j) {
                // pro kazdy jiny krok j
                for (unsigned z = 0; z < num_of_crossroads; ++z) {
                    for (unsigned k = 0; k < num_of_crossroads; ++k)
                    {
                        // pro kazdu dvojici krizovatek (z, k)
                        Clause* cl = create_new_clause(formula);
                        add_literal_to_clause(cl, false, i, z, k);
                        add_literal_to_clause(cl, false, j, z, k);
                    }
                }
            }
        }
    }
}
