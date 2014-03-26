#pragma once


struct AbstractValue;

struct Environment {
private:
	map<string const, stack<AbstractValue const*>> m_Map;
	map<string const, stack<AbstractValue const*>> *const m_pMap;

public:
	explicit Environment(map<string const, AbstractValue const*> const &a_rMap = map<string const, AbstractValue const*>());
	virtual ~Environment();

	bool Has(string const &rstrName) const;
	AbstractValue *operator [] (string const &rstrName) const;
	void Push(string const &rstrName, AbstractValue const *const) const;
	AbstractValue *Pop(string const &rstrName) const;

	Environment Capture() const;
};


struct AugmentedEnvironment :
	public Environment
{
	string const m_strName;

	AugmentedEnvironment(string const &a_rstrName, AbstractValue const *const a_pValue);
	virtual ~AugmentedEnvironment();

};
