#ifndef BST_HPP
#define BST_HPP

#include "vector.hpp"

#include <cstdint>
#include <cstddef>
#include <utility>

struct BSTPrivate;

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
	size_t assignment(uint32_t key, uint32_t value);

	/**
	 * @returns Sum of values in the interval [left, right].
	 */
	uint64_t sum(size_t time, uint32_t left, uint32_t right) const;

	/**
	 * Clears the tree at @a time from the history.
	 * @returns the number of currently allocated nodes.
	 */
	size_t clear(size_t time);
	
	/**
	 * @returns the current time.
	 */
	size_t now() const;

private:
	struct Node
	{
		Node(uint32_t k, uint32_t v) :
			key(k), value(v), sum(v)
		{
		}
		
		uint32_t key, value;
		uint64_t sum;
		
		mutable uint32_t refs = 0;
		const Node *child[2] = {nullptr, nullptr};
	};

	void addRef(const Node *node);
	void remRef(const Node *node);
	const Node* create(uint32_t key, uint32_t value, const Node *left, const Node *right);
	const Node* find(const Node *node, uint32_t key) const;
	const Node* insert(const Node *node, uint32_t key, uint32_t value);
	std::pair<const Node *, const Node *> eraseMin(const Node *node);
	const Node* erase(const Node *node, uint32_t key);
	uint64_t sum(const Node *node) const;
	uint64_t sum(const Node *node, uint32_t left, uint32_t right, int side) const;
	uint64_t sum(const Node *node, uint32_t left, uint32_t right) const;

	Vector<const Node*> roots;
	size_t nodes = 0;
};

#endif // BST_HPP
