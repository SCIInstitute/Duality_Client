#include "iOS.h"
#include "SysTools.h"

#import "XQueryComponents.h"
#import <UIKit/UIKit.h>

std::string iOS::GetBundleFilename(const std::string& filename) {
	std::string name = IVDA::SysTools::RemoveExt(IVDA::SysTools::GetFilename(filename));
	std::string type = IVDA::SysTools::GetExt(filename);
	
	
	NSString* nsname = [NSString stringWithFormat:@"%s", name.c_str()];
	NSString* nstype = [NSString stringWithFormat:@"%s", type.c_str()];
	
  NSString* result = [[NSBundle mainBundle] pathForResource:nsname ofType:nstype];
  
  if (result)
    return [result UTF8String];
  else 
    return "";
}

void iOS::Log(const std::string& message) {
	NSLog(@"%s", message.c_str());
}

void iOS::OpenURL(const std::string& scheme, const std::string& url, bool escapeEncode) {
  @autoreleasepool {
  
	// Create query ...
	NSString* nsurl = [NSString stringWithFormat:@"%s", url.c_str()];
  
	// Be careful to always URL encode things like spaces and other symbols that aren't URL friendly
	nsurl =  [nsurl stringByAddingPercentEscapesUsingEncoding: NSUTF8StringEncoding];
	// Now create the URL string ...
	NSString* urlString = [NSString stringWithFormat:@"%s://%@", scheme.c_str(), nsurl];
  
  if (escapeEncode) 
    urlString = [urlString stringByEncodingURLFormat];
  
	// An the final magic ... openURL!
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:urlString]];	
  
  }
}

IVDA::FLOATVECTOR4 iOS::DetectScreenRect() {
	CGRect screenRect = CGRectMake(0.0f, 0.0f, 320.0f, 480.0f);
	if ([UIScreen instancesRespondToSelector:@selector(bounds)]) {
		screenRect = [[UIScreen mainScreen] bounds];
	}
	return IVDA::FLOATVECTOR4(screenRect.origin.x,screenRect.origin.y,
                      screenRect.size.width, screenRect.size.height);
}

IVDA::FLOATVECTOR2 iOS::DetectScreenSize() {
	if (DetectIPad()) {
    return IVDA::FLOATVECTOR2(197,148);
  } else {
    return IVDA::FLOATVECTOR2(51.42, 76.38);    
  }
}


float iOS::DetectScreenScale() {
	float fScale = 1.0f;
	if ([UIScreen instancesRespondToSelector:@selector(scale)]) {
		fScale = [[UIScreen mainScreen] scale];
	}
	return fScale;
}

bool iOS::DetectIPad() {
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= 30200
	return  (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad);
#else
	return false;
#endif
}

std::string iOS::DetectDeviceName() {
  return std::string([[[UIDevice currentDevice] name] UTF8String]);
}

int iOS::DetectOrientation() {
  return int([[UIDevice currentDevice] orientation]);
}

std::string iOS::DetectVersionString() {
  return std::string([[[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"] UTF8String]);
}
