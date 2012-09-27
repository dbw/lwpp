#ifndef LWPP_MESSAGE_H
#define LWPP_MESSAGE_H
#include <lwpp/interface.h>
#include <lwpp/xpanel.h>

namespace lwpp
{
	// constant strings for formatting text, not documented
#define TXT_JUSTIFY_LEFT "\03(j:0)"
#define TXT_JUSTIFY_CENTRE "\03(j:1)"
#define TXT_JUSTIFY_RIGHT "\03(j:2)"

#define CHAR_UP "\03(i:1)"
#define CHAR_DOWN "\03(i:2)"
#define CHAR_UPDOWN "\03(i:3)"
#define CHAR_CHECKMARK "\03(i:4)"
#define CHAR_LEFTRIGHT "\03(i:5)"
#define CHAR_QUAD "\03(i:6)"
#define CHAR_PAN "\03(i:7)"
#define CHAR_PLUS "\03(i:8)"
#define CHAR_MINUS "\03(i:9)"
#define CHAR_CIRCLE "\03(i:10)"
#define CHAR_ELLIPSIS "\03(i:28)"
#define CHAR_COLOUR(x) "\03(c:" #x ")"

//! Wrapped LWMessageFuncs
    DEFINE_GLOBAL(LWMessageFuncs)
	class LWMessage : protected GlobalBase<LWMessageFuncs>
	{
	public:
		LWMessage() {;}
		void static Info(const std::string &a)
		{
			globPtr->info(a.c_str(), 0);
		}
		void static Info(const std::string &a, const std::string &b)
		{
			globPtr->info(a.c_str(),b.c_str());
		}
		void static Error(const std::string &a)
		{
			globPtr->error(a.c_str(), 0);
		}
		void static Error(const std::string &a, const std::string &b)
		{
			globPtr->error(a.c_str(),b.c_str());
		}
		void static Warning(const std::string &a)
		{
			globPtr->warning(a.c_str(), 0);
		}
		void static Warning(const std::string &a, const std::string &b )
		{
			globPtr->warning(a.c_str(),b.c_str());
		}
		int static OkCancel(const std::string &t, const std::string &a, const std::string &b = "")
		{
			return globPtr->okCancel(t.c_str(),a.c_str(),b.c_str());
		}
		int static YesNo(const std::string &t, const std::string &a, const std::string &b = "")
		{
			return globPtr->yesNo(t.c_str(),a.c_str(),b.c_str());
		}
		int static YesNoCan(const std::string &t, const std::string &a, const std::string &b = "")
		{
			return globPtr->yesNoCan(t.c_str(),a.c_str(),b.c_str());
		}
		int static YesNoAll(const std::string &t, const std::string &a, const std::string &b = "")
		{
			return globPtr->yesNoAll(t.c_str(),a.c_str(),b.c_str());
		}
		void static InfoPanel (const char *title, const char *text)
		{
			enum {ID_TEXT = 0xb000};
			LWXPanelControl ctrl_list[] = 
			{
				{ID_TEXT, "sInfo", "string"},
				{ 0 }
			};
			LWXPanelDataDesc data_descrip[] = { { 0 } };
			LWXPanelHint hint[] = 
			{
				XpLABEL( 0, title),
				XpDLOGTYPE(LWXPDLG_OKONLY),
				XpLEFT(ID_TEXT),
				XpEND
			};
			lwpp::XPanel xpanel(LWXP_FORM, ctrl_list);
			if ( xpanel.isValid() ) 
			{
				xpanel.Hint(hint);
				xpanel.Describe(data_descrip);
				xpanel.setForm(ID_TEXT, text);
				if (xpanel.Post() == 1) 
				{
					;
				}
			}
		};
	};

}
#endif // LWPP_MESSAGE_H