#include "lwpp/lw_server.h"
#include "lwpp/platform.h"
#include <lwmodule.h>
#include <locale.h>

#if defined __GNUC__
	__attribute__ ((visibility("default")))
#else
	extern "C" __declspec( dllexport )
#endif
ModuleDescriptor _mod_descrip =
{
		MOD_SYSSYNC,
		MOD_SYSVER,
		MOD_MACHINE,
		&lwpp::LWServer::Startup,
		&lwpp::LWServer::Shutdown,
		0
};

namespace lwpp
{
	HandlerList *LWServer::startupList()
	{
		static HandlerList startList;
		return &startList;
	}
	HandlerList *LWServer::shutdownList()
	{
		static HandlerList shutList;
		return &shutList;
	}

	PluginList *LWServer::pluginList()
	{
		static PluginList plugList;
		return &plugList;
	}

	void LWServer::AddPlugin(const char *className,  const char *name, ActivateFunc *activate, ServerTagInfo  tagInfo[])
	{
		ServerRecord record = 
		{
			className, name, activate, tagInfo
		};

		pluginList()->push_back(record);
	}

	void *LWServer::Startup(void)
	{
#ifdef _WIN32
		setlocale(LC_ALL, "");
#ifdef _DEBUG
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
#endif
		/*
		for (HandlerIterator i = startupList()->begin(); i != startupList()->end(); ++i)
		{
			(*i)();
		}
		*/
		for ( auto i : *startupList())
		{
			(*i)();
		}

		ServerRecord *record = new ServerRecord[pluginList()->size()+1];
		// filer ServerRecord
		int idx = 0;
		for (PluginIterator i = pluginList()->begin(); i != pluginList()->end(); ++i, ++idx)
		{
			record[idx].name = i->name;
			record[idx].className = i->className;
			record[idx].activate = i->activate;
			record[idx].tagInfo = i->tagInfo;
		}    
		record[idx].name = 0;
		record[idx].className = 0;
		record[idx].activate = 0;
		record[idx].tagInfo = 0;
		::_mod_descrip.serverDefs = record;
		return record;
	}

	void LWServer::Shutdown(void *rec)
	{
		ServerRecord *record = static_cast<ServerRecord *>(rec);
		delete[] record;
    /*
		for (HandlerIterator i = shutdownList()->begin(); i != shutdownList()->end(); ++i)
		{
			(*i)();
		}
    */
    for ( auto i : *shutdownList() )
    {
      (*i)();
    }
#if defined (_DEBUG) && defined (_WIN32)
		
		_CrtDumpMemoryLeaks();
#endif
	}
}