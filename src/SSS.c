/*
 * A compact implementation using custom headers for the SSS algorithm
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "shamir.h"

int main(int argc, char *argv[])
{
    // Passed argument is encryption
    if (strcmp(argv[1], "encrypt") == 0)
    {
        srand((unsigned) time(NULL));
        u_int8_t essential_shares = 2;
        u_int8_t total_shares = 4;
        char secret[32];
        char * keys;

        printf("Enter string: ");

        fgets(secret, sizeof secret, stdin);
        u_int8_t secret_size = strlen(secret);

        keys = encrypt(secret, secret_size, essential_shares, total_shares);
        printf("KEYS = \n%s", keys);

    }

    // Passed argument is decryption
    else if (strcmp(argv[1], "decrypt") == 0)
    {
        int essential_shares = argc - 2;
        int secret_size = strlen(argv[2]) - 1;
        u_int8_t **shares = (u_int8_t **) malloc(essential_shares * sizeof(u_int8_t *));

        for (int i=0; i < essential_shares; i++)
            shares[i] = hex_str_to_arr(argv[i + 2]);

        u_int8_t *message = decrypt(shares, secret_size, essential_shares);

        printf("\nDecoded String: %s\n", (char *) message);
    }

    // Default argument
    else {
        printf(
                "Shamir Secret Sharing Scheme Implementation: SSS\n\n"
                "1. Split a code in 4 shares, so 2 are needed to reconstruct it:\n"
                "$ SSS encrypt\n\n"
                "2. Join k shares to decrypt the original message::\n"
                "$ SSS decrypt SHARE_1 SHARE_2 ... SHARE_K\n\n"
        );
    }


    return 0;
}
