#include <PCH.h>
#include <AST.h>
#include <Error.h>


AbstractNode::~AbstractNode() {}


AbstractValue &AbstractNode::Evaluate() const {
	Environment Environment;
	return Evaluate(Environment);
}


VariableNode::VariableNode(string const &a_rstrName)
	:
m_strName(a_rstrName) {}


VariableNode::~VariableNode() {}


VariableNode *VariableNode::Clone() const {
	return new VariableNode(m_strName);
}


AbstractValue &VariableNode::Evaluate(Environment &rEnvironment) const {
	return rEnvironment[m_strName];
}


string const VariableNode::ToString(AbstractEnvironment const &rEnvironment) const {
	if (rEnvironment.Has(m_strName)) {
		return (string const)(rEnvironment[m_strName]);
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
	return new FunctionNode(vector<string>(m_Arguments), m_pBody->Clone());
}


AbstractValue &FunctionNode::Evaluate(Environment &rEnvironment) const {
	if (m_Arguments.size() > 1) {
		vector<string> SubArguments;
		for (auto it = ++(m_Arguments.begin()); it != m_Arguments.end(); ++it) {
			SubArguments.push_back(*it);
		}
		return *new Closure(*(m_Arguments.begin()), new FunctionNode(move(SubArguments), m_pBody->Clone()), rEnvironment.Capture());
	} else {
		return *new Closure(*(m_Arguments.begin()), m_pBody->Clone(), rEnvironment.Capture());
	}
}


string const FunctionNode::ToString(AbstractEnvironment const &rEnvironment) const {
	assert(m_Arguments.size() > 0);
	auto it = m_Arguments.begin();
	string str = "lambda " + *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	set<string> Names;
	for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it) {
		Names.insert(*it);
	}
	return str += " . " + m_pBody->ToString(OverrideEnvironment(rEnvironment, move(Names)));
}


ApplicationNode::ApplicationNode(Ptr<AbstractNode const> &&a_rrpLeft, Ptr<AbstractNode const> &&a_rrpRight)
	:
m_pLeft(move(a_rrpLeft)),
	m_pRight(move(a_rrpRight)) {}


ApplicationNode::~ApplicationNode() {}


ApplicationNode *ApplicationNode::Clone() const {
	return new ApplicationNode(m_pLeft->Clone(), m_pRight->Clone());
}


AbstractValue &ApplicationNode::Evaluate(Environment &rEnvironment) const {
	Ptr<AbstractValue> pLeftResult = &(m_pLeft->Evaluate(rEnvironment));
	if (pLeftResult->m_Type != AbstractValue::TYPE_CLOSURE) {
		throw RuntimeError();
	} else {
		Ptr<Closure> pClosure = (Closure*)(pLeftResult.Detach());
		AbstractValue &rRightResult = m_pRight->Evaluate(rEnvironment);
		AugmentEnvironment AugmentEnvironment(pClosure->m_Environment, pClosure->m_strArgument, rRightResult);
		return pClosure->m_pBody->Evaluate(pClosure->m_Environment);
	}
}


string const ApplicationNode::ToString(AbstractEnvironment const &rEnvironment) const {
	return "(" + m_pLeft->ToString(rEnvironment) + ")(" + m_pRight->ToString(rEnvironment) + ")";
}
