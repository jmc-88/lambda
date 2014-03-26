#pragma once


struct Lexer {
public:
	enum Token {
		TOKEN_IDENTIFIER,
		TOKEN_KEYWORD_LAMBDA,
		TOKEN_LITERAL_INTEGER,
		TOKEN_LITERAL_LONG,
		TOKEN_LITERAL_FLOAT,
		TOKEN_LITERAL_STRING,
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
