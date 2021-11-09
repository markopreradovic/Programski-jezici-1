#pragma once
#include "Complex.h"
#include "Exceptions.h"
#include <iostream>
#include <memory>
#include <exception>
#include <vector>
#include <utility>

namespace collection {
	class BufferedCollection {
		struct VectorData {
			std::weak_ptr<complex::Complex>* elements{ nullptr };
			size_t size{ 0 };
			size_t capacity{ 0 };

			explicit VectorData(size_t) noexcept(false);
			VectorData(const VectorData&) noexcept(false);
			VectorData& operator=(const VectorData&) noexcept(false);
			VectorData(VectorData&&) noexcept;
			VectorData& operator=(VectorData&&) noexcept;
			~VectorData() noexcept;

			void copyData(const VectorData&) noexcept(false);
			void moveData(VectorData&&) noexcept;
			void destroyData() noexcept;

			const std::weak_ptr<complex::Complex> operator[](int) const noexcept(false);
			std::weak_ptr<complex::Complex> operator[](int) noexcept(false);
			void insertData(const std::weak_ptr<complex::Complex>&) noexcept(false);
			std::weak_ptr<complex::Complex> removeDataAt(int) noexcept(false);
			int searchData(const std::weak_ptr<complex::Complex>&) const noexcept;
			bool inline emptyData() const noexcept { return size == 0; }
			bool inline fullData() const noexcept { return size == capacity; }

		private:
			void checkValidIndex(int) const noexcept(false);
		};

		struct Node {
			VectorData data;
			Node* next;
			Node* previous;

			explicit Node(size_t) noexcept(false);
			explicit Node(const VectorData&) noexcept(false);
		};

		Node* head{ nullptr };
		Node* tail{ nullptr };
		size_t n{ 0 };
		size_t k{ 0 };

	public:
		explicit BufferedCollection(size_t) noexcept(false);
		BufferedCollection(const std::initializer_list<std::shared_ptr<complex::Complex>>&, size_t) noexcept(false);
		BufferedCollection(const BufferedCollection&) noexcept(false);
		BufferedCollection& operator=(const BufferedCollection&) noexcept(false);
		BufferedCollection(BufferedCollection&&) noexcept;
		BufferedCollection& operator=(BufferedCollection&&) noexcept;
		virtual ~BufferedCollection() noexcept;

		bool inline empty() const noexcept { return n == 0 && head == nullptr; }
		const std::weak_ptr<complex::Complex> operator[](int) noexcept(false);
		void insert(const std::weak_ptr<complex::Complex>&) noexcept(false);
		std::weak_ptr<complex::Complex> removeAt(int) noexcept(false);
		size_t inline listSize() const noexcept { return n; }
		size_t inline arrayCapacity() const noexcept { return k; }
		size_t inline totalElementsNum() const noexcept { return ((n == 0) ? 0 : ((n - 1) * k + tail->data.size)); }
		friend std::ostream& operator<<(std::ostream&, const BufferedCollection&) noexcept;

	private:
		void copy(const BufferedCollection&) noexcept(false);
		void move(BufferedCollection&&) noexcept;
		void destroy() noexcept;
		bool search(const std::weak_ptr<complex::Complex>&) const noexcept;
		Node* getAt(size_t) const;
		void checkIndex(int) const noexcept(false);
		std::pair<size_t, size_t> composeIndices(int index) const noexcept(false);
		void moveElements(Node*) noexcept;
	};
};