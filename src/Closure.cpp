#include <PCH.h>
#include <Closure.h>
#include <AST.h>


Closure::Closure(string const &a_rstrArgument, Ptr<AbstractNode const> &&a_rrpBody, map<string, Closure*> &&a_rrCapture)
	:
m_strArgument(a_rstrArgument),
	m_pBody(move(a_rrpBody)),
	m_Environment(move(a_rrCapture)) {}


Closure::operator string const () const {
	return "lambda " + m_strArgument + " . " + m_pBody->ToString(m_Environment);
}


ostream &operator << (ostream &ros, Closure const &rClosure) {
	return ros << (string const)rClosure;
}
