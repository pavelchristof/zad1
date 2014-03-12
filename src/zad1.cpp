#include "driver.hpp"

#include <iostream>

int main()
{
	Driver driver(std::cin, std::cout);
	driver.run();
	return 0;
}
