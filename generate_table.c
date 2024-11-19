#include <stdio.h>

int main(int argc, char* argv[]){
    /* Table of CRCs of all 8-bit messages. */
    unsigned long crc_table[256];

    /* Make the table for a fast CRC. */
    unsigned long c;
    int n, k;
    for (n = 0; n < 2; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 1; k++) {
        if (c & 1) {
            c = 0xedb88320L ^ (c >> 1);
        } else {
            c = c >> 1;
        }
        }
        crc_table[n] = c;
    }

    for (int i = 0; i < 2; i++) {
        printf("0x%08lX, ", crc_table[i]);
    }

    printf("\n");
}