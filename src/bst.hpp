#ifndef BST_HPP
#define BST_HPP

#include "vector.hpp"

#include <cstdint>
#include <cstddef>
#include <utility>

/**
 * Persistent binary search tree.
 */
class BST
{
public:
	BST();
	~BST();

	/**
	 * Sets the value of a node.
	 * @returns the number of currently allocated nodes.
	 */
	int assignment(int key, int value);

	/**
	 * @returns Sum of values in the interval [left, right].
	 */
	int sum(int time, int left, int right) const;

	/**
	 * Clears the tree at @a time from the history.
	 * @returns the number of currently allocated nodes.
	 */
	int clear(int time);

	/**
	 * @returns the current time.
	 */
	int now() const;

private:
	struct Node
	{
		Node(int k, int v) :
			key(k), value(v), sum(v)
		{
		}

		int key, value;
		int sum;

		mutable uint32_t refs = 0;
		const Node *child[2] = {nullptr, nullptr};
	};

	void addRef(const Node *node);
	void remRef(const Node *node);
	const Node* create(int key, int value, const Node *left, const Node *right);
	const Node* find(const Node *node, int key) const;
	const Node* insert(const Node *node, int key, int value);
	std::pair<const Node *, const Node *> eraseMin(const Node *node);
	const Node* erase(const Node *node, int key);
	int sum(const Node *node) const;
	int sum(const Node *node, int left, int right, int side) const;
	int sum(const Node *node, int left, int right) const;

	Vector<const Node*> roots;
	int nodes = 0;
};

#endif // BST_HPP
