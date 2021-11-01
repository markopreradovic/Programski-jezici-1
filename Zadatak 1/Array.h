#pragma once
#include <iostream>

namespace zad1 {
	class Array {
		unsigned char* array;
		int arrSize;

	public:
		explicit Array(int);
		explicit Array(const std::initializer_list<unsigned char>);
		Array(const Array&);
		Array(Array&&)noexcept;
		~Array();

		Array& operator=(const Array&) = delete;
		Array& operator=(Array&&) = delete;

		static unsigned char retVal;

		const unsigned char& getElement(const int, bool&) const;
		unsigned char& getElement(const int, bool&);
		inline int getArrSize() const { return arrSize; }
		int compareArr(const Array&)const;
		void printArr() const;
		void printArrHex() const;

	private:
		inline bool checkValidIndexRange(const int index) const { return (index < 0 || index >= arrSize) ? false : true; }
	};
}
