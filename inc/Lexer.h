#pragma once


struct Lexer {
public:
	enum Token {
		TOKEN_LAMBDA,
		TOKEN_IDENTIFIER,
		TOKEN_COMMA,
		TOKEN_POINT,
		TOKEN_LEFT_PARENS,
		TOKEN_RIGHT_PARENS,
		TOKEN_END
	};

private:
	istream &m_ris;

	stack<Token> m_Buffer;

	Token m_Token;

	string m_str;

public:
	explicit Lexer(istream &a_ris);

	struct InvalidCharacterException {};

	Token Current() const;
	bool End() const;

	bool HasNext() const;
	Token Next();

	void PushBack(Token const Token);

	string const GetString() const;
};
