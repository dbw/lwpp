#ifndef LWPP_DEBUG_H
#define LWPP_DEBUG_H

// always include plattform specific include
#include <lwpp/platform.h>
#include <cassert>
#include <fstream>

#if (_WIN32 && _DEBUG)
#define DBG_ENTER_FUNC ::OutputDebugStringA( "Enter (" __FUNCTION__  ")\n") 
#else
#define DBG_ENTER_FUNC
#endif

namespace lwpp
{
#ifdef _DEBUG
	#ifdef _DEBUGTOFILE
		class dostream : public std::ofstream
		{
			public:
				dostream() : std::ofstream("lwpp.log", std::ios::out | std::ios::app)
				{
					;
				}
		};
	#else
		#ifdef LWPP_PLATFORM_OSX_UB
	/*
	#ifdef NOTHING
	//#ifdef __MWERKS__
// #warning Debugging OSX

		class dostream : public std::ofstream
		{
			public:
				dostream() : std::ofstream("lwpp.log", std::ios::out | std::ios::app)
				{
					;
				}
		};
	#else
*/
		//#warning Debugging OSX

		class dostream : public std::stringstream
		{
			public:
				dostream()
				{
					;
				}

				virtual ~dostream()
				{
					sync();
				}

				virtual int sync()
				{
					std::cerr << str(); // << std::endl;
					str("");    // Clear the string buffer
					return 0;
				}
		};
		//#endif

		#elif defined(LWPP_PLATFORM_WIN)

		template <class CharT, class TraitsT = std::char_traits<CharT> >
		class basic_debugbuf :  public std::basic_stringbuf<CharT, TraitsT>
		{
		public:
				virtual ~basic_debugbuf()
				{
						sync();
				}

		protected:
				int sync()
				{
						::OutputDebugStringA(str().c_str());
						str(std::basic_string<CharT>());    // Clear the string buffer
						return 0;
				}
		};

		template<class CharT, class TraitsT = std::char_traits<CharT> >
		class basic_dostream :
				public std::basic_ostream<CharT, TraitsT>
		{
		public:
				basic_dostream()
					: std::basic_ostream<CharT, TraitsT>(new basic_debugbuf<CharT, TraitsT>())
				{
					setf(std::ios::unitbuf);
				}
				~basic_dostream()
				{
						delete rdbuf();
				}
		};

		typedef basic_dostream<char> dostream;

		#endif // _MSWIN
	#endif // _DEBUGTOFILE
#endif // _DEBUG
}

#endif // LWPP_DEBUG_H