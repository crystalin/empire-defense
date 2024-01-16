#include "render2D_common.h"
#include "ColorManager.h" 
#include "Color.h" 
#include "effect.h"
#include "configreader.h"

vector<Color> ColorManager::teamColors;
vector<Color> ColorManager::effectColors;

void ColorManager::init(ConfigReader& configReader) {

	map<string,map<string,string>>::iterator ci = configReader.find("team-color");
	if (ci != configReader.end()) {
		map<string,string>& teamsConfig = ci->second;

		teamColors.resize(8);
		configReader.read(teamsConfig,"0",teamColors[0]);
		configReader.read(teamsConfig,"1",teamColors[1]);
		configReader.read(teamsConfig,"2",teamColors[2]);
		configReader.read(teamsConfig,"3",teamColors[3]);
		configReader.read(teamsConfig,"4",teamColors[4]);
		configReader.read(teamsConfig,"5",teamColors[5]);
		configReader.read(teamsConfig,"6",teamColors[6]);
		configReader.read(teamsConfig,"7",teamColors[7]);
	}

	if (teamColors.empty()) {
		teamColors.push_back(Color(1.0f, 1.0f, 0.0f, 1.0f));
	}

	ci = configReader.find("effect-color");
	if (ci != configReader.end()) {
		map<string,string>& effectsConfig = ci->second;

		effectColors.resize(EFFECT_NUMBER);
		configReader.read(effectsConfig,"fire",effectColors[EFFECT_FIRE]);
		configReader.read(effectsConfig,"ice",effectColors[EFFECT_ICE]);
		configReader.read(effectsConfig,"earth",effectColors[EFFECT_EARTH]);
		configReader.read(effectsConfig,"poison",effectColors[EFFECT_POISON]);
		configReader.read(effectsConfig,"gold",effectColors[EFFECT_GOLD]);
		configReader.read(effectsConfig,"light",effectColors[EFFECT_LIGHT]);
		configReader.read(effectsConfig,"malediction",effectColors[EFFECT_MALEDICTION]);
		configReader.read(effectsConfig,"electricity",effectColors[EFFECT_ELECTRICITY]);
	}

	if (effectColors.empty()) {
		effectColors.push_back(Color(1.0f, 1.0f, 0.0f, 1.0f));
	}
}

void ColorManager::destroy()
{
}