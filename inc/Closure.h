#pragma once


struct AbstractNode;
struct Environment;

struct Closure {
	string const m_strArgumentName;
	Ptr<AbstractNode const> m_pBody;
	map<string, Closure const*> m_Capture;

	Closure(string const &a_rstrArgumentName, Ptr<AbstractNode const> &&a_rrpBody, Environment const &rEnvironment);

	operator string const () const;
};


ostream &operator << (ostream &ros, Closure const &rClosure);
