#ifndef LWPP_PRESETS_H
#define LWPP_PRESETS_H

#include <lwpp/io.h>
#include <lwpp/wrapper.h>
#include <lwpp/message.h>
#include <lwpp/file_request.h>

namespace lwpp
{
	class PresetHandler
	{
	public:
		virtual ~PresetHandler() {}
		virtual LWError LoadPreset(lwpp::LoadState& ls) = 0;
		virtual LWError SavePreset(lwpp::SaveState& ss) = 0;
	};

	class PresetEntry
	{
		std::string name;
		std::string filename;
		std::string gui_name;
		std::string catName;
	public:
		static int longest;
		PresetEntry(const char *f, const char *n, std::string tName);
		const char *GetName() {return name.c_str();}
		const char *GetGuiName();
		const char *GetFileName() {return filename.c_str();}
	};

	class PresetCollection :  public PopUpCallback, public std::vector<PresetEntry>
	{
		protected:
			virtual size_t popCount(void);
			virtual const char *popName(int n);
		public:
			static bool showDecorated;
			void AddEntry(const char *file, const char *name, std::string type)
			{
				push_back(PresetEntry(file, name, type));
			}
			const char *GetName(int n) {return popName(n);}
	};

  /*!
  * Steps to use the Simple Presets:
  * # Derive your plugin class from PresetHandler
  * # Implement PresetHandler::LoadPreset and PresetHandler::SavePreset
  * # Add a SimplePreset class as a member variable
  * # Initialise the SimplePreset in the constructor with the plugin type as a string as well as the plugin name
  * # Call SimplePreset::SetHandler using this in the constructor
  * # in XPanels, add a vPopCmd control for the preset, i.e. {ID_PRES, "Presets", "vPopCmd"}
  * # During setup of the XPanel, call SimplePreset.Setup() to register the panel and control mID
  */ 
	class SimplePreset : public PopUpCallback, public PopCmdCallback<SimplePreset>
	{
		const char *plug_name;
		const char *plug_type;
		PresetHandler *handler;
		long controlID;
		PresetCollection Presets;
		XPanel xpanel;
		FileRequest2 loadReq, saveReq;
		bool isPreset(std::string filename);
	protected:
		void ScanPresets(std::string path);
		void doSavePreset();
		void doDeletePreset();
		void doSaveAsPreset();
		void doLoadFromPreset();
		LWError LoadPreset(lwpp::LoadState& ss, std::string &name);
		LWError SavePreset(lwpp::SaveState& ls, std::string name);
		std::string makeFileName(std::string name, int type);
		std::string makeFilePath(std::string name);
		std::string grabPresetName(std::string filename, std::string name);
	public:
		SimplePreset(const char *type, const char *name);
		void SetHandler(PresetHandler *inst);
		LWError Setup(XPanel& panel, long control);
		virtual void popCommand(int cid, int cmd);
		void doCommand(int cmd);
        bool doLoadPreset(const char *filename, bool quiet = false);
		virtual size_t popCount(void);
		virtual const char *popName(int n);
	};
}

#endif // LWPP_PRESETS_H
