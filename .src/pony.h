/// @file  pony.h
/// @brief Contains the Pony cipher
#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint32_t word;

/// @brief Pony implementation of arx on words
static inline void parxw(word a[1], word b[1], word c[1], word d[1], const word key){
    *a += (*b >> 16) | (*b << 16);
    *c ^= *d ^ key;
    *d += (*a >> 8) | (*a << 24);
    *b ^= *c;
}
/// @brief Completes a Pony block given round keys
static inline void pblck(word block[4], const word rkeys[16]){
    for(size_t r = 0; r < 16; ++r)
        parxw(block, &block[1], &block[2], &block[3], rkeys[r]);
}
/// @brief Pony implementation of a key expansion
static inline void pkeyx(const uint8_t key[16], word out[16]){
    out[0] = 0x13524EB7 + ((uint32_t*)key)[0] + ((uint32_t*)key)[1] + ((uint32_t*)key)[2] + ((uint32_t*)key)[3];
    word block[4] = {0};
    for(size_t r = 1; r < 16; ++r){
        parxw(block, &block[1], &block[2], &block[3], out[r - 1] + r);
        out[r] = block[0];
    }
}
/// @brief The Pony stream cipher
void* psc(void* const __restrict buf, size_t ctr, const size_t len, const uint8_t key[16], const word iv[3], const word keys[16]){
    for(size_t i = 0; i < len; i += 16, ++ctr){
        word block[4] = {iv[0], iv[1], iv[2], ctr};
        pblck(block, keys);
        for(size_t k = 0; k < 16; ++k)
            ((uint8_t*)block)[k] ^= key[k];
        const size_t limit = ((len - i) > 16) ? 16: (len - i);
        for(size_t x = 0; x < limit; ++x)
            ((uint8_t*)buf)[i + x] ^= ((uint8_t*)block)[x];
    }
    return buf;
}