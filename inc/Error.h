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
