#include <PCH.h>
#include <Lexer.h>


Lexer::Lexer(istream &a_ris)
	:
m_ris(a_ris) {
	m_ris.exceptions(ios::failbit);
	Next();
}


Lexer::Token Lexer::Current() const {
	return m_Token;
}


bool Lexer::HasNext() const {
	while (true) {
		int const ch = m_ris.peek();
		if (ch < 0) {
			return false;
		} else if (ch > 127) {
			throw InvalidCharacterException();
		} else if (::isspace(ch)) {
			m_ris.get();
			continue;
		} else {
			return true;
		}
	}
}


Lexer::Token Lexer::Next() {
	if (m_Buffer.size()) {
		Token const Token = m_Buffer.top();
		m_Buffer.pop();
		return Token;
	} else {
		while (true) {
			int const ch = m_ris.get();
			if (ch < 0) {
				return m_Token = TOKEN_END;
			} else if (ch > 127) {
				throw InvalidCharacterException();
			} else if (::isspace(ch)) {
				continue;
			} else if (::isalpha(ch)) {
				string str(1, (unsigned char)ch);
				while (true) {
					int const ch = m_ris.peek();
					if (ch < 0) {
						break;
					} else if (ch > 127) {
						throw InvalidCharacterException();
					} else if (::isalnum(ch)) {
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
					m_Token = TOKEN_LAMBDA;
				}
				return m_Token;
			} else {
				switch (ch) {
				case '(':
					return m_Token = TOKEN_LEFT_PARENS;
				case ')':
					return m_Token = TOKEN_RIGHT_PARENS;
				case '.':
					return m_Token = TOKEN_POINT;
				default:
					throw InvalidCharacterException();
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
