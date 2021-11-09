#include "pch.h"
#include "IPrintable.h"
#include <iostream>

namespace Set {
	namespace Interface {
		std::ostream& operator<<(std::ostream& stream, const IPrintable& item) {
			item.printItem(stream);
			return stream;
		}

		std::ostream& operator<<(std::ostream& stream, const IPrintable* item) {
			if (item != nullptr)
				stream << (*item);
			else
				stream << "nullptr";
			return stream;
		}

		bool operator==(const IPrintable& first, const IPrintable& second) {
			return first.equalItems(second);
		}

		bool operator!=(const IPrintable& first, const IPrintable& second) {
			return !(first == second);
		}
	};
};