/// @file  pony.h
/// @brief Contains the Pony cipher
#pragma once
#include <stdint.h>
#include <stddef.h>

typedef uint32_t word;

#define PONY_KSIZE  16
#define PONY_NSIZE  3
#define PONY_ROUNDS 16
#define PONY_WORDBY sizeof(word)
#define PONY_WORDBI (PONY_WORDBY * 8)

/// @brief Pony implementation of half rol word
static inline word prolhw(word x){
    return (x << (PONY_WORDBI / 2)) | (x >> (PONY_WORDBI / 2));
}
/// @brief Pony implementation of quater rol word
static inline word prolqw(word x){
    return (x << (PONY_WORDBI / 4)) | (x >> (PONY_WORDBI - (PONY_WORDBI / 4)));
}
/// @brief Pony implementation of arx on words
static inline void parxw(word a[1], word b[1], word c[1], word d[1], const word key){
    *a += prolhw(*b);
    *c ^= *d ^ key;
    *d += prolqw(*a);
    *b ^= *c;
}
/// @brief Completes a Pony block given round keys
static inline void pblck(word block[4], const word rkeys[PONY_ROUNDS]){
    for(size_t r = 0; r < PONY_ROUNDS; ++r)
        parxw(block, &block[1], &block[2], &block[3], rkeys[r]);
}
/// @brief Pony implementation of a key expansion
static inline void pkeyx(const uint8_t key[PONY_KSIZE], word out[PONY_ROUNDS]){
    out[0] = 0x13524EB7 + ((uint32_t*)key)[0] + ((uint32_t*)key)[1] + ((uint32_t*)key)[2] + ((uint32_t*)key)[3];
    word block[4] = {0};
    for(size_t r = 1; r < PONY_ROUNDS; ++r){
        parxw(block, &block[1], &block[2], &block[3], out[r - 1] + r);
        out[r] = block[0];
    }
}
/// @brief The Pony stream cipher
void* psc(void* const __restrict buf, uint32_t ctr, const size_t len, const uint8_t key[PONY_KSIZE], const word iv[PONY_NSIZE], const word keys[PONY_ROUNDS]){
    for(size_t i = 0; i < len; i += (4 * PONY_WORDBY), ++ctr){
        word block[4] = {iv[0], iv[1], iv[2], ctr};
        pblck(block, keys);
        for(size_t k = 0; k < PONY_KSIZE; ++k)
            ((uint8_t*)block)[k] ^= key[k];
        const size_t limit = ((len - i) > (4 * PONY_WORDBY)) ? (4 * PONY_WORDBY): (len - i);
        for(size_t x = 0; x < limit; ++x)
            ((uint8_t*)buf)[i + x] ^= ((uint8_t*)block)[x];
    }
    return buf;
}