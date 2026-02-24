#include "pony.h"
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

int main(void){
    const size_t SIZE = 1ull << 30;   // 1 GiB
    uint8_t *buf = calloc(SIZE, sizeof(uint8_t));
    if(!buf){
        fprintf(stderr, "alloc failed\n");
        return 1;
    }

    uint8_t key[16] = {0};
    word iv[3] = {0, 0, 0};
    word keys[16];
    pkeyx(key, keys);

    // --- benchmark ---
    uint64_t start = __rdtsc();
    psc(buf, 0, SIZE, key, iv, keys);
    uint64_t end = __rdtsc();

    uint64_t cycles = end - start;
    double cpb = (double)cycles / (double)SIZE;
    double cycles_per_block = (double)cycles / (double)(SIZE / (4 * sizeof(word)));

    printf("Total cycles:        %llu\n", (unsigned long long)cycles);
    printf("Cycles per byte:     %.3f\n", cpb);
    printf("Cycles per %zu-byte block: %.3f\n", (size_t)(4 * sizeof(word)), cycles_per_block);

    // --- write ciphertext to file ---
    FILE *f = fopen("ct", "wb");
    if(!f){
        fprintf(stderr, "failed to open ct\n");
        return 1;
    }
    fwrite(buf, 1, SIZE, f);
    fclose(f);

    free(buf);
    return 0;
}