#include <lwserver.h>
#include <lwhandler.h>
#include <lwrender.h>
#include <lwcustobj.h>
#include <lwhost.h>
#include <cstdlib>

#include "ringtest.h"

#include <lwmodule.h>


extern "C" void *Startup (void)
{
  return (void *) -1;
}

extern "C" void Shutdown (void *serverData)
{
	serverData; // do nothing

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

extern "C"
ServerTagInfo comringTags[] =
{
	{"ComRingTest",SRVTAG_USERNAME},
	{"ComRingTest",SRVTAG_DESCRIPTION },
	{ NULL }
};

extern "C"
ServerRecord ServerDesc[] =
{
	{ LWCUSTOMOBJ_HCLASS, "ComRingTest", lwpp::CustomObjectAdaptor<ringtest>::Activate, comringTags  },
	{ NULL }
};


extern "C" ModuleDescriptor __declspec( dllexport ) _mod_descrip =
{
  MOD_SYSSYNC,
  MOD_SYSVER,
  MOD_MACHINE,
  Startup,
  Shutdown,
  ServerDesc
};