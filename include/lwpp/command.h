#ifndef LWPP_COMMAND_H
#define LWPP_COMMAND_H

#include <lwpp/global.h>
#include <string>

typedef int LWCommandFunc (const char*);

namespace lwpp
{
	//! Send a Command to layout during any context
	/*!
	* @warning This is based on an undocumented global and may fail
	*/
	inline int SendCommand(const char *cmdname, const char *fmt, ... )
	{
		static char cmd[ 512 ], arg[ 512 ];
		//LWCommandFunc *lwcom = 0;
		static LWCommandFunc *lwcom = (LWCommandFunc *) lwpp::SuperGlobal (LWPP_COMMANDFUNC_GLOBAL, GFUSE_TRANSIENT);
		//if ((lwcom = (LWCommandFunc *) lwpp::SuperGlobal (LWPP_COMMANDFUNC_GLOBAL, GFUSE_TRANSIENT)) != 0)
		if (lwcom)
		{
			if ( fmt )
			{
				va_list ap;
				va_start( ap, fmt );
				vsprintf( arg, fmt, ap );
				va_end( ap );
				sprintf( cmd, "%s %s", cmdname, arg );
				//lwpp::LWMessage::Info("Command:", cmd);
				return (lwcom)( cmd );
			}
			else
			{
				//message->info("Command:", cmdname);
				return (lwcom)( cmdname );
			}
		}
		return AFUNC_BADGLOBAL;
	}
	inline int SendCommand(const std::string cmd)
	{
#ifdef _DEBUG
		lwpp::dostream dout;
		dout << "Command: " << cmd << std::endl;
#endif
		//LWCommandFunc *lwcom = 0;
		static LWCommandFunc *lwcom = (LWCommandFunc *) lwpp::SuperGlobal (LWPP_COMMANDFUNC_GLOBAL, GFUSE_TRANSIENT);
		//if ((lwcom = (LWCommandFunc *) lwpp::SuperGlobal (LWPP_COMMANDFUNC_GLOBAL, GFUSE_TRANSIENT)) != 0)
		if (lwcom)
		{
				return (lwcom)( cmd.c_str() );
		}
		return AFUNC_BADGLOBAL;
	}
}

#endif // LWPP_COMMAND_H