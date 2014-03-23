#include <PCH.h>
#include <Environment.h>
#include <AST.h>
#include <Error.h>


Environment::Environment(map<string, Closure const*> &&a_rrMap) {
	for (auto it = a_rrMap.begin(); it != a_rrMap.end(); ++it) {
		m_Map[it->first].push(move(it->second));
	}
}


Environment::NotFoundException::NotFoundException(string const &a_rstrName)
	:
m_strName(a_rstrName) {}


Closure const &Environment::operator [] (string const &rstrName) const {
	if (m_Map.count(rstrName)) {
		return *(m_Map.at(rstrName).top());
	} else {
		throw NotFoundException(rstrName);
	}
}


void Environment::Push(string const &rstrName, Closure const &rClosure) {
	m_Map[rstrName].push(&rClosure);
}


Closure const &Environment::Pop(string const &rstrName) {
	if (m_Map.count(rstrName)) {
		stack<Closure const*> &rStack = m_Map[rstrName];
		assert(rStack.size() > 0);
		Closure const &rClosure = *(rStack.top());
		rStack.pop();
		return rClosure;
	} else {
		throw InternalError();
	}
}


map<string, Closure const*> Environment::Capture() const {
	map<string, Closure const*> Result;
	for (auto it = m_Map.begin(); it != m_Map.end(); ++it) {
		Result[it->first] = it->second.top();
	}
	return Result;
}


AugmentEnvironment::AugmentEnvironment(Environment &a_rEnvironment, string const &a_rstrName, Closure const &a_rClosure)
	:
m_rEnvironment(a_rEnvironment),
	m_strName(a_rstrName)
{
	m_rEnvironment.Push(m_strName, a_rClosure);
}


AugmentEnvironment::~AugmentEnvironment() {
	m_rEnvironment.Pop(m_strName);
}
