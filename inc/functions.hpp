#pragma once
#include "structure.hpp"
#include "parameters.hpp"
#include <boost/multiprecision/cpp_int.hpp>

void gen_priv_key(u8 *d, SEQUENCE *paramSetA256);
void print(cu8 *what, size_t size);
void rand_bytes(u8 *buf, int num);
void reverse(u8 *t, size_t mode);
uint1024_t init_1024(u8 *a, size_t size);

uint1024_t negative(uint1024_t, uint1024_t);
uint1024_t inverse(uint1024_t, uint1024_t);
uint1024_t sum_x(Point, Point, uint1024_t, uint1024_t);
uint1024_t sum_y(Point, Point, uint1024_t, uint1024_t, uint1024_t);

Point mul(SEQUENCE*, uint1024_t);
Point muL(Point, uint1024_t, uint1024_t, uint1024_t, uint1024_t);
// Point fast_mul(uint1024_t, uint1024_t, uint1024_t,
//                     uint1024_t, uint1024_t, uint1024_t);

Point sum_p(Point p1, Point p2, uint1024_t p, uint1024_t d, uint1024_t t);
Point convert(SEQUENCE*, Point);
uint1024_t prime(uint1024_t, uint1024_t p);