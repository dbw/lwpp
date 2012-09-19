#ifndef LWPP_EXCEPTION_H
#define LWPP_EXCEPTION_H

#include <string>

namespace lwpp
{
	class exception : public std::exception
	{
		std::string mDescription;
	public:
		exception(const std::string desc) throw()
		  : mDescription(desc)
	  {
			;
		}
		virtual ~exception() throw() {;}
		/** Returns a C-style character string describing the general cause
		*  of the current error.  */
		virtual const char* what() const throw()
		{
			return mDescription.c_str();
		}
	};

	class MissingGlobal : public exception
	{
	public:
		MissingGlobal(const char *name) : exception(name)
		{}
	};
}

#endif // LWPP_EXCEPTION_H