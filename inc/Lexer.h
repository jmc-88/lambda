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
		TOKEN_TRUE,
		TOKEN_FALSE,
		TOKEN_INTEGER,
		TOKEN_LONG,
		TOKEN_FLOAT,
		TOKEN_STRING,
		TOKEN_END
	};

private:
	istream &m_ris;

	stack<Token> m_Buffer;

	Token m_Token;

	string m_str;
	signed int m_n;
	signed long long m_l;
	double m_f;

public:
	explicit Lexer(istream &a_ris);

	Token Current() const;
	bool End() const;

	bool HasNext() const;
	Token Next();

	void PushBack(Token const Token);

	string const GetString() const;
	signed int GetInteger() const;
	signed long long GetLong() const;
	double GetFloat() const;
};
