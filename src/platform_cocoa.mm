#include "platform_cocoa.h"
#include "lwdialog.h"
#include <Cocoa/Cocoa.h>

void cocOpenURL(const std::string &s)
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSURL *url = [NSURL URLWithString: [[NSString alloc] initWithCString: s.c_str()]];
	[[NSWorkspace sharedWorkspace] openURL: url];
	[pool release];
}

std::string cocGetTempPath()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSString *tmpPath = NSTemporaryDirectory();
	std::string ret = [ tmpPath cStringUsingEncoding: NSUTF8StringEncoding];
	[pool release];
	return ret;
}

cocFileRequest::cocFileRequest(const int _reqType, char *_title,
							   const char *fileType, const char *ext, const char *baseName,
							   const char *path, const char *_baseDirType, int bufLen)
: title(_title),
reqType(_reqType),
extension(ext),
savePath(path),
fName(baseName)
{
    
}

bool cocFileRequest::Post()
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
    NSString *nsExt =  [[NSString alloc] initWithCString: extension];
    NSString *nsfName =  [[NSString alloc] initWithCString: fName];
    // NSURL *dirUrl = [NSURL fileURLWithPath:[[NSString alloc] initWithCString: savePath]];
    NSArray *extensions = [nsExt componentsSeparatedByString:@";"];
	int runResult = 0;
	switch (reqType)
	{
		case FREQ_SAVE:
        {
            NSSavePanel *save;            
            save = [NSSavePanel savePanel];
            [save setTitle: [[NSString alloc] initWithCString: title]];
            [save setAllowsOtherFileTypes: true];
          //[save setDirectoryURL: dirUrl];
            [save setAllowedFileTypes:extensions];
          //[save setNameFieldStringValue: nsfName];
            runResult = [save runModalForDirectory:NSHomeDirectory() file:nsfName];
            if (runResult == NSFileHandlingPanelOKButton)
            {
                retValue = [ [[save URL] path] cStringUsingEncoding: NSUTF8StringEncoding];
            }
        }
            break;
            
		case FREQ_LOAD:
        {
            NSOpenPanel *open;
            open = [NSOpenPanel openPanel];
            [open setTitle: [[NSString alloc] initWithCString: title]];
            [open setCanChooseFiles: true];
            [open setCanChooseDirectories: false];
            [open setAllowedFileTypes:extensions];
          //[open setNameFieldStringValue: nsfName];
            
          //[open setDirectoryURL: dirUrl];            
            runResult = [open runModalForDirectory:NSHomeDirectory() file:nsfName];
            if (runResult == NSFileHandlingPanelOKButton)
            {
                NSArray *URLs = [open URLs];
                retValue = [[[URLs objectAtIndex:0 ] path] cStringUsingEncoding: NSUTF8StringEncoding];
            }
        }
            break;
			
		default:
			break;
			
	} 
    [pool release];
	return (runResult == NSFileHandlingPanelOKButton);
}
const char *cocFileRequest::getFullName()
{
	return retValue.c_str();
}