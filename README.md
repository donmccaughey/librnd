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
    
    uint32_t number = rnd_next_uniform_value_in_range(rnd, 1, 100);
    // number is in the interval [1, 100]
    
    ...
    rnd_free(rnd);

The `struct rnd` pointer holds information about the pseudorandom number 
generator being used.  The `rnd_alloc()` function returns a `struct rnd` that 
uses `arc4random()` as its generator; it provides high quality pseudorandom 
numbers and is automatically seeded from `/dev/urandom`.

The `rnd_next_uniform_value_in_range()` function produces a pseudorandom number 
uniformly from an inclusive lower bound to an inclusive upper bound.
[Modulo bias][1] is accounted for.

The `rnd_next_uniform_value()` function produces a number uniformly between 
zero (inclusive) and a given exclusive upper bound:

    uint32_t number = rnd_next_uniform_value(rnd, 100);
    // number is in the interval [0, 99]

The `rnd_next_value()` function produces a number between zero (inclusive) and 
`UINT32_MAX` (inclusive):

    uint32_t number = rnd_next_value(rnd);
    // number is in the interval [0, 4_294_967_295]

The `global_rnd` variable points to a predefined instance of `struct rnd` that
uses the arc4 generator.  `global_rnd` doesn't need to be allocated or freed,
so it's useful as a step in refactoring code that uses a function like `rand()`
or `arc4random()` to use _rnd_.

    #include "rnd.h"
    ...

    uint32_t number = rnd_next_uniform_value(global_rnd, 100);
    // number is in the interval [0, 99]


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

The `rnd_alloc_fake_X()` group of functions allocate different types of fake
pseudorandom number generators.

- `rnd_alloc_fake_ascending()`: the returned value increments for each call, 
    from a given starting value; for bounded values, the returned value
    is modulo the range of possible values
- `rnd_alloc_fake_fixed()`: always returns a fixed value, modulo the range of
    possible values
- `rnd_alloc_fake_min()`: always returns the smallest value in the range
- `rnd_alloc_fake_max()`: always returns the largest value in the range
- `rnd_alloc_fake_median()`: always returns the median value of the range;
    for a range with an even number of possible values, returns the higher of
    the two middle values

Here is an example of `rnd_alloc_fake_median()`:

    #include "rnd.h"
    ...

    struct rnd *rnd = rnd_alloc_fake_median();

    uint32_t number1 = rnd_next_uniform_value(rnd, 100);
    // number1 will always be 50

    uint32_t number2 = rnd_next_uniform_value(rnd, 7);
    // number2 will always be 3

    ...
    rnd_free(rnd);


Building rnd
------------

_rnd_ is written in C99 and should build on most modern POSIX compatible
operating systems with a recent GCC or LLVM compiler.

The source contains just two files: `rnd.h` and `rnd.c`. 
The included build system produces a static library, `librnd.a`.

### From Repository Source

You must have GNU Autoconf 2.69 or later and Automate 1.14 or later installed

    $ autoreconf -i
    $ mkdir tmp && cd tmp
    $ ../configure
    $ make
    $ sudo make install

### From Source Distribution Tarball

The `rnd` distribution supports the standard GNU build steps.

    $ mkdir tmp && cd tmp
    $ ../configure
    $ make
    $ sudo make install




[1]: https://en.wikipedia.org/wiki/Fisher–Yates_shuffle#Modulo_bias
