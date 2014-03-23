#pragma once

#include <Environment.h>
#include <Closure.h>


struct AbstractNode :
	public Clonable
{
	virtual ~AbstractNode();
	virtual AbstractNode *Clone() const = 0;
	virtual Ptr<AbstractNode const> Evaluate(Environment &rEnvironment) const = 0;
};


struct VariableNode :
	public AbstractNode
{
	string m_strName;

	explicit VariableNode(string const &a_rstrName);
	virtual ~VariableNode();

	virtual VariableNode *Clone() const;
	virtual Ptr<AbstractNode const> Evaluate(Environment &rEnvironment) const;
};


struct FunctionNode :
	public AbstractNode
{
	string m_strArgumentName;
	Ptr<AbstractNode const> m_pBody;

	FunctionNode(string const &a_rstrArgumentName, Ptr<AbstractNode const> &&a_rrpBody);
	virtual ~FunctionNode();

	virtual FunctionNode *Clone() const;
	virtual Ptr<AbstractNode const> Evaluate(Environment &rEnvironment) const;
};


struct ApplicationNode :
	public AbstractNode
{
	Ptr<AbstractNode const> m_pLeft;
	Ptr<AbstractNode const> m_pRight;

	ApplicationNode(Ptr<AbstractNode const> &&a_rrpLeft, Ptr<AbstractNode const> &&a_rrpRight);
	virtual ~ApplicationNode();

	virtual ApplicationNode *Clone() const;
	virtual Ptr<AbstractNode const> Evaluate(Environment &rEnvironment) const;
};
