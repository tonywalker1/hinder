# hinder::units

Useful conversions.

# Usage

## Memory

Like all programmers, I often have to specify a buffer size. But sometimes, I will make a stupid 
typo in the number. These functions help reduce those errors:
```c++
constexpr size_t KiB(size_t val);
constexpr size_t MiB(size_t val);
constexpr size_t GiB(size_t val);
constexpr size_t TiB(size_t val);
```

These macros do exactly what you expect:
```c++
    // do this...
    buffer = make_buffer(MiB(16));
    // instead of this...
    buffer = make_buffer(16772716);
```
Oh crap! Did you catch the typo? 16 MiB is actually 16777216. **Use this function! It is free 
(constexpr). It also makes the code easier to read and verify.**

One question I get is, "Why KiB. MiB, etc. instead of KB, MB, etc.?" Technically, KB, etc. are 
incorrect. KB, etc. are SI units and, therefore, base 10. KiB, etc. are base 2 and technically 
correct. Yes, everyone uses the wrong terminology. *(shrugs)* 
See [Wikipedia](https://en.wikipedia.org/wiki/Byte#Multiple-byte_units) for a discussion if you 
don't believe me.

## More?

Yes, there will be more to come...
