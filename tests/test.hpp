#include <cstdio>
#include <iostream>

class Ok {};
class Error {};

std::ostream& operator << (std::ostream &s, Ok)
{
	return s << "\033[32m[OK]\033[0m";
}

std::ostream& operator << (std::ostream &s, Error)
{
	return s << "\033[31m[ERROR]\033[0m";
}

class Test
{
public:
	Test(const char *name)
	{
		std::cout << "Running test \"" << name << "\"...\n";
	}

	~Test()
	{
		if (testsFailed == 0) {
			std::cout << Ok() << " All test cases correct.\n";
		} else {
			std::cout << Error() << ' ' << testsFailed << '/' << testsTotal << " test cases failed.\n";
		}
	}

	void addCase(bool ok, const char *name)
	{
		++testsTotal;
		if (ok) {
			std::cout << Ok() << " Test case \"" << name << "\" correct.\n";
		} else {
			++testsFailed;
			std::cout << Error() << " Test case \"" << name << "\" failed.\n";
		}
	}

private:
	int testsFailed = 0;
	int testsTotal = 0;
};

#define TEST_ASSERT(ok, msg) { \
	if (!(ok)) { \
		std::cout << Error() << ' ' << __FILE__ << ':' << __LINE__ << ": " << msg << '\n'; \
		return false; \
	} \
}