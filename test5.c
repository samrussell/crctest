#include "x86intrin.h"
#include <stdio.h>

int main (int argc, char* argv[]) {
    char data[] = "ABCDEFGHIJKLMNOPabcdefghijklmnop";
    __m128i a, b;
    long long unsigned int out;

    // folding by 1; taking N>256 to N-128 bits
    
    a = _mm_set_epi64x(0, 0);
    b = _mm_set_epi64x(0, 0);
    // final reduction
    // folding 128 bits into 64
    // first 32 bits multiplied by x^96modp gives us a 64 bit number we can dump on the end
    a = _mm_set_epi64x(0, 0x44434241);
    // x^96modp = 0x0ccaa009e
    b = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x0ccaa009e), a, 0);
    printf("fold to 96 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(b, 8)), _mm_cvtsi128_si64(b));
    // take second 32 bits and multiply by x^64modp
    // this shifts it along the right amount
    a = _mm_set_epi64x(0, 0x48474645);
    // x^64modp = 0x163cd6124
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x163cd6124), a, 0);
    a = _mm_xor_si128(a, b);
    // then xor in the final 64 bits
    a = _mm_xor_si128(a, _mm_set_epi64x(0, 0x504f4e4d4c4b4a49));
    printf("fold to 64 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    // barrett reduction
    // multiply by mu
    //a = _mm_xor_si128(a, _mm_set_epi64x(0, 0x44434241));
    //printf("xor with dword %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    // DWORDs go backwards so leave bottom 32 bits empty if you want to just do 32 bits
    //a = _mm_set_epi64x(0, 0x4443424100000000);
    //a = _mm_set_epi64x(0, 0x4847464544434241);
    // NOTE: the original paper makes a mistake in the barrett reduction, you can't reduce a 64-bit number with a 32-bit mu
    // you need a 64-bit mu which is x^128/P(x)
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0xB4E5B025F7011641), a, 0x00);
    printf("multiply by mu %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    // then take top 32 bits and multiply by P
    //a = _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff));
    //a = _mm_and_si128(_mm_srli_si128(a, 4), _mm_set_epi64x(0, 0xffffffff));
    //a = _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff));
    a = _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff00000000));
    printf("take 32 bits %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    a = _mm_clmulepi64_si128(_mm_set_epi64x(0, 0x1DB710641), a, 0x00);
    printf("multiply by p %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    printf("CRC32: %08llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)));
    return 0;
}