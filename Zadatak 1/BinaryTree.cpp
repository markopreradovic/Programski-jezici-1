#include "pch.h"
#include "Array.h"
#include "BinaryTree.h"
#include <iostream>

using std::cout;
using std::endl;

namespace zad1 {
	BinaryTree::BTNode::BTNode(const Array& info) : data(info), left(nullptr), right(nullptr) {  }

	BinaryTree::BTNode::~BTNode() {
		if (left)
			delete left;
		if (right)
			delete right;
	}

	void BinaryTree::BTNode::printNode() const {
		data.printArrHex();
	}

	BinaryTree::BTNode* BinaryTree::BTNode::copy() {
		BTNode* newNode = new BTNode(this->data);

		if (left)
			newNode->left = left->copy();
		if (right)
			newNode->right = right->copy();

		return newNode;
	}


	BinaryTree::BinaryTree(const BinaryTree& other) : BinaryTree() {
		if (other.root != nullptr) 
			this->root = other.root->copy();
	}

	BinaryTree::BinaryTree(BinaryTree&& other) noexcept : BinaryTree() {
		this->root = std::move(other.root);
		other.root = nullptr;
	}

	BinaryTree::~BinaryTree() {
		delete root;
	}


	BinaryTree::BTNode* BinaryTree::addNode(BinaryTree::BTNode* node, const Array& info) {
		if (node == nullptr)
			return new BinaryTree::BTNode(info);
		int comparationValue = (info.*compareFunctionPointer)(node->getData());
		if (comparationValue < 0)
			node->getLeft() = addNode(node->getLeft(), info);
		else if (comparationValue > 0)
			node->getRight() = addNode(node->getRight(), info);
		return node;
	}

	void BinaryTree::insertNewNode(const Array& info) {
		this->root = addNode(root, info);
	}

	const void BinaryTree::inorderIterative(BinaryTree::BTNode* node) {
		if (node == nullptr) {
			std::cout << "Tree is empty." << std::endl;
			return;
		}
		Stack stack;
		BinaryTree::BTNode* curr = node;
		bool endSignal = false;

		while (!endSignal) {
			while (curr != nullptr) {
				stack.push(curr);
				curr = curr->getLeft();
			}

			if (!stack.isStackEmpty()) {
				curr = (BinaryTree::BTNode*)stack.pop();
				curr->printNode();
				curr = curr->getRight();
			}
			else
				endSignal = true;
		}
	}

	void BinaryTree::inorderIterative() const {
		inorderIterative(this->root);
	}
}