#include <PCH.h>
#include <Environment.h>
#include <Values.h>


Environment::Environment(map<string const, AbstractValue const*> const &a_rMap)
	:
m_pMap(&m_Map) {
	for (auto it = a_rMap.begin(); it != a_rMap.end(); ++it) {
		m_Map[it->first].push(it->second);
	}
}


Environment::~Environment() {}
