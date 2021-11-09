#pragma once
#include "BufferedCollectionHeader.h"
#include "IPrintable.h"
#include <iostream>
#include <vector>
#include <exception>
#include <limits>
#include <algorithm>

namespace GenericCollection {
	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::VectorT::VectorT(size_t cap) noexcept(false)
		: elements(new T[cap]), capacity(cap), size(0) { }

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::VectorT::VectorT(const BufferedCollection::VectorT& other) {
		copyVector(other);
	}

	template <typename T>
	requires AddPrintCmp<T>
		typename BufferedCollection<T>::VectorT& BufferedCollection<T>::VectorT::operator=(const BufferedCollection::VectorT& other) {
		if (this != &other) {
			typename BufferedCollection::VectorT tmp = std::move(*this);
			try {
				copyVector(other);
				tmp.destroyVector();
			}
			catch (const std::exception&) {
				*this = std::move(tmp);
				throw;
			}
		}
		return *this;
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::VectorT::VectorT(BufferedCollection::VectorT&& other) noexcept {
		moveVector(std::move(other));
	}

	template <typename T>
	requires AddPrintCmp<T>
		typename BufferedCollection<T>::VectorT& BufferedCollection<T>::VectorT::operator=(BufferedCollection::VectorT&& other) noexcept {
		if (this != &other) {
			destroyVector();
			moveVector(std::move(other));
		}
		return *this;
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::VectorT::~VectorT() noexcept {
		destroyVector();
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::VectorT::destroyVector() noexcept {
		if (elements)
			delete[] elements;
		elements = nullptr;
		size = capacity = 0;
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::VectorT::moveVector(BufferedCollection::VectorT&& other) noexcept {
		elements = std::move(other.elements);
		size = other.size;
		capacity = other.capacity;

		other.elements = nullptr;
		other.size = other.capacity = 0;
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::VectorT::copyVector(const BufferedCollection::VectorT& other) noexcept(false) {
		elements = new T[other.capacity];
		capacity = other.capacity;
		size = other.size;
		std::copy(other.elements, other.elements + size, elements);
	}

	template <typename T>
	requires AddPrintCmp<T>
		void inline BufferedCollection<T>::VectorT::insertItem(const T& info) noexcept {
		elements[size++] = info;
	}

	template <typename T>
	requires AddPrintCmp<T>
		T BufferedCollection<T>::VectorT::removeItemAt(size_t index) noexcept {
		T toReturn = elements[index];
		for (; index < size - 1; ++index)
			elements[index] = elements[index + 1];
		--size;
		return toReturn;
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::VectorT::checkIndexRange(int index) const noexcept(false) {
		if (index < 0 || (unsigned)index >= size)
			throw std::out_of_range("Index is out of range.");
	}

	template <typename T>
	requires AddPrintCmp<T>
		const T& BufferedCollection<T>::VectorT::at(size_t index) const noexcept(false) {
		checkIndexRange(index);
		return elements[index];
	}

	template <typename T>
	requires AddPrintCmp<T>
		const T& BufferedCollection<T>::VectorT::operator[](size_t index) const noexcept(false) {
		return at(index);
	}

	template <typename T>
	requires AddPrintCmp<T>
		T& BufferedCollection<T>::VectorT::operator[](size_t index) noexcept(false) {
		return const_cast<T&>(at(index));
	}

	template <typename T>
	requires AddPrintCmp<T>
	T BufferedCollection<T>::VectorT::elemSum() const noexcept {
		T sum;
		if (std::is_arithmetic<T>::value)
			sum = 0;
		std::for_each(elements, elements + size, [&sum](const T& t) {sum = sum + t; });
		return sum;
	}

	template <typename T>
	requires AddPrintCmp<T>
		int BufferedCollection<T>::VectorT::findData(const T& info) const noexcept {
		if (emptyVector())
			return -1;
		for (size_t i = 0; i < size; ++i) {
			if (elements[i] == info)
				return i;
		}
		return -1;
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::BufferedCollection(size_t cap) noexcept(false)
		: k(cap), n(0), head(nullptr), tail(nullptr) {
		if (cap == 0 || cap >= std::numeric_limits<size_t>::max())
			throw std::invalid_argument("Capacity value is undefined.");
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::BufferedCollection(const std::vector<T>& vector, size_t cap) noexcept(false)
		: BufferedCollection(cap) {
		try {
			for (auto m : vector)
				push_back(m);
		}
		catch (const std::exception&) {
			destroy();
			throw;
		}
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::BufferedCollection(const BufferedCollection& other) noexcept(false) {
		copy(other);
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>& BufferedCollection<T>::operator=(const BufferedCollection& other) noexcept(false) {
		if (this != &other) {
			BufferedCollection tmp = std::move(*this);
			try {
				copy(other);
				tmp.destroy();
			}
			catch (const std::exception&) {
				*this = std::move(tmp);
				throw;
			}
		}
		return *this;
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::BufferedCollection(BufferedCollection&& other) noexcept {
		move(std::move(other));
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>& BufferedCollection<T>::operator=(BufferedCollection&& other) noexcept {
		if (this != &other) {
			destroy();
			move(std::move(other));
		}
		return *this;
	}

	template <typename T>
	requires AddPrintCmp<T>
		BufferedCollection<T>::~BufferedCollection() noexcept {
		destroy();
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::copy(const BufferedCollection& other) noexcept(false) {
		if (other.k == 0)
			throw std::invalid_argument("Invalid object.");
		k = other.k;
		n = other.n;
		BufferedCollection::Node* current = other.head;
		if (current) {
			try {
				BufferedCollection::Node* tmp = head = new Node(current->data);
				while (current->next) {
					try {
						tmp->next = new Node(current->next->data);
						tmp->next->previous = tmp;
						tmp = tmp->next;
						current = current->next;
					}
					catch (const std::bad_alloc&) {
						throw;
					}
				}
				tail = tmp;
			}
			catch (const std::exception&) {
				destroy();
				throw;
			}
		}
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::move(BufferedCollection&& other) noexcept {
		head = std::move(other.head);
		tail = std::move(other.tail);
		n = std::move(other.n);
		k = std::move(other.k);

		other.head = other.tail = nullptr;
		other.k = other.n = 0;
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::destroy() noexcept {
		while (head) {
			Node* tmp = head;
			head = head->next;
			delete tmp;
		}
		head = tail = nullptr;
		k = n = 0;
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::checkValidIndex(int index) const noexcept(false) {
		if (index < 0 || (unsigned)index >= total())
			throw std::out_of_range("Index is out of range.");
	}

	template <typename T>
	requires AddPrintCmp<T>
		std::pair<size_t, size_t> BufferedCollection<T>::composeIndices(int index) const noexcept(false) {
		checkValidIndex(index);
		size_t node_num = (unsigned)index / k;
		size_t index_num = ((unsigned)index + k) % k;
		return std::pair<size_t, size_t>(node_num, index_num);
	}

	template <typename T>
	requires AddPrintCmp<T>
		typename BufferedCollection<T>::Node* BufferedCollection<T>::getNodeAt(size_t index) const noexcept {
		BufferedCollection::Node* node;
		size_t middle = n / 2;
		if (index <= middle)  // ako je cvor koji trazimo u prvoj polovini liste, krecemo od pocetka liste
			for (node = head; index; node = node->next, --index);
		else {                // ali ako se cvor nalazi u drugoj polovini liste, krecemo of kraja liste
			index = n - index - 1;
			for (node = tail; index; node = node->previous, --index);
		}
		return node;
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::moveElements(typename BufferedCollection::Node* current) noexcept {
		while (current != tail) {
			current->data.insertItem(current->next->data.removeItemAt(0));
			current = current->next;
		}

		if (current->data.emptyVector()) {
			if (current == head && current == tail)
				head = tail = nullptr;
			else {
				tail = tail->previous;
				tail->next = nullptr;
			}
			delete current;
			--n;
		}
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::push_back(const T& info) noexcept(false) {
		if (find(info) < 0) {
			if (empty() || tail->data.fullVector()) {
				BufferedCollection::Node* newNode = nullptr;
				try {
					newNode = new Node(k);
				}
				catch (const std::exception&) {
					if (newNode != nullptr)
						delete newNode;
					throw;
				}
				newNode->data.insertItem(info);
				if (empty())
					head = tail = newNode;
				else {
					tail->next = newNode;
					newNode->previous = tail;
					tail = tail->next;
				}
				++n;
			}
			else
				tail->data.insertItem(info);
		}
	}

	template <typename T>
	requires AddPrintCmp<T>
		T BufferedCollection<T>::pop_back() noexcept(false) {
		return removeAt(total() - 1);
	}

	template <typename T>
	requires AddPrintCmp<T>
		T BufferedCollection<T>::removeAt(int index) noexcept(false) {
		auto [nodeNum, indexNum] = composeIndices(index);
		BufferedCollection::Node* current = getNodeAt(nodeNum);
		T toReturn = current->data.removeItemAt(indexNum);
		moveElements(current);
		return toReturn;
	}

	template <typename T>
	requires AddPrintCmp<T>
		T BufferedCollection<T>::remove(const T& info) noexcept(false) {
		int arg = find(info);
		if (arg < 0)
			throw std::invalid_argument("Element does not exist in list.");
		return removeAt(arg);
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::clear() noexcept {
		destroy();
	}

	template <typename T>
	requires AddPrintCmp<T>
		const T& BufferedCollection<T>::getElementAt(int index) const noexcept(false) {
		auto [nodeNum, indexNum] = composeIndices(index);
		typename BufferedCollection::Node* current = getNodeAt(nodeNum);
		return current->data[indexNum];
	}

	template <typename T>
	requires AddPrintCmp<T>
		const T& BufferedCollection<T>::operator[](int index) const noexcept(false) {
		return getElementAt(index);
	}

	template <typename T>
	requires AddPrintCmp<T>
		T& BufferedCollection<T>::operator[](int index) noexcept(false) {
		return const_cast<T&>(getElementAt(index));
	}

	template <typename T>
	requires AddPrintCmp<T>
		void BufferedCollection<T>::printItem(std::ostream& stream) const noexcept {
		if (empty())
			stream << "List is empty.";
		std::vector<T> sorted;
		BufferedCollection::Node* current = head;
		while (current) {
			for (size_t i = 0; i < current->data.size; ++i)
				sorted.push_back(current->data[i]);
			current = current->next;
		}
		std::sort(sorted.begin(), sorted.end());
		for (auto m : sorted)
			stream << m << " ";
	}

	template <typename T>
	requires AddPrintCmp<T>
		T BufferedCollection<T>::sum(T initialValue) const {
		T sum = initialValue;
		BufferedCollection<T>::Node* current = head;
		while (current) {
			sum = sum + current->data.elemSum();
			current = current->next;
		}
		return sum;
	}

	template <typename T>
	requires AddPrintCmp<T>
		int BufferedCollection<T>::find(const T& info) const noexcept {
		if (empty())
			return -1;
		size_t nodeIndex = 0;
		int elementIndex = 0;
		BufferedCollection::Node* tmpNode = head;
		while (tmpNode) {
			elementIndex = tmpNode->data.findData(info);
			if (elementIndex < 0) {
				tmpNode = tmpNode->next;
				++nodeIndex;
			}
			else
				return (nodeIndex * k + (unsigned)elementIndex);
		}
		return -1;
	}
};