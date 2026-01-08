// This include:
#include "IniParser.h"

// Local includes:

// Library includes:
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

bool IniParser::LoadIniFile(const std::string& filename) 
{
    ifstream file(filename);

    if (!file.is_open()) 
    {
        return false;
    }

    string line;
    string currentSection;

    while (getline(file, line)) 
    {
        // Trim whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

        // Skip empty lines and comments
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        // Section header
        if (line.front() == '[' && line.back() == ']') 
        {
            currentSection = line.substr(1, line.size() - 2);
        }
        else 
        {
            // Key-value pair
            size_t equalsPos = line.find('=');
            if (equalsPos == string::npos) continue;

            string key = line.substr(0, equalsPos);
            string value = line.substr(equalsPos + 1);
            string fullKey = MakeKey(currentSection, key);
            dataMap[fullKey] = value;
        }
    }

    file.close();
    return true;
}

std::string IniParser::GetValueAsString(const std::string& iniSection, const std::string& key) const
{
    string mapKey = MakeKey(iniSection, key);
    auto it = dataMap.find(mapKey);
    return it != dataMap.end() ? it->second : "";
}

int IniParser::GetValueAsInt(const std::string& iniSection, const std::string& key) const
{
    string value = GetValueAsString(iniSection, key);
    return !value.empty() ? stoi(value) : 0;
}

float IniParser::GetValueAsFloat(const std::string& iniSection, const std::string& key) const
{
    string value = GetValueAsString(iniSection, key);
    return !value.empty() ? stof(value) : 0.0f;
}

bool IniParser::GetValueAsBoolean(const std::string& iniSection, const std::string& key) const
{
    string value = GetValueAsString(iniSection, key);
    transform(value.begin(), value.end(), value.begin(), ::tolower);
    return (value == "true" || value == "1" || value == "yes" || value == "on");
}

std::string IniParser::MakeKey(const std::string& section, const std::string& key) const
{
    return section + "|" + key;
}