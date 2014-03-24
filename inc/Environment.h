#pragma once


struct AbstractValue;

struct AbstractEnvironment {
	virtual AbstractValue &operator [] (string const &rstrName) const = 0;
	virtual bool Has(string const &rstrName) const = 0;
};


struct Environment :
	public AbstractEnvironment
{
private:
	map<string, stack<AbstractValue*>> m_Map;

public:
	explicit Environment(map<string, AbstractValue*> &&a_rrMap);
	Environment();

	virtual AbstractValue &operator [] (string const &rstrName) const;
	virtual bool Has(string const &rstrName) const;
	void Push(string const &rstrName, AbstractValue &rValue);
	AbstractValue &Pop(string const &rstrName);

	map<string, AbstractValue*> Capture() const;
};


struct OverrideEnvironment :
	public AbstractEnvironment
{
	AbstractEnvironment const &m_rOriginalEnvironment;
	set<string> m_Names;

	OverrideEnvironment(AbstractEnvironment const &a_rOriginalEnvironment, set<string> &&a_rrNames);

	virtual AbstractValue &operator [] (string const &rstrName) const;
	virtual bool Has(string const &rstrName) const;
};


struct AugmentEnvironment {
	Environment &m_rEnvironment;
	string const m_strName;

	AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, AbstractValue &a_rValue);
	virtual ~AugmentEnvironment();
};
