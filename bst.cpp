#include "bst.hpp"
#include "sharedptr.hpp"

#include <vector>
#include <algorithm>

struct BSTPrivate 
{
	struct Node 
	{
		typedef SharedPtr<const Node> Ptr;

		Node(uint32_t k, uint32_t v, uint32_t &c) :
			key(k), value(v), sum(v), counter(c) 
		{
			++counter;
		}

		~Node() 
		{
			--counter;
		}

		uint32_t key, value;
		uint64_t sum;

		int height = 0;
		Ptr child[2] = {nullptr, nullptr};

		// A node cannot outlive a BST so this reference is always valid.
		uint32_t &counter;
	};

	//TODO: skopiować własny vector
	std::vector<Node::Ptr> roots = {Node::Ptr()};

	// The number of nodes of this BST.
	uint32_t counter = 0;

	int height(Node::Ptr node) 
	{
		return node ? node->height : 0;
	}

	uint64_t sum(Node::Ptr node) 
	{
		return node ? node->sum : 0;
	}

	Node::Ptr create(uint32_t key, uint32_t value, Node::Ptr left, Node::Ptr right) 
	{
		Node *node = new Node(key, value, counter);
		node->child[0] = left;
		node->child[1] = right;
		node->height = 1 + std::max(height(left), height(right));
		node->sum += sum(left) + sum(right);
		return Node::Ptr(node);
	}

	Node::Ptr find(Node::Ptr node, uint32_t key) 
	{
		if (!node) {
			return Node::Ptr();
		}

		if (key == node->key) {
			return node;
		} else if (key < node->key) {
			return find(node->child[0], key);
		} else {
			return find(node->child[1], key);
		}
	}

	Node::Ptr insert(Node::Ptr node, uint32_t key, uint32_t value) 
	{
		if (!node) {
			return Node::Ptr(new Node(key, value, counter));
		}

		if (key == node->key) {
			return create(key, value, node->child[0], node->child[1]);
		} else if (key < node->key) {
			return create(node->key, node->value, insert(node->child[0], key, value), node->child[1]);
		} else {
			return create(node->key, node->value, node->child[0], insert(node->child[1], key, value));
		}
	}

	/**
	 * @returns the minimal node and the new tree.
	 */
	std::pair<Node::Ptr, Node::Ptr> eraseMin(Node::Ptr node) 
	{
		if (!node) {
			return {Node::Ptr(), Node::Ptr()};
		}

		if (node->child[0]) {
			auto r = eraseMin(node->child[0]);
			return {r.first, create(node->key, node->value, r.second, node->child[1])};
		} else {
			return {node, node->child[1]};
		}
	}

	Node::Ptr erase(Node::Ptr node, uint32_t key) 
	{
		if (!node) {
			return Node::Ptr();
		}

		if (key == node->key) {
			auto r = eraseMin(node->child[1]);
			if (r.first) {
				return create(r.first->key, r.first->value, node->child[0], r.second);
			} else {
				return node->child[0];
			}
		} else if (key < node->key) {
			return create(node->key, node->value, erase(node->child[0], key), node->child[1]);
		} else {
			return create(node->key, node->value, node->child[0], erase(node->child[1], key));
		}
	}

	uint64_t sum(Node::Ptr node, uint32_t left, uint32_t right, int side) 
	{
		if (!node) {
			return 0;
		}

		if (node->key < left) {
			return side == 0 ? sum(node->child[1], left, right, 0) : 0;
		} else if (node->key > right) {
			return side == 1 ? sum(node->child[0], left, right, 1) : 0;
		} else {
			if (side == 0) {
				return sum(node->child[0], left, right, 0) + node->value + sum(node->child[1]);
			} else {
				return sum(node->child[0]) + node->value + sum(node->child[1], left, right, 1);
			}
		}
	}

	uint64_t sum(Node::Ptr node, uint32_t left, uint32_t right) 
	{
		if (!node) {
			return 0;
		}

		if (node->key < left) {
			return sum(node->child[1], left, right);
		} else if (node->key > right) {
			return sum(node->child[0], left, right);
		} else {
			return sum(node->child[0], left, right, 0) + node->value + sum(node->child[1], left, right, 1);
		}
	}
};

BST::BST() : d(new BSTPrivate)
{
}

BST::~BST()
{
	delete d;
}

uint32_t BST::set(uint32_t key, uint32_t value)
{
	BSTPrivate::Node::Ptr root;
	if (value == 0) {
		root = d->erase(d->roots.back(), key);
	} else {
		BSTPrivate::Node::Ptr node = d->find(d->roots.back(), key);
		if (node && node->value == value) {
			root = d->roots.back();
		} else {
			root = d->insert(d->roots.back(), key, value);
		}
	}
	d->roots.push_back(root);
	return d->counter;
}

uint64_t BST::sum(uint32_t time, uint32_t left, uint32_t right) const
{
	return d->sum(d->roots[time], left, right);
}

uint32_t BST::clear(uint32_t time)
{
	d->roots[time].reset();
	return d->counter;
}

uint32_t BST::now() const
{
	return d->roots.size() - 1;
}
