#pragma once


struct Closure;

struct Environment {
private:
	map<string, stack<Closure*>> m_Map;

public:
	explicit Environment(map<string, Closure*> &&a_rrMap);
	Environment();

	struct NotFoundException {
		string const m_strName;
		explicit NotFoundException(string const &a_rstrName);
	};

	Closure &operator [] (string const &rstrName) const;
	void Push(string const &rstrName, Closure &rClosure);
	Closure &Pop(string const &rstrName);

	map<string, Closure*> Capture() const;
};


struct AugmentEnvironment {
	Environment &m_rEnvironment;
	string const m_strName;

	AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, Closure &a_rClosure);
	virtual ~AugmentEnvironment();
};
