#include "structure.hpp"

Verylong binaryToDecimal(const u8 *, size_t);
void gen_priv_key(
    u8 * d, SEQUENCE *paramSetA256, void (*randBytes)(u8 *, int ));
void print(cu8 *what, size_t size);
void rand_bytes(u8 *buf, int num);
