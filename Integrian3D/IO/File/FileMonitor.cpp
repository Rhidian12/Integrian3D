#include "IO/File/FileMonitor.h"

I_DISABLE_WARNING(4005) // warning C4005: 'APIENTRY': macro redefinition
#include <Windows.h>
I_ENABLE_WARNING(4005)

namespace Integrian3D::IO
{
	FileMonitor::FileMonitor(const std::string& Path)
		: Handle{ FindFirstChangeNotificationA(Path.c_str(), false, FILE_NOTIFY_CHANGE_SIZE) }
	{}
}