#include <driver.hpp>
#include "test.hpp"
#include <string>
#include <sstream>

Test test("driver");

std::string run(std::string s)
{
	std::stringstream in(s);
	std::stringstream out;

	Driver driver(in, out);
	driver.run();

	return out.str();
}

bool example()
{
	std::string in =
		"f(1):=1\n"
		"f(3)=3\n"
		"f(3):=5\n"
		"suma(0,1..4)\n"
		"suma(1,1..4)\n"
		"suma(2,1..4)\n"
		"suma(3,1..4)\n"
		"f(3):=5\n"
		"suma(3,1..4)\n"
		"\n"
		"f(2):=3\n"
		"f(4):=4\n"
		"f(3):=-3\n"
		"f(3):=0\n"
		"f(3) :=0\n"
		"\n"
		"suma(5,1..100)\n"
		"czysc(5)\n"
		"suma(5,1..100)\n"
		"\n"
		"suma(6,1..2)\n"
		"f(2):=7\n"
		"suma(7,1..2)\n";

	std::string out = 
		"wezlow: 1\n"
		"zignorowano\n"
		"wezlow: 3\n"
		"suma(0,1..4)=0\n"
		"suma(1,1..4)=1\n"
		"suma(2,1..4)=6\n"
		"zignorowano\n"
		"wezlow: 3\n"
		"suma(3,1..4)=6\n"
		"zignorowano\n"
		"wezlow: 6\n"
		"wezlow: 9\n"
		"zignorowano\n"
		"wezlow: 11\n"
		"zignorowano\n"
		"zignorowano\n"
		"suma(5,1..100)=13\n"
		"wezlow: 8\n"
		"suma(5,1..100)=0\n"
		"zignorowano\n"
		"suma(6,1..2)=4\n"
		"wezlow: 11\n"
		"suma(7,1..2)=8\n";

	TEST_ASSERT(run(in) == out, "Invalid output.");

	return true;
}

int main()
{
	test.addCase(example(), "example");
	return 0;
}
