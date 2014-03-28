#include <PCH.h>
#include <Environment.h>
#include <Values.h>
#include <Error.h>


AbstractEnvironment::AbstractEnvironment(map<string const, stack<AbstractValue const*>> *const a_pMap)
	:
m_pMap(a_pMap) {}


AbstractEnvironment::~AbstractEnvironment() {}


bool AbstractEnvironment::Has(string const &rstrName) const {
	return !!m_pMap->count(rstrName);
}


AbstractValue const *AbstractEnvironment::operator [] (string const &rstrName) const {
	if (m_pMap->count(rstrName)) {
		return (*m_pMap)[rstrName].top();
	} else {
		throw RuntimeError();
	}
}


BaseEnvironment AbstractEnvironment::Capture(set<string> const &rVariables) const {
	map<string const, AbstractValue const*> Map;
	for (auto it = rVariables.begin(); it != rVariables.end(); ++it) {
		if (m_pMap->count(*it)) {
			Map[*it] = (*m_pMap)[*it].top();
		} else {
			throw RuntimeError();
		}
	}
	return BaseEnvironment(Map);
}


BaseEnvironment::BaseEnvironment(map<string const, AbstractValue const*> const &a_rMap)
	:
AbstractEnvironment(&m_Map) {
	for (auto it = a_rMap.begin(); it != a_rMap.end(); ++it) {
		m_Map[it->first].push(it->second);
	}
}


BaseEnvironment::BaseEnvironment(BaseEnvironment const &rEnvironment)
	:
AbstractEnvironment(&m_Map),
	m_Map(rEnvironment.m_Map) {}


BaseEnvironment::BaseEnvironment(BaseEnvironment &&rrEnvironment)
	:
AbstractEnvironment(&m_Map),
	m_Map(move(rrEnvironment.m_Map)) {}


BaseEnvironment::~BaseEnvironment() {}


AugmentedEnvironment::AugmentedEnvironment(AbstractEnvironment const &a_rEnvironment, map<string const, AbstractValue const*> const &a_rValues)
	:
AbstractEnvironment(a_rEnvironment.m_pMap),
	m_rEnvironment(a_rEnvironment)
{
	for (auto it = a_rValues.begin(); it != a_rValues.end(); ++it) {
		(*m_pMap)[it->first].push(it->second);
		m_Names.insert(it->first);
	}
}


AugmentedEnvironment::AugmentedEnvironment(AugmentedEnvironment &&rrEnvironment)
	:
AbstractEnvironment(rrEnvironment.m_pMap),
	m_rEnvironment(rrEnvironment.m_rEnvironment),
	m_Names(move(rrEnvironment.m_Names)) {}


AugmentedEnvironment::~AugmentedEnvironment() {
	for (auto it = m_Names.begin(); it != m_Names.end(); ++it) {
		stack<AbstractValue const*> &rStack = (*m_pMap)[*it];
		rStack.pop();
		if (rStack.empty()) {
			m_pMap->erase(*it);
		}
	}
}


OverrideEnvironment::OverrideEnvironment(AbstractEnvironment const &a_rEnvironment, set<string> &&a_rrNames)
	:
AbstractEnvironment(nullptr),
	m_rEnvironment(a_rEnvironment),
	m_Names(move(a_rrNames)) {}


OverrideEnvironment::~OverrideEnvironment() {}


bool OverrideEnvironment::Has(string const &rstrName) const {
	return !m_Names.count(rstrName) && m_rEnvironment.Has(rstrName);
}


AbstractValue const *OverrideEnvironment::operator [] (string const &rstrName) const {
	if (m_Names.count(rstrName)) {
		throw RuntimeError();
	} else {
		return m_rEnvironment[rstrName];
	}
}


BaseEnvironment OverrideEnvironment::Capture(set<string> const &rVariables) const {
	assert(false);
	throw InternalError();
}
