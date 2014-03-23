#include <PCH.h>
#include <Closure.h>
#include <AST.h>
#include <Environment.h>


Closure::Closure(string const &a_rstrArgumentName, Ptr<AbstractNode const> &&a_rrpBody, Environment const &rEnvironment)
	:
m_strArgumentName(a_rstrArgumentName),
	m_pBody(move(a_rrpBody)),
	m_Capture(rEnvironment.Capture()) {}


Closure::operator string const () const {
	return "lambda " + m_strArgumentName + " . " + (string const)*m_pBody;
}


ostream &operator << (ostream &ros, Closure const &rClosure) {
	return ros << (string const)rClosure;
}
