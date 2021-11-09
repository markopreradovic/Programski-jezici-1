#include "pch.h"
#include "IPrintable.h"
#include "IComparable.h"
#include "BufferedSet.h"
#include "UnorderedSet.h"
#include <iostream>
#include <cstddef>

using std::size_t;

namespace Set {
	BufferedSet::BufferedSet() : UnorderedSet(), additional_capacity(1) {  }

	BufferedSet::BufferedSet(size_t n, size_t add_c) : UnorderedSet(n), additional_capacity((add_c == 0) ? 1 : add_c) {  }

	BufferedSet::BufferedSet(const std::initializer_list<IPrintable*>& list, size_t add_c)
		: UnorderedSet(list), additional_capacity(add_c) { }

	BufferedSet::BufferedSet(const std::vector<IPrintable*>& vector, std::size_t add_c)
		: UnorderedSet(vector), additional_capacity(add_c) {  }

	BufferedSet::BufferedSet(const BufferedSet& other) {
		this->copy(other);
	}

	BufferedSet& BufferedSet::operator=(const BufferedSet& other) {
		if (this != &other) {
			this->destroy();
			this->copy(other);
		}
		return *this;
	}

	BufferedSet::BufferedSet(BufferedSet&& other) noexcept {
		this->move(std::move(other));
	}

	BufferedSet& BufferedSet::operator=(BufferedSet&& other) noexcept {
		if (this != &other) {
			this->destroy();
			this->move(std::move(other));
		}
		return *this;
	}

	BufferedSet::~BufferedSet() {
		additional_capacity = 0;
	}

	void BufferedSet::resize() {
		while (capacity <= number_of_elements)
			capacity += additional_capacity;
		IPrintable** tptr = new IPrintable * [capacity];
		for (size_t i = 0; i < number_of_elements; ++i)
			tptr[i] = elements[i];
		for (size_t i = number_of_elements; i < capacity; ++i)
			tptr[i] = nullptr;
		delete[] elements;
		elements = tptr;
	}

	void BufferedSet::copy(const BufferedSet& other) {
		this->UnorderedSet::copy(other);
		this->additional_capacity = other.additional_capacity;
	}

	void BufferedSet::move(BufferedSet&& other) {
		this->UnorderedSet::move(std::move(other));
		this->additional_capacity = std::move(other.additional_capacity);
	}
};