#pragma once

#include "parameters.hpp"

struct SEQUENCE {

    u8 p[PARAM_SIZE];
    u8 a[PARAM_SIZE];
    u8 b[PARAM_SIZE];
    u8 e[PARAM_SIZE];
    u8 d[PARAM_SIZE];
    u8 m[PARAM_SIZE];
    u8 q[PARAM_SIZE];
    u8 x[PARAM_SIZE];
    u8 y[PARAM_SIZE];
    u8 u[PARAM_SIZE];
    u8 v[PARAM_SIZE];
    size_t mode;

    SEQUENCE(cu8 *P, cu8 *A, cu8 *B,
             cu8 *E, cu8 *D, cu8 *M,
             cu8 *Q, cu8 *X, cu8 *Y,
             cu8 *U, cu8 *V, size_t mode);

    void setmode(size_t mode);
    void printStruct();
    void setval(cu8 *from, u8 *to);

};