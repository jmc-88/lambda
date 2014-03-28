#include <PCH.h>
#include <AST.h>
#include <Error.h>


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

	m_FreeVariables = m_pBody->GetFreeVariables();
	for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it) {
		m_FreeVariables.erase(*it);
	}
}


FunctionNode::~FunctionNode() {}


FunctionNode *FunctionNode::Clone() const {
	return new FunctionNode(vector<string>(m_Arguments.begin(), m_Arguments.end()), m_pBody->Clone());
}


set<string> FunctionNode::GetFreeVariables() const {
	return m_FreeVariables;
}


AbstractValue const *FunctionNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	return new Closure(
		vector<string>(m_Arguments.begin(), m_Arguments.end()),
		m_pBody->Clone(),
		rEnvironment.Capture(set<string>(m_FreeVariables.begin(), m_FreeVariables.end()))
		);
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


ApplicationNode::ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms)
	:
m_Terms(move(a_rrTerms)) {
	assert(m_Terms.size() > 1);

	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it) {
		set<string> const FreeVariables = (*it)->GetFreeVariables();
		m_FreeVariables.insert(FreeVariables.begin(), FreeVariables.end());
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
	return m_FreeVariables;
}


AbstractValue const *ApplicationNode::Evaluate(AbstractEnvironment const &rEnvironment) const {
	auto i = m_Terms.begin();
	AbstractValue const *pLeft = (*(i++))->Evaluate(rEnvironment);
	unsigned int cTerms = m_Terms.size() - 1;
	while (true) {
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
				for (; j != pClosure->m_Arguments.end(); ++j) {
					OtherArguments.push_back(*j);
				}
				return new Closure(
					move(OtherArguments),
					pClosure->m_pBody->Clone(),
					AugmentedEnvironment(pClosure->m_Environment, move(Arguments)).Capture(pClosure->m_pBody->GetFreeVariables())
					);
			} else {
				for (auto j = pClosure->m_Arguments.begin(); j != pClosure->m_Arguments.end(); ++j, --cTerms) {
					Arguments[*j] = (*(i++))->Evaluate(rEnvironment);
				}
				pLeft = pClosure->m_pBody->Evaluate(AugmentedEnvironment(pClosure->m_Environment, move(Arguments)));
			}
		}
	}
}


string const ApplicationNode::ToString(AbstractEnvironment const &rEnvironment) const {
	string str;
	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it) {
		str += "(" + (*it)->ToString(rEnvironment) + ")";
	}
	return str;
}
