#include "pch.h"
#include "Array.h"
#include <iostream>

unsigned char zad1::Array::retVal = 0b1111'1111;

namespace zad1 {
	Array::Array(int n) {
		arrSize = (n <= 0) ? 1 : n;
		array = new unsigned char[arrSize];
		for (int i = 0; i < arrSize; i++)
			array[i] = 0;
	}

	Array::Array(const std::initializer_list<unsigned char> list) : Array(list.size()) {
		int i = 0;
		for (auto x : list)
			this->array[i++] = x;
	}

	Array::Array(const Array& other) : Array(other.getArrSize()) {
		for (int i = 0; i < arrSize; i++)
			array[i] = other.array[i];
	}

	Array::Array(Array&& other) noexcept : array(other.array), arrSize(other.arrSize) {
		other.array = nullptr;
		other.arrSize = 0;
	}

	Array::~Array() {
		if (array != nullptr)
			delete[] array;
	}

	int Array::compareArr(const Array& other) const {
		int firstIndex = 0, secondIndex = 0;          
		while ((firstIndex < this->arrSize) && (secondIndex < other.arrSize)) {
			if ((this->array[firstIndex] != 0) && (other.array[secondIndex] != 0)) {
				if (this->array[firstIndex] == other.array[secondIndex]) {
					++firstIndex;
					++secondIndex;
					break;
				}
				else if ((this->arrSize - firstIndex) > (other.arrSize - secondIndex))
					return 1;
				else if ((this->arrSize - firstIndex) < (other.arrSize - secondIndex))
					return -1;
				else
					return this->array[firstIndex] - other.array[secondIndex];
			}
			else {
				if (this->array[firstIndex] == 0)
					++firstIndex;
				if (other.array[secondIndex] == 0)
					++secondIndex;
			}
		}

		if ((firstIndex == this->arrSize) && (secondIndex == other.arrSize))
			return 0;


		while ((firstIndex < this->arrSize) && (secondIndex < other.arrSize) && (this->array[firstIndex] == other.array[secondIndex])) {
			++firstIndex;
			++secondIndex;
		}

		if ((firstIndex >= this->arrSize) && (secondIndex >= other.arrSize))
			return 0;
		else if ((firstIndex < this->arrSize) && (secondIndex >= other.arrSize))
			return 1;
		else if ((firstIndex >= this->arrSize) && (secondIndex < other.arrSize))
			return -1;
		else {
			if ((this->arrSize - firstIndex) > (other.arrSize - secondIndex))
				return 1;
			else if ((this->arrSize - firstIndex) < (other.arrSize - secondIndex))
				return -1;
			else
				return this->array[firstIndex] - other.array[secondIndex];
		}
	}

	const unsigned char& Array::getElement(const int ind, bool& err) const {
		err = !(checkValidIndexRange(ind));
		return err ? retVal : this->array[ind];
	}

	unsigned char& Array::getElement(const int ind, bool& err) {
		err = !(checkValidIndexRange(ind));
		return err ? retVal : this->array[ind];
	}

	void Array::printArr() const {
		for (int i = 0; i < arrSize; i++)
			std::cout << array[i] << "  ";
		std::cout << std::endl;
	}

	void Array::printArrHex() const {
		for (int i = 0; i < arrSize; i++)
			std::cout << std::hex << (int)this->array[i] << "   ";

		std::cout << std::endl;
	}
}