#pragma once
#include <iostream>

namespace Set {
	namespace Interface {
		class IPrintable {
		public:
			friend std::ostream& operator<<(std::ostream&, const IPrintable&);
			friend std::ostream& operator<<(std::ostream&, const IPrintable*);
			virtual ~IPrintable() { }
			friend bool operator==(const IPrintable&, const IPrintable&);
			friend bool operator!=(const IPrintable&, const IPrintable&);

		protected:
			virtual void printItem(std::ostream&) const = 0;
			virtual bool equalItems(const IPrintable&) const = 0;
		};
	};
};