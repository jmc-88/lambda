#include <PCH.h>
#include <Values.h>
#include <AST.h>


AbstractValue::AbstractValue(Type const a_Type)
	:
m_Type(a_Type) {}


AbstractValue::~AbstractValue() {}


BooleanValue::BooleanValue(bool a_f)
	:
AbstractValue(TYPE_BOOLEAN),
	m_f(a_f) {}


BooleanValue::~BooleanValue() {}


BooleanValue::operator string const () const {
	if (m_f) {
		return "true";
	} else {
		return "false";
	}
}


IntegerValue::IntegerValue(signed int a_n)
	:
AbstractValue(TYPE_INTEGER),
	m_n(a_n) {}


IntegerValue::~IntegerValue() {}


IntegerValue::operator string const () const {
	ostringstream oss;
	oss << m_n << flush;
	return oss.str();
}


LongValue::LongValue(signed long long a_l)
	:
AbstractValue(TYPE_LONG),
	m_l(a_l) {}


LongValue::~LongValue() {}


LongValue::operator string const () const {
	ostringstream oss;
	oss << m_l << flush;
	return oss.str();
}


FloatValue::FloatValue(double a_f)
	:
AbstractValue(TYPE_FLOAT),
	m_f(a_f) {}


FloatValue::~FloatValue() {}


FloatValue::operator string const () const {
	ostringstream oss;
	oss << m_f << flush;
	return oss.str();
}


StringValue::StringValue(string const &a_rstr)
	:
AbstractValue(TYPE_STRING),
	m_str(a_rstr) {}


StringValue::~StringValue() {}


StringValue::operator string const () const {
	string str = "\'";
	for (auto it = m_str.begin(); it != m_str.end(); ++it) {
		if (*it == '\'') {
			str += '\\';
		}
		str += *it;
	}
	return str += "\'";
}


Closure::Closure(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody, map<string, AbstractValue*> &&a_rrCapture)
	:
AbstractValue(TYPE_CLOSURE),
	m_Arguments(move(a_rrArguments)),
	m_pBody(move(a_rrpBody)),
	m_Environment(move(a_rrCapture))
{
	assert(m_Arguments.size() > 0);
}


Closure::~Closure() {}


Closure::operator string const () const {
	assert(m_Arguments.size() > 0);
	auto it = m_Arguments.begin();
	string str = "lambda " + *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	set<string> Names;
	for (auto it = m_Arguments.begin(); it != m_Arguments.end(); ++it) {
		Names.insert(*it);
	}
	return str += " . " + m_pBody->ToString(OverrideEnvironment(m_Environment, move(Names)));
}


ostream &operator << (ostream &ros, AbstractValue const &rValue) {
	return ros << (string const)rValue;
}
