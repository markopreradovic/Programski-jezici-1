#include "pch.h"
#include "Complex.h"
#include <iostream>
#include <exception>
#include <cmath>

namespace collection {
	namespace complex {
		Complex::Complex(Complex&& other) noexcept {
			move(std::move(other));
		}

		Complex& Complex::operator=(Complex&& other) noexcept {
			if (this != &other) {
				clear();
				move(std::move(other));
			}
			return *this;
		}

		Complex::~Complex() noexcept {
			clear();
		}

		void Complex::move(Complex&& other) noexcept {
			re = other.re;
			im = other.im;
			other.clear();
		}

		Complex Complex::calculate(const Complex& other, const std::function<Complex(const Complex&, const Complex&)>& func) const noexcept {
			return Complex(func((*this), other));
		}

		Complex Complex::operator+(const Complex& other) const noexcept {
			return Complex(re + other.re, im + other.im);
		}

		Complex Complex::operator-(const Complex& other) const noexcept {
			return Complex(re - other.re, im - other.im);
		}

		Complex Complex::operator*(const Complex& other) const noexcept {
			return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
		}

		Complex Complex::operator/(const Complex& other) const noexcept(false) {
			if (other.isZeroComplex())
				throw std::invalid_argument("Division by zero.");
			double denominator = std::pow(other.abs(), 2);
			Complex tmp = (*this) * (other.conj());
			tmp.re /= denominator;
			tmp.im /= denominator;
			return tmp;
		}

		Complex& Complex::operator+=(const Complex& other) noexcept {
			(*this) = calculate(other, add);
			return *this;
		}

		Complex& Complex::operator-=(const Complex& other) noexcept {
			(*this) = calculate(other, sub);
			return *this;
		}

		Complex& Complex::operator*=(const Complex& other) noexcept {
			(*this) = calculate(other, mul);
			return *this;
		}

		Complex& Complex::operator/=(const Complex& other) noexcept(false) {
			if (other.isZeroComplex())
				throw std::invalid_argument("Division by zero.");
			(*this) = (*this) / other;
			return *this;
		}

		Complex& Complex::operator+=(double scalar) noexcept {
			return ((*this) += (Complex)scalar);
		}

		Complex& Complex::operator-=(double scalar) noexcept {
			return ((*this) -= (Complex)scalar);
		}

		Complex& Complex::operator*=(double scalar) noexcept {
			return ((*this) *= (Complex)scalar);
		}

		Complex& Complex::operator/=(double scalar) noexcept(false) {
			return ((*this) /= (Complex)scalar);
		}

		Complex Complex::operator+(double scalar) const noexcept {
			return ((*this) + (Complex)scalar);
		}
		Complex operator+(double scalar, const Complex& other) noexcept {
			return (other + scalar);
		}

		Complex Complex::operator-(double scalar) const noexcept {
			return ((*this) - (Complex)scalar);
		}

		Complex operator-(double scalar, const Complex& other) noexcept {
			return (other - scalar);
		}

		Complex Complex::operator*(double scalar) const noexcept {
			return ((*this) * (Complex)scalar);
		}

		Complex operator*(double scalar, const Complex& other) noexcept {
			return (other * scalar);
		}

		Complex Complex::operator/(double scalar) const noexcept(false) {
			return ((*this) / (Complex)scalar);
		}

		Complex operator/(double scalar, const Complex& other) noexcept(false) {
			return ((Complex)scalar / other);
		}

		Complex pow(const Complex& z, int deg) {
			if (z.isZeroComplex())
				return Complex(z);
			Complex init = 1;
			if (deg == 0)
				return Complex(init);

			int a_deg = std::abs(deg);
			while (a_deg) {
				init *= z;
				--a_deg;
			}
			return ((deg < 0) ? (1 / init) : init);
		}

		double Complex::abs() const noexcept {
			return std::sqrt(re * re + im * im);
		}

		Complex Complex::conj() const noexcept {
			return Complex(re, -im);
		}

		double Complex::arg() const noexcept {
			if (isZeroComplex())
				return 0.0;
			else
				return std::atan(im / re);
		}

		std::ostream& operator<<(std::ostream& stream, const Complex& item) noexcept {
			stream << "( " << item.re << " , " << item.im << " )";
			return stream;
		}

		bool Complex::operator==(const Complex& other) const noexcept {
			return (re == other.re && im == other.im);
		}

		bool Complex::operator!=(const Complex& other) const noexcept {
			return !(operator==(other));
		}

		void Complex::ComplexTest() {
			Complex z1(4, 5), z2(1, 4), z3, z4 = 9, z5(5, 5), z6 = 3;
			double s1 = 4, s2 = 6;
			std::cout << "z1 + z2 = " << (z1 + z2) << std::endl;
			std::cout << "z3 - z4 = " << (z3 - z4) << std::endl;
			std::cout << "z5 * z6 = " << (z5 * z6) << std::endl;

			try {
				std::cout << "z1 / z3 = " << (z1 / z3) << std::endl;
			}
			catch (const std::invalid_argument& ex) {
				std::cout << ex.what() << std::endl;
			}

			std::cout << "s1 + z1 = " << (s1 + z1) << std::endl;
			std::cout << "z2 * s2 = " << (z2 * s2) << std::endl;
			std::cout << "s1 / z5 = " << (s1 / z5) << std::endl;

			try {
				std::cout << "s1 / z3 = " << (s1 / z3) << std::endl;
			}
			catch (const std::invalid_argument& ex) {
				std::cout << ex.what() << std::endl;
			}

			std::cout << "z1 == z3 ? " << std::boolalpha << (z1 == z3) << std::endl;
			std::cout << "arg(z4) = " << z4.arg() << std::endl;
			std::cout << "|z2| = " << z2.abs() << std::endl;
			std::cout << "z6.conj() = " << z6.conj() << std::endl;
			std::cout << "z5 ^ 3 = " << pow(z5, 3) << std::endl;
		}
	};
};