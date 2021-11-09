#pragma once
#include <iostream>

namespace Interface {

	class IPrintable {
	protected:
		virtual void printItem(std::ostream&) const noexcept = 0;

	public:
		friend std::ostream& operator<<(std::ostream&, const IPrintable&) noexcept;
		virtual ~IPrintable() { }
	};
};