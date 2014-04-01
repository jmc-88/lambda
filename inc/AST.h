#pragma once

#include <Preprocess.h>
#include <Environment.h>
#include <Values.h>


struct AbstractNode :
	public Clonable
{
	enum Type {
		TYPE_LITERAL,
		TYPE_VARIABLE,
		TYPE_FUNCTION,
		TYPE_MACRO,
		TYPE_APPLICATION,
		TYPE_LET,
		TYPE_NATIVE
	};

	Type const m_Type;

	explicit AbstractNode(Type const a_Type);
	virtual ~AbstractNode();

	virtual AbstractNode *Clone() const = 0;
	virtual set<string> GetFreeVariables() const = 0;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const = 0;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment = BaseEnvironment()) const = 0;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const = 0;
};


struct LiteralNode :
	public AbstractNode
{
	AbstractValue const *m_pValue;

	explicit LiteralNode(AbstractValue const *a_pValue);
	virtual ~LiteralNode();

	virtual LiteralNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct VariableNode :
	public AbstractNode
{
	string m_strName;

	explicit VariableNode(string const &a_rstrName);
	virtual ~VariableNode();

	virtual VariableNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
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
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct MacroNode :
	public AbstractNode
{
	vector<string> m_Arguments;
	Ptr<AbstractNode const> m_pBody;

	MacroNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody);
	virtual ~MacroNode();

	virtual MacroNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct ApplicationNode :
	public AbstractNode
{
	vector<Ptr<AbstractNode const>> m_Terms;

	explicit ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms);
	explicit ApplicationNode(initializer_list<Ptr<AbstractNode const>> a_Terms);
	virtual ~ApplicationNode();

	virtual ApplicationNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct LetNode :
	public AbstractNode
{
	string m_strName;
	Ptr<AbstractNode const> m_pExpression;
	Ptr<AbstractNode const> m_pRest;

	LetNode(string const &a_rstrName, Ptr<AbstractNode const> &&a_rrpExpression, Ptr<AbstractNode const> &&a_rrpRest);
	virtual ~LetNode();

	virtual LetNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};


struct NativeNode :
	public AbstractNode
{
	NativeNode();
	virtual ~NativeNode();
	virtual NativeNode *Clone() const = 0;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const = 0;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};
