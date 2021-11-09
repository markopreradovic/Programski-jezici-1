#include "pch.h"
#include "IPrintable.h"
#include "IComparable.h"
#include "UnorderedSet.h"
#include <iostream>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <typeinfo>
#include <vector>

using std::size_t;

namespace Set {
	UnorderedSet::UnorderedSet() : UnorderedSet(1) {  }

	UnorderedSet::UnorderedSet(size_t n) {
		this->allocate(n);
		std::cout << "constructor" << std::endl;
	}

	UnorderedSet::UnorderedSet(const std::initializer_list<IPrintable*>& list)
		: UnorderedSet(list.size()) {
		if (list.size() != 0) {
			for (auto m : list)
				insert(m);
		}
	}

	UnorderedSet::UnorderedSet(const std::vector<IPrintable*>& vector)
		: UnorderedSet(vector.size()) {
		if (!vector.empty()) {
			for (auto m : vector)
				insert(m);
		}
	}

	UnorderedSet::UnorderedSet(const UnorderedSet& other) {
		this->copy(other);
	}

	UnorderedSet& UnorderedSet::operator=(const UnorderedSet& other) {
		if (this != &other) {
			this->destroy();
			this->copy(other);
		}
		return *this;
	}

	UnorderedSet::UnorderedSet(UnorderedSet&& other) noexcept {
		this->move(std::move(other));
	}

	UnorderedSet& UnorderedSet::operator=(UnorderedSet&& other) noexcept {
		if (this != &other) {
			this->destroy();
			this->move(std::move(other));
		}
		return *this;
	}

	UnorderedSet::~UnorderedSet() {
		this->destroy();
		std::cout << "destructor" << std::endl;
	}

	void UnorderedSet::allocate(size_t n) {
		this->elements = new IPrintable * [this->capacity = (n == 0) ? 1 : n];
		std::fill(elements, elements + capacity, nullptr);
		number_of_elements = 0;
	}

	void UnorderedSet::destroy() {
		if (elements != nullptr) {
			delete[] elements;
		}
		elements = nullptr;
		capacity = number_of_elements = 0;
	}

	void UnorderedSet::copy(const UnorderedSet& other) {
		if (other.empty() || other.elements == nullptr)
			this->allocate(1);
		else {
			this->elements = new IPrintable * [this->capacity = other.capacity];
			this->number_of_elements = other.number_of_elements;
			std::uninitialized_copy(other.elements, other.elements + other.number_of_elements, this->elements);
		}
	}

	void UnorderedSet::move(UnorderedSet&& other) {
		this->elements = std::move(other.elements);
		this->number_of_elements = std::move(other.number_of_elements);
		this->capacity = std::move(other.capacity);

		other.elements = nullptr;
		other.capacity = other.number_of_elements = 0;
	}

	int UnorderedSet::search(const IPrintable* data) const {
		if (empty())
			return -1;
		for (size_t i = 0; i < number_of_elements; ++i) {
			if (*elements[i] == *data)
				return i;
		}

		return -1;
	}

	const Interface::IPrintable* UnorderedSet::operator[](int index) const {
		if (!validIndexRange(index))
			return nullptr;
		else
			return elements[index];
	}

	void UnorderedSet::printItem(std::ostream& stream) const {
		if (empty())
			stream << "Skup je prazan.";
		else {
			stream << "{ ";
			for (size_t i = 0; i < number_of_elements; ++i) {
				if (elements[i] != nullptr)
					stream << *elements[i] << " ";
			}
			stream << " }";
		}
	}

	bool UnorderedSet::equalItems(const IPrintable& other) const {
		const UnorderedSet* other_u = dynamic_cast<const UnorderedSet*>(&other);
		if (!other_u)
			return false;
		if (other_u->number_of_elements != this->number_of_elements)
			return false;

		for (size_t i = 0; i < number_of_elements; ++i) {
			if (search(other_u->elements[i]) < 0)
				return false;
		}
		return true;
	}

	Interface::IPrintable* UnorderedSet::erase(int index) {
		if (empty() || !validIndexRange(index))
			return nullptr;
		else {
			size_t pos = (unsigned)index;
			IPrintable* tmp_ptr = elements[pos];
			for (; pos < number_of_elements - 1; ++pos)
				elements[pos] = elements[pos + 1];
			--number_of_elements;
			return tmp_ptr;
		}
	}

	Interface::IPrintable* UnorderedSet::erase() {
		return erase(number_of_elements - 1);
	}

	bool UnorderedSet::insert(const IPrintable* data) {
		if (data == nullptr)
			return false;

		if (empty()) {
			if (number_of_elements >= capacity)
				resize();
			elements[number_of_elements++] = const_cast<IPrintable*>(data);
			return true;
		}
		
		if (strcmp(typeid(*data).name(), typeid(*elements[0]).name()) != 0)
			return false;
		else {
			if (search(data) >= 0)
				return false;
			else {
				if (number_of_elements >= capacity)
					resize();
				elements[number_of_elements++] = const_cast<IPrintable*>(data);
				return true;
			}
		}
	}

	void UnorderedSet::resize() {
		while (capacity <= number_of_elements)
			capacity *= 2;
		IPrintable** tmp_ptr = new IPrintable * [capacity];
		for (size_t i = 0; i < number_of_elements; ++i)
			tmp_ptr[i] = elements[i];
		for (size_t i = number_of_elements; i < capacity; ++i)
			tmp_ptr[i] = nullptr;
		delete[] elements;
		elements = tmp_ptr;
	}
};