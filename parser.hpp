#ifndef PARSER_HPP
#define PARSER_HPP

#include <istream>

class ParserEventHandler
{
public:
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

struct ParserPrivate;

class Parser
{
public:
	/**
	 * Constructs a parser that will report events to @a handler.
	 */
	Parser(ParserEventHandler *handler);

	~Parser();

	/**
	 * Parses all stream input until EOF.
	 */
	void parse(std::istream &stream);

private:
	ParserPrivate *d;
};

#endif // PARSER_HPP
