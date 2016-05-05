#include <stdint.h>

uint8_t Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

extern uint8_t *s_box;

static uint8_t keyring[4][4][11];

void set_key( uint8_t key[4][4] );
void key_expansion( uint8_t input_key[4][4], uint8_t output_keys[4][4][11] );

void set_key( uint8_t key[4][4] ) {
    key_expansion( key, keyring );
}

void key_expansion( uint8_t input_key[4][4], uint8_t output_keys[4][4][11] )
{
    uint8_t i, j, key;

    /* copy initial key to output buffer */
    for( i = 0; i < 4; i++ ) {
        for( j = 0; j < 4; j++ ) {
            output_keys[i][j][0] = input_key[i][j];
        }
    }

    /* start from 1st key as 0th one is the original */
    for( key = 1; key < 11; key++ ) {
        /* generate 0th column of a new key */
        output_keys[0][0][key] = output_keys[0][0][key-1] ^ s_box[output_keys[1][3][key-1]] ^ Rcon[key];
        output_keys[1][0][key] = output_keys[1][0][key-1] ^ s_box[output_keys[2][3][key-1]];
        output_keys[2][0][key] = output_keys[2][0][key-1] ^ s_box[output_keys[3][3][key-1]];
        output_keys[3][0][key] = output_keys[3][0][key-1] ^ s_box[output_keys[0][3][key-1]];

        /* generate all the other columns */
        for( i = 1; i < 4; i++ ) {
            for( j = 0; j < 4; j++ ) {
                output_keys[j][i][key] = output_keys[j][i-1][key] ^ output_keys[j][i][key-1];
            }
        }
    }
}

void AES128_encrypt( uint8_t block[4][4] )
{
    int8_t i;

    xor_blocks( block, keyring, 0 );

    for( i = 1; i < 10; i++ ) {
        byte_sub( block );
        shift_rows( block );
        mix_columns( block );
        xor_blocks( block, keyring, i );
    }

    byte_sub( block );
    shift_rows( block );
    xor_blocks( block, keyring, 10 );
}

void AES128_decrypt( uint8_t block[4][4] )
{
    int8_t i;

    xor_blocks( block, keyring, 10 );
    inv_shift_rows( block );
    inv_byte_sub( block );

    for( i = 9; i > 0; i-- ) {
        xor_blocks( block, keyring, i );
        inv_mix_columns( block );
        inv_shift_rows( block );
        inv_byte_sub( block );
    }

    xor_blocks( block, keyring, 0 );
}
