#ifndef LWPP_DEBUG_H
#define LWPP_DEBUG_H

// always include plattform specific include
#include <lwpp/platform.h>
#include <cassert>
#include <fstream>

#if (_WIN32 && _DEBUG)
#define S1(x) #x
#define S2(x) S1(x)
#define DBG_ENTER_FUNC ::OutputDebugStringA( __FILE__ "(" S2(__LINE__) "): " __FUNCSIG__ "\n") 
//#define LWPP_DBG_ENTER_FUNC DBG_ENTER_FUNC
#define LWPP_DBG_ENTER_FUNC
#else
#define DBG_ENTER_FUNC
#define LWPP_DBG_ENTER_FUNC
#endif

namespace lwpp
{
#ifdef _DEBUGTOFILE
	class dfstream : public std::ofstream
	{
	public:
		dfstream() : std::ofstream("c:\\lwpp.log", std::ios::out | std::ios::app)
		{
			;
		}
	};
#endif

#ifdef _DEBUG
	#ifdef _DEBUGTOFILE
		class dostream : public std::ofstream
		{
			public:
				dostream() : std::ofstream("c:\\lwpp.log", std::ios::out | std::ios::app)
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
						::OutputDebugStringA(basic_debugbuf::str().c_str());
						basic_debugbuf::str(std::basic_string<CharT>());    // Clear the string buffer
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
					basic_dostream::setf(std::ios::unitbuf);
				}
				~basic_dostream()
				{
						delete basic_dostream::rdbuf();
				}
		};

		typedef basic_dostream<char> dostream;

		#endif // _MSWIN
	#endif // _DEBUGTOFILE
	
#endif // _DEBUG
}

#ifdef _DEBUG
extern lwpp::dostream dout;
#endif

#endif // LWPP_DEBUG_H