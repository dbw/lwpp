#ifndef LWPP_UTILITY
#define LWPP_UTILITY

#include <lwpp/debug.h>

#include <lwenvel.h>
#include <lwxpanel.h>
#include <algorithm>

namespace lwpp
{
	#pragma warning(disable : 4996)
	//! Base class for items that can be named. Will store the name locally in a copy
	class Nameable
	{
	public:
		std::string name;
	public:
		/*
		 * @param _name Optional name to set the object to
		 */
		explicit Nameable(const char *_name = 0) {SetName(_name);}
		explicit Nameable(std::string _name) {SetName(_name);}
		virtual ~Nameable()	{;}
		//! Copy a name
		virtual Nameable &operator=(const Nameable &from)
		{
			if (this != &from) 
			{
				name = from.name;
			}
			return *this;
		}
		//! Set a new name
		virtual void SetName(const std::string _name)
		{
			name = _name;
		}
		virtual void SetName(const char *_name)
		{
			if (_name == 0)
			{
				name.clear(); // empty string				
			}
			else
			{
				SetName (std::string(_name));
			}
		}
		//! Get a name
		/*
		virtual const char *GetName() const
		{
			return name.empty() ? 0 : name.c_str();
		}
		*/

		virtual const std::string &GetName() const
		{
			return name;
		}
	};

	class ReferenceCounted
	{
		public:
			int nReferences;
			ReferenceCounted() : nReferences(0) {}
		private:
			ReferenceCounted(const ReferenceCounted &);
			ReferenceCounted &operator=(const ReferenceCounted &);
	};

	template <class T>
	class Reference
	{
		private:
			T *ptr;
		public:
			Reference(T *p = 0)
				: ptr(p)
			{
				if (ptr) ++ptr->nReferences;
			}
			Reference(const Reference<T> &r)
			{
				ptr = r.ptr;
				if (ptr) ++ptr->nReferences;
			}
			Reference &operator=(const Reference<T> &r)
			{
				if (r.ptr) r.ptr->nReferences++;
				if (ptr && --ptr->nReferences == 0) delete ptr;
				ptr = r.ptr;
				return *this;
			}

			Reference &operator=(T *p)
			{
				if (p) p->nReferences++;
				if (ptr && --ptr->nReferences == 0) delete ptr;
				ptr = p;
				return *this;
			}
			~Reference()
			{
				if (ptr && --ptr->nReferences == 0) delete ptr;
			}
			T *operator->()
			{
				assert(ptr);
				return ptr;
			}
			T *adress() const
			{
				return ptr;
			}
			const T *operator->() const
			{
				assert(ptr);
				return ptr;
			}
			operator bool() const {return ptr != 0;}
			bool operator<(const Reference<T> &r) const
			{
				return ptr < r.ptr;
			}
	};

	//! Open a URL in the default application, usually a browser
	void OpenURL(const std::string urlString);
	/*
	{
		// Thanks to Willard Myers, on the qt-interest mailing list.
		ICInstance icInstance;
		if (ICStart ( &icInstance, '????') == noErr)  
		{
			ConstStr255Param hint (0x0);
			long l = urlString.length();
			long s = 0;
			ICLaunchURL (icInstance, hint, urlString.c_str(), l, &s, &l);
			ICStop (icInstance);
		}
	};
			*/

	//! Check if the time in milliseconds is short enough to be registered as a double click
	bool isDoubleClick(unsigned int milliseconds);

	struct to_lower
	{
		char operator() ( char ch )
		{
			return static_cast<char>(::tolower ( ch ));
		}
	};

    //!return the current mouse poairion, platform dependent
	bool getMousePosition(int &x, int &y);

	//! convert a string in-place to lower case
	inline std::string &toLowercase(std::string &str)
	{
		std::transform (str.begin (), str.end (), str.begin (), to_lower());
		return str;
	}

	//! Capitalizes a string so the first character and every character after a space is in upper case
	inline std::string &toCapitalize(std::string &str)
	{
		bool caps = true;
		for (size_t i = 0; i < str.length(); ++i)
		{
			if (caps)
			{
				str[i] = (char)::toupper(str[i]);
				caps = false;
			}
			else
			{
				str[i] = (char)::tolower(str[i]);
			}
			caps = str[i] == ' ';
		}
		return str;
	}	

	bool inline nocase_compare (const char c1, const char c2)
	{
			return ::toupper(c1) == ::toupper(c2);
	}

	//! Case independant string comparison
	bool inline cmpNoCase(const std::string &s1, const std::string &s2)
	{
		std::string::const_iterator pos;
		pos = std::search (s1.begin(),s1.end(),   // haystack
													 s2.begin(),s2.end(),   // needle
														nocase_compare);    // comparison criterion
		return (pos != s1.end());
	}

	//! rotate within a container, can be used for re-ordering in a list / sheet control
	template <typename T>
	void rotateContainer(T& container, const size_t from, const size_t to)
	{
		if (from < to)
		{
			std::rotate(container.begin() + from, container.begin() + from + 1, container.begin() + to);
		}
		else
		{
			std::rotate(container.rend() - from - 1, container.rend() - from, container.rend() - to);
		}
	}

}

#endif //LWPP_UTILITY
