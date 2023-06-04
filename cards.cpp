#include<string>
#include"cards.h"
#include<stdexcept>

card::card(int level, int soul, int trigger,  std::string color, std::string type) {

	if (level < 0 || level >3) {
		throw std::invalid_argument(std::string("Level must be between 0 and 3 for a card, your level is: ")+std::to_string(level));
	}
	if (_stricmp(color.c_str(), "yellow") != 0 && _stricmp(color.c_str(), "green") != 0 && _stricmp(color.c_str(), "blue")!=0 && _stricmp(color.c_str(), "red")!=0) {
		throw std::invalid_argument(std::string("Color must be green, yellow, blue, or red. Purple is not supported. Your color is: ") + color);
	}
	if (_stricmp(type.c_str(), "character") != 0 && _stricmp(type.c_str(), "event")!=0 && _stricmp(type.c_str(), "climax")!=0) {
		throw std::invalid_argument(std::string("Invalid type -- Needs to be character, event, or climax. Your type is: ")+type);
	}
	card::mLevel = level;
	card::mSoul = soul;
	card::mTrigger = trigger;
	card::mColor = color;
	card::mType = type;

}

card::card(std::string type) {
	if (_stricmp(type.c_str(), "character") != 0 && _stricmp(type.c_str(), "event") != 0 && _stricmp(type.c_str(), "climax") != 0) {
		throw std::invalid_argument(std::string("Invalid type -- Needs to be character, event, or climax. Your type is: ") + type);
	}
	
	card::mLevel = 1;
	card::mSoul = 1;
	card::mTrigger = 0;
	card::mColor = "blue";
	card::mType = type;

}

card::card(const card& p1): 
	mLevel(p1.mLevel),
	mSoul(p1.mSoul),
	mTrigger(p1.mTrigger),
	mColor(p1.mColor),
	mType(p1.mType)
{

}