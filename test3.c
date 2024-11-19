#include "x86intrin.h"
#include <stdio.h>

int main (int argc, char* argv[]) {
    __m128i a, b;
    long long unsigned int out;
    long long unsigned int out_high;
    // confirming inverse
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), _mm_set_epi64x(0, 0xB4E5B025F7011641), 0);
    out = _mm_cvtsi128_si64(a);
    printf("combined: %016llx\n", out);

    // algorithm is
    // data * quotient
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0xB4E5B025F7011641), _mm_set_epi64x(0, 0x1a2a3a4a5a6a7a8a), 0);
    out = _mm_cvtsi128_si64(a);
    out_high = _mm_cvtsi128_si64(_mm_srli_si128(a, 8));
    printf("first part %016llx%016llx\n", out_high, out);
    b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710640), _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff00000000)), 0);
    //b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), a, 0);
    out = _mm_cvtsi128_si64(b);
    printf("second part %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(b, 8)), out);
    printf("CRC: %08llx\n", _mm_cvtsi128_si64(_mm_srli_si128(b, 8)));
    return 0;
}