#pragma once
#include <functional>

namespace collection {
	namespace complex {
		class Complex {
			double re{ 0.0 };
			double im{ 0.0 };

		public:
			Complex(double r = 0.0, double i = 0.0) noexcept : re(r), im(i) {  }
			Complex(const Complex&) noexcept = default;
			Complex& operator=(const Complex&) = default;
			Complex(Complex&&) noexcept;
			Complex& operator=(Complex&&) noexcept;
			~Complex() noexcept;

			Complex operator+(const Complex&) const noexcept;
			Complex operator-(const Complex&) const noexcept;
			Complex operator*(const Complex&) const noexcept;
			Complex operator/(const Complex&) const noexcept(false);

			Complex& operator+=(const Complex&) noexcept;
			Complex& operator-=(const Complex&) noexcept;
			Complex& operator*=(const Complex&) noexcept;
			Complex& operator/=(const Complex&) noexcept(false);

			Complex operator+(double) const noexcept;
			friend Complex operator+(double, const Complex&) noexcept;
			Complex operator-(double) const noexcept;
			friend Complex operator-(double, const Complex&) noexcept;
			Complex operator*(double) const noexcept;
			friend Complex operator*(double, const Complex&) noexcept;
			Complex operator/(double) const noexcept(false);
			friend Complex operator/(double, const Complex&) noexcept(false);

			Complex& operator+=(double) noexcept;
			Complex& operator-=(double) noexcept;
			Complex& operator*=(double) noexcept;
			Complex& operator/=(double) noexcept(false);

			friend Complex pow(const Complex&, int);
			double abs() const noexcept;
			Complex conj() const noexcept;
			double arg() const noexcept;

			friend std::ostream& operator<<(std::ostream&, const Complex&) noexcept;
			bool operator==(const Complex&) const noexcept;
			bool operator!=(const Complex&) const noexcept;

			static void ComplexTest();

		private:
			bool inline isZeroComplex() const noexcept { return (re == 0.0 && im == 0.0); }
			void clear() noexcept { re = im = 0.0; }
			void move(Complex&&) noexcept;
			Complex calculate(const Complex&, const std::function<Complex(const Complex&, const Complex&)>&) const noexcept;

			std::function<Complex(const Complex&, const Complex&)> add = [](const Complex& lhs, const Complex& rhs) {return (lhs + rhs); };
			std::function<Complex(const Complex&, const Complex&)> sub = [](const Complex& lhs, const Complex& rhs) {return (lhs - rhs); };
			std::function<Complex(const Complex&, const Complex&)> mul = [](const Complex& lhs, const Complex& rhs) {return (lhs * rhs); };
		};
	}
};