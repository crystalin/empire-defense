#ifndef __COLOR_MANAGER__
#define COLOR_MANAGER

class ConfigReader;
struct Color;

class ColorManager
{
public:

	static void init(ConfigReader& configReader);
	static void destroy();

	static const Color& getTeamColor(int index) { return teamColors[index%teamColors.size()]; }
	static const Color& getEffectColor(int index) { return effectColors[index%effectColors.size()]; }

protected:
	ColorManager();
	ColorManager(const ColorManager& colorManager); 
	~ColorManager();

	static vector<Color> teamColors;
	static vector<Color> effectColors;

};

#endif
