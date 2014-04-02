#include <PCH.h>
#include <Error.h>


Error::~Error() {}


InternalError::~InternalError() {}


SyntaxError::~SyntaxError() {}


PreprocessError::~PreprocessError() {}


RuntimeError::~RuntimeError() {}


UserException::UserException(AbstractValue const *const a_pValue)
	:
m_pValue(a_pValue) {}


UserException::~UserException() {}


ExitException::ExitException(IntegerValue const *const a_pCode)
	:
m_pCode(a_pCode) {}


ExitException::~ExitException() {}
