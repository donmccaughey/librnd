rnd
===

A pseudorandom number generator abstraction for C.


License
-------
_rnd_ is made available under a BSD-style license; see the LICENSE file for 
details.


Uniform Pseudorandom Integers
-----------------------------
_rnd_ generates pseudorandom 32 bit unsigned integers uniformly over a given 
range.  Here is the basic usage pattern:

    #include "rnd.h"
    ...
    
    struct rnd *rnd = rnd_alloc();
    
    uint32_t number = rnd_next_uniform_value(rnd, 100);
    // number is in the interval [0, 99]
    
    ...
    rnd_free(rnd);

The `struct rnd` pointer holds information about the pseudorandom number 
generator being used.  The `rnd_alloc()` function returns a `struct rnd` that 
uses `arc4random_uniform()` as its generator; it provides high quality 
pseudorandom numbers and is automatically seeded from `/dev/urandom`.

The `rnd_next_uniform_value()` function produces a pseudorandom number 
uniformly from zero (inclusive) to the given upper bound (exclusive).
[Modulo bias][1] is accounted for.


Alternate Generators
--------------------
When testing or debugging, it can be useful to have control over the 
pseudorandom numbers being generated.  _rnd_ provides two alternate generators.


### jrand48

The `jrand48()` function generates a 32 bit pseudorandom integer using 
generator state provided by the caller.  The `rnd_alloc_jrand48()` function
creates a `struct rnd` pointer that uses `jrand48()` and the provided initial 
state.  `rnd_alloc_jrand48()` is great when you want a "normal" stream of
pseudorandom numbers, but you want the same stream every time.

    #include "rnd.h"
    ...

    struct rnd *rnd = rnd_alloc_jrand48((unsigned short[]){2, 3, 5});

    uint32_t number1 = rnd_next_uniform_value(rnd, 100);
    // number1 will always be 56

    uint32_t number2 = rnd_next_uniform_value(rnd, 7);
    // number2 will always be 1

    ...
    rnd_free(rnd);


### fake

The `rnd_alloc_fake()` function allocates one of three different types of fake
pseudorandom number generators.  When called with `rnd_fake_type_min`, 
`rnd_next_uniform_value()` will always return zero for that `struct rnd` 
pointer.  When called with `rnd_fake_type_max`, `rnd_next_uniform_value()` will
always return the given upper bound minus one.  The `rnd_fake_type_median` 
option always returns the given upper bound divided by two, rounded down.

    #include "rnd.h"
    ...

    struct rnd *rnd = rnd_alloc_fake(rnd_fake_type_median);

    uint32_t number1 = rnd_next_uniform_value(rnd, 100);
    // number1 will always be 50

    uint32_t number2 = rnd_next_uniform_value(rnd, 7);
    // number2 will always be 3

    ...
    rnd_free(rnd);




[1]: https://en.wikipedia.org/wiki/Fisher–Yates_shuffle#Modulo_bias
