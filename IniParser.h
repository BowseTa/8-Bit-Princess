#ifndef INIPARSER_H
#define INIPARSER_H

#include <map>
#include <string>

class IniParser
{
	// Member methods:
public:
	bool LoadIniFile(const std::string& filename);

	std::string GetValueAsString(const std::string& iniSection, const std::string& key) const;
	int GetValueAsInt(const std::string& iniSection, const std::string& key) const;
	float GetValueAsFloat(const std::string& iniSection, const std::string& key) const;
	bool GetValueAsBoolean(const std::string& iniSection, const std::string& key) const;

protected:
	// Internal method to build composite key from section and item name
	std::string MakeKey(const std::string& section, const std::string& key) const;

private:

	// Member data:
public:

protected:
	// Map with string keys and string data
	std::map<std::string, std::string> dataMap;

private:
};

#endif // INIPARSER_H