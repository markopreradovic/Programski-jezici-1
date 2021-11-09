#pragma once
#include "VectorN.h"
#include <iostream>
#include <functional>

class MyMatrix;

namespace Vector {

	class VectorSetN {
		VectorN* elements{ nullptr };
		size_t elemDimension{ 0 };
		size_t elemNum{ 0 };
		size_t capacity{ 0 };

	public:
		VectorSetN(size_t c = 1, size_t d = 1);
		explicit VectorSetN(const std::initializer_list<VectorN>&);
		VectorSetN(const VectorN*, size_t);
		VectorSetN(const VectorN&);
		VectorSetN(const VectorSetN&);
		VectorSetN& operator=(const VectorSetN&);
		VectorSetN(VectorSetN&&) noexcept;
		VectorSetN& operator=(VectorSetN&&) noexcept;
		~VectorSetN();

		VectorSetN& operator+=(const VectorSetN&);
		VectorSetN& operator-=(const VectorSetN&);

		VectorSetN filter(const std::function<bool(const VectorN&)>&) const;
		std::vector<double> transform(const std::function<double(const VectorN&)>&) const;

		friend bool operator==(const VectorSetN&, const VectorSetN&);
		friend bool operator!=(const VectorSetN&, const VectorSetN&);
		bool subset(const VectorSetN&) const; 

		bool linearyIndependentElements()const;

		size_t size() const { return elemNum; }

		inline bool isEmpty() const { return elemNum == 0; }

		void addElement(const VectorN&);
		void deleteElement(const VectorN&);

	private:
		void copyVectorSetN(const VectorSetN&);
		void moveVectorSetN(VectorSetN&&);
		void set_and_allocate(size_t, size_t);
		VectorSetN& calculate_and_change(const VectorSetN&, void(VectorSetN::*)(const VectorN&));

		friend std::ostream& operator<<(std::ostream&, const VectorSetN&);

		VectorN& operator[](size_t index) { return const_cast<VectorN&>(getElementAt(index)); }
		const VectorN& operator[](size_t index) const { return getElementAt(index); }
		const VectorN& getElementAt(size_t index) const { return elements[index]; }
		int search(const VectorN&) const;
		void resize();
		void deleteElementAt(size_t);

		friend MyMatrix;
	};
}
