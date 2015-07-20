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

#ifndef RND_H_INCLUDED
#define RND_H_INCLUDED


#include <stdint.h>


struct rnd {
    void *user_data;
    uint32_t (*next_uniform_value_in_range)(void *user_data,
                                            uint32_t inclusive_lower_bound,
                                            uint32_t inclusive_upper_bound);
    void (*free_user_data)(void *user_data);
};


extern struct rnd *const global_rnd;


struct rnd *
rnd_alloc(void);

struct rnd *
rnd_alloc_fake_ascending(uint32_t initial_value);

struct rnd *
rnd_alloc_fake_fixed(uint32_t fixed_value);

struct rnd *
rnd_alloc_fake_max(void);

struct rnd *
rnd_alloc_fake_median(void);

struct rnd *
rnd_alloc_fake_min(void);

struct rnd *
rnd_alloc_jrand48(unsigned short const state[3]);

void
rnd_free(struct rnd *rnd);

uint32_t
rnd_next_value(struct rnd *rnd);

uint32_t
rnd_next_uniform_value(struct rnd *rnd,
                       uint32_t normalized_exclusive_upper_bound);

uint32_t
rnd_next_uniform_value_in_range(struct rnd *rnd,
                                uint32_t inclusive_lower_bound,
                                uint32_t inclusive_upper_bound);


#endif
