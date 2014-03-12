#include <bst.hpp>
#include "test.hpp"

Test test("BST");

bool sum()
{
	BST bst;

	TEST_ASSERT(bst.sum(0, 0, 1000) == 0, "Sum on an empty BST should return 0.");
	TEST_ASSERT(bst.assignment(1, 100) == 1, "Should be 1 allocation.");
	TEST_ASSERT(bst.sum(1, 1, 1) == 100, "Invalid sum.");
	TEST_ASSERT(bst.sum(1, 0, 1000) == 100, "Invalid sum.");
	TEST_ASSERT(bst.assignment(2, 50) == 3, "Should be 3 allocations.");
	TEST_ASSERT(bst.sum(2, 0, 2) == 150, "Invalid sum.");
	TEST_ASSERT(bst.sum(2, 2, 3) == 50, "Invalid sum.");
	TEST_ASSERT(bst.sum(2, 1, 2) == 150, "Invalid sum.");
	TEST_ASSERT(bst.assignment(25, 25) == 6, "Should be 6 allocations.");
	TEST_ASSERT(bst.sum(3, 0, 1000) == 175, "Invalid sum.");
	TEST_ASSERT(bst.sum(3, 3, 24) == 0, "Invalid sum.");
	TEST_ASSERT(bst.sum(3, 26, 100) == 0, "Invalid sum.");
	TEST_ASSERT(bst.assignment(0, 12) == 8, "Should be 8 allocations.");
	TEST_ASSERT(bst.sum(4, 0, 100) == 187, "Invalid sum.");
	TEST_ASSERT(bst.sum(4, 0, 24) == 162, "Invalid sum.");
	TEST_ASSERT(bst.sum(4, 0, 25) == 187, "Invalid sum.");

	return true;
}

bool clear()
{
	BST bst;

	TEST_ASSERT(bst.assignment(0, 1) == 1, "Should be 1 allocation.");
	TEST_ASSERT(bst.assignment(1, 1) == 3, "Should be 3 allocations.");
	TEST_ASSERT(bst.assignment(2, 1) == 6, "Should be 6 allocations.");
	TEST_ASSERT(bst.assignment(3, 1) == 10, "Should be 10 allocations.");
	TEST_ASSERT(bst.assignment(4, 1) == 15, "Should be 15 allocations.");
	TEST_ASSERT(bst.assignment(5, 1) == 21, "Should be 21 allocations.");
	TEST_ASSERT(bst.clear(1) == 20, "Should be 20 allocations.");
	TEST_ASSERT(bst.sum(1, 0, 100) == 0, "Invalid sum.");
	TEST_ASSERT(bst.clear(2) == 18, "Should be 18 allocations.");
	TEST_ASSERT(bst.sum(2, 0, 100) == 0, "Invalid sum.");
	TEST_ASSERT(bst.clear(3) == 15, "Should be 15 allocations.");
	TEST_ASSERT(bst.sum(3, 0, 100) == 0, "Invalid sum.");
	TEST_ASSERT(bst.clear(4) == 11, "Should be 11 allocations.");
	TEST_ASSERT(bst.sum(4, 0, 100) == 0, "Invalid sum.");
	TEST_ASSERT(bst.clear(5) == 6, "Should be 6 allocations.");
	TEST_ASSERT(bst.sum(5, 0, 100) == 0, "Invalid sum.");
	TEST_ASSERT(bst.clear(6) == 0, "Should be 0 allocations.");
	TEST_ASSERT(bst.sum(6, 0, 100) == 0, "Invalid sum.");

	return true;
}

bool now()
{
	BST bst;

	TEST_ASSERT(bst.now() == 0, "Invalid time.");
	bst.assignment(0, 1);
	TEST_ASSERT(bst.now() == 1, "Invalid time.");
	bst.assignment(0, 1);
	TEST_ASSERT(bst.now() == 2, "Invalid time.");
	bst.assignment(2, 3);
	TEST_ASSERT(bst.now() == 3, "Invalid time.");
	bst.assignment(1, 6);
	TEST_ASSERT(bst.now() == 4, "Invalid time.");

	return true;
}

int main()
{
	test.addCase(sum(), "sum");
	test.addCase(clear(), "clear");
	test.addCase(now(), "now");
	return 0;
}
