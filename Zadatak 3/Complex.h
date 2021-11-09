#pragma once
#include "IPrintable.h"
#include "IComparable.h"
#include <iostream>
#include <functional>

namespace Set {
	class Complex : virtual public Interface::IComparable {
		double re;
		double im;

	public:
		Complex();
		Complex(double r, double i);
		Complex(double r);

		Complex(const Complex&) = default;
		Complex& operator=(const Complex&) = default;
		Complex(Complex&&) noexcept = default;
		Complex& operator=(Complex&&) noexcept = default;
		~Complex();

		friend Complex operator+(const Complex&, const Complex&);
		friend Complex operator-(const Complex&, const Complex&);
		friend Complex operator*(const Complex&, const Complex&);
		friend Complex operator/(const Complex&, const Complex&);
		Complex operator-() const;
		Complex& operator+=(const Complex&);
		Complex& operator-=(const Complex&);
		Complex& operator/=(const Complex&);
		Complex& operator*=(const Complex&);

		friend Complex operator+(double, const Complex&);
		Complex operator+(double) const;
		friend Complex operator-(double, const Complex&);
		Complex operator-(double) const;
		friend Complex operator*(double, const Complex&);
		Complex operator*(double) const;
		Complex operator/(double) const;
		Complex& operator+=(double);
		Complex& operator-=(double);
		Complex& operator/=(double);
		Complex& operator*=(double);

		friend Complex pow(const Complex&, int);
		double abs() const;
		Complex conj() const;
		double arg() const;

		static double error_value;
		constexpr static double error_returned_value = NAN;

		static void complex_test();

	protected:
		virtual void printItem(std::ostream&) const override;
		virtual std::partial_ordering compareItems(const IComparable&) const override;
		virtual bool equalItems(const IPrintable&) const override;

	private:
		static void compare_print(std::partial_ordering);
	};
}


