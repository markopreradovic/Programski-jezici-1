#include "pch.h"
#include "IPrintable.h"
#include "IComparable.h"
#include "Complex.h"
#include <iostream>
#include <functional>
#include <cmath>

namespace Set {
	double Complex::error_value = (double)Complex::error_returned_value;
	constexpr double PI = 3.14159;

	Complex::Complex(double r, double i) : re(r), im(i) {  }

	Complex::Complex(double r) : Complex(r, 0.0) {  }

	Complex::Complex() : Complex(0.0) {  }


	Complex::~Complex() {
		this->re = this->im = 0.0;
	}

	Complex operator+(const Complex& f, const Complex& s) {
		return Complex(f.re + s.re, f.im + s.im);
	}
	Complex operator-(const Complex& f, const Complex& s) {
		return Complex(f.re - s.re, f.im - s.im);
	}
	Complex operator*(const Complex& f, const Complex& s) {
		return Complex(f.re * s.re - f.im * s.im, f.re * s.im + f.im * s.re);
	}
	Complex operator/(const Complex& f, const Complex& s) {
		if (s.re == 0.0 && s.im == 0.0)
			return Complex(f.error_value, f.error_value);
		double dv = std::pow(s.abs(), 2);
		Complex tmp = f * (s.conj());
		tmp.re /= dv;
		tmp.im /= dv;
		return tmp;
	}

	Complex Complex::operator-() const {
		return Complex(-re, -im);
	}

	Complex& Complex::operator+=(const Complex& other) {
		(*this) = (*this) + other;
		return *this;
	}

	Complex& Complex::operator-=(const Complex& other) {
		(*this) = (*this) - other;
		return *this;
	}

	Complex& Complex::operator*=(const Complex& other) {
		(*this) = (*this) * other;
		return *this;
	}

	Complex& Complex::operator/=(const Complex& other) {
		(*this) = (*this) / other;
		return *this;
	}

	Complex operator+(double scalar, const Complex& c) {
		return c + scalar;
	}

	Complex Complex::operator+(double scalar) const {
		return (*this) + (Complex)scalar;
	}

	Complex operator-(double scalar, const Complex& c) {
		return -(c - scalar);
	}

	Complex Complex::operator-(double scalar) const {
		return (*this) - (Complex)scalar;
	}

	Complex operator*(double scalar, const Complex& c) {
		return c * scalar;
	}

	Complex Complex::operator*(double scalar) const {
		return ((*this) * (Complex)scalar);
	}

	Complex Complex::operator/(double scalar) const {
		return ((*this) / (Complex)scalar);
	}

	Complex& Complex::operator+=(double scalar) {
		return (*this) += (Complex)scalar;
	}

	Complex& Complex::operator-=(double scalar) {
		return ((*this) -= (Complex)scalar);
	}

	Complex& Complex::operator*=(double scalar) {
		return ((*this) *= (Complex)scalar);
	}

	Complex& Complex::operator/=(double scalar) {
		return ((*this) /= (Complex)scalar);
	}

	double Complex::abs() const {
		return std::sqrt(re * re + im * im);
	}

	Complex pow(const Complex& c, int index) {
		Complex init = 1;
		if (index == 0)
			return init;

		Complex tmp = init;
		int a_index = std::abs(index);

		while (a_index) {
			tmp = tmp * c;
			--a_index;
		}

		return (index < 0) ? (init / tmp) : tmp;
	}

	Complex Complex::conj() const {
		return Complex(re, -im);
	}

	double Complex::arg() const { 
		if (re == 0.0 && im == 0.0)
			return 0.0;
		else if (re != 0 && im == 0.0)
			return ((re > 0.0) ? 0.0 : PI);
		else if (re == 0.0 && im != 0.0)
			return ((im > 0.0) ? (PI / 2) : (3 * PI / 2));
		else
			return std::atan(im / re);
	}

	void Complex::printItem(std::ostream& stream) const {
		stream << "( " << re << " , " << im << " )";
	}

	std::partial_ordering Complex::compareItems(const IComparable& other) const {
		const Complex* other_c = dynamic_cast<const Complex*>(&other);
		if (!other_c)
			return std::partial_ordering::unordered;
		else
			return (this->abs() <=> other_c->abs());
	}

	bool Complex::equalItems(const IPrintable& other) const {
		const Complex* other_c = dynamic_cast<const Complex*>(&other);
		if (!other_c)
			return false;
		return ((this->re == other_c->re) && (this->im == other_c->im));
	}

	void Complex::compare_print(std::partial_ordering r) {
		if (r == std::partial_ordering::less)
			std::cout << " < ";
		else if (r == std::partial_ordering::greater)
			std::cout << " > ";
		else if (r == std::partial_ordering::equivalent)
			std::cout << " = ";
		else if (r == std::partial_ordering::unordered)
			std::cout << "unordered";
	}

	void Complex::complex_test() {
		Complex c1(5, 3), c2(4, 9), c3 = 5, c4;
		std::cout << "(c1 + c3) = " << (c1 + c3) << std::endl;
		std::cout << "(c2 - c3) = " << (c2 - c3) << std::endl;
		std::cout << "(c1 * c2) = " << (c1 * c2) << std::endl;
		std::cout << "(c1 / c4) = " << (c1 / c4) << std::endl;
		std::cout << "|c1| = " << c1.abs() << std::endl;
		std::cout << "c2.conj() = " << c2.conj() << std::endl;
		c1 = c3;
		std::cout << c1 << c2 << c3 << c4 << std::endl;
		std::cout << "(c2 + 5) = " << (c4 = (c2 + 5)) << std::endl;

		std::cout << "c4 = " << c4 << std::endl;

		Complex c5 = c1 + c2;
		std::cout << c5;
		std::cout << "c5 / 3 = " << (c5 / 3) << std::endl;
		std::cout << "5 * c1 = " << (5 * c1) << std::endl;

		std::cout << "(5, 3) ^ 2 = " << pow(Complex(5, 3), 2) << std::endl;
		std::cout << "arg(c2) = " << c2.arg() << " rad" << std::endl;

		std::cout << "c1";
		compare_print(c1 <=> c2);
		std::cout << "c2" << std::endl;

		std::cout << "c4";
		compare_print(c4 <=> c5);
		std::cout << "c5" << std::endl;

		Complex c6(1, 1), c7(1, 1), c8 = 5;
		std::cout << "c6 < c7? " << (c6 < c7) << std::endl;
		std::cout << "c6 > c7? " << (c6 > c7) << std::endl;
		std::cout << "c6 <= c7? " << (c6 <= c7) << std::endl;

		std::cout << "c8 >= c7? " << (c8 >= c7) << std::endl;
		std::cout << "c8 == c7? " << (c8 == c7) << std::endl;
		std::cout << "c8 != c7? " << (c8 != c7) << std::endl;

	}
};