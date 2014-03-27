#include <PCH.h>
#include <Scanner.h>
#include <Error.h>


Ptr<AbstractNode const> Scanner::ScanApplication() {
	vector<Ptr<AbstractNode const>> Terms;
	Terms.push_back(ScanTerm());
	while (m_rLexer.Current() != Lexer::TOKEN_END) {
		Terms.push_back(ScanTerm());
	}
	return new ApplicationNode(move(Terms));
}


Ptr<AbstractNode const> Scanner::ScanTerm() {
	// TODO
}


Ptr<AbstractNode const> Scanner::ScanFunction() {
	assert(m_rLexer.Current() == Lexer::TOKEN_KEYWORD_LAMBDA);
	vector<string> Arguments;
	if (m_rLexer.Next() != Lexer::TOKEN_IDENTIFIER) {
		throw SyntaxError();
	} else {
		Arguments.push_back(m_rLexer.GetString());
		// TODO
	}
}


Scanner::Scanner(Lexer &a_rLexer)
	:
m_rLexer(a_rLexer) {}


Ptr<AbstractNode const> Scanner::Scan() {
	return ScanApplication();
}
