#include <PCH.h>
#include <AST.h>


AbstractNode::~AbstractNode() {}


VariableNode::VariableNode(string const &a_rstrName)
	:
m_strName(a_rstrName) {}


VariableNode::~VariableNode() {}


VariableNode *VariableNode::Clone() const {
	return new VariableNode(m_strName);
}


Ptr<AbstractNode const> VariableNode::Evaluate(Environment &rEnvironment) const {
	// TODO
}
