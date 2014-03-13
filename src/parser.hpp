#ifndef PARSER_HPP
#define PARSER_HPP

#include <istream>

class ParserEventHandler
{
public:
	virtual ~ParserEventHandler() {};

	/**
	 * Called on " f(@a key):=@a value ".
	 */
	virtual void assignment(uint64_t key, uint64_t value) = 0;

	/**
	 * Called on " suma(@a time, @a left .. @a right) ".
	 */
	virtual void sum(uint64_t time, uint64_t left, uint64_t right) = 0;

	/**
	 * Called on " czysc(@a time) ".
	 */
	virtual void clear(uint64_t time) = 0;

	/**
	 * Called on an invalid statement.
	 */
	virtual void error() = 0;
};

class Parser
{
public:
	/**
	 * Constructs a parser that will report events to @a handler.
	 */
	Parser(ParserEventHandler *handler);

	/**
	 * Parses all stream input until EOF.
	 */
	void parse(std::istream &in);

private:
	void error();
	bool expect(char c);
	bool expect(const char *s);

	/**
	 * <statement> ::= <assignment>
	 *               | <sum>
	 *               | <clear>
	 */
	void statement();

	/**
	 * <assignment> ::= f(<number>):=<number>EOL
	 */
	void assignment();

	/**
	 * <sum> ::= suma(<number>,<number>..<number>)EOL
	 */
	void sum();

	/**
	 * <clear> ::= czysc(<number>)EOL
	 */
	void clear();

	/**
	 * <number> ::= 0 
	 *            | [1-9][0-9]{1,9}
	 */
	bool number(uint64_t &value);

	ParserEventHandler *handler;
	std::istream *stream;
};

#endif // PARSER_HPP
