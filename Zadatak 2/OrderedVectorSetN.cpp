#include "pch.h"
#include "OrderedVectorSetN.h"
#include "VectorSetN.h"
#include "VectorN.h"
#include <iostream>
#include <iomanip>
#include <functional>

namespace Vector {

	OrderedVectorSetN::OrderedVectorSetN(size_t cap, size_t dim, double(*func)(const VectorN&)) {
		this->set_and_allocate(cap, dim, func);
	}

	OrderedVectorSetN::OrderedVectorSetN(const std::initializer_list<ComparableVectorN>& list) {
		if (list.size() != 0) {

			set_and_allocate(list.size(), list.begin()->dimension(), list.begin()->norm);

			for (auto x : list)
				add(x);
		}
		else
			set_and_allocate();
	}

	OrderedVectorSetN::OrderedVectorSetN(const ComparableVectorN* info, size_t n) {
		if (info != nullptr && n != 0) {
			set_and_allocate(n, info->dimension(), info->norm);

			for (size_t i = 0; i < n; ++i)
				add(info[i]);
		}
		else
			set_and_allocate();
	}

	OrderedVectorSetN::OrderedVectorSetN(const ComparableVectorN& info) {
		set_and_allocate(1, info.vector->dim, info.norm);
		elements[elemNum++] = info;
	}

	OrderedVectorSetN::OrderedVectorSetN(const OrderedVectorSetN& other) {
		this->copyOrderedVectorSetN(other);
	}

	OrderedVectorSetN& OrderedVectorSetN::operator=(const OrderedVectorSetN& other) {
		if (this == &other)
			return *this;
		this->copyOrderedVectorSetN(other);
		return *this;
	}

	OrderedVectorSetN::OrderedVectorSetN(OrderedVectorSetN&& other) noexcept {
		this->moveOrderedVectorSetN(std::move(other));
	}

	OrderedVectorSetN& OrderedVectorSetN::operator=(OrderedVectorSetN&& other) noexcept {
		if (this == &other)
			return *this;
		this->moveOrderedVectorSetN(std::move(other));
		return *this;
	}

	OrderedVectorSetN::~OrderedVectorSetN() {
		delete[] elements;
		elements = nullptr;
		norm = nullptr;
		elemNum = elemDimension = capacity = 0;
	}

	void OrderedVectorSetN::copyOrderedVectorSetN(const OrderedVectorSetN& other) {
		this->~OrderedVectorSetN();
		elemDimension = other.elemDimension;
		elemNum = other.elemNum;
		capacity = other.capacity;
		norm = other.norm;
		elements = new ComparableVectorN[capacity];
		for (size_t i = 0; i < elemNum; ++i)
			elements[i] = other.elements[i];
	}

	void OrderedVectorSetN::moveOrderedVectorSetN(OrderedVectorSetN&& other) {
		this->~OrderedVectorSetN();
		elemDimension = std::move(other.elemDimension);
		elemNum = std::move(other.elemNum);
		capacity = std::move(other.capacity);
		norm = std::move(other.norm);
		elements = std::move(other.elements);

		other.elements = nullptr;
		other.norm = nullptr;
		other.capacity = other.elemDimension = other.elemNum = 0;
	}

	void OrderedVectorSetN::set_and_allocate(size_t cap, size_t dim, double(* const func)(const VectorN&)) {
		elemDimension = (dim == 0) ? 1 : dim;
		elemNum = 0;
		capacity = (cap == 0) ? 1 : cap;
		norm = func;
		elements = new ComparableVectorN[cap];
		elements[elemNum] = ComparableVectorN(dim, func);
	}

	int OrderedVectorSetN::search(const ComparableVectorN& info) const {
		for (size_t i = 0; i < elemNum; ++i) {
			if ((*this)[i] == info)
				return i;
		}
		return -1;
	}

	void OrderedVectorSetN::resize() {
		while (capacity <= elemNum)
			capacity *= 2;
		ComparableVectorN* newElements = new ComparableVectorN[capacity];
		for (size_t i = 0; i < elemNum; ++i)
			newElements[i] = elements[i];
		delete[] elements;
		elements = newElements;
	}

	void OrderedVectorSetN::deleteElementAt(size_t index) {
		if (index >= elemNum)
			return;

		elemNum -= 1;
		for (size_t j = index; j < elemNum; ++j)
			elements[j] = elements[j + 1];
	}

	void OrderedVectorSetN::add(const ComparableVectorN& info) {
		if (isEmpty()) {
			elements[elemNum] = info;
			++elemNum;
			return;
		}

		if (search(info) < 0 && info.dimension() == this->elemDimension && info.norm == this->norm) {
			if (elemNum >= capacity)
				resize();

			size_t i;
			for (i = elemNum - 1; i >= 0 && (operator<(info, elements[i])); i--)
				elements[i + 1] = elements[i];

			elements[i + 1] = info;
			++elemNum;
		}
	}

	void OrderedVectorSetN::erase(const ComparableVectorN& info) {
		int index = search(info);
		if (index >= 0 || !isEmpty())
			deleteElementAt(index);
	}

	OrderedVectorSetN& OrderedVectorSetN::calculate_and_change(const OrderedVectorSetN& info, void(OrderedVectorSetN::* f)(const ComparableVectorN&)) {
		if (elemDimension != info.elemDimension || info.norm != norm)
			return (*this);

		for (size_t i = 0; i < info.elemNum; ++i)
			(this->*f)(info[i]);

		return (*this);
	}

	OrderedVectorSetN& OrderedVectorSetN::operator+=(const OrderedVectorSetN& info) {
		return calculate_and_change(info, &OrderedVectorSetN::add);
	}
	OrderedVectorSetN& OrderedVectorSetN::operator-=(const OrderedVectorSetN& info) {
		return calculate_and_change(info, &OrderedVectorSetN::erase);
	}

	bool operator==(const OrderedVectorSetN& first, const OrderedVectorSetN& second) {
		if (first.isEmpty() && second.isEmpty())
			return true;
		else if (first.isEmpty() || second.isEmpty())
			return false;
		else if (first.elemNum != second.elemNum || first.elemDimension != second.elemDimension || first.norm != second.norm)
			return false;
		for (size_t i = 0; i < first.elemNum; ++i) {
			if (second.search(first[i]) < 0)
				return false;
		}

		return true;
	}

	bool operator!=(const OrderedVectorSetN& first, const OrderedVectorSetN& second) {
		return !(first == second);
	}

	bool OrderedVectorSetN::subset(const OrderedVectorSetN& other) const {
		if ((*this) == other)
			return true;
		else if (this->isEmpty())
			return false;
		else if (other.isEmpty())
			return true;
		else if (elemDimension != other.elemDimension || norm != other.norm || other.elemNum > elemNum)
			return false;

		for (size_t i = 0; i < other.elemNum; ++i) {
			if (search(other[i]) < 0)
				return false;
		}

		return true;
	}

	OrderedVectorSetN::operator VectorSetN() const {
		if (isEmpty())
			return VectorSetN();
		else {
			std::unique_ptr<VectorSetN> tmp(new VectorSetN(elemNum, elemDimension));
			for (size_t i = 0; i < elemNum; ++i) {
				tmp->addElement((VectorN)elements[i]);
			}
			return VectorSetN(*tmp);
		}
	}

	bool operator==(const OrderedVectorSetN& ovs, const VectorSetN& vs) {
		return (((VectorSetN)ovs) == vs);
	}

	bool operator==(const VectorSetN& vs, const OrderedVectorSetN& ovs) {
		return (ovs == vs);
	}

	bool operator!=(const OrderedVectorSetN& ovs, const VectorSetN& vs) {
		return !(ovs == vs);
	}

	bool operator!=(const VectorSetN& vs, const OrderedVectorSetN& ovs) {
		return !(ovs == vs);
	}

	std::ostream& operator<<(std::ostream& stream, const OrderedVectorSetN& info) {
		if (info.isEmpty())
			stream << "Skup je prazan." << std::endl;
		else {
			stream << "{  " << std::endl;
			for (size_t i = 0; i < info.elemNum; ++i)
				stream << "\t" << info[i] << std::endl;
			stream << "}  " << std::endl;
		}

		return stream;
	}

	bool OrderedVectorSetN::linearyIndependentElements() const {
		VectorSetN tmp = (VectorSetN)(*this);
		return tmp.linearyIndependentElements();
	}
}