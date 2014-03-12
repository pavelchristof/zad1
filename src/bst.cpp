#include "bst.hpp"
#include "vector.hpp"

#include <algorithm>

struct BSTPrivate
{
	struct Node
	{
		Node(uint32_t k, uint32_t v) :
			key(k), value(v), sum(v)
		{
		}

		uint32_t key, value;
		uint64_t sum;

		int height = 0;
		mutable uint32_t refs = 0;
		const Node *child[2] = {nullptr, nullptr};
	};

	Vector<const Node*> roots;
	size_t nodes = 0;

	int height(const Node *node)
	{
		return node ? node->height : 0;
	}

	uint64_t sum(const Node *node)
	{
		return node ? node->sum : 0;
	}

	void addRef(const Node *node)
	{
		if (node) {
			++node->refs;
		}
	}

	void remRef(const Node *node)
	{
		if (node && --node->refs == 0) {
			--nodes;
			remRef(node->child[0]);
			remRef(node->child[1]);
			delete node;
		}
	}

	const Node* create(uint32_t key, uint32_t value, const Node *left, const Node *right)
	{
		++nodes;
		addRef(left);
		addRef(right);
		Node *node = new Node(key, value);
		node->child[0] = left;
		node->child[1] = right;
		node->height = 1 + std::max(height(left), height(right));
		node->sum += sum(left) + sum(right);
		return node;
	}

	const Node* find(const Node *node, uint32_t key)
	{
		if (!node) {
			return nullptr;
		}

		if (key == node->key) {
			return node;
		} else if (key < node->key) {
			return find(node->child[0], key);
		} else {
			return find(node->child[1], key);
		}
	}

	const Node* insert(const Node *node, uint32_t key, uint32_t value)
	{
		if (!node) {
			return create(key, value, nullptr, nullptr);
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
	std::pair<const Node *, const Node *> eraseMin(const Node *node)
	{
		if (!node) {
			return {nullptr, nullptr};
		}

		if (node->child[0]) {
			auto r = eraseMin(node->child[0]);
			return {r.first, create(node->key, node->value, r.second, node->child[1])};
		} else {
			return {node, node->child[1]};
		}
	}

	const Node* erase(const Node *node, uint32_t key)
	{
		if (!node) {
			return nullptr;
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

	uint64_t sum(const Node *node, uint32_t left, uint32_t right, int side)
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

	uint64_t sum(const Node *node, uint32_t left, uint32_t right)
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
	d->roots.push_back(nullptr);
}

BST::~BST()
{
	for (size_t i = 0; i < d->roots.size(); ++i) {
		d->remRef(d->roots[i]);
	}
	delete d;
}

size_t BST::assignment(uint32_t key, uint32_t value)
{
	const BSTPrivate::Node *root;
	if (value == 0) {
		root = d->erase(d->roots.back(), key);
	} else {
		const BSTPrivate::Node *node = d->find(d->roots.back(), key);
		if (node && node->value == value) {
			root = d->roots.back();
		} else {
			root = d->insert(d->roots.back(), key, value);
		}
	}
	d->addRef(root);
	d->roots.push_back(root);
	return d->nodes;
}

uint64_t BST::sum(size_t time, uint32_t left, uint32_t right) const
{
	return d->sum(d->roots[time], left, right);
}

size_t BST::clear(size_t time)
{
	d->remRef(d->roots[time]);
	d->roots[time] = nullptr;
	return d->nodes;
}

size_t BST::now() const
{
	return d->roots.size() - 1;
}
