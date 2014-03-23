#include <PCH.h>
#include <AST.h>


AbstractNode::~AbstractNode() {}


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


FunctionNode::FunctionNode(string const &a_rstrArgumentName, Ptr<AbstractNode const> &&a_rrpBody)
	:
m_strArgumentName(a_rstrArgumentName),
	m_pBody(move(a_rrpBody)) {}


FunctionNode::~FunctionNode() {}


FunctionNode::operator string const () const {
	return "lambda " + m_strArgumentName + " . " + (string const)*m_pBody;
}


FunctionNode *FunctionNode::Clone() const {
	return new FunctionNode(m_strArgumentName, m_pBody->Clone());
}


Closure const &FunctionNode::Evaluate(Environment &rEnvironment) const {
	return *new Closure(m_strArgumentName, m_pBody->Clone(), rEnvironment);
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
	Ptr<Closure const> pLeftResult = &(m_pLeft->Evaluate(rEnvironment));
	Ptr<Closure const> pRightResult = &(m_pRight->Evaluate(rEnvironment));
	AugmentEnvironment AugmentEnvironment(rEnvironment, pLeftResult->m_strArgumentName, *pRightResult);
	return pLeftResult->m_pBody->Evaluate(rEnvironment);
}
