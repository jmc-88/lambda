#include <PCH.h>
#include <Values.h>
#include <AST.h>


AbstractValue::AbstractValue(Type const a_Type)
	:
m_Type(a_Type) {}


AbstractValue::~AbstractValue() {}


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


ComplexValue::ComplexValue(double a_fReal, double a_fImaginary)
	:
AbstractValue(TYPE_COMPLEX),
	m_fReal(a_fReal),
	m_fImaginary(a_fImaginary) {}


ComplexValue::~ComplexValue() {}


ComplexValue::operator string const () const {
	ostringstream oss;
	oss << fixed << m_fReal << "+" << m_fImaginary << "i" << flush;
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
			str += "\\\'";
		} else if (*it == '\\') {
			str += "\\\\";
		} else {
			// TODO escape special characters
			str += *it;
		}
	}
	return str += "\'";
}


Closure::Closure(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody, BaseEnvironment &&a_rrEnvironment)
	:
AbstractValue(TYPE_CLOSURE),
	m_Arguments(move(a_rrArguments)),
	m_pBody(move(a_rrpBody)),
	m_Environment(move(a_rrEnvironment))
{
	assert(m_Arguments.size() > 0);
}


Closure::~Closure() {}


Closure::operator string const () const {
	string str = "lambda ";
	auto it = m_Arguments.begin();
	str += *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	return str += " . " + m_pBody->ToString(m_Environment);
}


ostream &operator << (ostream &ros, AbstractValue const &rValue) {
	return ros << (string const)rValue;
}


Macro::Macro(vector<string> &&a_rrArguments, Ptr<AbstractNode const> &&a_rrpBody)
	:
AbstractValue(TYPE_MACRO),
	m_Arguments(move(a_rrArguments)),
	m_pBody(move(a_rrpBody))
{
	assert(m_Arguments.size() > 0);
}


Macro::~Macro() {}


Macro::operator string const () const {
	string str = "macro ";
	auto it = m_Arguments.begin();
	str += *it;
	for (++it; it != m_Arguments.end(); ++it) {
		str += ", " + *it;
	}
	return str += " . " + m_pBody->ToString(BaseEnvironment());
}
