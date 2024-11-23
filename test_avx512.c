#include <x86intrin.h>

int
main (void)
{
    __m512i a, b;
    _mm512_clmulepi64_epi128(a, b, 0x00);
    _mm512_shuffle_epi8(a, b);
    return __builtin_cpu_supports ("avx512f");
}