#ifndef __BLOCKBREAKER_PLAYER_H__
#define __BLOCKBREAKER_PLAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class Player :public Ref {
public:
	Player(int difficulty);
	int getLife();
	void addOneLife();
	void minusOneLife();

	int getStage();
	void addOneStage();
	void minusOneStage();
	void setStage(int _stage);

	int getCombo();
	void addOneCombo();
	void emptyCombo();

	long getScore();
	void addScore(long _score);
	void emptyScore();
private:
	int life;
	long score;
	int stage;
	int combo;
};
#endif // __BLOCKBREAKER_PLAYER_H__