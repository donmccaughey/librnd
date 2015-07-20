#include <stdio.h>
#include <stdlib.h>

#include "rnd.h"


void
print_random_numbers(char const *description,
                     struct rnd *rnd,
                     uint32_t inclusive_lower_bound,
                     uint32_t inclusive_upper_bound)
{
    printf("%s in [%lu, %lu]\n",
           description,
           (unsigned long)inclusive_lower_bound,
           (unsigned long)inclusive_upper_bound);
    for (int i = 0; i < 20; ++i) {
        uint32_t n = rnd_next_uniform_value_in_range(rnd,
                                                     inclusive_lower_bound,
                                                     inclusive_upper_bound);
        printf("%lu, ", (unsigned long)n);
    }
    printf("\n\n");
}


int
main(int argc, char const *argv[])
{
    print_random_numbers("global_rnd", global_rnd, 0, 9);
    
    print_random_numbers("global_rnd", global_rnd, 10, 29);
    
    struct rnd *rnd = rnd_alloc();
    print_random_numbers("rnd_alloc()", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_jrand48((unsigned short[]){2, 3, 5});
    print_random_numbers("rnd_alloc_jrand48({2, 3, 5})", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_jrand48((unsigned short[]){2, 3, 5});
    print_random_numbers("another rnd_alloc_jrand48({2, 3, 5})", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_min();
    print_random_numbers("rnd_alloc_fake_min()", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_median();
    print_random_numbers("rnd_alloc_fake_median()", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_max();
    print_random_numbers("rnd_alloc_fake_max()", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_fixed(7);
    print_random_numbers("rnd_alloc_fake_fixed(7)", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_fixed(13);
    print_random_numbers("rnd_alloc_fake_fixed(13)", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_ascending(3);
    print_random_numbers("rnd_alloc_fake_ascending(3)", rnd, 0, 9);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake_ascending(5);
    print_random_numbers("rnd_alloc_fake_ascending(5)", rnd, 10, 19);
    rnd_free(rnd);
    
    return EXIT_SUCCESS;
}
