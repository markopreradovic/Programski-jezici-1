#pragma once
#include "IPrintable.h"
#include "IComparable.h"
#include "UnorderedSet.h"
#include <iostream>
#include <cstddef>
#include <vector>

namespace Set {
	class BufferedSet : virtual public UnorderedSet {

	protected:
		std::size_t additional_capacity;

	public:
		using UnorderedSet::UnorderedSet;
		BufferedSet();
		BufferedSet(std::size_t, std::size_t);
		BufferedSet(const std::initializer_list<IPrintable*>&, std::size_t);
		BufferedSet(const std::vector<IPrintable*>&, std::size_t);

		BufferedSet(const BufferedSet&);
		BufferedSet& operator=(const BufferedSet&);
		BufferedSet(BufferedSet&&) noexcept;
		BufferedSet& operator=(BufferedSet&&) noexcept;
		virtual ~BufferedSet();

	protected:
		void copy(const BufferedSet&);
		void move(BufferedSet&&);
		virtual void resize() override;
	};
};