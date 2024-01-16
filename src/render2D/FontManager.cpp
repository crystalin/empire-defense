#include "render2D_common.h"
#include "FontManager.h" 
#include "RessourceManager.h" 
#include "configreader.h"

vector<Font> FontManager::fonts;
FT_Library	FontManager::freetypeLibrary;


bool Font::createFont(const std::string &font_file, int font_size)
{
	FT_Face face;

	int error = FT_New_Face( FontManager::getFTLib(), font_file.c_str(), 0, &face );
	if ( error == FT_Err_Unknown_File_Format ) {
		// the font file could be opened and read, but it appears 
		// that its font format is unsupported
		cerr << "ERROR: unsupported font file format (file : " << font_file << endl;
		return NULL;
	}
	else if ( error ) {
		// another error code means that the font file could not
		// be opened or read, or simply that it is broken...
		cerr << "ERROR: could not load font file " << font_file << endl;
		return NULL;
	}
	cout << "\t-loaded font: " << font_file << " [" << font_size << "]" << endl;

	// set the font size
	error = FT_Set_Char_Size( face, /* handle to face object */ 
								0, /* char_width in 1/64th of points */
								font_size*64, /* char_height in 1/64th of points */
								0, /* horizontal device resolution (0 for default 72dpi) */
								0 ); /* vertical device resolution (0 for default 72dpi) */

	// create the texture with the fonts
	int texW = 512;
	int texH = 512;
	unsigned char* pixels= new unsigned char[texW*texH*4];

	int xpos = 2;
	int ypos = 2;
	int maxH = 0;
	int maxHFont = 0;
	characters.clear();
	startChar = 20;
	for (int i=startChar; i<=255; ++i)
	{
		unsigned char char_id = (unsigned char)i;
		
		// load glyph image into the slot (erase previous one)
		error = FT_Load_Char( face, char_id, FT_LOAD_RENDER );
		if ( error )
			continue; /* ignore errors */
		FT_GlyphSlot slot = face->glyph;

		// check where we should draw the new character
		if (xpos + slot->bitmap.width+1 > texW-1)
		{
			xpos = 2;
			ypos += maxH;
			maxH = 0;
		}
		
		// now, draw to our target surface
		int bitmapPos = 0;
		for (int dy=0; dy<slot->bitmap.rows; ++dy) {
			for (int dx=0; dx<slot->bitmap.width; ++dx)
			{
				int x = xpos + dx;
				int y = ypos + (slot->bitmap.rows - dy - 1);
				pixels[4*(y*texW + x)  ] = 255;//slot->bitmap.buffer[bitmapPos + dx];
				pixels[4*(y*texW + x)+1] = 255;//slot->bitmap.buffer[bitmapPos + dx];
				pixels[4*(y*texW + x)+2] = 255;//slot->bitmap.buffer[bitmapPos + dx];
				pixels[4*(y*texW + x)+3] = slot->bitmap.buffer[bitmapPos + dx];//0;
			}
			bitmapPos += abs(slot->bitmap.pitch);
		}
		
		Character charInfo;
		charInfo.character = char_id;
		charInfo.left = xpos;
		charInfo.right = xpos + slot->bitmap.width;
		if (char_id == ' ')
			charInfo.right = xpos + int(font_size/2.6);
		charInfo.horiBearingX = face->glyph->metrics.horiBearingX/64;
		charInfo.horiBearingY = face->glyph->metrics.horiBearingY/64;
		charInfo.horiAdvance  = face->glyph->metrics.horiAdvance/64;
		charInfo.top = ypos;
		charInfo.bottom = ypos + slot->bitmap.rows;
		
		// get character metrics
		charInfo.fw = charInfo.right-charInfo.left; // font width
		charInfo.fh = charInfo.bottom-charInfo.top; // font height
		charInfo.tx = (float)charInfo.left/texW;
		charInfo.ty = (float)(charInfo.top)/texH;
		charInfo.tw = (float)charInfo.fw/texW;
		charInfo.th = (float)charInfo.fh/texH;

		characters.push_back(charInfo);
		
		// increment rendering position
		xpos += slot->bitmap.width + 1;
		if ((slot->bitmap.rows+1) > maxH)
			maxH = slot->bitmap.rows+1;
		if (maxH > maxHFont)
			maxHFont = maxH;
	}
	texture = new Texture(texW, texH, GL_RGBA, 4, GL_UNSIGNED_BYTE, &pixels[0]);
	texture->request();

	middleLineHeight = characters['A'].fh / 2;
	cout << "middleLineHeight : " << middleLineHeight << endl;
	lineHeight = maxHFont;

	return true;
}

void Font::renderString(int x, int y, const string &str, Align align) const
{
	// set the texture with the font
	texture->bind();

	int colorCount = 0; // every 4 number we change the color
	float colors[4] = {0}; // The format for the color is \x for 0.x

	glBegin(GL_QUADS);
	int xpos = x;
	if (align == middle) {
		xpos -= stringLength(str) / 2;
	} else if (align == right) {
		xpos -= stringLength(str);
	}
	int ypos = y-middleLineHeight;

	for (size_t i=0; i<str.length(); ++i)
	{
		// handle new line characters
		if (str[i] == '\n')
		{
			xpos = x;
			if (align == middle) {
				xpos -= stringLength(str,i+1) / 2;
			} else if (align == right) {
				xpos -= stringLength(str,i+1);
			}
			ypos -= lineHeight;
			continue;
		}

		unsigned char cc = str[i];
		if (cc > 255 ) {
			continue;
		}
		if (cc < startChar) {
			cout << "value : " << (int)(cc); 
			if (cc <= 10) { 
				colors[(colorCount++)%4] = (int)(cc)/10.0f;
				cout << " new color " << (int)(cc); 
				if (colorCount==4) {
					colorCount = 0;
					glColor4fv(colors);
				}
			}
			continue;
		}
		const Character& charInfo = characters[cc - startChar];

		// render the character
		int left = xpos+charInfo.horiBearingX;
		int right = xpos+charInfo.fw+charInfo.horiBearingX;
		int top = ypos + charInfo.horiBearingY;
		int bottom = ypos-charInfo.fh + charInfo.horiBearingY;

		glTexCoord2f(charInfo.tx,charInfo.ty+charInfo.th);				
		glVertex2f((float)left, (float)top);
		glTexCoord2f(charInfo.tx+charInfo.tw,charInfo.ty+charInfo.th);	
		glVertex2f((float)right, (float)top);
		glTexCoord2f(charInfo.tx+charInfo.tw,charInfo.ty);				
		glVertex2f((float)right, (float)bottom);
		glTexCoord2f(charInfo.tx,charInfo.ty);							
		glVertex2f((float)left, (float)bottom);

		xpos += charInfo.horiAdvance;
	}
	glEnd();
}

void Font::renderStringMultiline(int x, int y, int lineWidth, const string &str) const
{
	int curLinePos = 0;
	int ypos = y-middleLineHeight;
	size_t i = 0;

	while (i < str.length())
	{
		// scan the remaining string until the first non-space, measuring width
		for (; i<str.length(); ++i)
		{
			unsigned char cc = str[i];

			if (!isspace(cc))//isprint(str[i]))
				break;

			if (cc == '\n' || (cc >= startChar && 
				curLinePos + characters[cc - startChar].horiAdvance > lineWidth))
			{
				ypos -= lineHeight;
				curLinePos = 0;
				++i;
				cc = str[i];
				break;
			}
			if (cc == '\t') {
				curLinePos += tabSize*characters[' ' - startChar].horiAdvance;
			}
			else {	
				// add an empty entry for non-printable characters
				if (cc - startChar < 0)
					continue;
				curLinePos += characters[cc - startChar].horiAdvance;
			}
		}
		// find the next word
		string word = "";
		for (; i<str.length(); ++i)
		{
			unsigned char cc = str[i];
			if (isspace(cc) )
				break;
			word += cc;
		}
		// get the length of the word
		int wordLen = stringLength(word);

		// if the word gets beyond the end of this line, start a new line.
		// if we are at the beginning of the line but still the word doesnt fit, render
		// it anyway on this line.
		if (curLinePos != 0 && curLinePos + wordLen > lineWidth)
		{
			curLinePos = 0;
			ypos -= lineHeight;
		}
		// render the word
		renderString(x+curLinePos, ypos, word);
		curLinePos += wordLen;
	}
}

int Font::stringLength(const string &str, size_t begin) const
{
	int len = 0;
	for (size_t i=begin; i<str.length(); ++i)
	{
		unsigned char cc = str[i];
		if (cc =='\n') {
			break;
		}
		if (cc < startChar || cc > 255)
			continue;
		const Character& charInfo = characters[cc - startChar];
		len += charInfo.horiAdvance;
	}
	return len;
}

void FontManager::init(ConfigReader& configReader) {

	int error = FT_Init_FreeType( &freetypeLibrary );
	if ( error ) {
		cout << "ERROR: failed to initialize FreeType\n";
		return;
	}

	map<string,map<string,string>>::iterator ci = configReader.find("fonts");
	if (ci != configReader.end()) {
		map<string,string>& fontsConfig = ci->second;

		for(map<string,string>::iterator it = fontsConfig.begin(); it != fontsConfig.end(); it++) {
			const string& name = it->first;
			const string& fontLine = it->second;
			
			string fontName;
			int fontSize = 16;
			configReader.readFont(fontLine,fontName,fontSize);

			fonts.push_back(Font(name));
			Font& font = fonts.back();
			if (!font.createFont("../../data/fonts/"+fontName,fontSize)) {
				fonts.pop_back();
			}
		}
	}
}

void FontManager::destroy()
{
	for_each(fonts.begin(), fonts.end(), mem_fun_ref(&Font::deleteFont));
}


const Font* FontManager::getFont(const string& name) {

	if (fonts.empty()) {
		return NULL;
	}

	vector<Font>::iterator ci = find(fonts.begin(), fonts.end(), Font(name));
	if (ci != fonts.end()) {
		return &(*ci);
	} else {
		return &fonts.front();
	}
}