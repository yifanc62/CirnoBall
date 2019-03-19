#ifndef __BLOCKBREAKER_BLOCKS_H__
#define __BLOCKBREAKER_BLOCKS_H__

#include "cocos2d.h"
#include "cocostudio/CCArmature.h"

USING_NS_CC;
using namespace cocostudio;

class Blocks :public Ref {
public:
	static Blocks* getInstance(); //��ȡһ���¶���
	virtual bool init();
	Sprite* createBlockSprite(int x, int y, int type, int item); //����x���ꡢy���ꡢש�����ࡢ���������ཨ��һ��ש�龫��
private:
	static Blocks* m_Blocks; 
};
#endif // __BLOCKBREAKER_BLOCKS_H__