#include <PCH.h>
#include <Error.h>
#include <Lexer.h>
#include <Scanner.h>
#include <Values.h>
#include <BuiltIn.h>

namespace {

bool prompt(string& input) {
	cout << "lambda> " << flush;
	return !getline(cin, input).eof();
}

}

int main() {
	string input;
	while (prompt(input)) {
		istringstream iss(input);
		try {
			cout << "      = " << flush;
			Lexer Lexer(iss);
			Scanner Scanner(Lexer);
			cout << *(Scanner.Scan()->Evaluate(BuiltInEnvironment())) << endl;
		} catch (InternalError const &re) {
			cerr << "internal error" << endl;
		} catch (SyntaxError const &re) {
			cerr << "syntax error" << endl;
		} catch (PreprocessError const &re) {
			cerr << "preprocess error" << endl;
		} catch (RuntimeError const &re) {
			cerr << "runtime error" << endl;
		} catch (UserException const &re) {
			cerr << "exception: " << *(re.m_pValue) << endl;
		} catch (ExitException const &re) {
			return re.m_pCode->m_n;
		}
	}
	cout << endl;
}
