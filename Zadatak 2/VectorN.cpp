#include "pch.h"
#include "VectorN.h"
#include "ComparableVectorN.h"
#include "VectorSetN.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <functional>

namespace Vector {

	double VectorN::errorValue = (double)VectorN::ERROR_RETURNED_ELEMENT_VALUE;

	VectorN::VectorN(double d) : VectorN(1, d) { }

	VectorN::VectorN(size_t n, double d) {
		dim = (n <= 0) ? 1 : n;
		elements = new double[dim];
		std::fill(elements, elements + dim, d);
	}

	VectorN::VectorN(size_t n) : VectorN(n, 0.0) {  }

	VectorN::VectorN(const std::initializer_list<double> list) : VectorN(list.size()) {
		std::copy(list.begin(), list.end(), elements);
	}

	VectorN::VectorN(const VectorN& other) {
		this->copyVectorN(other);
	}

	VectorN& VectorN::operator=(const VectorN& other) {
		if (this == &other)
			return *this;
		this->copyVectorN(other);
		return *this;
	}

	VectorN::VectorN(VectorN&& other) noexcept {
		this->moveVectorN(std::move(other));
	}

	VectorN& VectorN::operator=(VectorN&& other) noexcept {
		this->moveVectorN(std::move(other));
		return *this;
	}

	VectorN::~VectorN() {
		if (elements != nullptr)
			delete[] elements;

		elements = nullptr;
		dim = 0;
	}

	const double& VectorN::operator[](int index) const {
		return getElementAt(index);
	}

	double& VectorN::setElementAt(int index) {
		return const_cast<double&>(getElementAt(index));
	}

	const double& VectorN::getElementAt(int index) const {
		if (!validIndexRange(index))
			return errorValue;
		else
			return elements[index];
	}

	size_t VectorN::dimension() const {
		return dim;
	}

	void VectorN::copyVectorN(const VectorN& other) {
		if (this->dim != other.dim) {
			this->~VectorN();
			elements = new double[dim = other.dim];
		}
		std::copy(other.elements, other.elements + dim, elements);
	}

	void VectorN::moveVectorN(VectorN&& other) {
		this->~VectorN();
		elements = std::move(other.elements);
		dim = std::move(other.dim);

		other.elements = nullptr;
		other.dim = 0;
	}

	std::istream& operator>>(std::istream& in, VectorN& vec) {
		vec.~VectorN();
		int t;
		do {
			std::cout << "Unesite dimenziju vektora : ";
			in >> t;
		} while (t <= 0);

		vec.dim = static_cast<size_t>(t);
		vec = VectorN(vec.dim);
		std::cout << "Unesite koorinate : " << std::endl;
		for (size_t i = 0; i < vec.dim; i++) {
			std::cout << "[ " << i + 1 << " ] ";
			in >> vec.elements[i];
		}

		return in;
	}

	std::ostream& operator<<(std::ostream& out, const VectorN& vec) {
		out << "( ";
		for (size_t i = 0; i < vec.dim; i++) {
			out << std::setprecision(2) << vec.elements[i] << " ";
		}
		out << ") ";
		out << std::fixed;
		return out;
	}

	VectorN VectorN::zeroVector(size_t n) {
		return VectorN(n);
	}

	bool VectorN::isZeroVector() const {
		for (size_t i = 0; i < dim; ++i) {
			if (elements[i] != 0.0)
				return false;
		}
		return true;
	}

	double VectorN::operator()(double x) const {
		double sum = 0.0;
		for (size_t i = 0; i < dim; i++)
			sum += ((*this)[i] * pow(x, i));

		return sum;
	}

	bool operator==(const VectorN& vec1, const VectorN& vec2) {
		if (!vec1.equalDimensions(vec2.dim))
			return false;

		for (size_t i = 0; i < vec1.dim; i++) {
			if (vec1[i] != vec2[i])
				return false;
		}

		return true;
	}

	bool operator!=(const VectorN& vec1, const VectorN& vec2) {
		return !(vec1 == vec2);
	}

	VectorN VectorN::calculate(const VectorN& other, const std::function<double(double, double)>& func) const {
		if (!equalDimensions(other.dim))
			return VectorN(1, errorValue);

		VectorN tmp(dim);
		for (size_t i = 0; i < dim; i++)
			tmp.setElementAt(i) = func((*this)[i], other[i]);

		return tmp;
	}

	VectorN VectorN::operator*(double scalar) const {
		VectorN tmpVector(this->dim, scalar);
		return calculate(tmpVector, mul);
	}

	VectorN operator* (double scalar, const VectorN& vec) {
		return vec * scalar;
	}

	double operator*(const VectorN& vec1, const VectorN& vec2) {
		if (vec1.equalDimensions(vec2.dim)) {
			double sum = 0.0;
			for (size_t i = 0; i < vec1.dim; i++)
				sum += vec1[i] * vec2[i];
			return sum;
		}
		else
			return vec1.errorValue;
	}

	VectorN operator+ (const VectorN& vec1, const VectorN& vec2) {
		return vec1.calculate(vec2, vec1.add);
	}

	VectorN operator- (const VectorN& vec1, const VectorN& vec2) {
		return vec1.calculate(vec2, vec1.sub);
	}

	VectorN operator-(const VectorN& vec) {
		return vec * (-1);
	}

	VectorN& VectorN::operator+=(const VectorN& other) {
		if (equalDimensions(other.dim))
			(*this) = (*this) + other;

		return *this;
	}

	VectorN& VectorN::operator-=(const VectorN& other) {
		if (equalDimensions(other.dim))
			(*this) = (*this) - other;

		return *this;
	}

	double VectorN::EuclidDistance(const VectorN& vec) {
		double result = 0.0;
		for (size_t i = 0; i < vec.dimension(); i++) {
			result += (vec[i] * vec[i]);
		}

		result = std::sqrt(result);
		return result;
	}

	double VectorN::sumOfElements() const {
		double result = 0.0;
		for (size_t i = 0; i < dim; ++i)
			result += elements[i];
		return result;
	}

	VectorN::operator ComparableVectorN() const {
		return ComparableVectorN((*this), EuclidDistance);
	}

	VectorN::operator VectorSetN() const {
		return VectorSetN((*this));
	}
}