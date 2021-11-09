#pragma once

#include "ComparableVectorN.h"
#include "VectorN.h"
#include "VectorSetN.h"
#include <iostream>
#include <functional>

namespace Vector {

	class OrderedVectorSetN {
		ComparableVectorN* elements{ nullptr };
		size_t elemNum{ 0 };
		size_t elemDimension{ 0 };
		size_t capacity{ 0 };
		double(*norm)(const VectorN&) { nullptr };

	public:
		OrderedVectorSetN(size_t c = 1, size_t d = 1, double(* const f)(const VectorN&) = &VectorN::EuclidDistance);
		OrderedVectorSetN(const std::initializer_list<ComparableVectorN>&);
		OrderedVectorSetN(const ComparableVectorN*, size_t);
		OrderedVectorSetN(const ComparableVectorN&);
		OrderedVectorSetN(const OrderedVectorSetN&);
		OrderedVectorSetN& operator=(const OrderedVectorSetN&);
		OrderedVectorSetN(OrderedVectorSetN&&) noexcept;
		OrderedVectorSetN& operator=(OrderedVectorSetN&&) noexcept;
		~OrderedVectorSetN();
		OrderedVectorSetN& operator+=(const OrderedVectorSetN&);
		OrderedVectorSetN& operator-=(const OrderedVectorSetN&);
		void add(const ComparableVectorN&);
		void erase(const ComparableVectorN&);
		friend bool operator==(const OrderedVectorSetN&, const OrderedVectorSetN&);
		friend bool operator!=(const OrderedVectorSetN&, const OrderedVectorSetN&);
		bool subset(const OrderedVectorSetN&) const;
		friend bool operator==(const OrderedVectorSetN&, const VectorSetN&);
		friend bool operator==(const VectorSetN&, const OrderedVectorSetN&);
		friend bool operator!=(const OrderedVectorSetN&, const VectorSetN&);
		friend bool operator!=(const VectorSetN&, const OrderedVectorSetN&);
		operator VectorSetN() const;
		bool linearyIndependentElements() const;

	private:
		void copyOrderedVectorSetN(const OrderedVectorSetN&);
		void moveOrderedVectorSetN(OrderedVectorSetN&&);
		void set_and_allocate(size_t cap = 1, size_t dim = 1, double(* const func)(const VectorN&) = &VectorN::EuclidDistance);
		OrderedVectorSetN& calculate_and_change(const OrderedVectorSetN&, void(OrderedVectorSetN::*)(const ComparableVectorN&));
		int search(const ComparableVectorN&) const;
		void resize();
		inline bool isEmpty() const { return elemNum == 0; }
		void deleteElementAt(size_t);

		const ComparableVectorN& getElementAt(size_t index) const { return elements[index]; }
		const ComparableVectorN& operator[](size_t id) const { return getElementAt(id); }
		ComparableVectorN& operator[](size_t id) { return const_cast<ComparableVectorN&>(getElementAt(id)); }

		friend std::ostream& operator<<(std::ostream&, const OrderedVectorSetN&);
	};
}