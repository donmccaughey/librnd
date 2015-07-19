#include <stdio.h>
#include <stdlib.h>

#include "rnd.h"


void
print_random_numbers(char const *description, struct rnd *rnd)
{
    printf("Some %s numbers in [0, 10)\n", description);
    for (int i = 0; i < 10; ++i) {
        uint32_t n = rnd_next_uniform_value(rnd, 10);
        printf("%lu, ", (unsigned long)n);
    }
    printf("\n");
}


int
main(int argc, char const *argv[])
{
    struct rnd *rnd = rnd_alloc();
    print_random_numbers("default real", rnd);
    rnd_free(rnd);
    
    rnd = rnd_alloc_jrand48((unsigned short[]){2, 3, 5});
    print_random_numbers("jrand48 real", rnd);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake(rnd_fake_type_min);
    print_random_numbers("fake min", rnd);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake(rnd_fake_type_median);
    print_random_numbers("fake median", rnd);
    rnd_free(rnd);
    
    rnd = rnd_alloc_fake(rnd_fake_type_max);
    print_random_numbers("fake max", rnd);
    rnd_free(rnd);
    
    return EXIT_SUCCESS;
}