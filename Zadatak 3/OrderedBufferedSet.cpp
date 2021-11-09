#include "pch.h"
#include "IPrintable.h"
#include "IComparable.h"
#include "UnorderedSet.h"
#include "OrderedSet.h"
#include "BufferedSet.h"
#include "OrderedBufferedSet.h"
#include <iostream>
#include <cstddef>

using std::size_t;

namespace Set {
	using namespace Interface;

	OrderedBufferedSet::OrderedBufferedSet()
		: UnorderedSet(), BufferedSet(), OrderedSet() {  }

	OrderedBufferedSet::OrderedBufferedSet(size_t c, size_t add_cap)
		: UnorderedSet(c), BufferedSet(c, add_cap), OrderedSet(c) {  }

	OrderedBufferedSet::OrderedBufferedSet(const std::initializer_list<IComparable*>& list, size_t add_cap)
		: UnorderedSet(list.size()), BufferedSet(list.size(), add_cap), OrderedSet(list) {  }

	OrderedBufferedSet::OrderedBufferedSet(const std::vector<IComparable*>& v, std::size_t add_cap)
		: UnorderedSet(v.size()), BufferedSet(v.size(), add_cap), OrderedSet(v) {  }

	OrderedBufferedSet::OrderedBufferedSet(const OrderedBufferedSet& other)
		: UnorderedSet(other), BufferedSet(other), OrderedSet(other) {  }

	OrderedBufferedSet& OrderedBufferedSet::operator=(const OrderedBufferedSet& other) {
		if (this != &other)
			BufferedSet::operator=(other);
		return *this;
	}

	OrderedBufferedSet::OrderedBufferedSet(OrderedBufferedSet&& other) noexcept
		: UnorderedSet(std::move(other)), BufferedSet(std::move(other)), OrderedSet(std::move(other)) {  }

	OrderedBufferedSet& OrderedBufferedSet::operator=(OrderedBufferedSet&& other) noexcept {
		if (this != &other)
			BufferedSet::operator=(std::move(other));
		return *this;
	}

	bool OrderedBufferedSet::insert(const IPrintable* data) {
		IComparable* data_c = dynamic_cast<IComparable*>((const_cast<IPrintable*>(data)));
		if (data_c == nullptr)
			return false;
		else
			return BufferedSet::insert(data_c);
	}

};