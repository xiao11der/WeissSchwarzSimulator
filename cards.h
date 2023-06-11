#pragma once
#include<string>

enum cardColor { BLUE, RED, YELLOW, GREEN };
enum cardType { CHARACTER, EVENT, CLIMAX };

class card {

public:
	card(int level = 1, int soul = 1, int trigger = 0, cardColor = BLUE, cardType = CHARACTER); //Normal constructor with specific traits
	card(cardType type); //Ctor with type only

	card(const card& p1); //Copy Constructor

	int getLevel(void) { return mLevel; };
	int getSoul(void) { return mSoul; };
	int getTrigger(void) { return mTrigger; };
	cardColor getColor(void) { return mColor; };
	cardType getType(void) { return mType; };
	

private:
	int mLevel;
	int mSoul;
	int mTrigger;
	cardColor mColor;
	cardType mType;

	


};