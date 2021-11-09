#include "pch.h"
#include "Exceptions.h"
#include <exception>
#include <iostream>
#include <string>

namespace collection {
	namespace exception {
		IndexOutOfRangeException::IndexOutOfRangeException(const std::string& message, int ind)
			: out_of_range(message), index(ind) { }

		IndexOutOfRangeException::IndexOutOfRangeException(const IndexOutOfRangeException& other) noexcept
			: out_of_range(other), index(other.index) { }

		IndexOutOfRangeException& IndexOutOfRangeException::operator=(const IndexOutOfRangeException& other) noexcept {
			if (this != &other) {
				out_of_range::operator=(other);
				index = other.index;
			}
			return *this;
		}

		const char* AddingElementException::what() const noexcept {
			return "New element cannot be added.";
		}
	};
};