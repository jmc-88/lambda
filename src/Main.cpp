#include <PCH.h>
#include <Lexer.h>
#include <Scanner.h>

int main() {
	string strLine;
	while (true) {
		cout << "lambda> " << flush;
		::getline(cin, strLine);
		istringstream iss(strLine);
		Lexer Lexer(iss);
		Scanner Scanner(Lexer);
		Ptr<AbstractNode const> pRoot = Scanner.Scan();
		// TODO
	}
	return 0;
}
