#pragma once


struct Error {
};


struct InternalError :
	public Error
{
};


struct SyntaxError :
	public Error
{
};


struct PreprocessError :
	public Error
{
};


struct RuntimeError :
	public Error
{
};
