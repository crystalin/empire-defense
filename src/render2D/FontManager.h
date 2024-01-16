#ifndef __FONT_MANAGER__
#define __FONT_MANAGER__

#include "Texture.h"

class ConfigReader;


class Character {
public:
	int character;
	int left, right, top, bottom;
	int horiBearingX;
	int horiBearingY;
	int horiAdvance;

	int fw;
	int fh;
	float tx;
	float ty;
	float tw;
	float th;
};

class Font {
private:
	string				name;
	Texture*			texture;
	vector<Character>	characters;
	int					startChar;	// the first char in the letters
	int					lineHeight;
	int					middleLineHeight; // half of the 'a' height
	int					tabSize;	// size of tabs in spaces (not pixels)
public:

	enum Align { left, middle, right };

	Font(const string& name) : name(name), tabSize(5) {}

	bool createFont(const string &font_file, int font_size);
	void deleteFont() {texture->release();}
	void renderString(int x, int y, const string &str, Align align = left) const;
	void renderStringMultiline(int x, int y, int lineWidth, const string &str) const;
	int stringLength(const string &str, size_t begin = 0) const;

	int getLineHeight() const { return lineHeight; }

	bool operator==(const Font& font) { return font.name == name; }
	

};

class FontManager
{
private:
	static vector<Font> fonts;
	static FT_Library	freetypeLibrary;
public:

	static void init(ConfigReader& configReader);
	static void destroy();
	static FT_Library	getFTLib()	{ return freetypeLibrary; }

	static const Font* getFont(const string& name);
	
};

#endif
