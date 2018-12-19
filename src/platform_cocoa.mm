#include "platform_cocoa.h"
#include "lwdialog.h"
#include <Cocoa/Cocoa.h>

void cocOpenURL(const std::string &s)
{
    @autoreleasepool {
        NSURL *url = [NSURL URLWithString: [[NSString alloc] initWithCString: s.c_str() encoding: NSISOLatin1StringEncoding]];
		[[NSWorkspace sharedWorkspace] openURL: url];
	}
}

void cocMouseLocation(int &x, int &y)
{
    NSRect screenRect;
    NSArray *screenArray = [NSScreen screens];
    unsigned screenCount = [screenArray count];
    
    NSPoint mouseLoc = [NSEvent mouseLocation]; //get current mouse position
    x = mouseLoc.x;
    y = mouseLoc.y;
    
    for (unsigned index = 0; index < screenCount; index++)
    {
        NSScreen *screen = [screenArray objectAtIndex: index];
        //screenRect = [screen visibleFrame];
        screenRect = [screen frame];
        //bool inside = [screen isMousePoint];
        //if ([screen isMousePoint point: mouseLoc rect: screenRect])
        if (NSPointInRect(mouseLoc, screenRect))
        {
            y = screenRect.size.height - y;
            return;
        }
    }
}

std::string cocGetTempPath()
{
    //NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSString *tmpPath = NSTemporaryDirectory();
	std::string ret = [ tmpPath cStringUsingEncoding: NSUTF8StringEncoding];
    //[pool release];
	return ret;
}

cocFileRequest::cocFileRequest(const int _reqType, const char *_title,
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
    
    NSString *nsExt =  [[NSString alloc] initWithCString: extension encoding: NSISOLatin1StringEncoding];
    NSString *nsfName =  [[NSString alloc] initWithCString: fName encoding: NSISOLatin1StringEncoding];
    // NSURL *dirUrl = [NSURL fileURLWithPath:[[NSString alloc] initWithCString: savePath]];
    NSArray *extensions = [nsExt componentsSeparatedByString:@";"];
    nsExt = nil;
	int runResult = 0;
	switch (reqType)
	{
		case FREQ_SAVE:
        {
            NSSavePanel *save;            
            save = [NSSavePanel savePanel];
            [save setTitle: [[NSString alloc] initWithCString: title encoding: NSISOLatin1StringEncoding]];
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
            [open setTitle: [[NSString alloc] initWithCString: title encoding: NSISOLatin1StringEncoding]];
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
            NSString *nsfName = nil;
			break;
			
	} 
    [pool release];
	return (runResult == NSFileHandlingPanelOKButton);
}
const char *cocFileRequest::getFullName()
{
	return retValue.c_str();
}
