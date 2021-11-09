#pragma once
#include "IPrintable.h"
#include "IComparable.h"
#include <iostream>
#include <cstddef>
#include <vector>

namespace Set {
	class UnorderedSet : public Interface::IPrintable {

	protected:
		IPrintable** elements{ nullptr };
		std::size_t number_of_elements{ 0 };
		std::size_t capacity{ 1 };

	public:
		UnorderedSet();
		explicit UnorderedSet(std::size_t);
		UnorderedSet(const std::initializer_list<IPrintable*>&);
		UnorderedSet(const std::vector<IPrintable*>&);

		UnorderedSet(const UnorderedSet&);
		UnorderedSet& operator=(const UnorderedSet&);
		UnorderedSet(UnorderedSet&&) noexcept;
		UnorderedSet& operator=(UnorderedSet&&) noexcept;
		virtual ~UnorderedSet();

		virtual bool insert(const IPrintable*);
		virtual IPrintable* erase(int);
		virtual IPrintable* erase();

		virtual const IPrintable* operator[](int) const;
		std::size_t size() const { return number_of_elements; }
		std::size_t max_size() const { return capacity; }
		bool empty() const { return (number_of_elements == 0); }

	protected:
		void copy(const UnorderedSet&);
		void move(UnorderedSet&&);
		void destroy();
		bool validIndexRange(int index) const { return !((index < 0) || ((unsigned)index >= number_of_elements)); }
		virtual void printItem(std::ostream&) const override;
		virtual bool equalItems(const IPrintable&) const override;

		virtual void resize();

	private:
		void allocate(std::size_t);
		int search(const IPrintable*) const;
	};
};
