#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

/* from - https://en.wikipedia.org/wiki/Fletcher%27s_checksum */
static uint16_t Fletcher16( uint8_t *data, int count )
{
   uint16_t sum1 = 0;
   uint16_t sum2 = 0;
   int index;

   for ( index = 0; index < count; ++index ) {
      sum1 = (sum1 + data[index]) % 255;
      sum2 = (sum2 + sum1) % 255;
   }

   return (sum2 << 8) | sum1;
}

extern int errno;

int main(int argc, char *argv[])
{
    if (argc != 2){
        fprintf(stderr, "Fletcher's-16 checksum\n");
        fprintf(stderr, "Usage:fletcher file\n");
        return -1;
    }

    FILE * fid = fopen(argv[1], "r");
    if (!fid) {
        fprintf(stderr, "Error opening file %s:%s.\n", argv[1], strerror(errno));
        return -1;
    }

    int err = fseek(fid, 0L, SEEK_END);
    if (err) {
        fprintf(stderr, "fseek() returned error:%d.\n", err);
        return -1;
    }
    size_t sz = ftell(fid);
    uint8_t * buffer = malloc(sz);
    if (!buffer) {
        fprintf(stderr, "Could not allocate %zu bytes.\n", sz);
        return -1;
    }

    rewind(fid);
    /* read buffer at once! */
    size_t nRead = fread(buffer, 1, sz, fid);
    if (nRead != sz) {
        fprintf(stderr, "File read error - %zu bytes read failed\n", nRead);
    } else {
        fprintf(stdout, "0x%04x\n", Fletcher16(buffer, sz));
    }
    free(buffer);
    return 0;
}
