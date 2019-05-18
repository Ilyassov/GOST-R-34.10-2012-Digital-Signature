#include "../inc/structure.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"

static union {
    uint64_t v;
    u8 a[STATE_SIZE];
} st = {
    .v = 0x05fd8167b327f30ful,
};
static int pos = 0;

static inline uint64_t inv(uint64_t x) {
    if (x) {

        int shift = 0;
        while (x % 2 == 0) {
            ++shift;
            x /= 2;
        }

        uint64_t r = 1;
        uint64_t q = 1 - x;

        for (int i = 0; i < INV_POWER; ++i) {
            r *= 1 + q;
            q *= q;
        }

        return r << shift;
    } else {
        return 0;
    }
}

static uint64_t next_value(uint64_t val) {
    return inv(val) + (uint64_t) 0xc05aa5704efbc79ful;
}

void rand_bytes(u8 *buf, int num) {
    while (num) {

        int l = STATE_SIZE - pos;
        if (l > num) l = num;

        memcpy(buf, &st.a[pos], l);

        buf += l;
        num -= l;
        pos += l;

        if (pos == STATE_SIZE) {
            st.v = next_value(st.v);
            pos = 0;
        }
    }
}

void SEQUENCE::setval(cu8 *from, u8 *to) {
    for (size_t i = 0; i < PARAM_SIZE; i++) {
        to[i] = from[i];
    }
}

void SEQUENCE::setmode(size_t mode) {
    this->mode = mode;
}

void print(cu8 *what, size_t size) {
    for (size_t i = 0; i < size ; i++) {
        printf("%02x", what[i]);
    }
    printf("\n");
}

void SEQUENCE::printStruct() {
    printf("p\n"); print(p, this->mode); printf("a\n"); print(a, this->mode);
    printf("b\n"); print(b, this->mode); printf("e\n"); print(e, this->mode);
    printf("d\n"); print(d, this->mode); printf("m\n"); print(m, this->mode);
    printf("q\n"); print(q, this->mode); printf("x\n"); print(x, this->mode);
    printf("y\n"); print(y, this->mode); printf("u\n"); print(u, this->mode);
    printf("v\n"); print(v, this->mode);
}


SEQUENCE::SEQUENCE(cu8 *P, cu8 *A, cu8 *B,
                   cu8 *E, cu8 *D, cu8 *M,
                   cu8 *Q, cu8 *X, cu8 *Y,
                   cu8 *U, cu8 *V, size_t mode) {
    setval(P, p); setval(A, q); setval(B, b);
    setval(E, e); setval(D, d); setval(M, m);
    setval(Q, q); setval(X, x); setval(Y, y);
    setval(U, u); setval(V, v); setmode(mode);

}

bool Z_iszero(u8 * d, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (d[i]) {
            return false;
        }
    }
    return true;
}

bool Z_l(cu8 * t, cu8 * q, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (t[i] > q[i]) {
            return true;
        } else if (t[i] < q[i]) {
            return false;
        }
    }
    return true;
}

void reverse(u8 *t, size_t mode) {
	size_t size = PARAM_SIZE-1;
	for (size_t i = 0; i < PARAM_SIZE/2; i++) {
		u8 temp = t[i];
		t[i] = t[size-i];
		t[size-i] = temp;
	}
}

void gen_priv_key(
    u8 * d, SEQUENCE *paramSet) {

    u8 t[PARAM_SIZE];
    memset(t, 0, PARAM_SIZE);
    do {
        rand_bytes(t, PARAM_SIZE);
    } while (Z_iszero(t, PARAM_SIZE) or
            Z_l(t, paramSet->q, PARAM_SIZE));
	reverse(t, paramSet->mode);
	memcpy(d, t, paramSet->mode);
}