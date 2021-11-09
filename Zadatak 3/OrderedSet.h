#pragma once
#include "IPrintable.h"
#include "IComparable.h"
#include "UnorderedSet.h"
#include <iostream>
#include <cstddef>
#include <vector>

namespace Set {
	class OrderedSet : virtual public UnorderedSet {

	public:
		using UnorderedSet::UnorderedSet;
		OrderedSet(const std::initializer_list<Interface::IComparable*>&);
		OrderedSet(const std::vector<Interface::IComparable*>&);

		OrderedSet(const OrderedSet&);
		OrderedSet& operator=(const OrderedSet&);
		OrderedSet(OrderedSet&&) noexcept;
		OrderedSet& operator=(OrderedSet&&) noexcept;
		virtual ~OrderedSet() { }

		virtual const Interface::IComparable* operator[](int) const override;
		virtual Interface::IComparable* erase(int) override;
		virtual Interface::IComparable* erase() override;

	protected:
		virtual void printItem(std::ostream&) const override;
		void sort() const;

		using UnorderedSet::insert;
		using UnorderedSet::resize;

	private:
		static bool cmp(const IPrintable*, const IPrintable*);
	};
}
