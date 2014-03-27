#pragma once

#include <Environment.h>
#include <Values.h>


struct AbstractNode :
	public Clonable
{
	virtual ~AbstractNode();
	virtual AbstractNode *Clone() const = 0;
	virtual set<string> GetFreeVariables() const = 0;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const = 0;
};


struct LiteralNode :
	public AbstractNode
{
	AbstractValue const *m_pValue;

	explicit LiteralNode(AbstractValue const *a_pValue);
	virtual ~LiteralNode();

	virtual LiteralNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
};


struct VariableNode :
	public AbstractNode
{
	string m_strName;

	explicit VariableNode(string const &a_rstrName);
	virtual ~VariableNode();

	virtual VariableNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
};


struct FunctionNode :
	public AbstractNode
{
	vector<string> m_Arguments;
	Ptr<AbstractNode const> m_pBody;

	FunctionNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody);
	virtual ~FunctionNode();

	virtual FunctionNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
};


struct ApplicationNode :
	public AbstractNode
{
	vector<Ptr<AbstractNode const>> m_Terms;

	explicit ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms);
	virtual ~ApplicationNode();

	virtual ApplicationNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
};
