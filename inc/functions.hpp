#pragma once
#include "structure.hpp"
#include "parameters.hpp"

void gen_priv_key(u8 *d, SEQUENCE *paramSetA256);
void print(cu8 *what, size_t size);
void rand_bytes(u8 *buf, int num);
void reverse(u8 *t, size_t mode);
