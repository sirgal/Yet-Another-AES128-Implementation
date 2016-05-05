#include <stdint.h>

/* multiply byte in a finite field */
uint8_t mul_by_2(  uint8_t input );
uint8_t mul_by_9(  uint8_t input );
uint8_t mul_by_11( uint8_t input );
uint8_t mul_by_13( uint8_t input );
uint8_t mul_by_14( uint8_t input );
/* xors block with a selected key */
void xor_blocks( uint8_t block[4][4], uint8_t keyring[4][4][11], uint8_t key );

/* MULTIPLICATIONS IN A GALOIS FIELD */
uint8_t mul_by_2( uint8_t input )
{
    return (input << 1) ^ (0x1B & (uint8_t)((int8_t) input >> 7));
}
uint8_t mul_by_9( uint8_t input )
{
    /* ((((x*2) * 2) * 2) + x) = 9*x */
    return mul_by_2(mul_by_2(mul_by_2(input))) ^ input;
}
uint8_t mul_by_11( uint8_t input )
{
    /* ((((x*2) * 2) + x) * 2) + x) = 11*x */
    return mul_by_2(mul_by_2(mul_by_2(input)) ^ input) ^ input;
}
uint8_t mul_by_13( uint8_t input )
{
    /* ((((x*2) + x) * 2) * 2) + x) = 13*x */
    return mul_by_2(mul_by_2(mul_by_2(input) ^ input)) ^ input;
}
uint8_t mul_by_14( uint8_t input )
{
    /* ((((x*2) + x) * 2) + x) * 2) = 14*x */
    return mul_by_2(mul_by_2(mul_by_2(input) ^ input) ^ input);
}

void xor_blocks( uint8_t block[4][4], uint8_t keyring[4][4][11], uint8_t key )
{
    uint8_t i, j;

    for( j = 0; j < 4; j++ ) {
        for( i = 0; i < 4; i++ ) {
            block[i][j] = block[i][j] ^ keyring[i][j][key];
        }
    }
}
