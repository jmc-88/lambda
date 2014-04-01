#include <PCH.h>
#include <AST.h>
#include <Error.h>


AbstractNode::AbstractNode(Type const a_Type)
	:
m_Type(a_Type) {}


AbstractNode::~AbstractNode() {}


LiteralNode::LiteralNode(AbstractValue const *a_pValue)
	:
AbstractNode(TYPE_LITERAL),
	m_pValue(a_pValue) {}


LiteralNode::~LiteralNode() {}


LiteralNode *LiteralNode::Clone() const {
	return new LiteralNode(m_pValue);
}


set<string> LiteralNode::GetFreeVariables() const {
	return set<string>();
}


Ptr<AbstractNode const> LiteralNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	return new LiteralNode(m_pValue);
}


AbstractValue const *LiteralNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	return m_pValue;
}


string const LiteralNode::ToString(AbstractEnvironment const &rEnvironment) const {
	return (string const)*m_pValue;
}


VariableNode::VariableNode(string const &a_rstrName)
	:
AbstractNode(TYPE_VARIABLE),
	m_strName(a_rstrName) {}


VariableNode::~VariableNode() {}


VariableNode *VariableNode::Clone() const {
	return new VariableNode(m_strName);
}


set<string> VariableNode::GetFreeVariables() const {
	set<string> FreeVariables;
	FreeVariables.insert(m_strName);
	return FreeVariables;
}


Ptr<AbstractNode const> VariableNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	if (rContext.Has(m_strName)) {
		return rContext[m_strName]->Clone();
	} else {
		return new VariableNode(m_strName);
	}
}


AbstractValue const *VariableNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	return rEnvironment[m_strName];
}


string const VariableNode::ToString(AbstractEnvironment const &rEnvironment) const {
	if (rEnvironment.Has(m_strName)) {
		AbstractValue const &rValue = *(rEnvironment[m_strName]);
		if (rValue.m_Type != AbstractValue::TYPE_CLOSURE) {
			return rValue;
		} else {
			return "(" + (string const)rValue + ")";
		}
	} else {
		return m_strName;
	}
}


FunctionNode::FunctionNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody)
	:
AbstractNode(TYPE_FUNCTION),
	m_Arguments(move(a_rrArguments)),
	m_pBody(move(a_rrpBody))
{
	assert(m_Arguments.size() > 0);
}


FunctionNode::~FunctionNode() {}


FunctionNode *FunctionNode::Clone() const {
	return new FunctionNode(vector<string>(m_Arguments.begin(), m_Arguments.end()), m_pBody->Clone());
}


set<string> FunctionNode::GetFreeVariables() const {
	set<string> FreeVariables = m_pBody->GetFreeVariables();
	for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it) {
		FreeVariables.erase(*it);
	}
	return FreeVariables;
}


Ptr<AbstractNode const> FunctionNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	return new FunctionNode(vector<string>(m_Arguments.begin(), m_Arguments.end()), m_pBody->Preprocess(rContext));
}


AbstractValue const *FunctionNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	return new Closure(
		vector<string>(m_Arguments.begin(), m_Arguments.end()),
		m_pBody->Clone(),
		rEnvironment.Capture(GetFreeVariables())
		);
}


string const FunctionNode::ToString(AbstractEnvironment const &rEnvironment) const {
	string str = "lambda ";
	auto it = m_Arguments.begin();
	str += *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	return str += " . " + m_pBody->ToString(OverrideEnvironment(rEnvironment, set<string>(m_Arguments.begin(), m_Arguments.end())));
}


MacroNode::MacroNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody)
	:
AbstractNode(TYPE_MACRO),
	m_Arguments(move(a_rrArguments)),
	m_pBody(move(a_rrpBody))
{
	assert(m_Arguments.size() > 0);
}


MacroNode::~MacroNode() {}


MacroNode *MacroNode::Clone() const {
	return new MacroNode(vector<string>(m_Arguments.begin(), m_Arguments.end()), m_pBody->Clone());
}


set<string> MacroNode::GetFreeVariables() const {
	throw InternalError();
}


Ptr<AbstractNode const> MacroNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	return new MacroNode(vector<string>(m_Arguments.begin(), m_Arguments.end()), m_pBody->Preprocess(rContext));
}


AbstractValue const *MacroNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	throw PreprocessError();
}


string const MacroNode::ToString(AbstractEnvironment const &rEnvironment) const {
	string str = "macro ";
	auto it = m_Arguments.begin();
	str += *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	return str += " . " + m_pBody->ToString(BaseEnvironment());
}


ApplicationNode::ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms)
	:
AbstractNode(TYPE_APPLICATION),
	m_Terms(move(a_rrTerms))
{
	assert(m_Terms.size() > 1);
}


ApplicationNode::ApplicationNode(initializer_list<Ptr<AbstractNode const>> a_Terms)
	:
AbstractNode(TYPE_APPLICATION) {
	for (auto it = a_Terms.begin(); it != a_Terms.end(); ++it) {
		m_Terms.push_back((*it)->Clone()); // FIXME moving instead of cloning would be better
	}
}


ApplicationNode::~ApplicationNode() {}


ApplicationNode *ApplicationNode::Clone() const {
	vector<Ptr<AbstractNode const>> Terms;
	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it) {
		Terms.push_back((*it)->Clone());
	}
	return new ApplicationNode(move(Terms));
}


set<string> ApplicationNode::GetFreeVariables() const {
	set<string> Names;
	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it) {
		set<string> const FreeVariables = (*it)->GetFreeVariables();
		Names.insert(FreeVariables.begin(), FreeVariables.end());
	}
	return Names;
}


Ptr<AbstractNode const> ApplicationNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	vector<Ptr<AbstractNode const>> Terms;
	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it) {
		Terms.push_back((*it)->Preprocess(rContext));
	}
	// TODO expand macros
	return new ApplicationNode(move(Terms));
}


AbstractValue const *ApplicationNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	auto i = m_Terms.begin();
	AbstractValue const *pLeft = (*(i++))->Evaluate(rEnvironment);
	unsigned int cTerms = m_Terms.size() - 1;
	while (cTerms > 0) {
		if (pLeft->m_Type != AbstractValue::TYPE_CLOSURE) {
			throw RuntimeError();
		} else {
			Closure const *const pClosure = (Closure const*)pLeft;
			map<string const, AbstractValue const*> Arguments;
			if (cTerms < pClosure->m_Arguments.size()) {
				auto j = pClosure->m_Arguments.begin();
				while (cTerms-- > 0) {
					Arguments[*(j++)] = (*(i++))->Evaluate(rEnvironment);
				}
				vector<string> OtherArguments;
				set<string> FreeVariables = pClosure->m_pBody->GetFreeVariables();
				for (; j != pClosure->m_Arguments.end(); ++j) {
					OtherArguments.push_back(*j);
					FreeVariables.erase(*j);
				}
				return new Closure(
					move(OtherArguments),
					pClosure->m_pBody->Clone(),
					AugmentedEnvironment(pClosure->m_Environment, move(Arguments)).Capture(FreeVariables)
					);
			} else {
				for (auto j = pClosure->m_Arguments.begin(); j != pClosure->m_Arguments.end(); ++j, --cTerms) {
					Arguments[*j] = (*(i++))->Evaluate(rEnvironment);
				}
				pLeft = pClosure->m_pBody->Evaluate(AugmentedEnvironment(pClosure->m_Environment, move(Arguments)));
			}
		}
	}
	return pLeft;
}


string const ApplicationNode::ToString(AbstractEnvironment const &rEnvironment) const {
	string str;
	unsigned int cTerms = m_Terms.size();
	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it, --cTerms) {
		Type const Type = (*it)->m_Type;
		if (cTerms > 1) {
			if (Type == TYPE_FUNCTION || Type == TYPE_MACRO || Type == TYPE_APPLICATION) {
				str += "(" + (*it)->ToString(rEnvironment) + ") ";
			} else {
				str += (*it)->ToString(rEnvironment) + " ";
			}
		} else if (Type == TYPE_APPLICATION) {
			str += "(" + (*it)->ToString(rEnvironment) + ")";
		} else {
			str += (*it)->ToString(rEnvironment);
		}
	}
	return str;
}


LetNode::LetNode(string const &a_rstrName, Ptr<AbstractNode const> &&a_rrpExpression, Ptr<AbstractNode const> &&a_rrpRest)
	:
AbstractNode(TYPE_LET),
	m_strName(a_rstrName),
	m_pExpression(move(a_rrpExpression)),
	m_pRest(move(a_rrpRest)) {}


LetNode::~LetNode() {}


LetNode *LetNode::Clone() const {
	return new LetNode(m_strName, m_pExpression->Clone(), m_pRest->Clone());
}


set<string> LetNode::GetFreeVariables() const {
	set<string> Set = m_pExpression->GetFreeVariables();
	set<string> OtherSet = m_pRest->GetFreeVariables();
	Set.insert(OtherSet.begin(), OtherSet.end());
	return Set;
}


Ptr<AbstractNode const> LetNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	return new LetNode(m_strName, m_pExpression->Preprocess(rContext), m_pRest->Preprocess(rContext));
}


AbstractValue const *LetNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	map<string const, AbstractValue const*> Map;
	Map[m_strName] = m_pExpression->Evaluate(rEnvironment);
	return m_pRest->Evaluate(AugmentedEnvironment(rEnvironment, Map));
}


string const LetNode::ToString(AbstractEnvironment const &rEnvironment) const {
	return "let " + m_strName + " = " + m_pExpression->ToString(rEnvironment) + " in " + m_pRest->ToString(rEnvironment);
}


NativeNode::NativeNode()
	:
AbstractNode(TYPE_NATIVE) {}


NativeNode::~NativeNode() {}


set<string> NativeNode::GetFreeVariables() const {
	return set<string>();
}


Ptr<AbstractNode const> NativeNode::Preprocess(AbstractPreprocessContext const &rContext) const {
	return Clone();
}


string const NativeNode::ToString(AbstractEnvironment const &rEnvironment) const {
	return "{ native code }";
}
