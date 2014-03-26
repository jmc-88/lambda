#include <PCH.h>
#include <Error.h>
#include <Lexer.h>

int main() {
	string strLine;
	while (true) {
		cout << "lambda> " << flush;
		::getline(cin, strLine);
		istringstream iss(strLine);
		try {
			cout << "      = " << flush;
			Lexer Lexer(iss);
			// TODO
		} catch (SyntaxError const &re) {
			cerr << "syntax error" << endl;
		} catch (InternalError const &re) {
			cerr << "internal error" << endl;
		} catch (RuntimeError const &re) {
			cerr << "runtime error" << endl;
		}
	}
}
