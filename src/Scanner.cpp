#include <PCH.h>
#include <Scanner.h>
#include <Error.h>


Ptr<AbstractNode const> Scanner::ScanLet(Lexer::Token const Terminator) {
	if (m_rLexer.Current() != Lexer::TOKEN_KEYWORD_LET) {
		return ScanApplication(Terminator);
	} else if (m_rLexer.Next() == Lexer::TOKEN_IDENTIFIER) {
		string strName = m_rLexer.GetString();
		if (m_rLexer.Next() != Lexer::TOKEN_SYMBOL_EQUAL) {
			throw SyntaxError();
		} else {
			m_rLexer.Next();
			Ptr<AbstractNode const> pExpression = ScanLet(Lexer::TOKEN_KEYWORD_IN);
			assert(m_rLexer.Current() == Lexer::TOKEN_KEYWORD_IN);
			m_rLexer.Next();
			return new LetNode(strName, move(pExpression), ScanLet(Terminator));
		}
	} else {
		throw SyntaxError();
	}
}


Ptr<AbstractNode const> Scanner::ScanApplication(Lexer::Token const Terminator) {
	Ptr<AbstractNode const> pFirst = ScanTerm(Terminator);
	if (m_rLexer.Current() != Terminator) {
		vector<Ptr<AbstractNode const>> Terms;
		Terms.push_back(move(pFirst));
		while (m_rLexer.Current() != Terminator) {
			Terms.push_back(ScanTerm(Terminator));
		}
		return new ApplicationNode(move(Terms));
	} else {
		return pFirst;
	}
}


Ptr<AbstractNode const> Scanner::ScanTerm(Lexer::Token const Terminator) {
	switch (m_rLexer.Current()) {
	case Lexer::TOKEN_LITERAL_INTEGER:
		return ScanIntegerLiteral();
	case Lexer::TOKEN_LITERAL_LONG:
		return ScanLongLiteral();
	case Lexer::TOKEN_LITERAL_FLOAT:
		return ScanFloatLiteral();
	case Lexer::TOKEN_LITERAL_COMPLEX:
		return ScanComplexLiteral();
	case Lexer::TOKEN_LITERAL_STRING:
		return ScanStringLiteral();
	case Lexer::TOKEN_IDENTIFIER:
		return ScanVariable();
	case Lexer::TOKEN_KEYWORD_LAMBDA:
		return ScanFunction(Terminator);
	case Lexer::TOKEN_KEYWORD_MACRO:
		return ScanMacro(Terminator);
	case Lexer::TOKEN_LEFT_PARENS:
		return ScanParens();
	default:
		throw SyntaxError();
	}
}


Ptr<AbstractNode const> Scanner::ScanIntegerLiteral() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LITERAL_INTEGER);
	Ptr<AbstractNode const> pNode = new LiteralNode(new IntegerValue(m_rLexer.GetInteger()));
	m_rLexer.Next();
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanLongLiteral() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LITERAL_LONG);
	Ptr<AbstractNode const> pNode = new LiteralNode(new LongValue(m_rLexer.GetLong()));
	m_rLexer.Next();
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanFloatLiteral() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LITERAL_FLOAT);
	Ptr<AbstractNode const> pNode = new LiteralNode(new FloatValue(m_rLexer.GetFloat()));
	m_rLexer.Next();
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanComplexLiteral() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LITERAL_COMPLEX);
	Ptr<AbstractNode const> pNode = new LiteralNode(new ComplexValue(0, m_rLexer.GetFloat()));
	m_rLexer.Next();
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanStringLiteral() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LITERAL_STRING);
	Ptr<AbstractNode const> pNode = new LiteralNode(new StringValue(m_rLexer.GetString()));
	m_rLexer.Next();
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanVariable() {
	assert(m_rLexer.Current() == Lexer::TOKEN_IDENTIFIER);
	Ptr<AbstractNode const> pNode = new VariableNode(m_rLexer.GetString());
	m_rLexer.Next();
	return pNode;
}


Ptr<AbstractNode const> Scanner::ScanParens() {
	assert(m_rLexer.Current() == Lexer::TOKEN_LEFT_PARENS);
	m_rLexer.Next();
	Ptr<AbstractNode const> pTerm = ScanLet(Lexer::TOKEN_RIGHT_PARENS);
	if (m_rLexer.Current() != Lexer::TOKEN_RIGHT_PARENS) {
		throw SyntaxError();
	} else {
		m_rLexer.Next();
		return pTerm;
	}
}


Ptr<AbstractNode const> Scanner::ScanFunction(Lexer::Token const Terminator) {
	assert(m_rLexer.Current() == Lexer::TOKEN_KEYWORD_LAMBDA);
	if (m_rLexer.Next() != Lexer::TOKEN_IDENTIFIER) {
		throw SyntaxError();
	} else {
		vector<string> Arguments;
		Arguments.push_back(m_rLexer.GetString());
		while (m_rLexer.Next() == Lexer::TOKEN_COMMA) {
			if (m_rLexer.Next() != Lexer::TOKEN_IDENTIFIER) {
				throw SyntaxError();
			} else {
				Arguments.push_back(m_rLexer.GetString());
			}
		}
		if (m_rLexer.Current() != Lexer::TOKEN_POINT) {
			throw SyntaxError();
		} else {
			m_rLexer.Next();
			return new FunctionNode(move(Arguments), ScanLet(Terminator));
		}
	}
}


Ptr<AbstractNode const> Scanner::ScanMacro(Lexer::Token const Terminator) {
	assert(m_rLexer.Current() == Lexer::TOKEN_KEYWORD_MACRO);
	if (m_rLexer.Next() != Lexer::TOKEN_IDENTIFIER) {
		throw SyntaxError();
	} else {
		vector<string> Arguments;
		Arguments.push_back(m_rLexer.GetString());
		while (m_rLexer.Next() == Lexer::TOKEN_COMMA) {
			if (m_rLexer.Next() != Lexer::TOKEN_IDENTIFIER) {
				throw SyntaxError();
			} else {
				Arguments.push_back(m_rLexer.GetString());
			}
		}
		if (m_rLexer.Current() != Lexer::TOKEN_POINT) {
			throw SyntaxError();
		} else {
			m_rLexer.Next();
			return new MacroNode(move(Arguments), ScanLet(Terminator));
		}
	}
}


Scanner::Scanner(Lexer &a_rLexer)
	:
m_rLexer(a_rLexer) {}


Ptr<AbstractNode const> Scanner::Scan() {
	return ScanLet(Lexer::TOKEN_END);
}
