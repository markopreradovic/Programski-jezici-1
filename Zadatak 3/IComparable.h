#pragma once
#include "IPrintable.h"
#include <iostream>

namespace Set {
	namespace Interface {
		class IComparable : virtual public IPrintable {
		public:
			friend std::partial_ordering operator<=>(const IComparable&, const IComparable&);
			friend bool operator<(const IComparable&, const IComparable&);
			friend bool operator>(const IComparable&, const IComparable&);
			friend bool operator<=(const IComparable&, const IComparable&);
			friend bool operator>=(const IComparable&, const IComparable&);
			virtual ~IComparable() { }

		protected:
			virtual std::partial_ordering compareItems(const IComparable&) const = 0;
		};
	};
};
