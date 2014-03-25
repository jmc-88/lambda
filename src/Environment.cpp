#include <PCH.h>
#include <Environment.h>
#include <Values.h>
#include <Error.h>


Environment::Environment(map<string, AbstractValue*> &&a_rrMap) {
	for (auto it = a_rrMap.begin(); it != a_rrMap.end(); ++it) {
		m_Map[it->first].push(move(it->second));
	}
}

Environment::Environment() {}


AbstractValue &Environment::operator [] (string const &rstrName) const {
	if (m_Map.count(rstrName)) {
		return *(m_Map.at(rstrName).top());
	} else {
		throw RuntimeError();
	}
}


bool Environment::Has(string const &rstrName) const {
	return !!m_Map.count(rstrName);
}


void Environment::Push(string const &rstrName, AbstractValue &rValue) {
	m_Map[rstrName].push(&rValue);
}


AbstractValue &Environment::Pop(string const &rstrName) {
	if (m_Map.count(rstrName)) {
		stack<AbstractValue*> &rStack = m_Map[rstrName];
		assert(rStack.size() > 0);
		AbstractValue &rValue = *(rStack.top());
		rStack.pop();
		if (rStack.empty()) {
			m_Map.erase(rstrName);
		}
		return rValue;
	} else {
		throw InternalError();
	}
}


map<string, AbstractValue*> Environment::Capture() const {
	map<string, AbstractValue*> Result;
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it) {
		Result[it->first] = it->second.top();
	}
	return Result;
}


OverrideEnvironment::OverrideEnvironment(AbstractEnvironment const &a_rOriginalEnvironment, set<string> &&a_rrNames)
	:
m_rOriginalEnvironment(a_rOriginalEnvironment),
	m_Names(move(a_rrNames)) {}


AbstractValue &OverrideEnvironment::operator [] (string const &rstrName) const {
	if (m_Names.count(rstrName)) {
		throw RuntimeError();
	} else {
		return m_rOriginalEnvironment[rstrName];
	}
}


bool OverrideEnvironment::Has(string const &rstrName) const {
	return !m_Names.count(rstrName) && m_rOriginalEnvironment.Has(rstrName);
}


AugmentEnvironment::AugmentEnvironment(Environment &a_rEnvironment, map<string const, AbstractValue*> const &a_rValues)
	:
m_rEnvironment(a_rEnvironment) {
	for (auto it = a_rValues.begin(); it != a_rValues.end(); ++it) {
		m_Names.push_back(it->first);
		m_rEnvironment.Push(it->first, *(it->second));
	}
}


AugmentEnvironment::~AugmentEnvironment() {
	for (auto it = m_Names; it != m_Names.end(); ++it) {
		m_rEnvironment.Pop(*it);
	}
}
