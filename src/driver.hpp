#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "parser.hpp"
#include "bst.hpp"

#include <istream>
#include <ostream>

/**
 * Loads, parses and executes commands.
 */
class Driver : public ParserEventHandler
{
public:
	Driver(std::istream &in, std::ostream &out);

	void run();

	void assignment(uint64_t key, uint64_t value) override;
	void sum(uint64_t time, uint64_t left, uint64_t right) override;
	void clear(uint64_t time) override;
	void error() override;

private:
	bool stmtCheck();

	std::istream &in;
	std::ostream &out;

	BST bst;
	Parser parser;
	uint32_t statements;
};

#endif // DRIVER_HPP
