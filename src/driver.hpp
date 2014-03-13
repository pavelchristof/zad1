#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "bst.hpp"
#include "parser.hpp"

#include <istream>
#include <ostream>

/**
 * Loads, parses and executes commands.
 */
class Driver : private ParserEventHandler
{
public:
	static const uint64_t maxKey = 1000 * 1000 * 1000;
	static const uint64_t maxValue = 1000;
	static const uint32_t maxStatements = 1000 * 1000;

	Driver(std::istream &in, std::ostream &out);
	void run();

private:
	void assignment(uint64_t key, uint64_t value) override;
	void sum(uint64_t time, uint64_t left, uint64_t right) override;
	void clear(uint64_t time) override;
	void error() override;
	bool stmtCheck();

	std::istream &in;
	std::ostream &out;

	BST bst;
	Parser parser;
	uint32_t statements;
};

#endif // DRIVER_HPP
