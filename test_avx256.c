#include <x86intrin.h>

int
main (void)
{
    __m256i a, b;
    _mm256_clmulepi64_epi128(a, b, 0x00);
    _mm256_shuffle_epi8(a, b);
    return __builtin_cpu_supports ("avx2");
}