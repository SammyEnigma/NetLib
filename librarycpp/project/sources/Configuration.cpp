#include "Configuration.hpp"
#include <stdio.h>

#if defined(_WIN32) || defined(WIN32) || defined (_WIN64) || defined (WIN64)
    #include <memory.h>
    #include <direct.h>
    #define getcwd(ptr,n) _getcwd(ptr,n)
    #define chdir(str) _chdir(str)
#else
    #include <unistd.h>
    #include <string.h>
#endif

namespace CoreLibrary
{
	Configuration::Configuration()
	{
		_ConfigFileName = "";
	}

	Configuration::~Configuration()
	{
	}

	void Configuration::setFileName(GenericString fname)
	{
		_ConfigFileName = fname;
	}

	bool Configuration::isSection(const GenericString &section)
	{
		Map<GenericString, GenericString> *subsection = _ConfigurationMap.value(section);

		if (subsection == nullptr)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	GenericString Configuration::getValue(const GenericString &section, const GenericString &settingKey, const GenericString defval)
	{
		GenericString str;

		Map<GenericString, GenericString> *subsection = _ConfigurationMap.value(section);
		if (subsection == nullptr)
		{
			return defval;
		}
		else
		{
			GenericString *value = subsection->value(settingKey);

			if (value == nullptr)
			{
				return defval;
			}
			str = value->buffer();
		}
		return str;
	}

	bool Configuration::loadCustomConfiguration(const GenericString &configFile)
	{
		return loadConfiguration(configFile);
	}

	bool Configuration::loadConfiguration()
	{
		char filepathbuffer[1024];

		memset((char*)&filepathbuffer[0], 0, 1024);
		getcwd(&filepathbuffer[0], 1024);

		Directory standarddir(filepathbuffer);
		GenericString stdpdir = standarddir.getParentDirectory();
		stdpdir += "/etc/";
		stdpdir += _ConfigFileName;

		if (!loadConfiguration(stdpdir))
		{
			memset((char*)&filepathbuffer[0], 0, 1024);
			getcwd(&filepathbuffer[0], 1024);

			Directory customdir(filepathbuffer);
			GenericString custdir = customdir.getParentDirectory();
			custdir += "/config/";
			custdir += _ConfigFileName;

			if (!loadConfiguration(custdir))
			{
				return false;
			}
		}

		return true;
	}

	bool Configuration::loadConfiguration(const GenericString &configFile)
	{
		GenericString line, leftstr, rightstr;
		List<GenericString> linelist;

		FILE *fp = fopen(configFile.buffer(), "r");
		char *ptr = nullptr;
		char buffer[2048] = { 0 };

		if (fp == nullptr)
		{
			return false;
		}

		// Following is a Windows INI style configuration file parsing algorithm
		// The first iteration only loads relevent lines from as a list of strings

		while (true)
		{
			memset((char*)&buffer[0], 0, 2048);

			ptr = fgets((char*)&buffer[0], 2048, fp);

			if (ptr == nullptr || feof(fp))
			{
				break;
			}

			line = ptr;
			line.trim();

			if (line.length() < 1 || line.getAt(0) == ';' || line.getAt(0) == '#')
			{
				//Skip comment or blank lines;
				continue;
			}

			linelist.append(line);

			//if(!isalnum(line.getAt(0)))
			//{
			//    if(line.getAt(0)=='[' && line.getAt(line.length()-1)==']')
			//    {
			//        //Section header
			//        linelist.append(line);
			//    }
			//    //Garbage or Invalid line
			//    continue;
			//}
			//else
			//{
			//    //Normal line
			//
			//}
		}

		// The file can be closed off
		fclose(fp);

		//Now we would iterate the string list and segregate key value pairs by section groups
		GenericString curSecHeader = "";
		Map<GenericString, GenericString> kvlist;

		for (int i = 0; i != linelist.count(); i++)
		{
			line = *linelist.getAt(i);
			//Section header line
			if (line.getAt(0) == '[' && line.getAt(line.length() - 1) == ']')
			{
				//Check whether this is the first instance of a section header
				if (_ConfigurationMap.count() < 1)
				{
					//Don't need to do anything
					if (curSecHeader.length() < 1)
					{
					}
					else
					{
						//We reach here when a section is being read for the first time
						addSection(curSecHeader, kvlist);
					}
				}
				else
				{
					//Before staring a new section parsing we need to store the last one
					addSection(curSecHeader, kvlist);
				}

				//Store the string as current section header and clear the key value list
				curSecHeader = line;
				kvlist.clear();
			}
			else
			{
				leftstr = "";
				rightstr = "";
				line.getKeyValuePair(leftstr, rightstr, "=");
				kvlist.insert(leftstr.buffer(), rightstr.buffer());
			}
		}
		addSection(curSecHeader, kvlist);
		return true;
	}

	void Configuration::addSection(GenericString &str, const Map<GenericString, GenericString> &list)
	{
		str.remove('[');
		str.remove(']');
		_ConfigurationMap.insert(str, list);
	}
}
