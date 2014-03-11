#include "parser.hpp"

#include <limits>

struct ParserPrivate
{
	ParserEventHandler *handler;
	std::istream *stream;

	void error()
	{
		handler->error();

		// Ignore all characters until the next line.
		stream->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	bool expect(char c)
	{
		if (stream->peek() == c) {
			stream->get();
			return true;
		} else {
			error();
			return false;
		}
	}

	bool expect(const char *s)
	{
		while (*s != 0) {
			if (!expect(*s)) {
				return false;
			}
			++s;
		}
		return true;
	}

	/**
	 * Expects EOL or EOF.
	 */
	bool expectEOL()
	{
		if (stream->peek() == EOF) {
			stream->get();
			return true;
		}

		return expect('\n');
	}

	/**
	 * <statement> ::= <set>
	 *               | <sum>
	 *               | <clear>
	 */
	void statement()
	{
		switch (stream->peek()) {
		case 'f':
			set();
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

	/**
	 * <set> ::= f(<number>):=<number>EOL
	 */
	void set()
	{
		uint64_t key, value;

		if (expect("f(")  &&
			number(key)   &&
			expect("):=") &&
			number(value) &&
			expectEOL())
		{
			handler->set(key, value);
		}
	}

	/**
	 * <sum> ::= suma(<number>,<number>..<number>)EOL
	 */
	void sum()
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

	/**
	 * <clear> ::= czysc(<number>)EOL
	 */
	void clear()
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

	/**
	 * <number> ::= [0-9]{1,10}
	 */
	bool number(uint64_t &value)
	{
		value = 0;
		
		if (stream->peek() == '0') {
			//TODO: error?
		}

		int i;
		for (i = 0; i < 10 && isdigit(stream->peek()); ++i) {
			int digit = stream->get() - '0';
			value = 10 * value + digit;
		}

		return i != 0;
	}
};

Parser::Parser(ParserEventHandler *handler) :
	d(new ParserPrivate)
{
	d->handler = handler;
	d->stream = nullptr;
}

Parser::~Parser()
{
	delete d;
}

void Parser::parse(std::istream& stream)
{
	d->stream = &stream;
	while (stream) {
		d->statement();
	}
	d->stream = nullptr;
}
