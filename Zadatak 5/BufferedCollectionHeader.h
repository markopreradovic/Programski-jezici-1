#pragma once
#include "IPrintable.h"
#include <iostream>
#include <type_traits>
#include <vector>

namespace GenericCollection {
	template <typename T>
	concept Printable = requires (std::ostream & stream, const T & item) {
		{stream << item} -> std::convertible_to<decltype(stream)>;
	};

	template <typename T>
	concept Addable = requires (const T & lhs, const T & rhs) {
		{lhs + rhs} -> std::convertible_to<T>;
	};

	template <typename T>
	concept Comparable = requires {
		std::three_way_comparable<T>;
	};

	template <typename T>
	concept AddPrintCmp = Addable<T> && Printable<T> && Comparable<T>;

	template <typename T>
	requires AddPrintCmp<T> class BufferedCollection : virtual public Interface::IPrintable {
		static_assert(std::is_copy_constructible<T>::value, "Type must be copy-constructible.");
		static_assert(std::is_move_constructible<T>::value, "Type must be move-constructible.");
		static_assert(std::is_copy_assignable<T>::value, "Type must be copy-assignable.");
		static_assert(std::is_move_assignable<T>::value, "Type must be move-assignable.");
		static_assert(std::is_default_constructible<T>::value, "Type must have default constructor.");

		struct VectorT {
			T* elements{ nullptr };
			size_t size{ 0 };
			size_t capacity{ 0 };

			VectorT(size_t) noexcept(false);
			VectorT(const VectorT&) noexcept(false);
			VectorT& operator=(const VectorT&) noexcept(false);
			VectorT(VectorT&&) noexcept;
			VectorT& operator=(VectorT&&) noexcept;
			virtual ~VectorT() noexcept;

			const T& operator[](size_t) const noexcept(false);
			T& operator[](size_t) noexcept(false);
			void inline insertItem(const T&) noexcept; 
			T removeItemAt(size_t) noexcept;            
			bool inline emptyVector() const noexcept { return size == 0; }
			bool inline fullVector() const noexcept { return size == capacity; }
			T elemSum() const noexcept;
			int findData(const T&) const noexcept;

		private:
			void destroyVector() noexcept;
			void copyVector(const VectorT&) noexcept(false);
			void moveVector(VectorT&&) noexcept;
			void checkIndexRange(int) const noexcept(false);
			const T& at(size_t) const noexcept(false);
		};

		struct Node {
			VectorT data;
			Node* next;
			Node* previous;

			Node(size_t cap) : data(cap), next(nullptr), previous(nullptr) { }
			Node(const VectorT& other) : data(other), next(nullptr), previous(nullptr) { }
		};

		Node* head{ nullptr };
		Node* tail{ nullptr };
		size_t n{ 0 };
		size_t k{ 0 };

	public:
		explicit BufferedCollection(size_t) noexcept(false);
		BufferedCollection(const std::vector<T>&, size_t) noexcept(false);
		BufferedCollection(const BufferedCollection&) noexcept(false);
		BufferedCollection& operator=(const BufferedCollection&) noexcept(false);
		BufferedCollection(BufferedCollection&&) noexcept;
		BufferedCollection& operator=(BufferedCollection&&) noexcept;
		virtual ~BufferedCollection() noexcept;

		void push_back(const T&) noexcept(false);
		T pop_back() noexcept(false);
		T removeAt(int) noexcept(false);
		T remove(const T&) noexcept(false);
		const T& operator[](int) const noexcept(false);
		T& operator[](int) noexcept(false);
		void clear() noexcept;
		bool inline empty() const noexcept { return n == 0; }
		size_t inline size() const noexcept { return n; }
		T sum(T) const;
		int find(const T&) const noexcept;

	protected:
		virtual void printItem(std::ostream&) const noexcept override;

	private:
		void copy(const BufferedCollection&) noexcept(false);
		void move(BufferedCollection&&) noexcept;
		void destroy() noexcept;
		size_t inline total() const noexcept { return ((n == 0) ? 0 : ((n - 1) * k + tail->data.size)); }
		void checkValidIndex(int) const noexcept(false);
		std::pair<size_t, size_t> composeIndices(int) const noexcept(false);
		void moveElements(Node*) noexcept;
		Node* getNodeAt(size_t) const noexcept;
		const T& getElementAt(int) const noexcept(false);
	};
};