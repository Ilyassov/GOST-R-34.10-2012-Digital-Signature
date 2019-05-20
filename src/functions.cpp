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

void print(cu8 *what, size_t size) {
    for (size_t i = 0; i < size ; i++) {
        printf("%02x", what[i]);
    }
    printf("\n");
}

void SEQUENCE::printStruct() {
    std::cout << "p\n" << std::hex << p << std::endl;
    std::cout << "a\n" << std::hex << a << std::endl;
    std::cout << "b\n" << std::hex << b << std::endl;
    std::cout << "e\n" << std::hex << e << std::endl;
    std::cout << "d\n" << std::hex << d << std::endl;
    std::cout << "m\n" << std::hex << m << std::endl;
    std::cout << "q\n" << std::hex << q << std::endl;
    std::cout << "x\n" << std::hex << x << std::endl;
    std::cout << "y\n" << std::hex << y << std::endl;
    std::cout << "u\n" << std::hex << u << std::endl;
    std::cout << "v\n" << std::hex << v << std::endl;
}

SEQUENCE::SEQUENCE(uint1024_t P, uint1024_t A, uint1024_t B, uint1024_t E,
                   uint1024_t D, uint1024_t M, uint1024_t Q, uint1024_t X,
                   uint1024_t Y, uint1024_t U, uint1024_t V, size_t MODE){
    p = P;  a = A;  b = B;  e = E;  d = D;
    m = M;  q = Q;  x = X;  y = Y;  u = U;
    v = V;  mode = MODE;
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
    uint1024_t T = 0;
    uint1024_t q = (paramSet->q) << 256;
    memset(t, 0, PARAM_SIZE);
    do {
        rand_bytes(t, PARAM_SIZE);
        T = init_1024(t, PARAM_SIZE);
    } while (T == 0 or T >= q);
    reverse(t, paramSet->mode);
	memcpy(d, t, paramSet->mode);
}

uint1024_t init_1024(u8 *a, size_t size) {
    uint1024_t res = 0;
    for (size_t i = 0; i < size; i++) {
        res = (res << 8) + (uint)a[i];
    }
    return res;
}

Point::Point(uint1024_t x, uint1024_t y) {
    this->x = x;
    this->y = y;
}

std::ostream& operator << (std::ostream &s, Point p) {
    s << p.x << "\n" << p.y;
    return s;
}


///////////////////////////////////////////////////////////////////////////////

Point sum_p(Point p1, Point p2, uint1024_t p, uint1024_t d, uint1024_t a) {
    uint1024_t x1y2 = (p1.x * p2.y) % p;
    uint1024_t y1x2 = (p1.y * p2.x) % p;
    uint1024_t x1y2Sy1x2 = (x1y2 + y1x2) % p;
    uint1024_t y1y2 = (p1.y * p2.y) % p;
    uint1024_t x1x2 = (p1.x * p2.x) % p;
    uint1024_t ax1x2 = (a * x1x2) % p;
    uint1024_t y1y2Max1x2 = (y1y2 + negative(ax1x2, p)) % p;
    uint1024_t dx1x2 = (d * x1x2) % p;
    uint1024_t dx1x2y1y2 = (dx1x2 * y1y2) % p;
    uint1024_t res1 = x1y2Sy1x2 * inverse((1 + dx1x2y1y2) % p, p);    
    uint1024_t res2 = y1y2Max1x2 * inverse((1 + negative(dx1x2y1y2, p)) %p, p);
    return Point(res1 % p, res2 % p);
}

uint1024_t negative(uint1024_t a, uint1024_t p) {
    return p - (a % p);
}

uint1024_t inverse(uint1024_t a, uint1024_t p) {
	uint1024_t res = 1;
    uint1024_t one = 1;
    uint1024_t n = p - 2;

    while (n != 0) {
		if ((n & one) == one) {
			res *= a;
            res %= p;
			--n;
		}
		else {
			a *= a;
            a %= p;
			n >>= 1;
		}
    }
	return res % p;
}

Point operator+ (Point a, Point b) {
    return Point(a.x + b.x, a.y + b.y);
}


Point muL(Point x, uint1024_t k, uint1024_t p, uint1024_t d, uint1024_t a) {
    Point res(0, 1);

    Point pows[8 * PARAM_SIZE];
    pows[0] = x;

    for (size_t i = 1; i < 8 * PARAM_SIZE; ++i) {
        pows[i] = sum_p(pows[i - 1], pows[i - 1], p, d, a);
    }
    for (size_t i = 0; i < 8 * PARAM_SIZE; ++i) {
        if ((k >> i) & 1) {
            res = sum_p(res, pows[i], p, d, a);
        }
    }

    // Point point = x;

    // while (k != 0) {
    //     if (k & 1) {
    //         res = sum_p(res, point, p, d, a);
    //         --k;
    //     } else {
    //         point = sum_p(point, point, p, d, a);
    //         k >>= 1;
    //     }
    // }

    return res;
}

Point mul(SEQUENCE* paramSet, uint1024_t k) {
    uint1024_t u = paramSet->u;
    uint1024_t v = paramSet->v;
    uint1024_t p = paramSet->p;
    uint1024_t d = paramSet->d;
    uint1024_t a = paramSet->a;
    return muL(Point(u, v), k, p, d, a);
}

Point convert_uv_to_xy(SEQUENCE* paramSet, Point uv) {
    uint1024_t e = paramSet->e;
    uint1024_t d = paramSet->d;
    uint1024_t p = paramSet->p;
    uint1024_t eMd = (e + negative(d, p)) % p;
    uint1024_t s = (eMd * inverse(4, p)) % p;
    uint1024_t eSd = (e + d) % p;
    uint1024_t t = (eSd * inverse(6, p)) % p;
    uint1024_t num = (s * ((1 + uv.y) % p)) % p;
    uint1024_t oneMv = (1 + negative(uv.y, p)) % p;
    uint1024_t x = ((num * inverse(oneMv, p)) % p) + t;
    uint1024_t y = num * inverse((oneMv  * uv.x) % p, p);
    return Point(x % p, y % p);
}

Point convert_xy_to_uv(SEQUENCE* paramSet, Point xy) {
    uint1024_t e = paramSet->e;
    uint1024_t d = paramSet->d;
    uint1024_t p = paramSet->p;
    uint1024_t eMd = (e + negative(d, p)) % p;
    uint1024_t s = (eMd * inverse(4, p)) % p;
    uint1024_t eSd = (e + d) % p;
    uint1024_t t = (eSd * inverse(6, p)) % p;
    uint1024_t xMt = (xy.x + negative(t, p)) % p;
    uint1024_t xMtMs = (xMt + negative(s, p)) % p;
    uint1024_t xMtSs = (xMt + s) % p;
    uint1024_t u = xMt * inverse(xy.y, p);
    uint1024_t v = xMtMs * inverse(xMtSs, p);
    return Point(u % p, v % p);
}