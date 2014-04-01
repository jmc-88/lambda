#include <PCH.h>
#include <Lexer.h>
#include <Error.h>


bool Lexer::IsSpace(int const ch) {
	return ch >= 0 && ::isspace(ch);
}


bool Lexer::IsDigit(int const ch) {
	return ch >= 0 && ::isdigit(ch);
}


bool Lexer::IsAlpha(int const ch) {
	return ch >= 0 && ::isalpha(ch);
}


bool Lexer::IsIdentifierCharacter(int const ch) {
	return ch >= 0 && (::isalnum(ch) || ch == '_');
}


unsigned char Lexer::GetHex(int const ch) {
	if ((ch >= '0') && (ch <= '9')) {
		return ch - '0';
	} else if ((ch >= 'A') && (ch <= 'F')) {
		return ch - 'A' + 10;
	} else if ((ch >= 'a') && (ch <= 'f')) {
		return ch - 'a' + 10;
	} else {
		throw SyntaxError();
	}
}


int Lexer::SkipSeparators() {
	int ch;
	while (true) {
		ch = m_ris.get();
		if (ch < 0) {
			return ch;
		} else if (ch > 127) {
			throw SyntaxError();
		} else if (ch == '#') {
			do {
				if ((ch = m_ris.get()) < 0) {
					return ch;
				}
			} while (ch != '\n');
		} else if (!::isspace(ch)) {
			return ch;
		}
	}
}


Lexer::Token Lexer::ReadToken() {
	int const ch = SkipSeparators();
	if (ch < 0) {
		return m_Token = TOKEN_END;
	} else if (::isalpha(ch) || ch == '_') {
		char const sz[] = { (char)ch, 0 };
		string str = sz;
		while (true) {
			int const ch = m_ris.peek();
			if (IsIdentifierCharacter(ch)) {
				m_ris.get();
				str += (char)ch;
			} else {
				break;
			}
		}
		if (str == "lambda") {
			m_Token = TOKEN_KEYWORD_LAMBDA;
		} else if (str == "macro") {
			m_Token = TOKEN_KEYWORD_MACRO;
		} else if (str == "let") {
			m_Token = TOKEN_KEYWORD_LET;
		} else if (str == "in") {
			m_Token = TOKEN_KEYWORD_IN;
		} else {
			m_Token = TOKEN_IDENTIFIER;
			m_str = str;
		}
		return m_Token;
	} else if (::isdigit(ch)) {
		unsigned long long l = ch - '0';
		while (IsDigit(m_ris.peek())) {
			int const ch = m_ris.get();
			l = l * 10 + (ch - '0');
			if (l >= (unsigned long long)1 << 63) {
				throw SyntaxError();
			}
		}
		if (m_ris.peek() != '.') {
			if (m_ris.peek() != 'i') {
				if (l < (unsigned int)1 << 31) {
					m_Token = TOKEN_LITERAL_INTEGER;
					m_n = (signed int)l;
				} else {
					m_Token = TOKEN_LITERAL_LONG;
					m_l = (signed long long)l;
				}
			} else {
				m_ris.get();
				m_Token = TOKEN_LITERAL_COMPLEX;
				m_f = l;
			}
		} else {
			m_ris.get();
			int const ch = m_ris.get();
			if (!IsDigit(ch)) {
				throw SyntaxError();
			}
			double f = l + (ch - '0') / 10.0;
			for (unsigned int n = -2; IsDigit(m_ris.peek()); --n) {
				f += (m_ris.get() - '0') * ::pow(10, n);
			}
			if (m_ris.peek() != 'i') {
				m_Token = TOKEN_LITERAL_FLOAT;
			} else {
				m_ris.get();
				m_Token = TOKEN_LITERAL_COMPLEX;
			}
			m_f = f;
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
					int ch2 = m_ris.get();
					if (ch2 < 0) {
						throw SyntaxError();
					} else if (ch2 > 127) {
						throw SyntaxError();
					} else if (ch2 == '\'') {
						str += '\'';
					} else if (ch2 == '\\') {
						str += '\\';
					} else if (ch2 == 'n') {
						str += '\n';
					} else if (ch2 == 'r') {
						str += '\r';
					} else if (ch2 == 't') {
						str += '\t';
					} else if (ch2 == 'x') {
						unsigned char const nCodeHi = GetHex(m_ris.get());
						unsigned char const nCodeLo = GetHex(m_ris.get());
						unsigned char const nCode = (nCodeHi << 4) + nCodeLo;
						str += nCode;
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
		case '=':
			return m_Token = TOKEN_SYMBOL_EQUAL;
		default:
			throw SyntaxError();
		}
	}
}


Lexer::Lexer(istream &a_ris)
	:
m_ris(a_ris) {
	m_ris.exceptions(ios::badbit);
	m_Token = ReadToken();
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
		return m_Token = ReadToken();
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
