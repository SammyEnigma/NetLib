#ifndef _CONFIGURATION
#define _CONFIGURATION

#include "GenericString.h"
#include "Directory.h"
#include "Map.h"

namespace CoreLibrary
{
	class Configuration
	{
	public:
		Configuration();
		~Configuration();
		void setFileName(GenericString fname);
		bool loadConfiguration();
		bool loadCustomConfiguration(const GenericString &configFile);
		GenericString getValue(const GenericString &section, const GenericString &settingKey, const GenericString defval = "");
		bool isSection(const GenericString &section);
	private:
		bool loadConfiguration(const GenericString &configFile);
		void addSection(GenericString &str, const Map<GenericString, GenericString> &list);
		Map<GenericString, Map<GenericString, GenericString>> _ConfigurationMap;
		GenericString _ConfigFileName;
	};
}
#endif
