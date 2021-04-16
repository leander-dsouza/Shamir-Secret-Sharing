/*
 * A simplistic implementation of basic arithmetic operations of gf(256)
 */

#define IRREDUCIBLE_POLY 0x011b
u_int8_t **MULTIPLICATIVE_INVERSE_TABLE = NULL;


u_int8_t galois_256_add(u_int8_t a, u_int8_t b) {
    /*
     * gf(256) addition
     */
    return a ^ b;
}


u_int8_t time_x(u_int8_t a) {
    /*
     * Multiply any given polynomial with x
     */
    if ((a >> 7) & 0x1) {
        return (a << 1) ^ IRREDUCIBLE_POLY;
    } else {
        return (a << 1);
    }
}

u_int8_t time_x_power(u_int8_t a, u_int8_t x_power) {
    /*
     * Multiply any polynomial with a defined power of x
     */
    u_int8_t res = a;
    for (; x_power > 0; x_power--) {
        res = time_x(res);
    }
    return res;
}

u_int8_t galois_256_mul(u_int8_t a, u_int8_t b) {
    /*
     * Multiply two polynomials in gf(256)
     */
    u_int8_t res = 0;
    for (int degree = 7; degree >= 0; degree--) {
        if ((b >> degree) & 0x1) {
            res = galois_256_add(res, time_x_power(a, degree));
        }
    }
    return res;
}

u_int8_t galois_256_power(u_int8_t a, u_int8_t b){
    /*
     * Multiply two polynomials in gf(256) with a specified power
     */
    u_int8_t term = 0x01;

    for (int times = b; times > 0; times--)
        term = galois_256_mul(term, a);

    return term;
}

u_int8_t galois_256_inv(u_int8_t a) {
    /*
     * gf(256) multiplicative inverse table generation
     */

    if (MULTIPLICATIVE_INVERSE_TABLE == NULL) {
        MULTIPLICATIVE_INVERSE_TABLE = (u_int8_t **) malloc(256 * sizeof(u_int8_t *));
        for (int row = 0; row < 256; row++) {
            MULTIPLICATIVE_INVERSE_TABLE[row] = (u_int8_t *) malloc(256 * sizeof(u_int8_t));

            for (int col = 0; col < 256; col++)
                MULTIPLICATIVE_INVERSE_TABLE[row][galois_256_mul(row, col)] = col;

        }
    }
    return MULTIPLICATIVE_INVERSE_TABLE[a][1];
}


u_int8_t galois_256_div(u_int8_t a, u_int8_t b) {
    /*
     * Division of two polynomials in gf(256)
     */
    return galois_256_mul(a, galois_256_inv(b));
}
