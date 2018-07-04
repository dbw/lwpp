#include <lwpp/contextmenu.h>

namespace lwpp
{
	/** DynamicContextMenu **/
	DynamicContextMenu::DynamicContextMenu (int _width)
	{
		menu.Create(_width, countFn, nameFn, this); 
	}

	size_t DynamicContextMenu::count(void) 			
	{
		return Entries.size();
	}
	
	const char *DynamicContextMenu::name(int n) 
	{
		if ((n < 0) || (n >= count())) return "*void*";
#ifdef _DEBUG
	lwpp::dostream dout;
	dout << "Item: " << n << " = " << Entries[n].getName() << " ( 0x" << this << ")\n";	
	dout.flush();
#endif
		return Entries.at(n).getName();
	}

  int DynamicContextMenu::Deploy(LWPanel &panel, int select)
  {
    return Deploy(panel.GetID(), select);
  }

	int DynamicContextMenu::Deploy(LWPanelID panel, int select)
	{
#ifdef _DEBUG
	lwpp::dostream dout;
	std::hex(dout);
	dout << "Opening Context Menu (0x" << this << ") on Panel (0x" << panel  << ")\n";	
	dout.flush();
#endif
		if (select > (count() - 1))
		{
			select = -1;
		}
		select = menu.Deploy(panel, select);
		if ((select < 0) || (select >= count()))
		{
			return -1; // no valid entry
		}
		return Entries.at(select).getID();
	}

	/*!
		* @param *entry String to display
		* @param mID Id of the entry to return, this makes the ID independant of the menu position
		*/
	void DynamicContextMenu::AddEntry (const char *entry, unsigned int id)
	{
		Entries.push_back(menuEntry(entry, id));
	}
	void DynamicContextMenu::AddEntry (std::string entry, unsigned int id)
	{
		Entries.push_back(menuEntry(entry, id));
	}
}