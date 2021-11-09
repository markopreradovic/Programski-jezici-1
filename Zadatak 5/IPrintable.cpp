#include "pch.h"
#include "IPrintable.h"
#include <iostream>

namespace Interface {
	std::ostream& operator<<(std::ostream& stream, const IPrintable& item) noexcept {
		item.printItem(stream);
		return stream;
	}
};