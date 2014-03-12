#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <istream>
#include <ostream>

struct DriverPrivate;

/**
 * Loads, parses and executes commands.
 */
class Driver
{
public:
	Driver(std::istream &in, std::ostream &out);
	~Driver();

	void run();

private:
	DriverPrivate *d;
};

#endif // DRIVER_HPP
