#include <parser.hpp>
#include "test.hpp"
#include <string>
#include <sstream>

Test test("parser");

class ParserIdentity : public ParserEventHandler
{
public:
	void assignment(uint64_t key, uint64_t value) override
	{
		result << "f(" << key << "):=" << value << "\n";
	}

	void sum(uint64_t time, uint64_t left, uint64_t right) override
	{
		result << "suma(" << time << "," << left << ".." << right << ")\n";
	}

	void clear(uint64_t time) override
	{
		result << "czysc(" << time << ")\n";
	}

	void error() override
	{
		result << "error\n";
	}

	std::stringstream result;
};

std::string run(std::string in)
{
	ParserIdentity id;
	Parser parser(&id);
	std::stringstream ss(in);
	parser.parse(ss);
	return id.result.str();
}

bool isEqual(std::string s)
{
	return run(s) == s;
}

bool isError(std::string s)
{
	return run(s) == "error\n";
}

bool singleStatement()
{
	// correct
	TEST_ASSERT(isEqual("f(0):=0\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("f(1):=1\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("f(1234567890):=1234567890\n"), "Invalid parse.");

	// invalid
	TEST_ASSERT(isError("f(1234567890):=12345678900\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(12345678900):=1234567890\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(00):=1\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(1):=00\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(-1):=1\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(-0):=0\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(1):=-1\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(1):=1 \n"), "Invalid parse.");
	TEST_ASSERT(isError(" f(1):=1\n"), "Invalid parse.");
	TEST_ASSERT(isError("f(1) := 1\n"), "Invalid parse.");

	// correct
	TEST_ASSERT(isEqual("suma(0,0..0)\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("suma(0,1234567890..1234567890)\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("suma(1234567890,1234567890..1234567890)\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("suma(0,1..0)\n"), "Invalid parse.");

	// invalid
	TEST_ASSERT(isError("suma(12345678900,1234567890..1234567890)\n"), "Invalid parse.");
	TEST_ASSERT(isError("suma(1234567890,12345678900..1234567890)\n"), "Invalid parse.");
	TEST_ASSERT(isError("suma(1234567890,1234567890..12345678900)\n"), "Invalid parse.");
	TEST_ASSERT(isError("suma(1, 1..1)\n"), "Invalid parse.");
	TEST_ASSERT(isError("suma(1,1,1)\n"), "Invalid parse.");
	TEST_ASSERT(isError("suma(-1,1..1)\n"), "Invalid parse.");
	TEST_ASSERT(isError("suma(0,01..0)\n"), "Invalid parse.");

	// correct
	TEST_ASSERT(isEqual("czysc(1)\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("czysc(1234567890)\n"), "Invalid parse.");
	TEST_ASSERT(isEqual("czysc(0)\n"), "Invalid parse.");

	// invalid
	TEST_ASSERT(isError(" czysc(1)\n"), "Invalid parse.");
	TEST_ASSERT(isError("czysc(1) \n"), "Invalid parse.");
	TEST_ASSERT(isError("czysc( 1 )\n"), "Invalid parse.");
	TEST_ASSERT(isError("czysc(12345678900)\n"), "Invalid parse.");
	TEST_ASSERT(isError("czysc(0123)\n"), "Invalid parse.");
	TEST_ASSERT(isError("fghfsd\n"), "Invalid parse.");
	TEST_ASSERT(isError("czysc(f(1))\n"), "Invalid parse.");
	TEST_ASSERT(isError("\n"), "Invalid parse.");
	TEST_ASSERT(isError(" \n"), "Invalid parse.");

	return true;
}

bool multiStatement()
{
	std::string input =
	    "f(1) :=1\n"
	    "f(1):=1\n"
	    "suma(01,0..1)\n"
	    "suma(12,12345..67890)\n"
	    "czysc(0)\n"
	    "czysc(-123)\n"
	    " czysc(1)\n"
	    "czysc(12345678900)\n"
	    "czysc(1) \n";

	std::string output =
	    "error\n"
	    "f(1):=1\n"
	    "error\n"
	    "suma(12,12345..67890)\n"
	    "czysc(0)\n"
	    "error\n"
	    "error\n"
	    "error\n"
	    "error\n";

	TEST_ASSERT(run(input) == output, "Invalid parse.");

	return true;
}

int main()
{
	test.addCase(singleStatement(), "single statement");
	test.addCase(multiStatement(), "multi statement");
	return 0;
}
