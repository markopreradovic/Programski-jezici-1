#include "pch.h"
#include "IPrintable.h"
#include "IComparable.h"
#include "OrderedSet.h"
#include "UnorderedSet.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <algorithm>

using std::size_t;

namespace Set {
	using namespace Interface;

	OrderedSet::OrderedSet(const std::initializer_list<IComparable*>& list) : UnorderedSet(list.size()) {
		if (list.size() != 0) {
			for (auto m : list)
				insert(m);
		}
	}

	OrderedSet::OrderedSet(const std::vector<IComparable*>& v) : UnorderedSet(v.size()) {
		if (!v.empty()) {
			for (auto m : v)
				insert(m);
		}
	}

	OrderedSet::OrderedSet(const OrderedSet& other)
		: UnorderedSet(other) { }

	OrderedSet& OrderedSet::operator=(const OrderedSet& other) {
		if (this != &other)
			UnorderedSet::operator=(other);
		return *this;
	}

	OrderedSet::OrderedSet(OrderedSet&& other) noexcept
		: UnorderedSet(std::move(other)) { }

	OrderedSet& OrderedSet::operator=(OrderedSet&& other) noexcept {
		if (this != &other)
			UnorderedSet::operator=(std::move(other));
		return *this;
	}

	bool OrderedSet::cmp(const IPrintable* i, const IPrintable* j) {
		const IComparable* ci = dynamic_cast<const IComparable*>(i);
		const IComparable* cj = dynamic_cast<const IComparable*>(j);

		if (ci && cj) {
			return *ci < *cj;
		}
		else
			return false;
	}

	void OrderedSet::sort() const
	{
		std::sort(elements, elements + number_of_elements, cmp);
	}

	const IComparable* OrderedSet::operator[](int index) const {
		if (empty() || !validIndexRange(index))
			return nullptr;
		else {
			sort();
			return dynamic_cast<const IComparable*>(elements[index]);
		}
	}

	void OrderedSet::printItem(std::ostream& stream) const {
		sort();
		UnorderedSet::printItem(stream);
	}


	IComparable* OrderedSet::erase(int index) {
		if (!validIndexRange(index))
			return nullptr;
		else {
			sort();
			return dynamic_cast<IComparable*>(UnorderedSet::erase(index));
		}
	}

	IComparable* OrderedSet::erase() {
		sort();
		return dynamic_cast<IComparable*>(UnorderedSet::erase());
	}

};