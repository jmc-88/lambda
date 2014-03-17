#pragma once


struct AbstractNode :
	public Clonable
{
	virtual AbstractNode Clone() const = 0;
	virtual Ptr<AbstractNode> Reduce() const = 0;
	virtual Ptr<AbstractNode> ReduceOrClone() const = 0;

	// TODO Evaluate
};


struct VariableNode :
	public AbstractNode
{
	string m_strName;
};


struct FunctionNode :
	public AbstractNode
{
	string m_strArgumentName;
	Ptr<AbstractNode const> m_pBody;
};


struct ApplicationNode :
	public AbstractNode
{
	// TODO
};
