#ifndef __CONFIG_READER_H__
#define __CONFIG_READER_H__

struct Color;

using namespace std;

class EDMODEL_API ConfigReader : public map<string,map<string,string>>
{
public :
	ConfigReader(string filename);
	bool read(map<string, string>& configSection,const string& name, string& value);
	bool read(map<string, string>& configSection,const string& name, int& value);
	bool read(map<string, string>& configSection,const string& name, float& value);
	bool read(map<string, string>& configSection,const string& name, char& value);
	bool read(map<string, string>& configSection,const string& name, bool& value);
	bool read(map<string, string>& configSection,const string& name, Color& color);
	
	void readFont(const string& line, string& fontName, int& fontSize);
	void readColor(const string& line, Color& color);

protected :
	istringstream iss;
};

#endif