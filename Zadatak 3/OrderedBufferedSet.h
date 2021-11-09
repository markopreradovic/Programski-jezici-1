#pragma once
#include "IPrintable.h"
#include "IComparable.h"
#include "UnorderedSet.h"
#include "OrderedSet.h"
#include "BufferedSet.h"
#include <iostream>
#include <cstddef>

namespace Set {
	class OrderedBufferedSet : public OrderedSet, public BufferedSet {

	public:
		using BufferedSet::BufferedSet;
		using OrderedSet::OrderedSet;

		OrderedBufferedSet();
		OrderedBufferedSet(std::size_t, std::size_t);
		OrderedBufferedSet(const std::initializer_list<Interface::IComparable*>&, std::size_t);
		OrderedBufferedSet(const std::vector<Interface::IComparable*>&, std::size_t);

		OrderedBufferedSet(const OrderedBufferedSet&);
		OrderedBufferedSet& operator=(const OrderedBufferedSet&);
		OrderedBufferedSet(OrderedBufferedSet&&) noexcept;
		OrderedBufferedSet& operator=(OrderedBufferedSet&&) noexcept;
		virtual ~OrderedBufferedSet() { }

		virtual bool insert(const IPrintable*) override;

	};
};