#include "model_common.h"
#include "player.h"

const int Player::levelXP[PLAYER_MAX_LEVEL] = {
	1000,
	5713,
	15900,
	32960,
	92590,
	137420,
	193690,
	262440
};

void Player::gainExperience(int value)
{
	experience += value;
	if (level < PLAYER_MAX_LEVEL) {
		if (experience > levelXP[level]) {
			levelUP();
		}
	}
}

void Player::levelUP()
{
	++level;
	skillPoints += 5;
}

void Player::buyAttribute(int AttributeIndex)
{
	if (AttributeIndex<0 || AttributeIndex >= ATTRIBUTE_NUMBER) {
		return;
	}

	if (skillPoints<=0) {
		return;
	}

	skillPoints--;
	attributes[AttributeIndex]++;
}