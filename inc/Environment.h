#pragma once


struct Closure;

struct AbstractEnvironment {
	struct NotFoundException {
		string const m_strName;
		explicit NotFoundException(string const &a_rstrName);
	};

	virtual Closure &operator [] (string const &rstrName) const = 0;
	virtual bool Has(string const &rstrName) const = 0;
};


struct Environment :
	public AbstractEnvironment
{
private:
	map<string, stack<Closure*>> m_Map;

public:
	explicit Environment(map<string, Closure*> &&a_rrMap);
	Environment();

	virtual Closure &operator [] (string const &rstrName) const;
	virtual bool Has(string const &rstrName) const;
	void Push(string const &rstrName, Closure &rClosure);
	Closure &Pop(string const &rstrName);

	map<string, Closure*> Capture() const;
};


struct OverrideEnvironment :
	public AbstractEnvironment
{
	AbstractEnvironment const &m_rOriginalEnvironment;
	set<string> m_Names;

	OverrideEnvironment(AbstractEnvironment const &a_rOriginalEnvironment, set<string> &&a_rrNames);

	virtual Closure &operator [] (string const &rstrName) const;
	virtual bool Has(string const &rstrName) const;
};


struct AugmentEnvironment {
	Environment &m_rEnvironment;
	string const m_strName;

	AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, Closure &a_rClosure);
	virtual ~AugmentEnvironment();
};
