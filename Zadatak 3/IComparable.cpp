#include "pch.h"
#include "IPrintable.h"
#include "IComparable.h"

namespace Set {
	namespace Interface {
		std::partial_ordering operator<=>(const IComparable& first, const IComparable& second) {
			return first.compareItems(second);
		}

		bool operator<(const IComparable& first, const IComparable& second) {
			return ((first <=> second) == std::partial_ordering::less);
		}

		bool operator>(const IComparable& first, const IComparable& second) {
			return ((first <=> second) == std::partial_ordering::greater);
		}

		bool operator<=(const IComparable& first, const IComparable& second) {
			return !(first > second);
		}

		bool operator>=(const IComparable& first, const IComparable& second) {
			return !(first < second);
		}
	};
};