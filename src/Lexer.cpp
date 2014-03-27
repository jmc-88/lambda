#include <PCH.h>
#include <Lexer.h>
#include <Error.h>


Lexer::Lexer(istream &a_ris)
	:
m_ris(a_ris) {
	m_ris.exceptions(ios::badbit);
	Next();
}


Lexer::Token Lexer::Current() const {
	return m_Token;
}


bool Lexer::End() const {
	return m_Token == TOKEN_END;
}


bool Lexer::HasNext() const {
	return m_Token != TOKEN_END;
}


Lexer::Token Lexer::Next() {
	if (m_Buffer.size()) {
		Token const Token = m_Buffer.top();
		m_Buffer.pop();
		return m_Token = Token;
	} else {
		while (true) {
			int const ch = m_ris.get();
			if (ch < 0) {
				return m_Token = TOKEN_END;
			} else if (ch > 127) {
				throw SyntaxError();
			} else if (::isspace(ch)) {
				continue;
			} else if (::isalpha(ch) || ch == '_') {
				string str(1, (unsigned char)ch);
				while (true) {
					int const ch = m_ris.peek();
					if (::isalnum(ch) || ch == '_') {
						m_ris.get();
						str += (unsigned char)ch;
					} else {
						break;
					}
				}
				if (str != "lambda") {
					m_Token = TOKEN_IDENTIFIER;
					m_str = str;
				} else {
					m_Token = TOKEN_KEYWORD_LAMBDA;
				}
				return m_Token;
			} else if (::isdigit(ch)) {
				unsigned long long l = ch - '0';
				while (::isdigit(m_ris.peek())) {
					int const ch = m_ris.get();
					l = l * 10 + (ch - '0');
					if (l >= (unsigned long long)1 << 63) {
						throw SyntaxError();
					}
				}
				if (l < (unsigned int)1 << 31) {
					m_Token = TOKEN_LITERAL_INTEGER;
					m_n = (signed int)l;
				} else {
					m_Token = TOKEN_LITERAL_LONG;
					m_l = (signed long long)l;
				}
				return m_Token;
			} else if (ch == '\'') {
				string str;
				while (true) {
					int const ch = m_ris.peek();
					if (ch < 0) {
						throw SyntaxError();
					} else if (ch > 127) {
						throw SyntaxError();
					} else {
						m_ris.get();
						if (ch == '\'') {
							break;
						} else if (ch == '\\') {
							int const ch2 = m_ris.get();
							if (ch2 < 0) {
								throw SyntaxError();
							} else if (ch2 > 127) {
								throw SyntaxError();
							} else if (ch2 == '\'') {
								str += '\'';
							} else if (ch2 == '\\') {
								str += '\\';
							} else if (ch2 == '\n') {
								str += '\n';
							} else if (ch2 == '\r') {
								str += '\r';
							} else if (ch2 == '\t') {
								str += '\t';
							} else {
								str += ch2;
							}
						} else {
							str += (char)ch;
						}
					}
				}
				m_str = str;
				return m_Token = TOKEN_LITERAL_STRING;
			} else {
				switch (ch) {
				case '(':
					return m_Token = TOKEN_LEFT_PARENS;
				case ')':
					return m_Token = TOKEN_RIGHT_PARENS;
				case ',':
					return m_Token = TOKEN_COMMA;
				case '.':
					return m_Token = TOKEN_POINT;
				default:
					throw SyntaxError();
				}
			}
		}
	}
}


void Lexer::PushBack(Lexer::Token const Token) {
	m_Buffer.push(Token);
}


string const Lexer::GetString() const {
	return m_str;
}


signed int Lexer::GetInteger() const {
	return m_n;
}


signed long long Lexer::GetLong() const {
	return m_l;
}


double Lexer::GetFloat() const {
	return m_f;
}
