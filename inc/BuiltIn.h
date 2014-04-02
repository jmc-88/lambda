#pragma once

#include <Environment.h>
#include <Preprocess.h>


struct BuiltInPreprocessContext :
	public BasePreprocessContext
{
	BuiltInPreprocessContext();
};


struct BuiltInEnvironment :
	public BaseEnvironment
{
private:
	static map<string const, AbstractValue const*> BuildTerms();

public:
	BuiltInEnvironment();

};
