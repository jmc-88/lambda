#pragma once

#include <Environment.h>


struct BuiltInEnvironment :
	public BaseEnvironment
{
private:
	static map<string const, AbstractValue const*> BuildTerms();

public:
	BuiltInEnvironment();

};
