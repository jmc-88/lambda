#include <PCH.h>
#include <Environment.h>
#include <Closure.h>
#include <Error.h>


AbstractEnvironment::NotFoundException::NotFoundException(string const &a_rstrName)
	:
m_strName(a_rstrName) {}


Environment::Environment(map<string, Closure*> &&a_rrMap) {
	for (auto it = a_rrMap.begin(); it != a_rrMap.end(); ++it) {
		m_Map[it->first].push(move(it->second));
	}
}

Environment::Environment() {}


Closure &Environment::operator [] (string const &rstrName) const {
	if (m_Map.count(rstrName)) {
		return *(m_Map.at(rstrName).top());
	} else {
		throw NotFoundException(rstrName);
	}
}


bool Environment::Has(string const &rstrName) const {
	return !!m_Map.count(rstrName);
}


void Environment::Push(string const &rstrName, Closure &rClosure) {
	m_Map[rstrName].push(&rClosure);
}


Closure &Environment::Pop(string const &rstrName) {
	if (m_Map.count(rstrName)) {
		stack<Closure*> &rStack = m_Map[rstrName];
		assert(rStack.size() > 0);
		Closure &rClosure = *(rStack.top());
		rStack.pop();
		if (rStack.empty()) {
			m_Map.erase(rstrName);
		}
		return rClosure;
	} else {
		throw InternalError();
	}
}


map<string, Closure*> Environment::Capture() const {
	map<string, Closure*> Result;
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it) {
		Result[it->first] = it->second.top();
	}
	return Result;
}


OverrideEnvironment::OverrideEnvironment(AbstractEnvironment const &a_rOriginalEnvironment, set<string> &&a_rrNames)
	:
m_rOriginalEnvironment(a_rOriginalEnvironment),
	m_Names(move(a_rrNames)) {}


Closure &OverrideEnvironment::operator [] (string const &rstrName) const {
	if (m_Names.count(rstrName)) {
		throw NotFoundException(rstrName);
	} else {
		return m_rOriginalEnvironment[rstrName];
	}
}


bool OverrideEnvironment::Has(string const &rstrName) const {
	return !m_Names.count(rstrName) && m_rOriginalEnvironment.Has(rstrName);
}


AugmentEnvironment::AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, Closure &a_rClosure)
	:
m_rEnvironment(a_rEnvironment),
	m_strName(a_rstrName)
{
	m_rEnvironment.Push(m_strName, a_rClosure);
}


AugmentEnvironment::~AugmentEnvironment() {
	m_rEnvironment.Pop(m_strName);
}
