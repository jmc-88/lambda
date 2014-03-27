#include <PCH.h>
#include <AST.h>


AbstractNode::~AbstractNode() {}


LiteralNode::LiteralNode(AbstractValue const *a_pValue)
	:
m_pValue(a_pValue) {}


LiteralNode::~LiteralNode() {}


LiteralNode *LiteralNode::Clone() const {
	return new LiteralNode(m_pValue);
}


set<string> LiteralNode::GetFreeVariables() const {
	return set<string>();
}


AbstractValue const *LiteralNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	return m_pValue;
}


string const LiteralNode::ToString(AbstractEnvironment const &rEnvironment) const {
	return (string const)*m_pValue;
}


VariableNode::VariableNode(string const &a_rstrName)
	:
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


AbstractValue const *VariableNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	return rEnvironment[m_strName];
}


string const VariableNode::ToString(AbstractEnvironment const &rEnvironment) const {
	if (rEnvironment.Has(m_strName)) {
		return (string const)*(rEnvironment[m_strName]);
	} else {
		return m_strName;
	}
}


FunctionNode::FunctionNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody)
	:
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


AbstractValue const *FunctionNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	// FIXME free variable set must be cached
	return new Closure(vector<string>(m_Arguments.begin(), m_Arguments.end()), m_pBody->Clone(), rEnvironment.Capture(GetFreeVariables()));
}


string const FunctionNode::ToString(AbstractEnvironment const &rEnvironment) const {
	string str = "lambda ";
	auto it = m_Arguments.begin();
	str += *it;
	for (; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	return str += " . " + m_pBody->ToString(OverrideEnvironment(rEnvironment, set<string>(m_Arguments.begin(), m_Arguments.end())));
}
