#include <stdio.h>

int main(int argc, char* argv[]){
    size_t k;
    __uint128_t c, p, q, m;
    c = 0x100000000;
    c = c << 64;
    p = 0x104c11db7;
    p = p << 64;
    m = 0x100000000;
    m = m << 64;
    q = 0;
    for (k = 0; k < 65; k++) {
        q = q << 1;
        printf("q: %016lx%016lx\n", (__uint64_t) (q >> 64), (__uint64_t) (q & 0xFFFFFFFFFFFFFFFF));
        printf("Testing %016lx%016lx %016lx%016lx\n", (__uint64_t) (c >> 64), (__uint64_t) (c & 0xFFFFFFFFFFFFFFFF), (__uint64_t) (p >> 64), (__uint64_t) (p & 0xFFFFFFFFFFFFFFFF));
        if ((c ^ p) < m) {
            printf("bit set, xor\n");
            c = c ^ p;
            q |= 1;
        }
        p = p >> 1;
        m = m >> 1;
    }
    printf("Remainder: %016lx%016lx\n", (__uint64_t) (c >> 64), (__uint64_t) (c & 0xFFFFFFFFFFFFFFFF));
    printf("Quotient: %016lx%016lx\n", (__uint64_t) (q >> 64), (__uint64_t) (q & 0xFFFFFFFFFFFFFFFF));
}