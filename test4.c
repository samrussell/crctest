#include <stdio.h>

int main (int argc, char* argv[]) {
    // confirming inverse
    __uint64_t input, mid, output, mid101s;
    // we want to calculate for 0xFFFFFFFF
    // so split into 4 
    /*a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_set_epi64x(0, 0x1F7011641), 0);
    out = _mm_cvtsi128_si64(a);
    printf("combined: %llx\n", out);*/
    input = 0x1234567878563412;
    mid101s = input ^ (input << 2);

    mid = input ^ \
          (input << 6) ^ \
          (input << 9) ^ \
		  (mid101s << 10) ^ \
          (input << 16) ^ \
          (input << 24) ^ \
          (input << 25) ^ \
		  (mid101s << 26) ^ \
		  (mid101s << 29) ^ \
          (input << 30) ^ \
		  (mid101s << 32) ^ \
          (input << 37) ^ \
          (input << 44) ^ \
		  (mid101s << 45) ^ \
		  (mid101s << 48) ^ \
		  (mid101s << 53) ^ \
          (input << 54) ^ \
		  (mid101s << 58) ^ \
		  (mid101s << 61);

    printf("middle = %016lX\n", mid);
    
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

    printf("output = %016lX\n", output);

    printf("crc = %08lx\n", output >> 32);

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