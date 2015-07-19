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

#include <assert.h>
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


static bool
is_valid_fake_type(enum rnd_fake_type type)
{
    switch (type) {
        case rnd_fake_type_min: return true;
        case rnd_fake_type_median: return true;
        case rnd_fake_type_max: return true;
        default: return false;
    }
}


static uint32_t
next_fake_uniform_value(void *user_data, uint32_t upper_bound)
{
    enum rnd_fake_type *type = user_data;
    switch (*type) {
        case rnd_fake_type_min: return 0;
        case rnd_fake_type_median: return upper_bound / 2;
        case rnd_fake_type_max: return upper_bound - 1;
        default: return 0;
    }
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
    struct rnd *rnd = calloc(1, sizeof(struct rnd));
    if (!rnd) return NULL;
    
    rnd->next_uniform_value = next_arc4_uniform_value;
    
    return rnd;
}


struct rnd *
rnd_alloc_fake(enum rnd_fake_type type)
{
    assert(is_valid_fake_type(type));
    if (!is_valid_fake_type(type)) {
        errno = EINVAL;
        return NULL;
    }
    
    struct rnd *rnd = malloc(sizeof(struct rnd));
    if (!rnd) return NULL;
    
    enum rnd_fake_type *user_type = malloc(sizeof(enum rnd_fake_type));
    if (!user_type) {
        free(rnd);
        return NULL;
    }
    
    *user_type = type;
    rnd->user_data = user_type;
    rnd->next_uniform_value = next_fake_uniform_value;
    rnd->free_user_data = free;
    
    return rnd;
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
    assert(rnd);
    if (!rnd) return 0;
    
    assert(upper_bound);
    if (!upper_bound) return 0;
    
    assert(rnd->next_uniform_value);
    if (!rnd->next_uniform_value) return 0;
    
    return rnd->next_uniform_value(rnd->user_data, upper_bound);
}
