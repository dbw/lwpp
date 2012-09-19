#include <lwpp/interface.h>
#include <lwpp/item.h>

namespace lwpp
{

	lwpp::LWItem InterfaceInfo::selectedItem( void ) const
	{
		return lwpp::LWItem(globPtr->selItems[0]);
	}

	std::vector<lwpp::LWItem> InterfaceInfo::selectedItems() const
	{
		std::vector<LWItem> vecItems;
		const LWItemID *id = globPtr->selItems;
		// Loops the selection
		for ( int i = 0; id[ i ]; i++ ) {
			vecItems.push_back(lwpp::LWItem(id[i]));
		}
		return vecItems;
	}

} // end namespace lwpp