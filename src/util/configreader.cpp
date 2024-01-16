#include "model_common.h"
#include "configreader.h"
#include "Color.h"

void trim(std::string& s)
{
    size_t index = s.find_first_not_of(" \t\n");
    if (index > 0) {
        s.erase(0, index);
    }
    index = s.find_last_not_of(" \t\n");
    if (index >= 0 && index < s.length() - 1)  {
        s.erase(index + 1, s.length() - index);
    }
}

ConfigReader::ConfigReader(string filename)
{
	ifstream filestream(filename.c_str());
	if (!filestream) {
		cerr << "[ConfigReader] File \"" << filename << "\" not found" << endl;
		return;
	}

	string section = "default";
	string line;
	map<string,string>* currentSection = NULL;
	while ( getline(filestream, line, '\n' ) ) {

		if (line.size() == 0) continue;
		if (line[0]=='[' && line[line.size()-1] == ']') {
			section = line.substr(1, line.size()-2);
			transform(section.begin(), section.end(), section.begin(), tolower);
			trim(section);
			//cout << "[ConfigReader] reading section \"" << section << "\"" << endl;
		} else {
			int pos = line.find('=');
			if (pos != string::npos) {
				string title = line.substr(0, pos);
				string content = line.substr(pos + 1);
				trim(title);
				transform(title.begin(), title.end(), title.begin(), tolower);
				trim(content);
				(*this)[section][title] = content;
			} else {
				//cout << "[ConfigReader] incorrect data in \"" << section << "\" : " << line << endl;
			}
		}
	}
	filestream.close();

	/*
	for (map<string,map<string,string>>::iterator ci = begin(); ci != end(); ++ci) {
		cout << "section [" << ci->first << "]" << endl;
		for (map<string,string>::iterator si = ci->second.begin(); si != ci->second.end(); ++si) {
			cout << " - " << si->first << " = " << si->second << endl;
		}
	}
	cout << endl;
	*/
}

bool ConfigReader::read(map<string, string>& config, const string& name, string& value)
{
	if(config.find(name) != config.end()) {
		value = config[name];
	} else {
		return false;
	}
	return true;
}

void ConfigReader::readFont(const string& line, string& fontName, int& fontSize)
{

	string::size_type idx = line.rfind(',');

	if (idx != string::npos) {
		string size = line.substr(idx+1);
		iss.str(size);
		if (!(iss >> fontSize)) {
			cerr << "[ConfigReader] invalid font size " << size << " reading value = " << line << endl;
		}
		fontName = line.substr(0,idx);
		iss.clear();
	} else {
		fontName = line;
	}
}

void ConfigReader::readColor(const string& line, Color& color)
{
	iss.str(line.c_str());
	if (!(iss >> color.values[0] >> color.values[1] >> color.values[2] >> color.values[3])) {
		cerr << "[ConfigReader] invalid color reading value = " << line << endl;
	}
	iss.clear();
}

bool ConfigReader::read(map<string, string>& config, const string& name, int& value)
{
	bool ret = false;
	if(config.find(name) != config.end()) {
		iss.str(config[name].c_str());
		if (!(iss >> value)) {
			cerr << "[ConfigReader] invalid int " << name << " reading value = " << config[name] << endl;
		} else {
			ret =true;
		}
		iss.clear();
	}
	return ret;
}
bool ConfigReader::read(map<string, string>& config, const string& name, float& value)
{
	bool ret = false;
	if(config.find(name) != config.end()) {
		iss.str(config[name].c_str());
		if (!(iss >> value)) {
			cerr << "[ConfigReader] invalid float " << name << " reading value = " << config[name] << endl;
		} else {
			ret =true;
		}
		iss.clear();
	}
	return ret;
}
bool ConfigReader::read(map<string, string>& config, const string& name, char& value)
{
	bool ret = false;
	if(config.find(name) != config.end()) {
		iss.str(config[name].c_str());
		if (!(iss >> value)) {
			cerr << "[ConfigReader] invalid char " << name << " reading value = " << config[name] << endl;
		} else {
			ret =true;
		}
		iss.clear();
	}
	return ret;
}
bool ConfigReader::read(map<string, string>& config, const string& name, bool& value)
{
	bool ret = false;
	if(config.find(name) != config.end()) {
		string lowName = config[name];
		transform(lowName.begin(), lowName.end(), lowName.begin(), tolower);

		if (lowName.compare("false") == 0) {
			value = false;
			ret = true;
		} else if (lowName.compare("true") == 0) {
			value = true;
			ret = true;
		} else {
			iss.str(config[name].c_str());
			if (!(iss >> value)) {
				cerr << "[ConfigReader] invalid bool " << name << " reading value = " << config[name] << endl;
			} else {
				ret =true;
			}
			iss.clear();
		}
	}
	return ret;
}

bool ConfigReader::read(map<string, string>& config, const string& name, Color& color)
{
	bool ret = false;
	if(config.find(name) != config.end()) {
		iss.str(config[name].c_str());
		if (!(iss >> color.values[0] >> color.values[1] >> color.values[2] >> color.values[3])) {
			cerr << "[ConfigReader] invalid color " << name << " reading value = " << config[name] << endl;
		} else {
			ret =true;
		}
		iss.clear();
	}
	return ret;
}