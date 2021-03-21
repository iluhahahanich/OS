#include "number.h"

Number ZERO(0), ONE(1);

Number::Number(double val) : _val(val){}

Number Number::operator+(const Number& other) const {
    return Number(this->_val + other._val);
}

Number Number::operator-(const Number& other) const {
    return Number(this->_val - other._val);
}

Number Number::operator*(const Number& other) const {
    return Number(this->_val * other._val);
}

Number Number::operator/(const Number& other) const {
    return Number(this->_val / other._val);
}

std::ostream& operator<< (std::ostream& out, const Number& n) {
    out << n._val;
    return out;
}

double Number::GetVal() const{
    return this->_val;
}

bool Number::operator>(const Number &other) const {
    return this->_val > other._val;
}

Number CreateNumber(double number){
    return Number(number);
}
