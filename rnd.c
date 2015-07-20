/* rnd. https://github.com/donmccaughey/rnd
 Copyright (c) 2015, Don McCaughey. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 - Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 - Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include "rnd.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


static uint32_t
next_arc4random_uniform_value_in_range(void *user_data,
                                       uint32_t inclusive_lower_bound,
                                       uint32_t inclusive_upper_bound);


struct rnd *const global_rnd = &((struct rnd){
    .user_data=NULL,
    .next_uniform_value_in_range=next_arc4random_uniform_value_in_range,
    .free_user_data=NULL,
});


static struct rnd *
alloc_with_next_uniform_value_in_range(uint32_t (*next_uniform_value_in_range)(void *, uint32_t, uint32_t))
{
    struct rnd *rnd = calloc(1, sizeof(struct rnd));
    if (!rnd) return NULL;
    
    rnd->next_uniform_value_in_range = next_uniform_value_in_range;
    
    return rnd;
}


static struct rnd *
alloc_with_user_data_size(size_t size)
{
    struct rnd *rnd = calloc(1, sizeof(struct rnd));
    if (!rnd) return NULL;
    
    rnd->user_data = calloc(1, size);
    if (!rnd->user_data) {
        free(rnd);
        return NULL;
    }
    
    rnd->free_user_data = free;
    
    return rnd;
}


static uint32_t
next_arc4random_uniform_value_in_range(void *user_data,
                                       uint32_t inclusive_lower_bound,
                                       uint32_t inclusive_upper_bound)
{
    if (0 == inclusive_lower_bound && UINT32_MAX == inclusive_upper_bound) {
        return arc4random();
    } else {
        uint32_t normalized_exclusive_upper_bound = inclusive_upper_bound
                                                  - inclusive_lower_bound
                                                  + 1;
        return inclusive_lower_bound
             + arc4random_uniform(normalized_exclusive_upper_bound);
    }
}


struct rnd *
rnd_alloc(void)
{
    return alloc_with_next_uniform_value_in_range(next_arc4random_uniform_value_in_range);
}


static uint32_t
next_fake_ascending_uniform_value_in_range(void *user_data,
                                           uint32_t inclusive_lower_bound,
                                           uint32_t inclusive_upper_bound)
{
    uint32_t *ascending_value = user_data;
    uint32_t normalized_exclusive_upper_bound = inclusive_upper_bound
                                              - inclusive_lower_bound
                                              + 1;
    return inclusive_lower_bound
         + ((*ascending_value)++ % normalized_exclusive_upper_bound);
}


struct rnd *
rnd_alloc_fake_ascending(uint32_t initial_value)
{
    struct rnd *rnd = alloc_with_user_data_size(sizeof(uint32_t));
    if (!rnd) return NULL;
    
    uint32_t *user_initial_value = rnd->user_data;
    *user_initial_value = initial_value;
    rnd->next_uniform_value_in_range = next_fake_ascending_uniform_value_in_range;
    
    return rnd;
}


static uint32_t
next_fake_fixed_uniform_value_in_range(void *user_data,
                                       uint32_t inclusive_lower_bound,
                                       uint32_t inclusive_upper_bound)
{
    uint32_t *fixed_value = user_data;
    uint32_t normalized_exclusive_upper_bound = inclusive_upper_bound
                                              - inclusive_lower_bound
                                              + 1;
    return inclusive_lower_bound + (*fixed_value % normalized_exclusive_upper_bound);
}


struct rnd *
rnd_alloc_fake_fixed(uint32_t fixed_value)
{
    struct rnd *rnd = alloc_with_user_data_size(sizeof(uint32_t));
    if (!rnd) return NULL;
    
    uint32_t *user_fixed_value = rnd->user_data;
    *user_fixed_value = fixed_value;
    rnd->next_uniform_value_in_range = next_fake_fixed_uniform_value_in_range;
    
    return rnd;
}


static uint32_t
next_fake_max_uniform_value_in_range(void *user_data,
                                     uint32_t inclusive_lower_bound,
                                     uint32_t inclusive_upper_bound)
{
    return inclusive_upper_bound;
}


struct rnd *
rnd_alloc_fake_max(void)
{
    return alloc_with_next_uniform_value_in_range(next_fake_max_uniform_value_in_range);
}


static uint32_t
next_fake_median_uniform_value_in_range(void *user_data,
                                        uint32_t inclusive_lower_bound,
                                        uint32_t inclusive_upper_bound)
{
    uint32_t normalized_exclusive_upper_bound = inclusive_upper_bound
                                              - inclusive_lower_bound
                                              + 1;
    return inclusive_lower_bound + (normalized_exclusive_upper_bound / 2);
}


struct rnd *
rnd_alloc_fake_median(void)
{
    return alloc_with_next_uniform_value_in_range(next_fake_median_uniform_value_in_range);
}


static uint32_t
next_fake_min_uniform_value_in_range(void *user_data,
                                     uint32_t inclusive_lower_bound,
                                     uint32_t inclusive_upper_bound)
{
    return inclusive_lower_bound;
}


struct rnd *
rnd_alloc_fake_min(void)
{
    return alloc_with_next_uniform_value_in_range(next_fake_min_uniform_value_in_range);
}


static uint32_t
next_jrand48_value(void *user_data)
{
    unsigned short *state = user_data;
    return (uint32_t)jrand48(state);
}


static uint32_t
next_jrand48_uniform_value(void *user_data, uint32_t normalized_exclusive_upper_bound)
{
    uint32_t modulo_bias = UINT32_MAX % normalized_exclusive_upper_bound;
    uint32_t largest_multiple = UINT32_MAX - modulo_bias;
    uint32_t value;
    do {
        value = next_jrand48_value(user_data);
    } while (value > largest_multiple);
    return value % normalized_exclusive_upper_bound;
}


static uint32_t
next_jrand48_uniform_value_in_range(void *user_data,
                                    uint32_t inclusive_lower_bound,
                                    uint32_t inclusive_upper_bound)
{
    uint32_t normalized_exclusive_upper_bound = inclusive_upper_bound
                                              - inclusive_lower_bound
                                              + 1;
    return inclusive_lower_bound
         + next_jrand48_uniform_value(user_data, normalized_exclusive_upper_bound);
}


struct rnd *
rnd_alloc_jrand48(unsigned short const state[3])
{
    size_t state_size = sizeof(unsigned short[3]);
    
    struct rnd *rnd = alloc_with_user_data_size(state_size);
    if (!rnd) return NULL;
    
    memcpy(rnd->user_data, state, state_size);
    rnd->next_uniform_value_in_range = next_jrand48_uniform_value_in_range;
    
    return rnd;
}


void
rnd_free(struct rnd *rnd)
{
    if (global_rnd == rnd) return;
    if (rnd && rnd->free_user_data) {
        rnd->free_user_data(rnd->user_data);
    }
    free(rnd);
}


uint32_t
rnd_next_value(struct rnd *rnd)
{
    return rnd->next_uniform_value_in_range(rnd->user_data, 0, UINT32_MAX);
}


uint32_t
rnd_next_uniform_value(struct rnd *rnd,
                       uint32_t normalized_exclusive_upper_bound)
{
    if (normalized_exclusive_upper_bound <= 1) return 0;
    return rnd->next_uniform_value_in_range(rnd->user_data,
                                            0,
                                            normalized_exclusive_upper_bound - 1);
}


uint32_t
rnd_next_uniform_value_in_range(struct rnd *rnd,
                                uint32_t inclusive_lower_bound,
                                uint32_t inclusive_upper_bound)
{
    if (inclusive_upper_bound <= inclusive_lower_bound) {
        return inclusive_lower_bound;
    }
    return rnd->next_uniform_value_in_range(rnd->user_data,
                                            inclusive_lower_bound,
                                            inclusive_upper_bound);
}
