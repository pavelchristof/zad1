#include "bst.hpp"
#include "parser.hpp"

#include <iostream>

const uint64_t maxKey = 1000 * 1000 * 1000;
const uint64_t maxValue = 1000;
const uint32_t maxStatements = 1000 * 1000;

class Driver : public ParserEventHandler
{
public:
	Driver() : parser(this), statements(0) {}
	
	void run()
	{
		parser.parse(std::cin);
	}
	
	virtual void set(uint64_t key, uint64_t value)
	{
		if (key > maxKey || value > maxValue || !stmtCheck()) {
			error();
			return;
		}
		
		uint32_t nodes = bst.set(key, value);
		std::cout << "wezlow: " << nodes << '\n';
	}
	
	virtual void sum(uint64_t time, uint64_t left, uint64_t right)
	{
		if (time > bst.now() || left > right || right > maxKey || !stmtCheck()) {
			error();
			return;
		}
		
		uint64_t sum = bst.sum(time, left, right);
		std::cout << "suma(" << time << ',' << left << ".." << right << ")=" << sum << '\n';
	}
	
	virtual void clear(uint64_t time)
	{
		if (time > bst.now() || !stmtCheck()) {
			error();
			return;
		}
		
		uint32_t nodes = bst.clear(time);
		std::cout << "wezlow: " << nodes << '\n';
	}
	
	virtual void error()
	{
		std::cout << "zignorowano\n";
	}
	
private:
	bool stmtCheck() 
	{
		return statements++ < maxStatements;
	}
	
	BST bst;
	Parser parser;
	uint32_t statements;
};

int main()
{
	Driver driver;
	driver.run();
	return 0;
}
