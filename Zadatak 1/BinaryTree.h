#pragma once

#include "Stack.h"
#include "Array.h"
#include <iostream>

namespace zad1 {
	class BinaryTree {
		class BTNode {
			Array data;
			BTNode* left, *right;
		public:
			BTNode(const Array&);
			~BTNode();
			Array getData() { return this->data; }
			BTNode*& getLeft() { return this->left; } 
			BTNode*& getRight() { return this->right; }
			void printNode()const;
			BTNode* copy();
		};

		BTNode* root;
		int(Array::* const compareFunctionPointer)(const Array&)const;
		BTNode* addNode(BTNode*, const Array&);
		const static void inorderIterative(BTNode*);

	public:
		BinaryTree() : root(nullptr), compareFunctionPointer(&Array::compareArr) {}
		BinaryTree(const BinaryTree&);
		BinaryTree(BinaryTree&&) noexcept;
		~BinaryTree();
		BinaryTree& operator=(const BinaryTree&) = delete;
		BinaryTree& operator=(BinaryTree&&) = delete;
		void insertNewNode(const Array&);
		void inorderIterative()const;
	};

}

