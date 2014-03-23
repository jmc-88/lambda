#pragma once

#include <Lexer.h>
#include <AST.h>


struct Scanner {
private:
	Lexer &m_rLexer;

	Ptr<AbstractNode const> ScanApplication(Lexer::Token const Terminator);
	Ptr<AbstractNode const> ScanFunction(Lexer::Token const Terminator);
	Ptr<AbstractNode const> ScanParens();
	Ptr<AbstractNode const> ScanVariable();
	Ptr<AbstractNode const> ScanSubTerm(Lexer::Token const Terminator);

public:
	explicit Scanner(Lexer &a_rLexer);

	Ptr<AbstractNode const> Scan();

};
