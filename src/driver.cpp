#include "driver.hpp"
#include "parser.hpp"
#include "bst.hpp"

Driver::Driver(std::istream &in, std::ostream &out) :
	in(in), out(out), parser(this), statements(0)
{
}

void Driver::run()
{
	parser.parse(in);
}

void Driver::assignment(uint64_t key, uint64_t value)
{
	if (key > maxKey || value > maxValue || !stmtCheck()) {
		error();
		return;
	}

	int nodes = bst.assignment(key, value);
	out << "wezlow: " << nodes << '\n';
}

void Driver::sum(uint64_t time, uint64_t left, uint64_t right)
{
	if (time > (uint64_t)bst.now() || left > right || right > maxKey || !stmtCheck()) {
		error();
		return;
	}

	int sum = bst.sum(time, left, right);
	out << "suma(" << time << ',' << left << ".." << right << ")=" << sum << '\n';
}

void Driver::clear(uint64_t time)
{
	if (time > (uint64_t)bst.now() || !stmtCheck()) {
		error();
		return;
	}

	int nodes = bst.clear(time);
	out << "wezlow: " << nodes << '\n';
}

void Driver::error()
{
	out << "zignorowano\n";
}

bool Driver::stmtCheck()
{
	return statements++ < maxStatements;
}
