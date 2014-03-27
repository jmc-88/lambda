#pragma once

#include <Lexer.h>
#include <AST.h>


struct Scanner {
private:
	Lexer &m_rLexer;

	Ptr<AbstractNode const> ScanApplication();
	Ptr<AbstractNode const> ScanTerm();
	Ptr<AbstractNode const> ScanFunction();

public:
	explicit Scanner(Lexer &a_rLexer);

	Ptr<AbstractNode const> Scan();

};
