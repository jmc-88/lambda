#pragma once

#include <Lexer.h>


struct Scanner {
private:
	Lexer &m_rLexer;

public:
	explicit Scanner(Lexer &a_rLexer);

};
