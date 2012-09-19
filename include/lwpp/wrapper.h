/*!
 * @file
 * @brief Contains miscellanous base classes for LW SDK Wrappers
 */
#ifndef LWPP_WRAPPER_H
#define LWPP_WRAPPER_H

#include <lwpp/xpanel.h>

namespace lwpp
{
	//! Base class for PopUpCallbacks
	/*!
	 * Derive classes from this that use PopCount/PopName callbacks. This class provides the "static clall back" -> "member function" translation to interface with LW.
	 * @note Make sure the LWInstance passed to the function is a pointer to the derived class.
	 */
	class PopUpCallback
	{	
	protected:
		int index;
		//! Actual worker function to be supplied by the deriving class
		/*!
		 * @return The number of items
		 */
		virtual size_t popCount() = 0;
		//! Actual worker function to be supplied by the deriving class
		/*!
		 * @param n A item index
		 * @return The name of the item
		 */
		virtual const char *popName(int n) = 0;
	public:
		PopUpCallback()
			: index(0)
		{
			;
		}
		virtual ~PopUpCallback() {;}
		//! PopCount callback for LightWave
		static int PopCount(void *inst)
		{
			if (inst)
			{
				PopUpCallback *puc = static_cast<PopUpCallback *>(inst);
				return (int) puc->popCount();
			}
			return 0;
		}
		//! PopUpName name callback	for LightWave
		static const char* PopName(void *inst, int n)
		{			
			if (inst)
			{
				PopUpCallback *puc = static_cast<PopUpCallback *>(inst);
				return puc->popName(n);
			}
			return 0;
		}
		const char *GetSelectedName()
		{
			return popName(index);
		}
	};
	//! Base class for context menus
	/*!
	 * Derive a class from this to get instant static callbacks, an example would be DynamicContextMenu
	 */
	class ContextMenuCallback
	{
		protected:
			//! Count the number of items in the context menu
			virtual size_t count(void) = 0;			
			//! Retrieve the name of an item from the context menu
			virtual const char *name(int n) = 0;
	  public:	
			//! Static member function to be passed as a LW Callback
			static size_t countFn (void *inst);
			//! Static member function to be passed as a LW Callback
			static char* nameFn (void *inst, int n);			
	};

	template <class T>
	class ExtendedPopUp : public PopUpCallback
	{
	private:
		template <class C>
		class popupEntry
		{
		public:
			C value;
			const char *name;
			popupEntry(const char *_title, C _value) : name(_title), value(_value) {;}
			const char *getName() const {return name;}
			C getValue() const {return value;}
			bool equals(C _value) {return value == _value;}
		};

		std::vector< popupEntry<T> > Entries;
		
	protected:
		virtual size_t popCount(void)
		{
			return Entries.size();
		}
		virtual const char *popName(int n)
		{
			if (n < 0) return "(none)";
			else return Entries.at(n).getName();
		}

	public:
		ExtendedPopUp () {;}
		virtual ~ExtendedPopUp() {;}
		virtual void AddEntry (const char *entry, T id)
		{
			Entries.push_back(popupEntry<T>(entry, id));
		}
		virtual T GetEntry (int n)
		{
			return Entries.at(n).getValue();
		}
		//! Return the index number of a certain entry
		virtual size_t GetIndex (T val)
		{
			for (size_t n = 0; n < Entries.size(); ++n)
			{
				if (Entries.at(n).equals(val)) return n;
			}
			return 0; // not found
		}
	};

	template<class T>
	class PopCmdCallback
	{
		public:
			static void PopCommand (LWXPanelID pan, int cid, int cmd)
			{
				XPanelFuncs xpf;
				T *pcc = static_cast<T *>(xpf.getUserData(pan, cid));
				if (pcc) pcc->popCommand(cid, cmd);
			}
	};

	//! PopCount callback for LightWave
	template <typename T>
	class doPop
	{
	public:
		static int PopCount(void *inst)
		{
			if (inst)
			{
				T *puc = static_cast<T *>(inst);
				return puc->popCount();
			}
			return 0;
		}
		//! PopUpName name callback	for LightWave
		static const char* PopName(void *inst, int n)
		{			
			if (inst)
			{
				T *puc = static_cast<T *>(inst);
				return puc->popName(n);
			}
			return 0;
		}
	};
}
#endif // LWPP_WRAPPER_H