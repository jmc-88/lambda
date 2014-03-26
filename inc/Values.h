#pragma once

#include <Environment.h>
#include <Values.h>


struct AbstractValue {
	enum Type {
		TYPE_INTEGER,
		TYPE_LONG,
		TYPE_FLOAT,
		TYPE_COMPLEX,
		TYPE_STRING,
		TYPE_CLOSURE
	};

	Type const m_Type;

	explicit AbstractValue(Type const a_Type);
	virtual ~AbstractValue();

	virtual operator string const () const = 0;
};


struct IntegerValue :
	public AbstractValue
{
	signed int m_n;

	IntegerValue(signed int a_n);
	virtual ~IntegerValue();

	virtual operator string const () const;
};


struct LongValue :
	public AbstractValue
{
	signed long long m_l;

	LongValue(signed long long a_l);
	virtual ~LongValue();

	virtual operator string const () const;
};


struct FloatValue :
	public AbstractValue
{
	double m_f;

	FloatValue(double a_f);
	virtual ~FloatValue();

	virtual operator string const () const;
};


struct ComplexValue :
	public AbstractValue
{
	double m_fReal;
	double m_fImaginary;

	ComplexValue(double a_fReal, double a_fImaginary);
	virtual ~ComplexValue();

	virtual operator string const () const;
};


struct StringValue :
	public AbstractValue
{
	string m_str;

	StringValue(string const &a_rstr);
	virtual ~StringValue();

	virtual operator string const () const;
};


struct AbstractNode;

struct Closure :
	public AbstractValue
{
	vector<string> m_Arguments;
	Ptr<AbstractNode const> m_pBody;
	Environment m_Environment;

	Closure(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody, Environment &&a_rrEnvironment);
	virtual ~Closure();

	virtual operator string const () const;
};
