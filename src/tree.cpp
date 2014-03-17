#include "tree.hpp" 
#include "bst.hpp"

BST bst;

void init()
{
}

int assignment(int arg, int value)
{
	return bst.assignment(arg, value);
}

int sum(int t, int left, int right)
{
	return bst.sum(t, left, right);
}

int clear(int t)
{
	return bst.clear(t);
}
