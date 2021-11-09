#pragma once
#include "VectorN.h"
#include <functional>
#include <iostream>
#include <compare>

namespace Vector {

	class ComparableVectorN {
		VectorN* vector{ nullptr };
		double normValue{ 0.0 };
		double(*norm)(const VectorN&) { nullptr };

	public:
		ComparableVectorN(const VectorN&, double(* const f)(const VectorN&) = &VectorN::EuclidDistance);
		ComparableVectorN(size_t d = 1, double(* const f)(const VectorN&) = &VectorN::EuclidDistance);

		ComparableVectorN(const ComparableVectorN&);
		ComparableVectorN& operator=(const ComparableVectorN&);
		ComparableVectorN(ComparableVectorN&&) noexcept;
		ComparableVectorN& operator=(ComparableVectorN&&) noexcept;
		~ComparableVectorN();
		friend std::partial_ordering operator<=>(const ComparableVectorN&, const ComparableVectorN&);
		friend bool operator>(const ComparableVectorN&, const ComparableVectorN&);
		friend bool operator<(const ComparableVectorN&, const ComparableVectorN&);
		friend bool operator>=(const ComparableVectorN&, const ComparableVectorN&);
		friend bool operator<=(const ComparableVectorN&, const ComparableVectorN&);
		friend bool operator!=(const ComparableVectorN&, const ComparableVectorN&);
		friend bool operator==(const ComparableVectorN&, const ComparableVectorN&);
		operator VectorN() const;
		operator OrderedVectorSetN() const;
		size_t dimension()const;

	private:
		void copyComparableVectorN(const ComparableVectorN&);
		void moveComparableVectorN(ComparableVectorN&&);

		bool checkUnordered(const ComparableVectorN& other) const { return ((*this) <=> other) == std::partial_ordering::unordered; }
		friend std::ostream& operator<<(std::ostream&, const ComparableVectorN&);
		inline bool isZeroVector() const { return normValue == 0.0; }

		friend OrderedVectorSetN;
	};
}