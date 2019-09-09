#ifndef _CONFIGURATION
#define _CONFIGURATION

#include <string>
#include <fstream>
#include <map>

namespace CoreLib
{
	class Configuration
	{
	public:
		Configuration();
		~Configuration();
		void setFileName(std::string fname);
		bool loadConfiguration(const std::string &configFile);
		bool loadConfiguration();
		std::string getValue(const std::string &section, const std::string &settingKey, const std::string defval = "");
		bool isSection(const std::string &section);
	private:
		void addSection(std::string &str, const std::map<std::string, std::string> &list);
		std::map<std::string, std::map<std::string, std::string>> _Configuration;
		std::string _ConfigFileName;
	};
}
#endif
