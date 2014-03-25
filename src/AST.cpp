#include <PCH.h>
#include <AST.h>
#include <Error.h>


AbstractNode::~AbstractNode() {}


AbstractValue &AbstractNode::Evaluate() const {
	Environment Environment;
	return Evaluate(Environment);
}


LiteralNode::LiteralNode(AbstractValue &a_rValue)
	:
m_rValue(a_rValue) {}


LiteralNode::~LiteralNode() {}


LiteralNode *LiteralNode::Clone() const {
	return new LiteralNode(m_rValue);
}


AbstractValue &LiteralNode::Evaluate(Environment &rEnvironment) const {
	return m_rValue;
}


string const LiteralNode::ToString(AbstractEnvironment const &rEnvironment) const {
	return (string const)m_rValue;
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
		return *new Closure(vector<string>(m_Arguments), new FunctionNode(move(SubArguments), m_pBody->Clone()), rEnvironment.Capture());
	} else {
		return *new Closure(vector<string>(m_Arguments), m_pBody->Clone(), rEnvironment.Capture());
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


ApplicationNode::ApplicationNode(vector<Ptr<AbstractNode const>> &&a_rrTerms)
	:
m_Terms(move(a_rrTerms)) {
	assert(m_Terms.size() > 1);
}


ApplicationNode::~ApplicationNode() {}


ApplicationNode *ApplicationNode::Clone() const {
	vector<Ptr<AbstractNode const>> Terms;
	for (auto it = m_Terms.begin(); it != m_Terms.end(); ++it) {
		Terms.push_back((*it)->Clone());
	}
	return new ApplicationNode(move(Terms));
}


AbstractValue &ApplicationNode::Evaluate(Environment &rEnvironment) const {
	auto i = m_Terms.begin();
	assert(i != m_Terms.end());
	AbstractValue *pLeft = &((*i)->Evaluate(rEnvironment));
	unsigned int cTerms = m_Terms.size() - 1;
	while (true) {
		if (pLeft->m_Type != AbstractValue::TYPE_CLOSURE) {
			throw RuntimeError();
		} else {
			Closure &rClosure = *(Closure*)pLeft;
			map<string const, AbstractValue*> Arguments;
			for (auto j = rClosure.m_Arguments.begin(); j != rClosure.m_Arguments.end(); ++j, ++i) {
				Arguments[*j] = &((*i)->Evaluate(rEnvironment));
			}
			if (cTerms < rClosure.m_Arguments.size()) {
				vector<string> Names;
				for (auto it = Arguments.begin(); it != Arguments.end(); ++it) {
					Names.push_back(it->first);
				}
				AugmentEnvironment AugmentEnvironment(rClosure.m_Environment, Arguments);
				return *new Closure(move(Names), rClosure.m_pBody->Clone(), rClosure.m_Environment.Capture());
			} else {
				AugmentEnvironment AugmentEnvironment(rClosure.m_Environment, Arguments);
				pLeft = &(rClosure.m_pBody->Evaluate(rClosure.m_Environment));
				cTerms -= rClosure.m_Arguments.size();
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
