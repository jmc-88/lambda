#pragma once


struct AbstractValue;
struct BaseEnvironment;

struct AbstractEnvironment {
private:
	map<string const, stack<AbstractValue const*>> *const m_pMap;
	friend struct AugmentedEnvironment;

protected:
	explicit AbstractEnvironment(map<string const, stack<AbstractValue const*>> *const a_pMap);

public:
	virtual ~AbstractEnvironment();

	bool Has(string const &rstrName) const;
	AbstractValue const *operator [] (string const &rstrName) const;
	BaseEnvironment Capture(set<string> const &rVariables) const;

};


struct BaseEnvironment :
	public AbstractEnvironment
{
private:
	map<string const, stack<AbstractValue const*>> m_Map;

public:
	explicit BaseEnvironment(map<string const, AbstractValue const*> const &a_rMap = map<string const, AbstractValue const*>());
	virtual ~BaseEnvironment();

};


struct AugmentedEnvironment :
	public AbstractEnvironment
{
private:
	AbstractEnvironment const &m_rEnvironment;
	set<string> m_Names;

public:
	AugmentedEnvironment(AbstractEnvironment const &a_rEnvironment, map<string const, AbstractValue const*> const &a_rValues);
	virtual ~AugmentedEnvironment();

};
