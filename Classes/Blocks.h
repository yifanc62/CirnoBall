#ifndef __BLOCKBREAKER_BLOCKS_H__
#define __BLOCKBREAKER_BLOCKS_H__

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

class Blocks :public Ref {
public:
	static Blocks* getInstance(); //获取一个新对象
	virtual bool init();
	Sprite* createBlockSprite(int x, int y, int type, int item); //根据x坐标、y坐标、砖块种类、含道具种类建立一个砖块精灵
private:
	static Blocks* m_Blocks; 
};
#endif // __BLOCKBREAKER_BLOCKS_H__