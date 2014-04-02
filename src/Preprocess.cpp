#include <PCH.h>
#include <Preprocess.h>
#include <AST.h>
#include <Error.h>


AbstractPreprocessContext::AbstractPreprocessContext(map<string const, stack<Ptr<AbstractNode const>>> *const a_pMap)
	:
m_pMap(a_pMap) {}


AbstractPreprocessContext::~AbstractPreprocessContext() {}


bool AbstractPreprocessContext::Has(string const &rstrName) const {
	return !!m_pMap->count(rstrName);
}


Ptr<AbstractNode const> const &AbstractPreprocessContext::operator [] (string const &rstrName) const {
	if (m_pMap->count(rstrName)) {
		stack<Ptr<AbstractNode const>> const &rStack = (*m_pMap)[rstrName];
		assert(!rStack.empty());
		return rStack.top();
	} else {
		throw PreprocessError();
	}
}


BasePreprocessContext::BasePreprocessContext(map<string const, Ptr<AbstractNode const>> &&a_rrMap)
	:
AbstractPreprocessContext(&m_Map) {
	for (auto it = a_rrMap.begin(); it != a_rrMap.end(); ++it) {
		m_Map[it->first].push(it->second.Detach());
	}
}


BasePreprocessContext::~BasePreprocessContext() {}


AugmentedPreprocessContext::AugmentedPreprocessContext(AbstractPreprocessContext const &a_rContext, map<string const, Ptr<AbstractNode const>> &&a_rrMap)
	:
AbstractPreprocessContext(a_rContext.m_pMap),
	m_rContext(a_rContext)
{
	for (auto it = a_rrMap.begin(); it != a_rrMap.end(); ++it) {
		(*m_pMap)[it->first].push(it->second.Detach());
		m_Names.insert(it->first);
	}
}


AugmentedPreprocessContext::~AugmentedPreprocessContext() {
	for (auto it = m_Names.begin(); it != m_Names.end(); ++it) {
		string const strName = *it;
		assert(m_pMap->count(strName));
		stack<Ptr<AbstractNode const>> &rStack = (*m_pMap)[strName];
		assert(!rStack.empty());
		rStack.pop();
		if (rStack.empty()) {
			m_pMap->erase(strName);
		}
	}
}
