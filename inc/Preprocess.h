#pragma once


struct AbstractNode;

struct AbstractPreprocessContext {
private:
	map<string const, stack<Ptr<AbstractNode const>>> *const m_pMap;
	friend struct AugmentedPreprocessContext;

protected:
	explicit AbstractPreprocessContext(map<string const, stack<Ptr<AbstractNode const>>> *const a_pMap);

public:
	virtual ~AbstractPreprocessContext();

	bool Has(string const &rstrName) const;
	Ptr<AbstractNode const> const &operator [] (string const &rstrName) const;

};


struct BasePreprocessContext :
	public AbstractPreprocessContext
{
private:
	map<string const, stack<Ptr<AbstractNode const>>> m_Map;

public:
	explicit BasePreprocessContext(map<string const, Ptr<AbstractNode const>> &&a_rrMap = map<string const, Ptr<AbstractNode const>>());
	virtual ~BasePreprocessContext();

};


struct AugmentedPreprocessContext :
	public AbstractPreprocessContext
{
private:
	AbstractPreprocessContext const &m_rContext;
	set<string> m_Names;

public:
	explicit AugmentedPreprocessContext(AbstractPreprocessContext const &a_rContext, map<string const, Ptr<AbstractNode const>> &&a_rrMap);
	virtual ~AugmentedPreprocessContext();

};
