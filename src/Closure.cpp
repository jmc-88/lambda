#include <PCH.h>
#include <Closure.h>
#include <AST.h>
#include <Environment.h>


Closure::Closure(string const &a_rstrArgumentName, Ptr<AbstractNode const> &&a_rrpBody, Environment const &rEnvironment)
	:
m_strArgumentName(a_rstrArgumentName),
	m_pBody(move(a_rrpBody)),
	m_Capture(rEnvironment.Capture()) {}
