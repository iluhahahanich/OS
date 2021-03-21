#pragma once

#include "number.h"
#include <cmath>

class Vector {
private:
    Number _x, _y;

public:
    Vector(Number x, Number y);

    Number Ro() const;

    Number Fi() const;

    Vector operator+ (const Vector& other) const;

    friend std::ostream& operator<< (std::ostream& out, const Vector& n);
};