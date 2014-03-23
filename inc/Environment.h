#pragma once

#include <Closure.h>


struct Environment {
private:
	map<string, stack<Closure const*>> m_Map;

public:
	explicit Environment(map<string, Closure const*> &&a_rrMap);
	Environment();

	struct NotFoundException {
		string const m_strName;
		explicit NotFoundException(string const &a_rstrName);
	};

	Closure const &operator [] (string const &rstrName) const;
	void Push(string const &rstrName, Closure const &rClosure);
	Closure const &Pop(string const &rstrName);

	map<string, Closure const*> Capture() const;
};


struct AugmentEnvironment {
	Environment &m_rEnvironment;
	string const m_strName;

	AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, Closure const &a_rClosure);
	virtual ~AugmentEnvironment();
};
