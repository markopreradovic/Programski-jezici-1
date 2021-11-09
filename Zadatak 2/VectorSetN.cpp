#include "pch.h"
#include "VectorSetN.h"
#include "Matrix.h"
#include <iostream>
#include <functional>

namespace Vector {

	VectorSetN::VectorSetN(size_t cap, size_t dim) {
		set_and_allocate(dim, cap);
	}

	VectorSetN::VectorSetN(const std::initializer_list<VectorN>& list) {
		if (list.size() != 0) {

			set_and_allocate((list.begin())->dimension(), list.size());

			for (auto x : list)
				addElement(x);
		}
		else
			set_and_allocate(0, 0);
	}

	VectorSetN::VectorSetN(const VectorN* arr, size_t n) {
		if (n != 0) {
			set_and_allocate(arr[0].dimension(), n);
			for (size_t i = 0; i < n; ++i)
				addElement(arr[i]);
		}
		else
			set_and_allocate(0, 0);
	}

	VectorSetN::VectorSetN(const VectorN& info) : VectorSetN(1, info.dimension()) {
		elements[elemNum] = info;
		++elemNum;
	}

	VectorSetN::VectorSetN(const VectorSetN& other) {
		copyVectorSetN(other);
	}

	VectorSetN& VectorSetN::operator=(const VectorSetN& other) {
		if (this == &other)
			return *this;

		this->~VectorSetN();
		copyVectorSetN(other);
		return (*this);
	}

	VectorSetN::VectorSetN(VectorSetN&& other) noexcept {
		moveVectorSetN(std::move(other));
	}

	VectorSetN& VectorSetN::operator=(VectorSetN&& other) noexcept {
		if (this == &other)
			return (*this);

		this->~VectorSetN();
		moveVectorSetN(std::move(other));
		return *this;
	}

	VectorSetN::~VectorSetN() {
		if (elements != nullptr)
			delete[] elements;

		elemDimension = elemNum = capacity = 0;
	}

	void VectorSetN::copyVectorSetN(const VectorSetN& other) {
		elemDimension = other.elemDimension;
		elemNum = other.elemNum;
		capacity = other.capacity;
		elements = new VectorN[capacity];
		for (size_t i = 0; i < elemNum; i++)
			elements[i] = other.elements[i];
	}

	void VectorSetN::moveVectorSetN(VectorSetN&& other) {
		elemDimension = std::move(other.elemDimension);
		elemNum = std::move(other.elemNum);
		elements = std::move(other.elements);
		capacity = std::move(other.capacity);

		other.elemDimension = other.elemNum = 0;
		other.elements = nullptr;
	}

	void VectorSetN::set_and_allocate(size_t d, size_t c) {
		elemNum = 0;
		elemDimension = d;
		capacity = (c == 0) ? 1 : c;
		elements = new VectorN[capacity];
	}

	int VectorSetN::search(const VectorN& info) const {
		for (size_t i = 0; i < elemNum; ++i) {
			if (info == elements[i])
				return i;
		}

		return -1;
	}

	void VectorSetN::resize() {
		VectorN* newElements = new VectorN[capacity *= 2];
		for (size_t i = 0; i < elemNum; ++i)
			newElements[i] = elements[i];

		delete[] elements;
		elements = newElements;
	}

	void VectorSetN::deleteElementAt(size_t index) {
		if (index >= elemNum)
			return;

		elemNum -= 1;
		for (size_t j = index; j < elemNum; ++j)
			elements[j] = elements[j + 1];
	}

	void VectorSetN::addElement(const VectorN& info) {
		if (search(info) < 0 && info.dimension() == this->elemDimension) {
			if (elemNum >= capacity)
				resize();
			elements[elemNum++] = info;
		}
	}

	void VectorSetN::deleteElement(const VectorN& info) {
		int index = search(info);
		if (index >= 0 || !isEmpty())
			deleteElementAt(index);
	}

	VectorSetN& VectorSetN::calculate_and_change(const VectorSetN& info, void(VectorSetN::* f)(const VectorN&)) {
		if (elemDimension != info.elemDimension)
			return (*this);

		for (size_t i = 0; i < info.elemNum; ++i)
			(this->*f)(info[i]);

		return (*this);
	}

	VectorSetN& VectorSetN::operator+=(const VectorSetN& info) {
		return calculate_and_change(info, &VectorSetN::addElement);
	}

	VectorSetN& VectorSetN::operator-=(const VectorSetN& info) {
		return calculate_and_change(info, &VectorSetN::deleteElement);
	}

	std::ostream& operator<<(std::ostream& stream, const VectorSetN& vec) {
		if (vec.isEmpty()) {
			stream << "Skup je prazan.";
			stream << std::endl;
		}
		else {
			stream << "{  " << std::endl;
			for (size_t i = 0; i < vec.elemNum; ++i)
				stream << "\t" << vec[i] << std::endl;
			stream << "}  " << std::endl;
		}

		return stream;
	}

	VectorSetN VectorSetN::filter(const std::function<bool(const VectorN&)>& filterFunc) const {
		VectorSetN tmp(capacity, elemDimension);
		size_t currElemNum = 0;
		for (size_t i = 0; i < elemNum; ++i) {
			if (filterFunc((*this)[i]))
				tmp[currElemNum++] = (*this)[i];
		}

		tmp.elemNum = currElemNum;
		return VectorSetN(tmp);
	}

	std::vector<double> VectorSetN::transform(const std::function<double(const VectorN&)>& transformFunc) const {
		std::vector<double> tmp;
		for (size_t i = 0; i < elemNum; ++i)
			tmp.push_back(transformFunc((*this)[i]));

		return tmp;
	}

	bool operator==(const VectorSetN& first, const VectorSetN& second) {
		if (first.isEmpty() && second.isEmpty())
			return true;
		else if (first.isEmpty() || second.isEmpty())
			return false;
		else if (first.elemNum != second.elemNum || first.elemDimension != second.elemDimension)
			return false;
		for (size_t i = 0; i < first.elemNum; ++i) {
			if (second.search(first[i]) < 0)
				return false;
		}

		return true;
	}

	bool operator!=(const VectorSetN& first, const VectorSetN& second) {
		return !(first == second);
	}

	bool VectorSetN::subset(const VectorSetN& other) const {
		if ((*this) == other)
			return true;
		else if (this->isEmpty())
			return false;
		else if (other.isEmpty())
			return true;
		else if (elemDimension != other.elemDimension || other.elemNum > elemNum)
			return false;

		for (size_t i = 0; i < other.elemNum; ++i) {
			if (search(other[i]) < 0)
				return false;
		}

		return true;
	}

	bool VectorSetN::linearyIndependentElements() const {
		if (isEmpty()) 
			return false;
		else if (elemNum > elemDimension) 
			return false;

		for (size_t i = 0; i < elemNum; ++i) {
			if (elements[i].isZeroVector())
				return false;
		}

		size_t rank;
		size_t min = (elemNum <= elemDimension) ? elemNum : elemDimension;
		MyMatrix m((*this));
		rank = m.rank();
		std::cout << "Rang : " << rank << std::endl;
		if (rank < min)
			return false;
		else
			return true;
	}
}