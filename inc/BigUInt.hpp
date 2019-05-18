#pragma once

#include <vector>
#include <iostream>
#include <iomanip>

#include "parameters.hpp"
#include "functions.hpp"
#include "structure.hpp"

struct BigUInt {
    std::vector <u8> number;

    BigUInt(u8 *, size_t);
    BigUInt(u8, size_t);
    BigUInt(std::vector<u8>);

    friend std::ostream& operator << (std::ostream &s, BigUInt);

    BigUInt mul(BigUInt bigUInt);
    BigUInt div2();
    bool odd();

    friend BigUInt operator* (const BigUInt &, const BigUInt &);
    friend BigUInt operator+ (const BigUInt &, const BigUInt &);
    friend BigUInt operator- (const BigUInt &, const BigUInt &);
    friend int operator== (const BigUInt &, const BigUInt &);
    friend int operator!= (const BigUInt &, const BigUInt &);
};

bool is_bigger(std::vector<u8> t, std::vector<u8> q, size_t size);
