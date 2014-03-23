#include <PCH.h>
#include <Scanner.h>
#include <Error.h>


Scanner::Scanner(Lexer &a_rLexer)
	:
m_rLexer(a_rLexer) {}


Ptr<AbstractNode const> Scanner::ScanApplication(Lexer::Token const Terminator) {
	Ptr<AbstractNode const> pNode = ScanSubTerm(Terminator);
	while (m_rLexer.Current() != Terminator) {
		pNode = new ApplicationNode(pNode.Detach(), ScanSubTerm(Terminator));
	}
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanFunction(Lexer::Token const Terminator) {
	assert(m_rLexer.Current() == Lexer::TOKEN_LAMBDA);
	if (m_rLexer.Next() != Lexer::TOKEN_IDENTIFIER) {
		throw SyntaxError();
	} else {
		string const strArgumentName = m_rLexer.GetString();
		if (m_rLexer.Next() != Lexer::TOKEN_POINT) {
			throw SyntaxError();
		} else {
			return new FunctionNode(strArgumentName, ScanApplication(Terminator));
		}
	}
}


Ptr<AbstractNode const> Scanner::ScanParens() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LEFT_PARENS);
	m_rLexer.Next();
	Ptr<AbstractNode const> pSubTerm = ScanApplication(Lexer::TOKEN_RIGHT_PARENS);
	if (m_rLexer.Current() != Lexer::TOKEN_RIGHT_PARENS) {
		throw SyntaxError();
	} else {
		return pSubTerm;
	}
}


Ptr<AbstractNode const> Scanner::ScanVariable() {
	assert(m_rLexer.Current() == Lexer::TOKEN_IDENTIFIER);
	string const strName = m_rLexer.GetString();
	m_rLexer.Next();
	return new VariableNode(strName);
}


Ptr<AbstractNode const> Scanner::ScanSubTerm(Lexer::Token const Terminator) {
	switch (m_rLexer.Current()) {
	case Lexer::TOKEN_LAMBDA:
		return ScanFunction(Terminator);
	case Lexer::TOKEN_LEFT_PARENS:
		return ScanParens();
	case Lexer::TOKEN_IDENTIFIER:
		return ScanVariable();
	default:
		throw SyntaxError();
	}
}


Ptr<AbstractNode const> Scanner::Scan() {
	return ScanApplication(Lexer::TOKEN_END);
}
