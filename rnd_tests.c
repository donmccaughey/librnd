#include <stdlib.h>

#include "rnd.h"
#include "test_asserts.h"


static void
test_global_rnd(void)
{
    ASSERT_IN_RANGE(global_rnd, 0, 9, 1000);
    ASSERT_IN_RANGE(global_rnd, 7, 2819, 100000);
}


static void
test_rnd_alloc(void)
{
    struct rnd *rnd = rnd_alloc();
    ASSERT_IN_RANGE(rnd, 0, 9, 1000);
    ASSERT_IN_RANGE(rnd, 5171, 5810, 100000);
    rnd_free(rnd);
}


static void
test_alloc_jrand48(void)
{
    struct rnd *rnd = rnd_alloc_jrand48((unsigned short[]){ 2, 3, 5 });
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 6, 2, 8, 6, 1, 0, 3, 7, 8, 9 }), 10);
    rnd_free(rnd);
}


static void
test_alloc_mrand48(void)
{
    struct rnd *rnd = rnd_alloc_mrand48();
    ASSERT_IN_RANGE(rnd, 0, 9, 1000);
    ASSERT_IN_RANGE(rnd, 5171, 5810, 100000);
    rnd_free(rnd);
}


static void
test_alloc_fake_min(void)
{
    struct rnd *rnd = rnd_alloc_fake_min();
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }), 10);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17 }),
                           10);
    rnd_free(rnd);
}


static void
test_alloc_fake_median(void)
{
    struct rnd *rnd = rnd_alloc_fake_median();
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 }), 10);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 44, 44, 44, 44, 44, 44, 44, 44, 44, 44 }),
                           10);
    rnd_free(rnd);
}


static void
test_alloc_fake_max(void)
{
    struct rnd *rnd = rnd_alloc_fake_max();
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 }), 10);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 71, 71, 71, 71, 71, 71, 71, 71, 71, 71 }),
                           10);
    rnd_free(rnd);
}


static void
test_alloc_fake_fixed(void)
{
    struct rnd *rnd = rnd_alloc_fake_fixed(7);
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 }), 10);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 }),
                           10);
    rnd_free(rnd);

    rnd = rnd_alloc_fake_fixed(1003);
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }), 10);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 }),
                           10);
    rnd_free(rnd);
}


static void
test_alloc_fake_ascending(void)
{
    struct rnd *rnd = rnd_alloc_fake_ascending(3);
    ASSERT_RANDOM_SEQUENCE(rnd, 0, 9,
                           ((uint32_t[]){ 3, 4, 5, 6, 7, 8, 9, 0, 1, 2 }), 10);
    rnd_free(rnd);

    rnd = rnd_alloc_fake_ascending(3);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 }),
                           10);
    rnd_free(rnd);

    rnd = rnd_alloc_fake_ascending(5);
    ASSERT_RANDOM_SEQUENCE(rnd, 10, 19,
                           ((uint32_t[]){ 15, 16, 17, 18, 19, 10, 11, 12, 13, 14 }),
                           10);
    rnd_free(rnd);

    rnd = rnd_alloc_fake_ascending(50);
    ASSERT_RANDOM_SEQUENCE(rnd, 17, 71,
                           ((uint32_t[]){ 67, 68, 69, 70, 71, 17, 18, 19, 20, 21 }),
                           10);
    rnd_free(rnd);
}


static void
test_rnd_shuffle(void)
{
    uint32_t numbers[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    struct rnd *rnd = rnd_alloc_jrand48((unsigned short[]){ 2, 3, 5 });
    rnd_shuffle(rnd, numbers, 10, sizeof(uint32_t));
    rnd_free(rnd);
    uint32_t expected[] = { 6, 2, 4, 8, 7, 5, 1, 9, 3, 0 };
    ASSERT_SEQUENCE(expected, numbers, 10);
}


int
main(int argc, char const *argv[])
{
    test_global_rnd();
    test_rnd_alloc();
    test_alloc_jrand48();
    test_alloc_mrand48();
    test_alloc_fake_min();
    test_alloc_fake_median();
    test_alloc_fake_max();
    test_alloc_fake_fixed();
    test_alloc_fake_ascending();
    test_rnd_shuffle();
    return EXIT_SUCCESS;
}
