#include "pch.h"
#include "Stack.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

namespace zad1 {
	Stack::Stack() : tos(nullptr), elemNum(0) {
	}

	Stack::~Stack() {
		while (!isStackEmpty())
			pop();
	}

	Stack::Stack(const Stack& other) : Stack() {
		std::vector<void*> tempVector;
		Node* tempNode = other.tos;
		while (tempNode) {
			tempVector.insert(tempVector.begin(), tempNode->data);
			tempNode = tempNode->next;
		}
		for (auto x : tempVector)
			this->push(x);
	}

	Stack::Stack(Stack&& other) noexcept {
		tos = std::move(other.tos);
		elemNum = std::move(other.elemNum);
		other.tos = nullptr;
		other.elemNum = 0;
	}

	void Stack::push(void* info) {
		if (info == nullptr) {
			std::cout << "nullptr cannot be added." << std::endl;
			return;
		}

		Node* newNode = addNewNode(info);
		newNode->next = tos;
		tos = newNode;
		++elemNum;
	}

	void* Stack::pop() {
		if (isStackEmpty()) {
			std::cout << "Stack is empty." << endl;
			return nullptr;
		}
		void* ret_val = tos->data;
		Node* delete_node = tos;
		tos = tos->next;
		delete delete_node;
		--elemNum;
		return ret_val;
	}

	void* Stack::top() const {
		return (isStackEmpty()) ? nullptr : tos->data;
	}

	bool Stack::isStackEmpty() const {
		return (tos == nullptr) ? true : false;
	}

	int Stack::stackElemNum() const {
		return this->elemNum;
	}
}