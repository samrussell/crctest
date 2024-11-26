#include "x86intrin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int reduce32(__m128i in) {
    __m128i a, mu, p;
    // multiply by mu
    mu = _mm_set_epi64x(0, 0x1F7011641);
    a = _mm_and_si128(in, _mm_set_epi64x(0, 0xffffffff));
    a = _mm_clmulepi64_si128(a, mu, 0x00);
    //printf("multiply by mu %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    a = _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff));
    // multiply by p
    p = _mm_set_epi64x(0, 0x1DB710641);
    a = _mm_clmulepi64_si128(a, p, 0x00);
    //printf("multiply by p %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    a = _mm_xor_si128(in, a);
    //printf("xor with input %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    a = _mm_srli_si128(a, 4);
    return _mm_cvtsi128_si32(a);
}

__m128i reduce64(__m128i in_high, __m128i in_low) {
    // split up into 32-bit chunks
    __m128i a;
    // multiply by 64-bit shift
    __m128i shift64 = _mm_set_epi64x(0, 0xB8BC6765);
    a = _mm_clmulepi64_si128(shift64, in_high, 0x00);
    a = _mm_xor_si128(a, in_low);
    return a;
}

__m128i reduce128(__m128i in64) {
    // multiply first 64 bits against shift96
    __m128i shift96 = _mm_set_epi64x(0, 0x0ccaa009e);
    __m128i a = _mm_clmulepi64_si128(shift96, in64, 0x00);
    return a;
}

int main (int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: %s data\n", argv[0]);
        return -1;
    }

    int crc = 0;
    void* data = (void*) argv[1];
    int len = strlen(data);
    size_t bytes_remaining = len;
    __m128i a;
    __m128i xor_crc;
    __m128i in1 = {0};
    __m128i in2 = {0};
    __m128i in3 = {0};
    __m128i in4 = {0};
    __m128i in5 = {0};
    __m128i in6 = {0};
    __m128i in7 = {0};
    __m128i in8 = {0};
    __m128i final_buf[8] = {0};

    __m128i shift544_shift480 = _mm_set_epi64x(0x1D9513D7, 0x8F352D95);

    if (bytes_remaining >= 128)
      {
        // fold by 4x128-bits
        in1 = _mm_loadu_si128(data);
        in2 = _mm_loadu_si128(data + (16 * 1));
        in3 = _mm_loadu_si128(data + (16 * 2));
        in4 = _mm_loadu_si128(data + (16 * 3));

        /* Initialise with incoming CRC */
        xor_crc = _mm_set_epi32 (0, 0, 0, crc);
        in1 = _mm_xor_si128 (in1, xor_crc);

        while (bytes_remaining >= 128) {
            in5 = _mm_loadu_si128(data + (16 * 4));
            in6 = _mm_loadu_si128(data + (16 * 5));
            in7 = _mm_loadu_si128(data + (16 * 6));
            in8 = _mm_loadu_si128(data + (16 * 7));

            // now we fold in1 onto in2
            __m128i fold_high1 = _mm_clmulepi64_si128(in1, shift544_shift480, 0x11);
            __m128i fold_low1 = _mm_clmulepi64_si128(in1, shift544_shift480, 0x00);
            in1 = _mm_xor_si128(in5, fold_high1);
            in1 = _mm_xor_si128(in1, fold_low1);
            __m128i fold_high2 = _mm_clmulepi64_si128(in2, shift544_shift480, 0x11);
            __m128i fold_low2 = _mm_clmulepi64_si128(in2, shift544_shift480, 0x00);
            in2 = _mm_xor_si128(in6, fold_high2);
            in2 = _mm_xor_si128(in2, fold_low2);
            __m128i fold_high3 = _mm_clmulepi64_si128(in3, shift544_shift480, 0x11);
            __m128i fold_low3 = _mm_clmulepi64_si128(in3, shift544_shift480, 0x00);
            in3 = _mm_xor_si128(in7, fold_high3);
            in3 = _mm_xor_si128(in3, fold_low3);
            __m128i fold_high4 = _mm_clmulepi64_si128(in4, shift544_shift480, 0x11);
            __m128i fold_low4 = _mm_clmulepi64_si128(in4, shift544_shift480, 0x00);
            in4 = _mm_xor_si128(in8, fold_high4);
            in4 = _mm_xor_si128(in4, fold_low4);

            bytes_remaining -= 64;
            data += 64;
        }
        
        _mm_storeu_si128(final_buf, in1);
        _mm_storeu_si128(final_buf + 1, in2);
        _mm_storeu_si128(final_buf + 2, in3);
        _mm_storeu_si128(final_buf + 3, in4);
      }
    
    /* move everything to final_buf because it is RW */

    memcpy(final_buf + 4, data + 64, bytes_remaining - 64);
    data = final_buf;

    __m128i shift160_shift96 = _mm_set_epi64x(0xCCAA009E, 0xAE689191);

    while (bytes_remaining >= 32) {
        // fold by 128-bits
        __m128i in1 = _mm_loadu_si128(data);
        __m128i in2 = _mm_loadu_si128(data + 16);
        //printf("in1 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(in1, 8)), _mm_cvtsi128_si64(in1));
        //printf("in2 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(in2, 8)), _mm_cvtsi128_si64(in2));

        // now we fold in1 onto in2
        __m128i fold_high = _mm_clmulepi64_si128(in1, shift160_shift96, 0x11);
        __m128i fold_low = _mm_clmulepi64_si128(in1, shift160_shift96, 0x00);
        in2 = _mm_xor_si128(in2, fold_high);
        in2 = _mm_xor_si128(in2, fold_low);

        _mm_storeu_si128(data + 16, in2);
        bytes_remaining -= 16;
        data += 16;
    }

    // we have 16-31 bytes here
    // if we have 17-31 then we do another special case 128-bit fold
    // the padding trick works because we're effectively padding 0s on the front
    // which means with little endian we're shifting the number higher
    // we'll also use this step to pick the <16 byte case

    if (bytes_remaining != 16){
        // pad remainder and fold 128-bits
        // we're reading in up to 32 bytes here = 256 bits
        // this is super inefficient so we only actually want to hit this on the actual end of data
        // if we're reading a multiple of 32 bytes in the loop then this will never get hit
        char in256[32] = {0};

        // read in at an offset so we get the shift for free
        memcpy(in256 + (32 - bytes_remaining), data, bytes_remaining);
        __m128i in1 = _mm_loadu_si128((void*)in256);
        __m128i in2 = _mm_loadu_si128((void*)in256 + 16);
        //printf("in1 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(in1, 8)), _mm_cvtsi128_si64(in1));
        //printf("in2 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(in2, 8)), _mm_cvtsi128_si64(in2));

        // now we fold in1 onto in2
        __m128i shift160_shift96 = _mm_set_epi64x(0x0ccaa009e, 0x1751997d0);
        __m128i fold_high = _mm_clmulepi64_si128(in1, shift160_shift96, 0x11);
        __m128i fold_low = _mm_clmulepi64_si128(in1, shift160_shift96, 0x00);
        in2 = _mm_xor_si128(in2, fold_high);
        a = _mm_xor_si128(in2, fold_low);
        //printf("folded in2 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(a, 8)), _mm_cvtsi128_si64(a));
    }
    else {
        a = _mm_loadu_si128(data);
    }

    // we now have 16 bytes and fold as normal
    
    __m128i in64 = _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffffffffffff));
    __m128i in_high = _mm_and_si128(_mm_srli_si128(a, 8), _mm_set_epi64x(0, 0xffffffff));
    __m128i in_low = _mm_and_si128(_mm_srli_si128(a, 12), _mm_set_epi64x(0, 0xffffffff));
    //printf("in64 %016llx%016llx\n", _mm_cvtsi128_si64(_mm_srli_si128(in64, 8)), _mm_cvtsi128_si64(in64));
    //printf("in_high %016llx\n", _mm_cvtsi128_si64(in_high));
    //printf("in_low %016llx\n", _mm_cvtsi128_si64(in_low));
    a = reduce128(in64);
    // first 32 bits go on in_high
    in_high = _mm_xor_si128(in_high, _mm_and_si128(a, _mm_set_epi64x(0, 0xffffffff)));
    // next 64 bits go on in_low
    in_low = _mm_xor_si128(in_low, _mm_srli_si128(a, 4));
    // then do 64 from here
    a = reduce64(in_high, in_low);
    crc = reduce32(a);
    printf("CRC: %08x\n", crc);


    return 0;
}