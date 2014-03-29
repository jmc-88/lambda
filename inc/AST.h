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
		TYPE_EXPANSION
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
	AbstractValue const *const m_pValue;

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
	string const m_strName;

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
private:
	set<string> const m_FreeVariables;

	static set<string> ExtractFreeVariables(vector<string> const &rArguments, Ptr<AbstractNode const> const &rpBody);

public:
	vector<string> const m_Arguments;
	Ptr<AbstractNode const> const m_pBody;

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
private:
	set<string> const m_FreeVariables;

	static set<string> ExtractFreeVariables(vector<string> const &rArguments, Ptr<AbstractNode const> const &rpBody);

public:
	vector<string> const m_Arguments;
	Ptr<AbstractNode const> const m_pBody;

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
private:
	set<string> const m_FreeVariables;

	static set<string> ExtractFreeVariables(vector<Ptr<AbstractNode const>> const &rTerms);

public:
	vector<Ptr<AbstractNode const>> const m_Terms;

	explicit ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms);
	virtual ~ApplicationNode();

	virtual ApplicationNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;

};


struct ExpansionNode :
	public AbstractNode
{
	vector<Ptr<AbstractNode const>> const m_Terms;

	explicit ExpansionNode(vector<Ptr<AbstractNode const>> &&a_rrTerms);
	virtual ~ExpansionNode();

	virtual ExpansionNode *Clone() const;
	virtual set<string> GetFreeVariables() const;
	virtual Ptr<AbstractNode const> Preprocess(AbstractPreprocessContext const &rContext) const;
	virtual AbstractValue const *Evaluate(AbstractEnvironment const &rEnvironment) const;
	virtual string const ToString(AbstractEnvironment const &rEnvironment) const;
};
