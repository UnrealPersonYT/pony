#include "pony.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

int main(void){
    const size_t SIZE = 1ull << 30;      // 1 GiB
    const size_t PAGE = 4096;            // 4 KiB pages
    uint8_t key[16] = {0};
    word iv[3] = {0, 0, 0};
    word keys[PONY_ROUNDS];
    pkeyx(key, keys);

    FILE *f = fopen("ct", "wb");
    if(!f){
        fprintf(stderr, "failed to open ct\n");
        return 1;
    }

    uint8_t *buf = malloc(PAGE);
    if(!buf){
        fprintf(stderr, "alloc failed\n");
        fclose(f);
        return 1;
    }
    memset(buf, 0, PAGE);  // initialize plaintext to zeros

    size_t pages = SIZE / PAGE;
    word ctr = 0;
    size_t cycles = 0;

    for(size_t i = 0; i < pages; ++i, ctr += (PAGE / 16)){
        size_t start = __rdtsc();
        psc(buf, ctr, PAGE, key, iv, keys);
        size_t end = __rdtsc();
        cycles += end - start;
        fwrite(buf, 1, PAGE, f);
        memset(buf, 0, PAGE);
    }

    double cpb = (double)cycles / (double)SIZE;
    double cycles_per_block = (double)cycles / (double)(SIZE / (4 * sizeof(word)));

    printf("Total cycles:        %llu\n", (unsigned long long)cycles);
    printf("Cycles per byte:     %.3f\n", cpb);
    printf("Cycles per %zu-byte block: %.3f\n", (size_t)(4 * sizeof(word)), cycles_per_block);

    free(buf);
    fclose(f);
    return 0;
}