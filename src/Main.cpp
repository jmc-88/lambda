#include <PCH.h>
#include <Error.h>
#include <Lexer.h>
#include <Scanner.h>
#include <Values.h>

int main() {
	string strLine;
	while (true) {
		cout << "lambda> " << flush;
		::getline(cin, strLine);
		istringstream iss(strLine);
		try {
			cout << "      = " << flush;
			Lexer Lexer(iss);
			Scanner Scanner(Lexer);
			Ptr<AbstractValue const> pResult = Scanner.Scan()->Evaluate();
			cout << *pResult << endl;
		} catch (SyntaxError const &re) {
			cerr << "syntax error" << endl;
		} catch (InternalError const &re) {
			cerr << "internal error" << endl;
		} catch (RuntimeError const &re) {
			cerr << "runtime error" << endl;
		}
	}
}
