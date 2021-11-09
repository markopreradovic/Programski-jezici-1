#pragma once
#include <iostream>
#include <exception>

namespace collection {
	namespace exception {
		class IndexOutOfRangeException : public std::out_of_range {
			int index;

		public:
			IndexOutOfRangeException(const std::string&, int);
			IndexOutOfRangeException(const IndexOutOfRangeException&) noexcept;
			IndexOutOfRangeException& operator=(const IndexOutOfRangeException&) noexcept;
			virtual ~IndexOutOfRangeException() { }

		};

		class ExpiredPtrException : public std::exception {
		public:
			using std::exception::exception;
			virtual ~ExpiredPtrException() { }
		};

		class AddingElementException : public std::exception {
		public:
			using std::exception::exception;
			virtual ~AddingElementException() { }

			virtual const char* what() const noexcept override;
		};
	};
};