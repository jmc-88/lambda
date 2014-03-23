#include <PCH.h>
#include <Lexer.h>
#include <Scanner.h>
#include <Error.h>

int main() {
	string strLine;
	while (true) {
		cout << "lambda> " << flush;
		::getline(cin, strLine);
		istringstream iss(strLine);
		Lexer Lexer(iss);
		Scanner Scanner(Lexer);
		try {
			cout << "      > " << Scanner.Scan()->Evaluate() << endl;
		} catch (SyntaxError const &re) {
			cerr << "      > syntax error" << endl;
		} catch (InternalError const &re) {
			cerr << "      > internal error" << endl;
		} catch (Environment::NotFoundException const &re) {
			cerr << "      > variable not found: " << re.m_strName << endl;
		}
		// TODO
	}
	return 0;
}
