#include "parser.hpp"

#include <limits>

Parser::Parser(ParserEventHandler *handler) :
	handler(handler), stream(nullptr)
{
}

void Parser::parse(std::istream &in)
{
	stream = &in;
	while (in && in.peek() != EOF) {
		statement();
	}
	stream = nullptr;
}

void Parser::error()
{
	handler->error();

	// Ignore all characters until the next line.
	stream->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool Parser::expect(char c)
{
	if (stream->peek() == c) {
		stream->get();
		return true;
	} else {
		error();
		return false;
	}
}

bool Parser::expect(const char *s)
{
	while (*s != 0) {
		if (!expect(*s)) {
			return false;
		}
		++s;
	}
	return true;
}

bool Parser::expectEOL()
{
	if (stream->peek() == EOF) {
		stream->get();
		return true;
	}

	return expect('\n');
}

void Parser::statement()
{
	switch (stream->peek()) {
		case 'f':
			assignment();
			break;

		case 's':
			sum();
			break;

		case 'c':
			clear();
			break;

		default:
			error();
			break;
	}
}

void Parser::assignment()
{
	uint64_t key, value;

	if (expect("f(")  &&
		number(key)   &&
		expect("):=") &&
		number(value) &&
		expectEOL())
	{
		handler->assignment(key, value);
	}
}

void Parser::sum()
{
	uint64_t time, left, right;

	if (expect("suma(") &&
		number(time)    &&
		expect(',')     &&
		number(left)    &&
		expect("..")    &&
		number(right)   &&
		expect(')')     &&
		expectEOL())
	{
		handler->sum(time, left, right);
	}
}

void Parser::clear()
{
	uint64_t time;

	if (expect("czysc(") &&
		number(time)     &&
		expect(')')      &&
		expectEOL())
	{
		handler->clear(time);
	}
}

bool Parser::number(uint64_t &value)
{
	value = 0;

	if (stream->peek() == '0') {
		stream->get();
		return true;
	}

	int i;
	for (i = 0; i < 10 && isdigit(stream->peek()); ++i) {
		int digit = stream->get() - '0';
		value = 10 * value + digit;
	}

	return i != 0;
}
