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

struct Verylong {

    char *vlstr;
    int  vlen;
    int  vlsign;

    Verylong(const char* = NULL);
    Verylong(int);
    Verylong(const Verylong &);
    ~Verylong();

    Verylong multdigit(int) const;
    Verylong mult10(int) const;

    operator int() const;
    operator long() const;
    operator double() const;
    operator char * () const;

    operator bool () const;

    const Verylong& operator=(const Verylong&);

    Verylong operator- () const;
    Verylong operator++ ();
    Verylong operator++ (int);
    Verylong operator-- ();
    Verylong operator-- (int);

    Verylong operator+= (const Verylong &);
    Verylong operator-= (const Verylong &);
    Verylong operator*= (const Verylong &);
    Verylong operator/= (const Verylong &);
    Verylong operator%= (const Verylong &);

    friend Verylong operator+ (const Verylong &, const Verylong &);
    friend Verylong operator- (const Verylong &, const Verylong &);
    friend Verylong operator* (const Verylong &, const Verylong &);
    friend Verylong operator/ (const Verylong &, const Verylong &);
    friend Verylong operator% (const Verylong &, const Verylong &);

    friend int operator== (const Verylong &, const Verylong &);
    friend int operator!= (const Verylong &, const Verylong &);
    friend int operator< (const Verylong &, const Verylong &);
    friend int operator<= (const Verylong &, const Verylong &);
    friend int operator> (const Verylong &, const Verylong &);
    friend int operator>= (const Verylong &, const Verylong &);

    char *strrev(char *s) const;
    friend Verylong abs(const Verylong &);
    friend Verylong sqrt(const Verylong &);
    friend Verylong pow(const Verylong &, const Verylong &);
    friend double div(const Verylong &, const Verylong &);

    friend std::ostream & operator<< (std::ostream &, const Verylong &);
    friend std::istream & operator>> (std::istream &, Verylong &);

};
