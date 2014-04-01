#pragma once

#include <Lexer.h>
#include <AST.h>


struct Scanner {
private:
	Lexer &m_rLexer;

	Ptr<AbstractNode const> ScanLet(Lexer::Token const Terminator);
	Ptr<AbstractNode const> ScanApplication(Lexer::Token const Terminator);
	Ptr<AbstractNode const> ScanTerm(Lexer::Token const Terminator);
	Ptr<AbstractNode const> ScanIntegerLiteral();
	Ptr<AbstractNode const> ScanLongLiteral();
	Ptr<AbstractNode const> ScanFloatLiteral();
	Ptr<AbstractNode const> ScanComplexLiteral();
	Ptr<AbstractNode const> ScanStringLiteral();
	Ptr<AbstractNode const> ScanVariable();
	Ptr<AbstractNode const> ScanParens();
	Ptr<AbstractNode const> ScanFunction(Lexer::Token const Terminator);
	Ptr<AbstractNode const> ScanMacro(Lexer::Token const Terminator);

public:
	explicit Scanner(Lexer &a_rLexer);

	Ptr<AbstractNode const> Scan();

};
