#include "pch.h"
#include "BufferedColleection.h"
#include "Exceptions.h"
#include "Complex.h"
#include <exception>
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <limits>

namespace collection {
	using namespace complex;
	using namespace exception;
	BufferedCollection::VectorData::VectorData(size_t cap) noexcept(false) {
		try {
			elements = new std::weak_ptr<Complex>[cap];
			capacity = cap;
			size = 0;
		}
		catch (const std::bad_alloc&) {
			elements = nullptr;
			size = capacity = 0;
			throw;
		}
	}

	BufferedCollection::VectorData::VectorData(const BufferedCollection::VectorData& other) noexcept(false) {
		copyData(other);
	}

	BufferedCollection::VectorData& BufferedCollection::VectorData::operator=(const BufferedCollection::VectorData& other) noexcept(false) {
		if (this != &other) {
			BufferedCollection::VectorData tmp = std::move((*this));
			try {
				copyData(other);
				tmp.destroyData();
			}
			catch (const std::bad_alloc&) {
				(*this) = std::move(tmp);
				throw;
			}
		}
		return *this;
	}

	BufferedCollection::VectorData::VectorData(BufferedCollection::VectorData&& other) noexcept {
		moveData(std::move(other));
	}

	BufferedCollection::VectorData& BufferedCollection::VectorData::operator=(BufferedCollection::VectorData&& other) noexcept {
		if (this != &other) {
			destroyData();
			moveData(std::move(other));
		}
		return *this;
	}

	BufferedCollection::VectorData::~VectorData() noexcept {
		destroyData();
	}

	void BufferedCollection::VectorData::copyData(const BufferedCollection::VectorData& other) noexcept(false) {
		try {
			elements = new std::weak_ptr<Complex>[other.capacity];
			capacity = other.capacity;
			size = other.size;
			std::copy(other.elements, other.elements + size, elements);
		}
		catch (const std::bad_alloc&) {
			throw;
		}
	}

	void BufferedCollection::VectorData::moveData(BufferedCollection::VectorData&& other) noexcept {
		elements = std::move(other.elements);
		size = std::move(other.size);
		capacity = std::move(other.capacity);

		other.elements = nullptr;
		other.size = other.capacity = 0;
	}

	void BufferedCollection::VectorData::destroyData() noexcept {
		if (elements)
			delete[] elements;
		size = capacity = 0;
	}

	void BufferedCollection::VectorData::checkValidIndex(int index) const noexcept(false) {
		if (index < 0 || (unsigned)index >= size)
			throw IndexOutOfRangeException("Index " + std::to_string(index) + " is out of range.", index);
	}

	const std::weak_ptr<Complex> BufferedCollection::VectorData::operator[](int index) const noexcept(false) {
		checkValidIndex(index);
		return elements[index];
	}

	std::weak_ptr<Complex> BufferedCollection::VectorData::operator[](int index) noexcept(false) {
		checkValidIndex(index);
		return elements[index];
	}

	void BufferedCollection::VectorData::insertData(const std::weak_ptr<Complex>& data) noexcept(false) {
		if (size == capacity)
			throw std::overflow_error("Array is full.");
		elements[size++] = data;
	}

	std::weak_ptr<Complex> BufferedCollection::VectorData::removeDataAt(int index) noexcept(false) {
		checkValidIndex(index);
		size_t pos = (unsigned)index;
		std::weak_ptr<Complex> tmp = elements[pos];
		for (; pos < size - 1; ++pos)
			elements[pos] = elements[pos + 1];
		--size;
		return tmp;
	}

	int BufferedCollection::VectorData::searchData(const std::weak_ptr<Complex>& rhs) const noexcept {
		if (emptyData())
			return -1;
		else {
			for (size_t i = 0; i < size; ++i) {
				if (!elements[i].owner_before(rhs) && !rhs.owner_before(elements[i]))
					return i;
			}
			return -1;
		}
	}

	BufferedCollection::Node::Node(size_t cap) noexcept(false) : data(cap), next(nullptr), previous(nullptr) { }

	BufferedCollection::Node::Node(const VectorData& info) noexcept(false)
		: data(info), next(nullptr), previous(nullptr) { }


	BufferedCollection::BufferedCollection(size_t cap) noexcept(false)
		: k(cap), n(0), head(nullptr), tail(nullptr) {
		if (cap >= std::numeric_limits<size_t>::max())
			throw std::length_error("Capacity is too large.");
		if (cap == 0)
			throw std::invalid_argument("Size cannot be 0.");
	}

	BufferedCollection::BufferedCollection(const std::initializer_list<std::shared_ptr<Complex>>& list, size_t cap) noexcept(false)
		: BufferedCollection(cap) {
		try {
			for (auto m : list)
				insert(m);
		}
		catch (const std::exception&) {
			destroy();
			throw;
		}
	}

	BufferedCollection::BufferedCollection(const BufferedCollection& other) noexcept(false) {
		copy(other);
	}

	BufferedCollection& BufferedCollection::operator=(const BufferedCollection& other) noexcept(false) {
		if (this != &other) {
			BufferedCollection tmp = std::move((*this));
			try {
				copy(other);
				tmp.destroy();
			}
			catch (const std::bad_alloc&) {
				(*this) = std::move(tmp);
				throw;
			}
			catch (const std::invalid_argument&) {
				(*this) = std::move(tmp);
				throw;
			}
		}
		return *this;
	}

	BufferedCollection::BufferedCollection(BufferedCollection&& other) noexcept {
		move(std::move(other));
	}

	BufferedCollection& BufferedCollection::operator=(BufferedCollection&& other) noexcept {
		if (this != &other) {
			destroy();
			move(std::move(other));
		}
		return *this;
	}

	BufferedCollection::~BufferedCollection() noexcept {
		destroy();
	}

	void BufferedCollection::copy(const BufferedCollection& other) noexcept(false) {
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
			catch (const std::bad_alloc&) {
				destroy();
				throw;
			}
		}
	}

	void BufferedCollection::move(BufferedCollection&& other) noexcept {
		head = std::move(other.head);
		tail = std::move(other.tail);
		n = std::move(other.n);
		k = std::move(other.k);

		other.head = other.tail = nullptr;
		other.n = other.k = 0;
	}

	void BufferedCollection::destroy() noexcept {
		while (head) {
			Node* tmp = head;
			head = head->next;
			delete tmp;
		}
		head = tail = nullptr;
		n = k = 0;
	}


	bool BufferedCollection::search(const std::weak_ptr<Complex>& info) const noexcept {
		for (BufferedCollection::Node* tmp = head; tmp; tmp = tmp->next) {
			if (tmp->data.searchData(info) >= 0)
				return true;
		}
		return false;
	}

	void BufferedCollection::insert(const std::weak_ptr<Complex>& info) noexcept(false) {
		if (info.expired())
			throw AddingElementException("Expired object cannot be inserted.");
		if (empty()) {
			try {
				Node* newNode = new Node(k);
				newNode->data.insertData(info);
				head = tail = newNode;
				++n;
			}
			catch (const std::exception& ex) {
				throw AddingElementException(ex.what());
			}
		}
		else {
			if (!search(info)) {
				if (tail->data.fullData()) {
					try {
						Node* newNode = new Node(k);
						tail->next = newNode;
						newNode->previous = tail;
						tail = tail->next;
						++n;
					}
					catch (const std::exception&) {
						throw AddingElementException("Cannot add element.");
					}
				}
				tail->data.insertData(info);
			}
		}
	}

	void BufferedCollection::checkIndex(int index) const noexcept(false) {
		if (index < 0 || (unsigned)index >= totalElementsNum())
			throw IndexOutOfRangeException("Index " + std::to_string(index) + " is out of range.", index);
	}

	std::pair<size_t, size_t> BufferedCollection::composeIndices(int index) const noexcept(false) {
		checkIndex(index);
		size_t node_num = (unsigned)index / k;
		size_t index_num = ((unsigned)index + k) % k;
		return std::pair<size_t, size_t>(node_num, index_num);
	}

	BufferedCollection::Node* BufferedCollection::getAt(size_t i) const {
		BufferedCollection::Node* node;
		for (node = head; i; node = node->next, --i);
		return node;
	}

	void BufferedCollection::moveElements(BufferedCollection::Node* node) noexcept {
		while (node != tail) {
			node->data.insertData(node->next->data.removeDataAt(0));
			node = node->next;
		}

		if (node->data.emptyData()) {
			if (node == head && tail == head)
				head = tail = nullptr;
			else {
				tail = tail->previous;
				tail->next = nullptr;
			}
			delete node;
			--n;
		}
	}

	const std::weak_ptr<Complex> BufferedCollection::operator[](int index) noexcept(false) {
		auto [node_num, index_num] = composeIndices(index);
		BufferedCollection::Node* node = getAt(node_num);
		try {
			std::weak_ptr<Complex> toReturn = node->data[index_num];
			if (!toReturn.expired())
				return toReturn;
			else {
				node->data.removeDataAt(index_num);
				moveElements(node);
				throw ExpiredPtrException("Expired element access.");
			}
		}
		catch (const IndexOutOfRangeException&) {
			throw;
		}
	}

	std::weak_ptr<Complex> BufferedCollection::removeAt(int index) noexcept(false) {
		auto [node_num, index_num] = composeIndices(index);
		BufferedCollection::Node* node = getAt(node_num);
		try {
			std::weak_ptr<Complex> toDelete = node->data.removeDataAt(index_num);
			moveElements(node);
			return toDelete;
		}
		catch (const IndexOutOfRangeException&) {
			throw;
		}
	}

	std::ostream& operator<<(std::ostream& stream, const BufferedCollection& info) noexcept {
		if (info.empty())
			stream << "List is empty.";
		else {
			BufferedCollection::Node* current = info.head;
			while (current) {
				for (size_t i = 0; i < current->data.size; ++i) {
					//std::weak_ptr<Complex>& tmp = current->data[i];
					if (auto ptr = (current->data[i]).lock())
						stream << *ptr << " ";
				}
				if (current->next)
					stream << "," << std::endl;
				current = current->next;
			}
		}
		return stream;
	}
};