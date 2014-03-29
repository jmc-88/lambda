#include <PCH.h>
#include <Preprocess.h>
#include <AST.h>
#include <Error.h>


AbstractPreprocessContext::AbstractPreprocessContext(map<string const, stack<Ptr<AbstractNode const>>> *const a_pMap)
	:
m_pMap(a_pMap) {}


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
