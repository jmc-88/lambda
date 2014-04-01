#include <PCH.h>
#include <Error.h>
#include <Lexer.h>
#include <Scanner.h>
#include <Values.h>
#include <BuiltIn.h>

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
			cout << *(Scanner.Scan()->Evaluate(BuiltInEnvironment())) << endl;
		} catch (SyntaxError const &re) {
			cerr << "syntax error" << endl;
		} catch (InternalError const &re) {
			cerr << "internal error" << endl;
		} catch (RuntimeError const &re) {
			cerr << "runtime error" << endl;
		} catch (UserException const &re) {
			cerr << "exception: " << *(re.m_pValue) << endl;
		} catch (ExitException const &re) {
			cout << "program terminated with code " << *(re.m_pCode) << endl;
		}
	}
}
