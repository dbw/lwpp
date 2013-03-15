#include <lwpp/presets.h>
#include <lwpp/message.h>
#include <lwpp/math.h>
#include <cstdio>
#include <ctype.h>
#include <algorithm>
#include <map>
#include <fstream>

namespace lwpp
{

#define PRESETPATHS_CFG "SimplePresetPaths.cfg"

	class PathMap : public std::map<std::string, std::string>, public PopUpCallback
	{
	protected:
		virtual size_t popCount(void)	{return this->size();}
		virtual const char *popName(int n)
		{
			for (PathMap::iterator i = this->begin(); i != this->end(); ++i, --n)
			{
				if (n == 0) return i->first.c_str();
			}
			return "";
		}
	public:
		void Reset()
		{
			this->clear();
			lwpp::DirInfo di(LWFTYPE_INSTALL);
			std::string basedir;
#ifdef LWPP_PLATFORM_OSX_UB
			basedir = "/Library/Preferences/LightWave3D";
#elif defined LWPP_PLATFORM_WIN
			TCHAR szPath[MAX_PATH];

			if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_DOCUMENTS|CSIDL_FLAG_CREATE, NULL, 0, szPath))) 
			{
				basedir = szPath;
				basedir += "\\dbw\\LightWavePlugins";
			}
#endif
			insert(std::pair<std::string, std::string>("Global", basedir));			
			insert(std::pair<std::string, std::string>("User", di.GetDirectory(LWFTYPE_SETTING)));
			insert(std::pair<std::string, std::string>("Content", di.GetDirectory(LWFTYPE_CONTENT)));
		}

		void LoadPrefs(std::string filename)
		{
			if (!FileName::Exists(filename)) return;
			std::ifstream in(filename.c_str());
			if (!in.good()) return;
			while (!in.eof())
			{
				std::string line;
				std::getline(in, line);
				if (line[0] != '#')
				{
					std::string::size_type index = line.find_first_of(" ");
					if (index == std::string::npos)
						continue;

					std::string map = line.substr(0, index);
					if (map.empty())
						continue;

					std::string path = line.substr(index+1, line.size() - index - 1);
					if (path.empty())
						continue;

					toCapitalize(map);
					//insert(std::pair<std::string, std::string>(map, path));
					//insert(map, path);
					(*this)[map]=path;
				}
			}
		}
		void Load()
		{
			Reset();
			lwpp::DirInfo di;
			LoadPrefs(di.GetDirectoryStr(LWFTYPE_INSTALL) + PRESETPATHS_CFG);
			LoadPrefs(di.GetDirectoryStr(LWFTYPE_SETTING) + PRESETPATHS_CFG);
			LoadPrefs(di.GetDirectoryStr(LWFTYPE_CONTENT) + PRESETPATHS_CFG);
		}
	};

	static PathMap pathMap;

	#define IO_VERS LWID_('V','E','R','S')  //!< Version
	#define IO_PRES LWID_('P','R','E','S')  //!< Preset
	#define IO_NAME LWID_('N','A','M','E')  //!< Preset

	static LWBlockIdent idroot[] = 
	{
		IO_VERS, "Version", // 0
		IO_PRES, "Preset", // 1
		IO_NAME, "Name", // 2
		0
	};

	//Preset Entry

	int PresetEntry::longest = 0;

	//Preset Entry
	PresetEntry::PresetEntry(const char *f, const char *n, std::string tName)
		: name(n), filename(f), catName(tName)
	{
		longest = lwpp::Max(longest, static_cast<int>(catName.size() * 8));
	}

	const char *PresetEntry::GetGuiName()
	{
		std::ostringstream is;
		is <<  "[" << catName << "]\03(a:" << longest << ")" << name;
		gui_name = is.str();
		return gui_name.c_str();
	}

	// PresetCollection
	bool PresetCollection::showDecorated = false;

	size_t PresetCollection::popCount(void)
	{
		return size();
	}
	const char *PresetCollection::popName(int n)
	{
		return showDecorated ?  operator[](n).GetGuiName() : operator[](n).GetName();
	}

	// SimplePreset

	SimplePreset::SimplePreset(const char *type, const char *name)
		: plug_type(type), plug_name(name), controlID(0), handler(0),
		  loadReq(FREQ_LOAD, "Load Settings", "Settings|*.cfg"),
			saveReq(FREQ_SAVE, "Save Settings", "Settings|*.cfg")
	{
		xpanel.setAutoDestroy(false);
		pathMap.Load();
	}

	void SimplePreset::SetHandler(PresetHandler *inst)
	{
		handler = inst;
		for (PathMap::iterator i = pathMap.begin(); i != pathMap.end(); ++i)
		{
			ScanPresets(i->first);
		}
		const std::string def("default");
		for (PresetCollection::iterator i = Presets.begin(); i != Presets.end(); ++i)
		{
			if (cmpNoCase (i->GetName(), def))
			{
				doLoadPreset(i->GetFileName(), true);
				break; // exit for loop
			}
		}
	}

	std::string SimplePreset::grabPresetName(std::string filename, std::string name)
	{
		LWMessage lwm;
		File in(filename, File::FILE_LOAD);
		LoadState ls = in.getLoadState();
		short version;
		LWError err = 0;
		while (LWID id = ls.Find(idroot))
		{
			switch (id)
			{
				case IO_VERS:	version = ls.readShort(); break;

				case IO_NAME:	ls.read(name); break;

			default: 
				break;
			}
			ls.End();
		}
		if (err)
		{
			lwm.Error(std::string("Error loading Preset: ") + err);
		}
		return name;
	}

	bool SimplePreset::isPreset(std::string filename)
	{
		File in(filename, File::FILE_LOAD);
		LoadState ls = in.getLoadState();
		while (LWID id = ls.Find(idroot))
		{
			switch (id)
			{
			case IO_PRES:	return true;	break;

			default: break;
			}
			ls.End();
		}
		return false;
	}

	void SimplePreset::ScanPresets(std::string type)
	{
		std::string path = makeFilePath(type);
		Directory dir(path.c_str());
		if (dir.exists())
		{
			while (const char *entry = dir.getEntry())
			{
				std::string fullname(path);
				fullname += fSep;
				fullname += entry;
				if (entry && entry[0] == '.') continue; // skip all files starting with a dot
				if (lwpp::isDirectory(fullname)) continue;
				if (isPreset(fullname))
				{
					std::string name = grabPresetName(fullname, stripExtension(entry));
					Presets.AddEntry(fullname.c_str(), name.c_str(), type);
				}
			}
		}
	}

	size_t SimplePreset::popCount(void)
	{
		pathMap.Load();
		Presets.clear();
		PresetEntry::longest = 0;
		for (PathMap::iterator i = pathMap.begin(); i != pathMap.end(); ++i)
		{
			ScanPresets(i->first);
		}
		return Presets.size() + 5; // bar, save, delete, save as, load from
	}

	const char *SimplePreset::popName(int n)
	{
		size_t idx = n;
		if (idx < Presets.size()) return Presets[idx].GetGuiName();
		idx -= Presets.size();
		switch (idx)
		{
		case 0:
			return "---------------------------------";
			// Unfortunately there's no way to display a standard divider :(
			//return "==================================";
			break;

		case 1:
			return "\03(c:Nc_HILIGHT)Save as Preset...";
			break;

		case 2:
			return "\03(c:Nc_HILIGHT)Delete Preset...";
			break;

		case 3:
			return "\03(c:Nc_HILIGHT)Load Settings from File...";
			break;

		case 4:
			return "\03(c:Nc_HILIGHT)Save Settings to File...";
			break;


		default:
			break;
		}
		return "";
	}

	LWError SimplePreset::Setup(XPanel& panel, int control)
	{
		xpanel.setID(panel.getID());
		controlID = control;
		LWXPanelHint hint[] =
		{	
			// Set options
			XpPOPFUNCS(controlID, PopCount, PopName),
			XpPOPCMDFUNC(controlID, PopCommand),
			XpEND
		};
		panel.Hint(hint);
		panel.setUserData(this, control);
		return 0;
	}

	void SimplePreset::popCommand(int cid, int i_cmd) 
	{
		size_t cmd(i_cmd);
		if (cid != controlID) return;
		LWMessage lwm;
		if (cmd < Presets.size())
		{
			doLoadPreset(Presets[cmd].GetFileName());
			xpanel.ViewRefresh();
		}
		else
		{
			cmd -= Presets.size();
			switch (cmd)
			{
			case 1:	doSavePreset();	break;

			case 2:	doDeletePreset();	break;

			case 3:
				doLoadFromPreset();
				break;

			case 4:	doSaveAsPreset();	break;

			default:	break;
			}
		}
	}

	enum {ID_PSEL = 0x0800, ID_TYPE, ID_TEST};

	void SimplePreset::doLoadPreset(const char *filename, bool quiet)
	{
		LWMessage lwm;
		File in(filename, File::FILE_LOAD);
		LoadState ls = in.getLoadState();
		if (ls.isValid())
		{
			std::string name;
			LWError err = SimplePreset::LoadPreset(ls, name);
			if (err)
			{
				lwm.Error(std::string("Error loading Preset: ") + err);
				return;
			}
			if (!quiet) lwm.Info(std::string("Loaded Preset: ") + name);
		}
	}

	static bool isIllegalChar(char c)
	{
		switch (c)
		{
			case '/':
			case '\\':
			case '\"':
			case '>':
			case '<':
			case '|':
			case '*':
			case '?':
			case ':':
			case '~':
			case '%':
			case '+':
			return true;

		default:
			return false;
		}
	}

	void SimplePreset::doSaveAsPreset()
	{
		bool loop = false;
		do 
		{
			if (saveReq.Post())
			{
				lwpp::InterfaceInfo ii;
				if (ii.showWarning())
				{					
					FILE *outfile = fopen(saveReq.getFullPath(), "r");
					if (outfile)
					{
						fclose(outfile);
						int ret = LWMessage::YesNoCan("Save Preset", "Are you sure you want to overwrite preset?", saveReq.getFullName());
						switch (ret)
						{
						case 0: // CANCEL
							return;
							break;

						case 1: // NO
							loop = true;
							continue; 
							break;

						case 2: // YES
							break;

						default:
							break;
						}	
					}
				}
				if (!loop)
				{		
			    File out(saveReq.getFullPath(), File::FILE_SAVE);
			    lwpp::SaveState ss = out.getSaveState();
			    SavePreset(ss, saveReq.getFullPath());
			    LWMessage::Info(std::string("Saved Preset As: ") + saveReq.getFullPath());
				}
			}
		} 
		while (loop);
	}

	void SimplePreset::doLoadFromPreset()
	{
		if (loadReq.Post())
		{
			doLoadPreset(loadReq.getFullPath());
		}
	}


	void SimplePreset::doSavePreset()
	{	
		LWXPanelControl save_ctrl[] = 
		{
			{ID_PSEL, "Preset", "sPresetText"},
			//{ID_TYPE, "Type", "iChoice"},
			{ID_TYPE, "Type", "iPopChoice"},
			{ 0 }
		};

		/* matching array of data descriptors */
		LWXPanelDataDesc save_data[] = {
			{ ID_PSEL, "", "string" },
			{ ID_TYPE, "", "integer" },
			{ 0 }
		};
		LWXPanelHint hint[] = 
		{
			XpLABEL( 0, "Save Preset"),
			XpPOPFUNCS(ID_PSEL, Presets.PopCount, Presets.PopName),
			XpPOPFUNCS(ID_TYPE, pathMap.PopCount, pathMap.PopName),
			//XpSTRLIST(ID_TYPE, TypeLabels),
			XpEND
		};
		
		std::string pres_name("none");
		int pres_type = 1;
		bool loop = false;
		do
		{
			loop = false;
			XPanel xpan(LWXP_FORM, save_ctrl);
			if (xpan.isValid())
			{
				xpan.Describe(save_data);
				xpan.Hint(hint);
				xpan.setForm(ID_PSEL, pres_name.c_str());
				xpan.setForm(ID_TYPE, pres_type);
				xpan.setUserData(&Presets, ID_PSEL);
				xpan.setUserData(&pathMap, ID_TYPE);
				if (xpan.Post() == 1)
				{
					// check if we're overwriting and ask...
					xpan.getForm(ID_PSEL, pres_name);
					xpan.getForm(ID_TYPE, pres_type);
					//std::string pres_path = makeFilePath(pres_type);
					std::string pres_file = makeFileName(pres_name, pres_type);
					lwpp::InterfaceInfo ii;
					if (ii.showWarning())
					{					
						FILE *outfile = fopen(pres_file.c_str(), "r");
						if (outfile)
						{
						  fclose(outfile);
							int ret = LWMessage::YesNoCan("Save Preset", "Are you sure you want to overwrite preset?", pres_file.c_str());
							switch (ret)
							{
								case 0: // CANCEL
									return;
									break;

								case 1: // NO
									loop = true;
									continue; 
									break;

								case 2: // YES
									break;

								default:
									break;
							}	
						}
					}
					makeAllDirectories(pres_file.c_str());
					File out(pres_file.c_str(), File::FILE_SAVE);
          if (out.isValid())
          {
            lwpp::SaveState ss = out.getSaveState();
            SavePreset(ss, pres_name);
            LWMessage::Info(std::string("Preset saved: ") + pres_file);
          }
          else
          {
            LWMessage::Error(std::string("An error occured saving ") + pres_file);
          }
				}
			}
		} while (loop);
	}

	void SimplePreset::doDeletePreset(void)
	{	

		if (Presets.empty()) return;
		LWXPanelControl delete_ctrl[] = 
		{
			{ID_PSEL, "Preset", "iPopChoice"},
			{ 0 }
		};

		/* matching array of data descriptors */
		LWXPanelDataDesc delete_data[] = {
			{ ID_PSEL, "", "integer" },
			{ 0 }
		};
		LWXPanelHint hint[] = 
		{
			XpLABEL( 0, "Delete Preset"),
			XpPOPFUNCS(ID_PSEL, Presets.PopCount, Presets.PopName),
			XpEND
		};

		XPanel xpan(LWXP_FORM, delete_ctrl);
		if (xpan.isValid())
		{
			xpan.Describe(delete_data);
			xpan.Hint(hint);
			xpan.setForm(ID_PSEL, 0);
			xpan.setUserData(&Presets, ID_PSEL);
			PresetCollection::showDecorated = true;
			if (xpan.Post() == 1)
			{
				PresetCollection::showDecorated = false;
				// check if we're overwriting and ask...
				int index;
				xpan.getForm(ID_PSEL, index);
				std::string pres_file = Presets[index].GetFileName();
				lwpp::InterfaceInfo ii;
				if (ii.showWarning())
				{
					FILE *outfile = fopen(pres_file.c_str(), "r");
					if (outfile)
					{	
						int ret = LWMessage::YesNo("Save Preset", "Are you sure you want to delete the preset?", Presets[index].GetGuiName());
						switch (ret)
						{
							case 0: // NO
								return;
								break;

							case 1: // YES
								break;

							default:
								break;
						}
						fclose(outfile);
					}
				}
				if (_unlink( pres_file.c_str())  == -1 )
				{
					LWMessage::Error("Error Deleting file:", pres_file.c_str());
					return;
				}
			}
			PresetCollection::showDecorated = false;
		}
	}
	
	std::string SimplePreset::makeFileName(std::string name, int type)
	{
		std::ostringstream fname;
		replace_if (name.begin(), name.end(), isIllegalChar, '_');
		fname << makeFilePath(PathMap::PopName(&pathMap, type)) << fSep << name << ".cfg";
		return fname.str();
	}
	
	std::string SimplePreset::makeFilePath(std::string name)
	{
		std::string basedir;
		if (pathMap.find(name) != pathMap.end())
		{
			basedir = pathMap[name];
		}
		else
		{
			DirInfo di(LWFTYPE_CONTENT);
			std::string basedir = di.GetDirectory();
		}
		trimTrail(basedir);
		std::ostringstream fname;
		fname << basedir << fSep << "SimplePresets" << fSep << plug_type << fSep << plug_name;
		return fname.str();
	}
	
	LWError SimplePreset::SavePreset(lwpp::SaveState& ss, std::string name)
	{
		short version(1);
		LWError err = 0;
		ss.WriteBlock(idroot[0], version);
		ss.WriteBlock(idroot[2], name);
		ss.Begin(idroot[1], false);
		if (handler) err = handler->SavePreset(ss);
		ss.End();
		return err;
	}

	LWError SimplePreset::LoadPreset(lwpp::LoadState& ls, std::string &name)
	{
		short version;
		LWError err = 0;
		while (LWID id = ls.Find(idroot))
		{
			switch (id)
			{
				case IO_VERS:	version = ls.readShort(); break;

				case IO_NAME:	ls.read(name); break;

				case IO_PRES:	
					if (handler) err = handler->LoadPreset(ls);
					break;

			default: 
				break;
			}
			ls.End();
		}
		return err;
	}
}

