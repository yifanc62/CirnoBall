#ifndef __BLOCKBREAKER_ITEMS_H__
#define __BLOCKBREAKER_ITEMS_H__

#include "cocos2d.h"
#include "Player.h"

USING_NS_CC;

class Items :public Ref {
public:
	static Items* getInstance();
	virtual bool init();
	Sprite* createItemSprite(int type, Vec2 position);
	void lifeUp(Player* player);
	void ballSpeedUp(Sprite* ball, int difficulty);
	Sprite* paddleLengthUp(Sprite* paddle);
	Sprite* paddleLengthDown(Sprite* paddle);
	void superBall(Sprite* ball);
	void transparentBall(Sprite* ball);
	void multiBall(Sprite* ball);
	void magnetBall(Sprite* ball);
private:
	static Items* m_Items;
};
#endif // __BLOCKBREAKER_GAMEOVERSCENE_H__