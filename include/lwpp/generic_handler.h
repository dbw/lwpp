#ifndef LWPP_GENERIC_HANDLER_H
#define LWPP_GENERIC_HANDLER_H

#include "lwpp/item.h"
#include <sstream>
#include <lwgeneric.h>
#include "lwpp/plugin_handler.h"

namespace lwpp
{
	/*
	XCALL_( int ) MyGeneric( long version, GlobalFunc *global,
      LWLayoutGeneric *local, void *serverData );
	*/
	//! @ingroup Handler
	class LayoutGenericHandler
	{
	protected:
		LWLayoutGeneric *layoutGeneric;
    int saveScene(const char *file)
		{
			return layoutGeneric->saveScene(file);
		}
    int loadScene(const char *file, const char *name)
		{
			return layoutGeneric->loadScene(file, name);
		}
    LWCommandCode lookup   (const char *cmdName)
		{
			return layoutGeneric->lookup(layoutGeneric->data, cmdName);
		}
    int execute  (LWCommandCode cmd, int argc, const DynaValue *argv, DynaValue *result)
		{
			return layoutGeneric->execute(layoutGeneric->data, cmd, argc, argv, result);
		}
    int evaluate (const char *command)
		{
			return layoutGeneric->evaluate(layoutGeneric->data, command);
		}
		int evaluate (const std::string &command)
		{
			return layoutGeneric->evaluate(layoutGeneric->data, command.c_str());
		}

		void editServer(const char *type, const char *name, LWItemID id = 0)
		{
			lwpp::ItemInfo ii;
			int index = ii.findServer(type, name, id);
			if (index == 0)
			{
				addServer(type, name);
				index = ii.findServer(type, name, id);
			}			
			std::ostringstream cmd;
			cmd << "EditServer " << type << " " << index;
			evaluate(cmd.str());
		}
		static int findServer(const char *type, const char *name, LWItemID id = 0)
		{
			lwpp::ItemInfo ii;
			return ii.findServer(type, name, id);
		}
		void addServer(const char *type, const char *name)
		{
			std::ostringstream cmd;
			cmd << "ApplyServer " << type << " " << name;
			evaluate(cmd.str());
		}
		void addUniqueServer(const char *type, const char *name)
		{
			if (findServer(type, name, 0) > 0) return;
			std::ostringstream cmd;
			cmd << "ApplyServer " << type << " " << name;
			evaluate(cmd.str());
		}
		
		void addUniqueServer(const char *type, const char *name, LWItem &item)
		{
			if (findServer(type, name, item.GetID()) > 0) return;
			std::ostringstream cmd;
			cmd << "ApplyServerByItemID " << std::hex << item.GetID() << " " << type << " " << name;
			evaluate(cmd.str());
		}
		void removeServer(const char *type, const int index)
		{
			if (index != 0)
			{
				std::ostringstream cmd;
				cmd << "RemoveServer " << type << " " << index;
				evaluate(cmd.str());
			}
		}
		void removeServer(const char *type, const char *name, LWItemID id = 0)
		{
			while (int index = (findServer(type, name, id)))
			{
				std::ostringstream cmd;
				if (id)
					cmd << "RemoveServerByItemID " << std::hex << id << " " << type << " " << index;
				else
					cmd << "RemoveServer " << type << " " << index;
				evaluate(cmd.str());
			}
		}

	public:
		LayoutGenericHandler()
			: layoutGeneric(0)
		{
			;
		}
    virtual ~LayoutGenericHandler() {;}
		void setLG (LWLayoutGeneric *local)
		{
			layoutGeneric = local;
		}
		virtual int Activate() = 0;
	};

	//! @ingroup Adaptor
	template <class T>
	class LayoutGenericAdaptor
	{
		public:
			LayoutGenericAdaptor(const char *name, ServerTagInfo *tags = 0)
			{
				LWServer::AddPlugin(LWLAYOUTGENERIC_CLASS, name, Activate, tags);
			}
			//! Set static callbacks for the LightWave instance handler

			static int Activate(int version, GlobalFunc *global, LWInstance inst, void *serverData)
			{
				UNUSED(serverData);
				UNUSED(version);
				LWLayoutGeneric *local = static_cast<LWLayoutGeneric *>(inst);
				SetSuperGlobal(global);
				T plugin;
				plugin.setLG(local);
				return plugin.Activate();
			}
	};

	class LayoutGenericXPanelHandler : public LayoutGenericHandler, public XPanelInterface
	{
	};


	class LayoutGenericLWPanelHandler : public LayoutGenericHandler, public LWPanelInterface
	{
		public:
			LayoutGenericLWPanelHandler() {;}
			virtual ~LayoutGenericLWPanelHandler() {;}
	};


	// IMPLEMENT_LWPANELHANDLER(LayoutGeneric);

	//! @ingroup Adaptor
	template <class T>
	class LayoutNonModalGenericAdaptor
	{
		public:
			static T *plugin;
		public:
			LayoutNonModalGenericAdaptor(const char *name, ServerTagInfo *tags = 0)
			{
				LWServer::AddPlugin(LWLAYOUTGENERIC_CLASS, name, Activate, tags);
			}
			//! Set static callbacks for the LightWave instance handler

			static int Activate(int version, GlobalFunc *global, LWInstance inst, void *serverData)
			{
				UNUSED(serverData);
				UNUSED(version);
				LWLayoutGeneric *local = static_cast<LWLayoutGeneric *>(inst);
				SetSuperGlobal(global);
				if (plugin == 0) plugin = new T();
				plugin->setLG(local);
				return plugin->Activate();
			}
	};

	#define IMPLEMENT_NONMODAL_GENERIC(className) template<> className *lwpp::LayoutNonModalGenericAdaptor< className >::plugin = 0;
}

#endif //LWPP_GENERIC_HANDLER_H