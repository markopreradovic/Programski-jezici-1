#include "pch.h"
#include "ComparableVectorN.h"
#include "OrderedVectorSetN.h"
#include <iostream>
#include <iomanip>
#include <compare>
#include <functional>

namespace Vector {

	ComparableVectorN::ComparableVectorN(const VectorN& data, double(* const func)(const VectorN&))
		: norm(func), normValue(func(data)) {
		vector = new VectorN(data);
	}

	ComparableVectorN::ComparableVectorN(size_t d, double(* const func)(const VectorN&)) : norm(func) {
		vector = new VectorN(d);
		normValue = 0.0;
	}

	ComparableVectorN::ComparableVectorN(const ComparableVectorN& other) {
		this->copyComparableVectorN(other);
	}

	ComparableVectorN& ComparableVectorN::operator=(const ComparableVectorN& other) {
		if (this == &other)
			return (*this);
		this->copyComparableVectorN(other);
		return (*this);
	}

	ComparableVectorN::ComparableVectorN(ComparableVectorN&& other) noexcept {
		this->moveComparableVectorN(std::move(other));
	}

	ComparableVectorN& ComparableVectorN::operator=(ComparableVectorN&& other) noexcept {
		if (this == &other)
			return (*this);
		this->moveComparableVectorN(std::move(other));
		return (*this);
	}

	ComparableVectorN::~ComparableVectorN() {
		delete vector;
		normValue = 0;
		vector = nullptr;
		norm = nullptr;
	}

	void ComparableVectorN::copyComparableVectorN(const ComparableVectorN& other) {
		this->~ComparableVectorN();
		vector = new VectorN(*(other.vector));
		normValue = other.normValue;
		norm = other.norm;
	}

	void ComparableVectorN::moveComparableVectorN(ComparableVectorN&& other) {
		this->~ComparableVectorN();
		vector = std::move(other.vector);
		normValue = std::move(other.normValue);
		norm = std::move(other.norm);

		other.vector = nullptr;
		other.norm = nullptr;
		normValue = 0;
	}

	std::partial_ordering operator<=>(const ComparableVectorN& first, const ComparableVectorN& second) {
		if (first.norm != second.norm || first.vector->dimension() != second.vector->dimension())
			return std::partial_ordering::unordered;
		else
			return (first.normValue <=> second.normValue);
	}

	bool operator>(const ComparableVectorN& first, const ComparableVectorN& second) {
		return ((first <=> second) == std::partial_ordering::greater);
	}

	bool operator<(const ComparableVectorN& first, const ComparableVectorN& second) {
		return ((first <=> second) == std::partial_ordering::less);
	}

	bool operator>=(const ComparableVectorN& first, const ComparableVectorN& second) {
		if (first.checkUnordered(second))
			return false;
		return !(first < second);
	}

	bool operator<=(const ComparableVectorN& first, const ComparableVectorN& second) {
		if (first.checkUnordered(second))
			return false;
		return !(first > second);
	}

	bool operator==(const ComparableVectorN& first, const ComparableVectorN& second) {
		return ((first <=> second) == std::partial_ordering::equivalent);
	}

	bool operator!=(const ComparableVectorN& first, const ComparableVectorN& second) {
		if (first.checkUnordered(second))
			return false;
		return !(first == second);
	}

	ComparableVectorN::operator VectorN() const {
		return VectorN(*(this->vector));
	}

	ComparableVectorN::operator OrderedVectorSetN() const {
		return OrderedVectorSetN((*this));
	}

	std::ostream& operator<<(std::ostream& stream, const ComparableVectorN& cvec) {
		stream << std::setprecision(3);
		if (cvec.normValue != 0.0)
			stream << (1 / cvec.normValue);
		stream << std::fixed;
		stream << *(cvec.vector);
		return stream;
	}

	size_t ComparableVectorN::dimension() const {
		if (vector != nullptr)
			return vector->dim;
		else
			return 0;
	}
}