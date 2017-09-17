#include "test_asserts.h"

#include <stdlib.h>
#include <stdio.h>

#include "rnd.h"


void
assert_sequence(uint32_t *expected_values,
                uint32_t *actual_values,
                int count,
                char const *file,
                int line)
{
    for (int i = 0; i < count; ++i) {
        if (expected_values[i] != actual_values[i]) {
            fprintf(stderr,
                    "%s:%i: FAILURE: index %i: expected %lu, found %lu\n",
                    file, line, i,
                    (unsigned long)expected_values[i],
                    (unsigned long)actual_values[i]);
            exit(EXIT_FAILURE);
        }
    }
}


void
assert_random_sequence(struct rnd *rnd,
                       uint32_t inclusive_lower_bound,
                       uint32_t inclusive_upper_bound,
                       uint32_t *expected_values,
                       int count,
                       char const *file,
                       int line)
{
    uint32_t actual_values[count];
    for (int i = 0; i < count; ++i) {
        actual_values[i] = rnd_next_uniform_value_in_range(rnd,
                                                           inclusive_lower_bound,
                                                           inclusive_upper_bound);
    }
    assert_sequence(expected_values, actual_values, count, file, line);
}


void
assert_in_range(struct rnd *rnd,
                uint32_t inclusive_lower_bound,
                uint32_t inclusive_upper_bound,
                int count,
                char const *file,
                int line)
{
    for (int i = 0; i < count; ++i) {
        uint32_t n = rnd_next_uniform_value_in_range(rnd,
                                                     inclusive_lower_bound,
                                                     inclusive_upper_bound);
        if (n < inclusive_lower_bound || n > inclusive_upper_bound) {
            fprintf(stderr,
                    "%s:%i: FAILURE: index %i: %lu is outside range [%lu, %lu]\n",
                    file, line, i, (unsigned long)n,
                    (unsigned long)inclusive_lower_bound,
                    (unsigned long)inclusive_upper_bound);
            exit(EXIT_FAILURE);
        }
    }
}
