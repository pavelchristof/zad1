#include "driver.hpp"

const uint64_t maxKey = 1000 * 1000 * 1000;
const uint64_t maxValue = 1000;
const uint32_t maxStatements = 1000 * 1000;

Driver::Driver(std::istream &in, std::ostream &out) :
	in(in), out(out), parser(this),  statements(0)
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

	uint32_t nodes = bst.assignment(key, value);
	out << "wezlow: " << nodes << '\n';
}

void Driver::sum(uint64_t time, uint64_t left, uint64_t right)
{
	if (time > bst.now() || left > right || right > maxKey || !stmtCheck()) {
		error();
		return;
	}

	uint64_t sum = bst.sum(time, left, right);
	out << "suma(" << time << ',' << left << ".." << right << ")=" << sum << '\n';
}

void Driver::clear(uint64_t time)
{
	if (time > bst.now() || !stmtCheck()) {
		error();
		return;
	}

	uint32_t nodes = bst.clear(time);
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
