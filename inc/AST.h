#pragma once

#include <Environment.h>
#include <Closure.h>


struct AbstractNode :
	public Clonable
{
	virtual ~AbstractNode();
	virtual AbstractNode *Clone() const = 0;
	virtual Closure &Evaluate(Environment &rEnvironment) const = 0;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const = 0;

	Closure &Evaluate() const;
};


struct VariableNode :
	public AbstractNode
{
	string m_strName;

	explicit VariableNode(string const &a_rstrName);
	virtual ~VariableNode();

	virtual VariableNode *Clone() const;
	virtual Closure &Evaluate(Environment &rEnvironment) const;
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
	virtual Closure &Evaluate(Environment &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct ApplicationNode :
	public AbstractNode
{
	Ptr<AbstractNode const> m_pLeft;
	Ptr<AbstractNode const> m_pRight;

	ApplicationNode(Ptr<AbstractNode const> &&a_rrpLeft, Ptr<AbstractNode const> &&a_rrpRight);
	virtual ~ApplicationNode();

	virtual ApplicationNode *Clone() const;
	virtual Closure &Evaluate(Environment &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};
