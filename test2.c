#include <stdio.h>

int main (int argc, char* argv[]) {
    // confirming inverse
    __uint32_t input, mid, output;
    // we want to calculate for 0xFFFFFFFF
    // so split into 4 
    /*a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_set_epi64x(0, 0x1F7011641), 0);
    out = _mm_cvtsi128_si64(a);
    printf("combined: %llx\n", out);*/
    input = 0xffffffff;
    mid = input ^ \
          (input << 6) ^ \
          (input << 9) ^ \
          (input << 10) ^ \
          (input << 12) ^ \
          (input << 16) ^ \
          (input << 24) ^ \
          (input << 25) ^ \
          (input << 26) ^ \
          (input << 28) ^ \
          (input << 29) ^ \
          (input << 30) ^ \
          (input << 31);

    printf("middle = %08X\n", (__uint32_t) mid);
    
    output = mid ^ \
          (mid >> 1) ^ \
          (mid >> 2) ^ \
          (mid >> 4) ^ \
          (mid >> 5) ^ \
          (mid >> 7) ^ \
          (mid >> 8) ^ \
          (mid >> 10) ^ \
          (mid >> 11) ^ \
          (mid >> 12) ^ \
          (mid >> 16) ^ \
          (mid >> 22) ^ \
          (mid >> 23) ^ \
          (mid >> 26);

    printf("output = %08X\n", (__uint32_t) output);

    // algorithm is
    // data * quotient
    /*a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1F7011641), _mm_set_epi64x(0, 0x01000000), 0);
    out = _mm_cvtsi128_si64(a);
    printf("first part %llx\n", out);
    b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_set_epi64x(0, 0x41000000), 0);
    //b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), a, 0);
    out = _mm_cvtsi128_si64(b);
    printf("second part %llx\n", out);*/
    return 0;
}