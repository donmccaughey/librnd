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
next_arc4_uniform_value(void *user_data, uint32_t upper_bound);


struct rnd *const global_rnd = &((struct rnd){
    .user_data=NULL,
    .next_uniform_value=next_arc4_uniform_value,
    .free_user_data=NULL,
});


static struct rnd *
alloc_with_next_uniform_value(uint32_t (*next_uniform_value)(void *, uint32_t))
{
    struct rnd *rnd = calloc(1, sizeof(struct rnd));
    if (!rnd) return NULL;
    
    rnd->next_uniform_value = next_uniform_value;
    
    return rnd;
}


static uint32_t
next_jrand48_uniform_value(void *user_data, uint32_t upper_bound)
{
    unsigned short *state = user_data;
    uint32_t modulo_bias = UINT32_MAX % upper_bound;
    uint32_t largest_multiple = UINT32_MAX - modulo_bias;
    uint32_t value;
    do {
        value = (uint32_t)jrand48(state);
    } while (value > largest_multiple);
    
    return value % upper_bound;
}


static uint32_t
next_arc4_uniform_value(void *user_data, uint32_t upper_bound)
{
    return arc4random_uniform(upper_bound);
}


struct rnd *
rnd_alloc(void)
{
    return alloc_with_next_uniform_value(next_arc4_uniform_value);
}


static uint32_t
next_fake_max_uniform_value(void *user_data, uint32_t upper_bound)
{
    return upper_bound - 1;
}


struct rnd *
rnd_alloc_fake_max(void)
{
    return alloc_with_next_uniform_value(next_fake_max_uniform_value);
}


static uint32_t
next_fake_median_uniform_value(void *user_data, uint32_t upper_bound)
{
    return upper_bound / 2;
}


struct rnd *
rnd_alloc_fake_median(void)
{
    return alloc_with_next_uniform_value(next_fake_median_uniform_value);
}


static uint32_t
next_fake_min_uniform_value(void *user_data, uint32_t upper_bound)
{
    return 0;
}


struct rnd *
rnd_alloc_fake_min(void)
{
    return alloc_with_next_uniform_value(next_fake_min_uniform_value);
}


struct rnd *
rnd_alloc_jrand48(unsigned short const state[3])
{
    struct rnd *rnd = malloc(sizeof(struct rnd));
    if (!rnd) return NULL;
    
    size_t state_size = sizeof(unsigned short[3]);
    unsigned short *user_state = malloc(state_size);
    if (!user_state) {
        free(rnd);
        return NULL;
    }
    
    memcpy(user_state, state, state_size);
    rnd->user_data = user_state;
    rnd->next_uniform_value = next_jrand48_uniform_value;
    rnd->free_user_data = free;
    
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
rnd_next_uniform_value(struct rnd *rnd, uint32_t upper_bound)
{
    if (!upper_bound) return 0;
    return rnd->next_uniform_value(rnd->user_data, upper_bound);
}
