#pragma once


struct AbstractNode;
struct Environment;

struct Closure {
	string const m_strArgumentName;
	Ptr<AbstractNode const> m_pBody;

	// TODO environment

	Closure(string const &a_rstrArgumentName, Ptr<AbstractNode const> &&a_rrpBody);
};
