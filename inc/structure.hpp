#pragma once

#include "parameters.hpp"

struct SEQUENCE {

    uint1024_t p;
    uint1024_t a;
    uint1024_t b;
    uint1024_t e;
    uint1024_t d;
    uint1024_t m;
    uint1024_t q;
    uint1024_t x;
    uint1024_t y;
    uint1024_t u;
    uint1024_t v;
    size_t mode;

    SEQUENCE(uint1024_t, uint1024_t, uint1024_t, uint1024_t,
             uint1024_t, uint1024_t, uint1024_t, uint1024_t,
             uint1024_t, uint1024_t, uint1024_t, size_t);

    void printStruct();
};

struct Point {
    uint1024_t x;
    uint1024_t y;

    Point() : x(0), y(0) {};
    Point(uint1024_t, uint1024_t);
    friend std::ostream& operator << (std::ostream &s, Point);
};