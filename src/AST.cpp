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


AbstractValue const *LiteralNode::Evaluate(Environment const &rEnvironment) const {
	return m_pValue;
}
