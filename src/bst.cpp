#include "bst.hpp"
#include "vector.hpp"

BST::BST()
{
	roots.push_back(nullptr);
}

BST::~BST()
{
	for (size_t i = 0; i < roots.size(); ++i) {
		remRef(roots[i]);
	}
}

size_t BST::assignment(uint32_t key, uint32_t value)
{
	const Node *oldNode = find(roots.back(), key);
	uint64_t oldValue = oldNode ? oldNode->value : 0;

	const Node *root;
	if (oldValue == value) {
		root = roots.back();
	} else if (value == 0) {
		root = erase(roots.back(), key);
	} else {
		root = insert(roots.back(), key, value);
	}

	addRef(root);
	roots.push_back(root);
	return nodes;
}

uint64_t BST::sum(size_t time, uint32_t left, uint32_t right) const
{
	return sum(roots[time], left, right);
}

size_t BST::clear(size_t time)
{
	remRef(roots[time]);
	roots[time] = nullptr;
	return nodes;
}

size_t BST::now() const
{
	return roots.size() - 1;
}

void BST::addRef(const BST::Node* node)
{
	if (node) {
		++node->refs;
	}
}

void BST::remRef(const BST::Node* node)
{
	if (node && --node->refs == 0) {
		--nodes;
		remRef(node->child[0]);
		remRef(node->child[1]);
		delete node;
	}
}

const BST::Node* BST::create(uint32_t key, uint32_t value, const BST::Node* left, const BST::Node* right)
{
	++nodes;
	addRef(left);
	addRef(right);
	Node *node = new Node(key, value);
	node->child[0] = left;
	node->child[1] = right;
	node->sum += sum(left) + sum(right);
	return node;
}

const BST::Node* BST::find(const BST::Node* node, uint32_t key) const
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

const BST::Node* BST::insert(const BST::Node* node, uint32_t key, uint32_t value)
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
std::pair<const BST::Node *, const BST::Node *> BST::eraseMin(const BST::Node* node)
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

const BST::Node* BST::erase(const BST::Node* node, uint32_t key)
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

uint64_t BST::sum(const BST::Node* node) const
{
	return node ? node->sum : 0;
}

uint64_t BST::sum(const Node *node, uint32_t left, uint32_t right, int side) const
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

uint64_t BST::sum(const Node *node, uint32_t left, uint32_t right) const
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
