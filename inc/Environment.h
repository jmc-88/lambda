#pragma once

#include <Closure.h>


struct Environment {
private:
	map<string const, stack<Closure const>> m_Map;

public:
	explicit Environment(map<string const, Ptr<Closure const>> &&a_rrMap);

	struct NotFoundException {
		string const m_strName;
		explicit NotFoundException(string const &a_rstrName);
	};

	Closure const operator [] (string const &rstrName) const;
	void Push(string const &rstrName, Closure const Closure);
	Closure const Pop(string const &rstrName);

};


struct AugmentEnvironment {
	Environment &m_rEnvironment;
	string const m_strName;

	AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, Closure const a_Closure);
	virtual ~AugmentEnvironment();
};
