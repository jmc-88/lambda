#pragma once


struct AbstractValue;
struct BaseEnvironment;

struct AbstractEnvironment {
private:
	map<string const, stack<AbstractValue const*>> *const m_pMap;
	friend struct AugmentedEnvironment;

	AbstractEnvironment(AbstractEnvironment const&) = delete;
	AbstractEnvironment(AbstractEnvironment&&) = delete;

	void operator = (AbstractEnvironment const&) = delete;
	void operator = (AbstractEnvironment&&) = delete;

protected:
	explicit AbstractEnvironment(map<string const, stack<AbstractValue const*>> *const a_pMap);

public:
	virtual ~AbstractEnvironment();

	virtual bool Has(string const &rstrName) const;
	virtual AbstractValue const *operator [] (string const &rstrName) const;
	virtual BaseEnvironment Capture(set<string> const &rVariables) const;

};


struct BaseEnvironment :
	public AbstractEnvironment
{
private:
	map<string const, stack<AbstractValue const*>> m_Map;

public:
	explicit BaseEnvironment(map<string const, AbstractValue const*> const &a_rMap = map<string const, AbstractValue const*>());

	BaseEnvironment(BaseEnvironment const &rEnvironment);
	BaseEnvironment(BaseEnvironment &&rrEnvironment);

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

	AugmentedEnvironment(AugmentedEnvironment &&rrEnvironment);

	virtual ~AugmentedEnvironment();

};


struct OverrideEnvironment :
	public AbstractEnvironment
{
private:
	AbstractEnvironment const &m_rEnvironment;
	set<string> m_Names;

public:
	OverrideEnvironment(AbstractEnvironment const &a_rEnvironment, set<string> &&a_rrNames);
	virtual ~OverrideEnvironment();

	virtual bool Has(string const &rstrName) const;
	virtual AbstractValue const *operator [] (string const &rstrName) const;
	virtual BaseEnvironment Capture(set<string> const &rVariables) const;

};
