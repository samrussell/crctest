#include "x86intrin.h"
#include <stdio.h>

int main (int argc, char* argv[]) {
    __m128i a, b;
    long long unsigned int out;
    // confirming inverse
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_set_epi64x(0, 0x1F7011641), 0);
    out = _mm_cvtsi128_si64(a);
    printf("combined: %llx\n", out);

    // algorithm is
    // data * quotient
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1F7011641), _mm_set_epi64x(0, 0xffffffff), 0);
    out = _mm_cvtsi128_si64(a);
    printf("first part %llx\n", out);
    //b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_set_epi64x(0, 0xad00f23f), 0);
    b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff)), 0);
    //b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), a, 0);
    out = _mm_cvtsi128_si64(b);
    printf("second part %llx\n", out);
    return 0;
}