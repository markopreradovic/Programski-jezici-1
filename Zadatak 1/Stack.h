#pragma once
namespace zad1 {
	class Stack {
		struct Node {
			void* data;
			Node* next;
		};

		Node* tos;
		int elemNum;
		Node* addNewNode(void* info) {
			Node* newNode = new Node;
			newNode->data = info;
			return newNode;
		}

	public:
		Stack();
		~Stack();
		Stack(const Stack&);
		Stack(Stack&&) noexcept;

		Stack& operator=(const Stack&) = delete;
		Stack& operator=(Stack&&) = delete;

		int stackElemNum()const;
		bool isStackEmpty() const;
		void push(void*);
		void* pop();
		void* top() const;
	};
}
