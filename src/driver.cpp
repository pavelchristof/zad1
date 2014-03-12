#include "driver.hpp"
#include "parser.hpp"
#include "bst.hpp"

struct DriverPrivate : public ParserEventHandler
{
	const uint64_t maxKey = 1000 * 1000 * 1000;
	const uint64_t maxValue = 1000;
	const uint32_t maxStatements = 1000 * 1000;

	DriverPrivate(std::istream &in, std::ostream &out) :
		in(in), out(out), parser(this), statements(0)
	{
	}

	void assignment(uint64_t key, uint64_t value) override
	{
		if (key > maxKey || value > maxValue || !stmtCheck()) {
			error();
			return;
		}

		uint32_t nodes = bst.assignment(key, value);
		out << "wezlow: " << nodes << '\n';
	}

	void sum(uint64_t time, uint64_t left, uint64_t right) override
	{
		if (time > bst.now() || left > right || right > maxKey || !stmtCheck()) {
			error();
			return;
		}

		uint64_t sum = bst.sum(time, left, right);
		out << "suma(" << time << ',' << left << ".." << right << ")=" << sum << '\n';
	}

	void clear(uint64_t time) override
	{
		if (time > bst.now() || !stmtCheck()) {
			error();
			return;
		}

		uint32_t nodes = bst.clear(time);
		out << "wezlow: " << nodes << '\n';
	}

	void error() override
	{
		out << "zignorowano\n";
	}

	bool stmtCheck()
	{
		return statements++ < maxStatements;
	}

	std::istream &in;
	std::ostream &out;

	BST bst;
	Parser parser;
	uint32_t statements;
};

Driver::Driver(std::istream &in, std::ostream &out) :
	d(new DriverPrivate(in, out))
{
}

Driver::~Driver()
{
	delete d;
}

void Driver::run()
{
	d->parser.parse(d->in);
}
