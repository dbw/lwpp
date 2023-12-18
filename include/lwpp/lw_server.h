#ifndef LW_SERVER_H
#define LW_SERVER_H

#include <vector>
#include <lwserver.h>

namespace lwpp
{
	typedef void SimpleHandler();

	typedef std::vector<ServerRecord> PluginList;
	typedef PluginList::iterator PluginIterator;

	typedef std::vector<SimpleHandler *> HandlerList;
	typedef HandlerList::iterator HandlerIterator;

	class LWServer
	{
		static PluginList &pluginList();
		static HandlerList *startupList();
		static HandlerList *shutdownList();
	public:
		static void *Startup(void);
		static void Shutdown(void *);
		static void AddShutdownHandler(SimpleHandler *sh)
		{
			shutdownList()->push_back(sh);
		}
		static void AddStartupHandler(SimpleHandler *sh)
		{
			startupList()->push_back(sh);
		}
		static void AddPlugin(const char *className,  const char *name, ActivateFunc *activate, ServerTagInfo  tagInfo[]);
	};


	class ShutdownHandler
	{
		public:
			ShutdownHandler(SimpleHandler *sh)
			{
				lwpp::LWServer::AddShutdownHandler(sh);
			}
	};

	class StartupHandler
	{
		public:
			StartupHandler(SimpleHandler *sh)
			{
				lwpp::LWServer::AddStartupHandler(sh);
			}
	};
}

#endif // LW_SERVER_H