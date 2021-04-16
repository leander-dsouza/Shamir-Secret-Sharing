/*
 * A simplistic implementation of the SSS algorithm
 */

#include "galois_256.h"
#include "time.h"


// Encrypting Functions

u_int8_t * generate_constrained_x(u_int8_t total_shares){
    /*
     * One Time Function Call to generate X between [0, 255]
     * return: Array of X coordinates
     */
    u_int8_t *x_arr = malloc(total_shares * sizeof(u_int8_t));
    for (int i = 0; i < total_shares; i++)
        x_arr[i] = (rand() % 255) + 1;

    return x_arr;
}

u_int8_t * coefficients(u_int8_t secret_number, u_int8_t essential_shares){
    /*
     * Generate coefficient array for polynomial call
     * return: Array of coefficient constants
     */
    u_int8_t *coefficient_matrix = malloc(essential_shares * sizeof(u_int8_t));

    for (int i = 0; i <= essential_shares - 1; i++)
    {
        coefficient_matrix[i] = (rand() % 255) + 1;

        if (i == essential_shares - 1)
            coefficient_matrix[i] = secret_number;
    }
    return coefficient_matrix;
}

u_int8_t ** polynomial(const u_int8_t *x_arr, u_int8_t secret_size,
                       u_int8_t secret_number, u_int8_t essential_shares, u_int8_t total_shares){
    /*
     * Polynomial having degree = total_shares - 1, and roots = essential_shares
     * return: Matrix containing shares to be distributed
     */
    u_int8_t **shares = malloc(total_shares * sizeof(u_int8_t *));

    u_int8_t *coefficient_matrix;
    coefficient_matrix = coefficients(secret_number, essential_shares);

    for (int i = 0; i < total_shares; i++)
    {
        shares[i] = malloc((secret_size + 1) * sizeof(u_int8_t));
        u_int8_t x = x_arr[i];
        u_int8_t y = 0;

        // For Y
        for (int c_idx = 0; c_idx < essential_shares; c_idx ++)
            y = galois_256_add(y, galois_256_mul(galois_256_power(x, essential_shares - c_idx - 1), coefficient_matrix[c_idx]));

        shares[i][0] = x;
        shares[i][1] = y;
    }
    return shares;
}

u_int8_t **share_generator(u_int8_t secret_number, u_int8_t * x_arr,
                           u_int8_t secret_size, u_int8_t essential_shares, u_int8_t total_shares){
    /*
     * return: Generates and returns shares' matrix
     */
    u_int8_t **shares;
    shares = polynomial(x_arr, secret_size, secret_number, essential_shares, total_shares);
    return shares;
}


char * arr_to_hex_str(u_int8_t *arr, int arr_size) {
    /*
     * return: Generates a hex string from a 1D array
     */
    char *out = malloc(2 * arr_size + 1);
    for (int pos = 0; pos < arr_size; pos++) {
        sprintf(out + 2*pos, "%02x", arr[pos]);
    }
    out[2 * arr_size + 1] = 0x00;
    return out;
}

char * encrypt(const char *secret, u_int8_t secret_size, u_int8_t essential_shares, u_int8_t total_shares){
    /*
     * return: Encryption Keys of the Shamir Secret Sharing Scheme
     */
    u_int8_t dummy = 0;
    u_int8_t **shares;
    u_int8_t *x_arr = generate_constrained_x(total_shares);
    u_int8_t combined_arr[secret_size][total_shares][2];
    u_int8_t check_arr[secret_size + 1];
    char *keys = (char *)malloc(total_shares * 32 * sizeof(char));


    for (int i = 0; i < secret_size; i++)
    {
        dummy = secret[i];
        shares = share_generator(dummy, x_arr, secret_size, essential_shares, total_shares);

        for (int j = 0; j < total_shares; j++)
        {
            combined_arr[i][j][0] = shares[j][0];
            combined_arr[i][j][1] = shares[j][1];
        }

    }


    for (int j = 0; j < total_shares; j++)
    {
        for (int i = 0; i < secret_size; i++)
        {
            if (i ==0)
            {
                check_arr[i] = combined_arr[i][j][0];
                check_arr[i + 1] = combined_arr[i][j][1];
            }
            else
                check_arr[i + 1] = combined_arr[i][j][1];
        }

        strcat(keys, arr_to_hex_str(check_arr, secret_size + 1));
        strcat(keys, "\n");
    }
    return keys;
}




// Decryption

u_int8_t lagrangeInterpolation(u_int8_t *x_decrypted, u_int8_t *y_decrypted, int k) {
    /*
     * Function implementing elimination through differentiation and applying zero, to get coefficients
     * return: Y coordinate
     */
    u_int8_t y_lagrange = 0;

    for (int j = 0; j < k; j++) {
        u_int8_t prod = 0x01;
        for (int m = 0; m < k; m++) {
            if (m != j) {
                prod = galois_256_mul(prod, galois_256_div(x_decrypted[m], galois_256_add(x_decrypted[m], x_decrypted[j])));
            }
        }
        y_lagrange = galois_256_add(y_lagrange, galois_256_mul(y_decrypted[j], prod));
    }
    return y_lagrange;
}


u_int8_t * hex_str_to_arr(const char *s) {
    /*
     * Inverse to arr_to_hex_str, generates integer array from hex string
     */

    u_int8_t *res = malloc(strlen(s) * sizeof(u_int8_t));
    char buff[3] = {0x00, 0x00, 0x00};
    for (int pos = 0; pos < strlen(s); pos++) {
        strncpy(buff, s + pos*2, 2);
        res[pos] = strtoul(buff, NULL, 16);
    }
    return res;
}



u_int8_t * decrypt(u_int8_t **shares, int secret_size, int k) {
    /*
     * Main decryption function
     */
    u_int8_t *secret = malloc(secret_size * sizeof(u_int8_t));

    for (int i = 1; i <= secret_size; i++) {
        u_int8_t *x_decrypted = (u_int8_t *) malloc(k * sizeof(u_int8_t));
        u_int8_t *y_decrypted = (u_int8_t *) malloc(k * sizeof(u_int8_t));
        for (int j = 0; j < k; j++) {
            x_decrypted[j] = shares[j][0];
            y_decrypted[j] = shares[j][i];

            secret[i-1] = lagrangeInterpolation(x_decrypted, y_decrypted, k);
        }
    }

    return secret;
}

