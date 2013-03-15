#ifndef LWPP_STOREABLE_H
#define LWPP_STOREABLE_H

#include <lwpp/io.h>

namespace lwpp
{
	class Storeable {
		public:
			//! Load data from a LWLoadState
			/*!
			 * @param &ls LoadState to load from
			 * @return An error string if an error occured, 0 otherwise
			 */
			virtual LWError Load(const LoadState & ) {return 0;}
			//! Save data to a LWSaveState
			/*!
			 * @param &ss SaveState to save to
			 * @return An error string if an error occured, 0 otherwise
			 */
			virtual LWError Save(const SaveState & ) {return 0;}
	};
}

#endif // LWPP_STOREABLE