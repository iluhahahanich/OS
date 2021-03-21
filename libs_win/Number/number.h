#pragma once

#include <iostream>

class Number {
private:
    double _val;

public:
    explicit Number(double val);

    Number operator+ (const Number& other) const;

    Number operator- (const Number& other) const;

    Number operator* (const Number& other) const;

    Number operator/ (const Number& other) const;

    friend std::ostream& operator<< (std::ostream& out, const Number& n);


    bool operator> (const Number& other) const;

public:
    double GetVal() const;
};

extern Number ZERO, ONE;

Number CreateNumber(double val);