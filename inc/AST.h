#pragma once

#include <Environment.h>
#include <Values.h>


struct AbstractNode :
	public Clonable
{
	virtual ~AbstractNode();
	virtual AbstractNode *Clone() const = 0;
	virtual AbstractValue &Evaluate(Environment &rEnvironment) const = 0;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const = 0;

	AbstractValue &Evaluate() const;
};


struct LiteralNode :
	public AbstractNode
{
	AbstractValue &m_rValue;

	explicit LiteralNode(AbstractValue &a_rValue);
	virtual ~LiteralNode();

	virtual LiteralNode *Clone() const;
	virtual AbstractValue &Evaluate(Environment &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct VariableNode :
	public AbstractNode
{
	string m_strName;

	explicit VariableNode(string const &a_rstrName);
	virtual ~VariableNode();

	virtual VariableNode *Clone() const;
	virtual AbstractValue &Evaluate(Environment &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct FunctionNode :
	public AbstractNode
{
	vector<string> m_Arguments;
	Ptr<AbstractNode const> m_pBody;

	FunctionNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody);
	virtual ~FunctionNode();

	virtual FunctionNode *Clone() const;
	virtual AbstractValue &Evaluate(Environment &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct ApplicationNode :
	public AbstractNode
{
	vector<Ptr<AbstractNode const>> m_Terms;

	ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms);
	virtual ~ApplicationNode();

	virtual ApplicationNode *Clone() const;
	virtual AbstractValue &Evaluate(Environment &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};
