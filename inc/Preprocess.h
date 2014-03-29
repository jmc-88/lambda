#pragma once


struct AbstractNode;

struct AbstractPreprocessContext {
private:
	map<string const, stack<Ptr<AbstractNode const>>> *const m_pMap;

protected:
	explicit AbstractPreprocessContext(map<string const, stack<Ptr<AbstractNode const>>> *const a_pMap);

public:
	bool Has(string const &rstrName) const;
	Ptr<AbstractNode const> const &operator [] (string const &rstrName) const;

};
