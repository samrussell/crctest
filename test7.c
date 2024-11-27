#include <stdio.h>
#include <arm_neon.h>

int main (int argc, char* argv[]) {
    poly128_t a, b;
    uint64x2_t c;
    long long unsigned int out;
    long long unsigned int out_high;
    // confirming inverse
    a = vmull_p64(vget_lane_p64(vcreate_p64(0x1DB710641), 0), vget_lane_p64(vcreate_p64(0x1F7011641), 0));
    printf("combined: %016lx %016lx\n", vgetq_lane_u64(vreinterpretq_u64_p128(a), 1), vgetq_lane_u64(vreinterpretq_u64_p128(a), 0));

    // algorithm is
    // data * quotient
    a = vmull_p64(vget_lane_p64(vcreate_p64(0x1F7011641), 0), vget_lane_p64(vcreate_p64(0xffffffff), 0));
    printf("first part %016lx %016lx\n", vgetq_lane_u64(vreinterpretq_u64_p128(a), 1), vgetq_lane_u64(vreinterpretq_u64_p128(a), 0));
    // a & 0xffffffff
    // shl = vqshlq_s64()
    // xor is called eor veor_u64()
    c = vbicq_u64(vreinterpretq_u64_p128(a), vcombine_u64(vcreate_u64(0xffffffff00000000), vcreate_u64(0)));
    printf("masked %016lx %016lx\n", vgetq_lane_u64(c, 1), vgetq_lane_u64(c, 0));
    a = vmull_p64(vget_lane_p64(vcreate_p64(0x1DB710641), 0), vreinterpretq_p128_u64(c));
    printf("second part %016lx %016lx\n", vgetq_lane_u64(vreinterpretq_u64_p128(a), 1), vgetq_lane_u64(vreinterpretq_u64_p128(a), 0));
    //printf("CRC: %08llx\n", _mm_cvtsi128_si64(_mm_srli_si128(b, 8)));
    return 0;
}