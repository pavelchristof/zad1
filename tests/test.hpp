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
		std::cout << "Running test \"" << name << "\"..." << std::endl;
	}

	~Test()
	{
		if (testsFailed == 0) {
			std::cout << Ok() << " All test cases passed." << std::endl;
		} else {
			std::cout << Error() << ' ' << testsFailed << '/' << testsTotal << " test cases failed." << std::endl;
		}
	}

	void addCase(bool ok, const char *name)
	{
		++testsTotal;
		if (ok) {
			std::cout << Ok() << " Test case \"" << name << "\" passed." << std::endl;
		} else {
			++testsFailed;
			std::cout << Error() << " Test case \"" << name << "\" failed." << std::endl;
		}
	}

private:
	int testsFailed = 0;
	int testsTotal = 0;
};

#define TEST_ASSERT(ok, msg) { \
	if (!(ok)) { \
		std::cout << Error() << ' ' << __FILE__ << ':' << __LINE__ << ": " << (msg) << std::endl; \
		return false; \
	} \
}
