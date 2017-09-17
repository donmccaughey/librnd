#ifndef TEST_ASSERTS_H_INCLUDED
#define TEST_ASSERTS_H_INCLUDED


#include <stdint.h>


#define ASSERT_SEQUENCE(EXPECTED_VALUES, ACTUAL_VALUES, COUNT) \
    assert_sequence((EXPECTED_VALUES), \
                    (ACTUAL_VALUES), \
                    (COUNT), \
                    __FILE__, \
                    __LINE__)

#define ASSERT_RANDOM_SEQUENCE(RND, \
                               INCLUSIVE_LOWER_BOUND, \
                               INCLUSIVE_UPPER_BOUND, \
                               EXPECTED_VALUES, \
                               COUNT) \
    assert_random_sequence((RND), \
                           (INCLUSIVE_LOWER_BOUND), \
                           (INCLUSIVE_UPPER_BOUND), \
                           (EXPECTED_VALUES), \
                           (COUNT), \
                           __FILE__, \
                           __LINE__)

#define ASSERT_IN_RANGE(RND, INCLUSIVE_LOWER_BOUND, \
                        INCLUSIVE_UPPER_BOUND, \
                        COUNT) \
    assert_in_range((RND), \
                    (INCLUSIVE_LOWER_BOUND), \
                    (INCLUSIVE_UPPER_BOUND), \
                    (COUNT), \
                    __FILE__, \
                    __LINE__)


struct rnd;


void
assert_sequence(uint32_t *expected_values,
                uint32_t *actual_values,
                int count,
                char const *file,
                int line);

void
assert_random_sequence(struct rnd *rnd,
                       uint32_t inclusive_lower_bound,
                       uint32_t inclusive_upper_bound,
                       uint32_t *expected_values,
                       int count,
                       char const *file,
                       int line);

void
assert_in_range(struct rnd *rnd,
                uint32_t inclusive_lower_bound,
                uint32_t inclusive_upper_bound,
                int count,
                char const *file,
                int line);


#endif
