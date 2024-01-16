#ifndef __PLAYER_H__
#define __PLAYER_H__

#define ATTRIBUTE_BUILDING 0
#define ATTRIBUTE_TRAINING 1
#define ATTRIBUTE_MAGIC 2
#define ATTRIBUTE_SPIRIT 3

#define ATTRIBUTE_NUMBER 4

#define PLAYER_MAX_LEVEL 10

class EDMODEL_API Player
{

public:

	Player(int id, const string& name) : id(id), name(name), experience(0), level(0), partyMoney(0), attributes(ATTRIBUTE_NUMBER), skillPoints(0) { fill(attributes.begin(), attributes.end(), 0);}

	int					getId() const { return id; }
	const string&		getName() const { return name; }

	int					getTeam() const { return team; }
	int					getExperience() const { return experience; }
	int					getLevel() const { return level; }
	int					getPartyMoney() const { return partyMoney; }
	const vector<int>&	getAttributes() const { return attributes; }

	void				setTeam(int team) { this->team = team; }
	void				gainPartyMoney(int value) { partyMoney+= value; }
	void				losePartyMoney(int value) { partyMoney-= value; }
	void				setPartyMoney(int value) { partyMoney = value; }
	void				gainExperience(int value);
	void				buyAttribute(int AttributeIndex);

	virtual ostream& print(ostream& os) const { return os << "[Player " << id <<", " << name <<", ]";}

protected:
	Player(const Player&);
	Player() : id(id), name(""), experience(0), level(0), partyMoney(0) , attributes(ATTRIBUTE_NUMBER), skillPoints(0), team(0) {fill(attributes.begin(), attributes.end(), 0);}
	
	void levelUP();

	int id;
	int experience;
	int level;
	int skillPoints;
	int team;
	vector<int> attributes;
	string name;

	int partyMoney;

	static const int levelXP[PLAYER_MAX_LEVEL];
};

class PlayerRewardor {
private:
	int money;
	int exp;
public :
	PlayerRewardor(int money, int exp) : money(money), exp(exp) {}
	void operator()(Player* player) {
		player->gainExperience(exp);
		player->gainPartyMoney(money);
	}
};

#endif