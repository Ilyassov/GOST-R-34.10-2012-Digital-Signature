#include "../inc/BigUInt.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"
#include "../inc/structure.hpp"

int operator== (const BigUInt &u, const BigUInt &v) {
	return (u.number == v.number);
}

int operator!= (const BigUInt &u, const BigUInt &v) {
	return !(u == v);
}

bool is_bigger(std::vector<u8> t, std::vector<u8> q, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (t[i] > q[i]) {
            return true;
        } else if (t[i] < q[i]) {
            return false;
        }
    }
    return false;
}

BigUInt::BigUInt(u8 * src, size_t size) {
    for (size_t i = 0; i < size; i++) {
        this->number.push_back(src[i]);
    }
}

size_t max(size_t a, size_t b) {
    return a < b ? b : a;
}

BigUInt::BigUInt(u8 val, size_t size) {
    for (size_t i = 0; i < size; i++) {
        this->number.push_back(val);
    }
}

BigUInt::BigUInt(std::vector <u8> v) {
    for (size_t i = 0; i < v.size(); i++) {
        this->number.push_back(v[i]);
    }
}

std::ostream& operator << (std::ostream &s, BigUInt bigUInt) {
    for (size_t i = 0; i < bigUInt.number.size(); i++) {
        s << std::setw(2) << std::setfill('0') << std::hex;
        s << (int) bigUInt.number[i] << " ";
    }
	return s;
}

BigUInt operator+ (const BigUInt &a, const BigUInt &b) {
    //Если у одного из чисел больше разрядов
    //то его нужно поставить первым
    BigUInt res((u8)0x00, b.number.size());
    u16 overflow = 0;
    for (int i = a.number.size()-1; i >= 0; i--) {
        overflow = overflow + b.number[i] + a.number[i];
        res.number[i] = (u8) overflow;
        overflow >>= 8;
    }
    return res;
}

BigUInt operator- (const BigUInt &a, const BigUInt &b) {
     BigUInt res((u8)0x00, a.number.size());
    //Если вычитаемое число больше, то
    //вычитаем из FF*size число равное разности
    //со знаком плюс, то есть делаем разность в группе
    if (is_bigger(b.number, a.number, b.number.size())) {
        return BigUInt((u8) 0xFF,
            b.number.size()) - (b - a);
    }

    u16 overflow = a.number[a.number.size()-1];

    for (int i = a.number.size()-1; i >= 0; i--) {

        //Складываем в overflow по два разряда и вычитаем
        //крайний не использованный вычитаемого числа разряд
        //То есть делаем разность "в столбик"

        overflow =  i != 0 ?
            overflow +
            (((u16) (a.number[i-1])) << 8) -
            b.number[i] :
            overflow - b.number[i];

        res.number[i] = (u8) overflow;
        overflow >>= 8;
    }
    return res;
}

BigUInt BigUInt::div2() {
    
    u8 rem1 = 0x00;

    for (size_t i = 1; i < number.size(); i++) {
        u8 rem2 = (number[i] & 0x01) ? 0x80 : 0x00;
        number[i] = (number[i] >> 1) + rem1;
        rem1 = rem2;
    }
    return BigUInt(number);
}

bool BigUInt::odd(){
    return number[number.size()-1] & 0x01;
}

BigUInt operator* (const BigUInt &a, const BigUInt &b) {
    //number, this -> a
    //bigUInt -> b
    BigUInt num1 = a;
    BigUInt num2 = b;

    //Тут будет храниться результат
    BigUInt res((u8)0x00, num1.number.size() * 2);

    //Нужно для сравнения
    BigUInt zero((u8)0x00, num1.number.size());

    //Нужно для вычета
    BigUInt one(zero.number);
    one.number[one.number.size()-1] = (u8)0x01;

    //Алгоритм быстрого возведения в степень
    //только для суммы двух больших чисел
    while (num2 != zero) {
        if (num2.odd()) {
            res = res + num1;
            num2 = num2 - one;
        } else {
            num1 = num1 + num1;
            num2 = num2.div2();
        }
    }

    return res;
}