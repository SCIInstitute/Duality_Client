#ifndef IOS_H
#define IOS_H

#include <string>
#include "IVDA/Vectors.h"

class iOS {
public:
	static std::string GetBundleFilename(const std::string& filename);
	static void Log(const std::string& message);
	static void OpenURL(const std::string& scheme, const std::string& url, bool escapeEncode=false);
	
  static IVDA::FLOATVECTOR2 DetectScreenSize();
	static IVDA::FLOATVECTOR4 DetectScreenRect();
	static float DetectScreenScale();
	static bool DetectIPad();
  static std::string DetectDeviceName();
  static int DetectOrientation();
  static std::string DetectVersionString();
};


#endif // IOS_H

