#pragma once

#include <Values.h>


struct Error {
	virtual ~Error();
};


struct InternalError :
	public Error
{
	virtual ~InternalError();
};


struct SyntaxError :
	public Error
{
	virtual ~SyntaxError();
};


struct PreprocessError :
	public Error
{
	virtual ~PreprocessError();
};


struct RuntimeError :
	public Error
{
	virtual ~RuntimeError();
};


struct UserException :
	public Error
{
	AbstractValue const *const m_pValue;

	explicit UserException(AbstractValue const *const a_pValue);
	virtual ~UserException();
};


struct ExitException :
	public Error
{
	IntegerValue const *const m_pCode;

	explicit ExitException(IntegerValue const *const a_pCode);
	virtual ~ExitException();
};
