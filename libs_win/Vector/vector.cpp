#include "vector.h"

Number PI(acos(-1));

Vector::Vector(Number x, Number y) : _x(x), _y(y){}

Number Vector::Ro() const {
    return Number(hypot(this->_x.GetVal(), this->_y.GetVal()));
}

Number Vector::Fi() const {
    Number arccos = CreateNumber(acos((this->_x / Ro()).GetVal()));
    return this->_y > ZERO ? arccos : CreateNumber(2) * PI - arccos;
}

Vector Vector::operator+(const Vector &other) const {
    return Vector(Number(this->_x + other._x), Number(this->_y + other._y));
}

std::ostream &operator<<(std::ostream &out, const Vector &n) {
    out << "(" << n._x << ", " << n._y << ")";
    return out;
}
