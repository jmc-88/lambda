#pragma once

#include <Environment.h>


struct AbstractNode;

struct Closure {
	string const m_strArgument;
	Ptr<AbstractNode const> m_pBody;
	Environment m_Environment;

	Closure(string const &a_rstrArgument, Ptr<AbstractNode const> &&a_rrpBody, map<string, Closure*> &&a_rrCapture);

	operator string const () const;
};


ostream &operator << (ostream &ros, Closure const &rClosure);
