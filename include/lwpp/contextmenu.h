/*!
 * @file
 * @brief Wrapper for ContextMenuFuncs
 */

#ifndef LWPP_CONTEXMENU_H
#define LWPP_CONTEXMENU_H
#include <lwpp/wrapper.h>
#include <lwpp/utility.h>
#include <lwpp/panel.h>
#include <lwpanel.h>
#include <list>
#ifdef _DEBUG
#include "lwpp/debug.h"
#endif

namespace lwpp
{
	//! Wrapper for ContextMenuFuncs
	//! @ingroup Entities
	//! @ingroup Globals
	class ContextMenu : protected GlobalBase<ContextMenuFuncs>
	{
	private:
		LWContextMenuID menu;
		LWPanPopupDesc description;
	public:
		ContextMenu(void)
			: menu(0)
		{
			;
		}
		//! Create a new context menu
		/*!
		 * @param width Width of the menu in pixels
		 * @param countFn Callback to count the number of menu items
		 * @param nameFn Callback to return the name of the n'th item
		 * @param userdata User defined data to be associated with the menu (will be passed on to the Callbacks)
		 */
		ContextMenu(int width, size_t (*countFn)(void *), const char *(*nameFn)(void *, int),void *userdata)
			: menu(0)
		{
			Create(width, countFn, nameFn, userdata);
		}
		//! Destroy the context menu
		~ContextMenu()
		{
			if ((menu) && available()) globPtr->cmenuDestroy(menu);
		}
		void Create (void *userdata)
		{
			if ( available() )
			{
				if (menu) globPtr->cmenuDestroy(menu);
				menu = globPtr->cmenuCreate(&description, userdata);
			}
		}
		void Create(int width, size_t (*countFn)(void *), const char *(*nameFn)(void *, int), void *userdata)
		{
			if ( available() )
			{
				if (menu) globPtr->cmenuDestroy(menu);
				description.type = LWT_POPUP;
				description.width = width;
				description.countFn = (int (*)(void *))countFn;
				description.nameFn = nameFn;
				menu = globPtr->cmenuCreate(&description, userdata);
			}
		}
		//! Show the context menu
		/*!
		 * @param panel Parent LWPanelID
		 * @param select Selected item
		 * @return User selection or -1 if nothing was selected
		 */
		int Deploy(LWPanelID panel, int select)
		{
			if ((menu) && available())
			{
#ifdef _DEBUG
	std::hex(dout);
	dout << "Deploy Context Menu: 0x" << menu << "\n";	
	dout << "  Panel: 0x" << panel << "\n";	
	dout << "  Global: 0x" << globPtr << "\n";	
	dout << "  Function: 0x" << globPtr->cmenuDeploy << "\n";	
	dout.flush();
#endif
				return globPtr->cmenuDeploy( menu, panel, select);
			}
			else
			{
				return -1;
			}
		}
	};

	//! This is a simple version of a context menu
	//! @ingroup Entities
	class SimpleContextMenu : public ContextMenuCallback
	{
		private:
			ContextMenu menu;
			size_t num_entries;
			const char **entries;
			virtual size_t count(void)
			{
				return num_entries;
			};
			virtual const char *name(int n)
			{
				return entries[n];
			};
		public:
			SimpleContextMenu (const char **_entries, int width = 0) : entries(_entries), num_entries(0)
			{
				while (_entries[num_entries]) num_entries++;
				menu.Create(width, SimpleContextMenu::countFn, SimpleContextMenu::nameFn, this);
			}
			int Deploy(LWPanelID panel, int select)
			{
				return menu.Deploy(panel, select);
			}
	};

	//! This is a version of a context menu that can be filled dynamically, you can also supply IDs for the menu entries
	//! @ingroup Entities
	class DynamicContextMenu : public ContextMenuCallback
	{
		private:			
			class menuEntry
			{
			public:
				unsigned int id;
				std::string name;
				menuEntry(const char *_title, unsigned int _id) : name(_title), id(_id) {}
        menuEntry(std::string _title, unsigned int _id) : name(_title), id(_id) {}
        const char *getName(void) const {return name.c_str();}
				const int getID(void) const {return id;}
			};

			std::vector<menuEntry> Entries;
			ContextMenu menu;
		protected:
			size_t count(void);
			const char *name(int n);
	  public:	
			DynamicContextMenu (int _width = 0);
			int Deploy(int select);
			int Deploy(LWPanelID panel, int select);
      int Deploy(LWPanel &panel, int select);
			//! Add a new item to the context menu
			void AddEntry (const char *entry, unsigned int id);
      void AddEntry (std::string entry, unsigned int id);
			void Clear();
	};
}
#endif // LWPP_CONTEXMENU_H