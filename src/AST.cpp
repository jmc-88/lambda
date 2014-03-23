#include <PCH.h>
#include <AST.h>


AbstractNode::~AbstractNode() {}


Closure const &AbstractNode::Evaluate() const {
	Environment Environment;
	return Evaluate(Environment);
}


VariableNode::VariableNode(string const &a_rstrName)
	:
m_strName(a_rstrName) {}


VariableNode::~VariableNode() {}


VariableNode::operator string const () const {
	return m_strName;
}


VariableNode *VariableNode::Clone() const {
	return new VariableNode(m_strName);
}


Closure const &VariableNode::Evaluate(Environment &rEnvironment) const {
	return rEnvironment[m_strName];
}


FunctionNode::FunctionNode(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody)
	:
m_Arguments(move(a_rrArguments)),
	m_pBody(move(a_rrpBody))
{
	assert(m_Arguments.size() > 0);
}


FunctionNode::~FunctionNode() {}


FunctionNode::operator string const () const {
	assert(m_Arguments.size() > 0);
	auto it = m_Arguments.begin();
	string str = "lambda " + *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	return str += " . " + (string const)*m_pBody;
}


FunctionNode *FunctionNode::Clone() const {
	return new FunctionNode(vector<string>(m_Arguments), m_pBody->Clone());
}


Closure const &FunctionNode::Evaluate(Environment &rEnvironment) const {
	if (m_Arguments.size() > 1) {
		vector<string> SubArguments;
		for (auto it = ++(m_Arguments.begin()); it != m_Arguments.end(); ++it) {
			SubArguments.push_back(*it);
		}
		return *new Closure(*(m_Arguments.begin()), new FunctionNode(move(SubArguments), m_pBody->Clone()), rEnvironment);
	} else {
		return *new Closure(*(m_Arguments.begin()), m_pBody->Clone(), rEnvironment);
	}
}


ApplicationNode::ApplicationNode(Ptr<AbstractNode const> &&a_rrpLeft, Ptr<AbstractNode const> &&a_rrpRight)
	:
m_pLeft(move(a_rrpLeft)),
	m_pRight(move(a_rrpRight)) {}


ApplicationNode::~ApplicationNode() {}


ApplicationNode::operator string const () const {
	return (string const)*m_pLeft + " " + (string const)*m_pRight;
}


ApplicationNode *ApplicationNode::Clone() const {
	return new ApplicationNode(m_pLeft->Clone(), m_pRight->Clone());
}


Closure const &ApplicationNode::Evaluate(Environment &rEnvironment) const {
	Closure const &rLeftResult = m_pLeft->Evaluate(rEnvironment);
	Closure const &rRightResult = m_pRight->Evaluate(rEnvironment);
	AugmentEnvironment AugmentEnvironment(rEnvironment, rLeftResult.m_strArgument, rRightResult);
	return rLeftResult.m_pBody->Evaluate(rEnvironment);
}
