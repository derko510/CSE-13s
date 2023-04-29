#include "set.h"

#include <stdbool.h>

// returns an empty set
Set set_empty(void) {
    return 0;
}

// returns the universal set (i.e., all possible elements)
Set set_universal(void) {
    return ~0;
}

// adds an element to the set
Set set_insert(Set s, uint8_t x) {
    return s | (1 << (x % 32));
}

// removes an element from the set
Set set_remove(Set s, uint8_t x) {
    return s & ~(1 << (x % 32));
}

// returns true if an element is a member of the set, false otherwise
bool set_member(Set s, uint8_t x) {
    return s & (1 << (x % 32));
}

// returns the union of two sets
Set set_union(Set s, Set t) {
    return s | t;
}

// returns the intersection of two sets
Set set_intersect(Set s, Set t) {
    return s & t;
}

// returns the difference of two sets
Set set_difference(Set s, Set t) {
    return s & ~t;
}

// returns the complement of a set
Set set_complement(Set s) {
    return ~s;
}
