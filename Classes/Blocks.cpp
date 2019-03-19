#include "Blocks.h"

USING_NS_CC;

//根据砖块种类和道具种类不同用不同纹理图片初始化并设置不同tag
//x 从左上角起的砖块坐标系x坐标
//y 从左上角起的砖块坐标系y坐标
//type 砖块类型
//item 道具类型
Sprite* Blocks::createBlockSprite(int x, int y, int type, int item) {
	Sprite* block;
	//关于tag: 十位数表示道具 0表示没有道具; 个位数表示砖种类 0表示蓝色砖块 1金色 2绿色 3完整木块 4击打一次后的木块 5击打两次后的木块 6铁块
	switch (type) {
	case 1:
		block = Sprite::create("blocksnormal0.png");
		block->setTag(0 + item * 10);
		break;
	case 2:
		block = Sprite::create("blocksnormal1.png");
		block->setTag(1 + item * 10);
		break;
	case 3:
		block = Sprite::create("blocksnormal2.png");
		block->setTag(2 + item * 10);
		break;
	case 4:
		block = Sprite::create("blockswood0.png");
		block->setTag(5 + item * 10);
		break;
	case 5:
		block = Sprite::create("blockswood1.png");
		block->setTag(4 + item * 10);
		break;
	case 6:
		block = Sprite::create("blockswood2.png");
		block->setTag(3 + item * 10);
		break;
	case 7:
		block = Sprite::create("blocksiron.png");
		block->setTag(6 + item * 10);
		break;
	default:
		block = NULL;
		break;
	}
	//判断是否建立了精灵
	if (block != NULL) {
		auto blockBody = PhysicsBody::createBox(block->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.0f);
		blockBody->setDynamic(false);
		//砖块的掩码设定使砖与砖的碰撞既不发生实际物理效果也不发送碰撞事件
		if (block->getTag() % 10 == 6) {
			blockBody->setCategoryBitmask(0x141);
			blockBody->setContactTestBitmask(0x200);
			blockBody->setCollisionBitmask(0x12);
		}
		else {
			blockBody->setCategoryBitmask(0x21);
			blockBody->setContactTestBitmask(0x12);
			blockBody->setCollisionBitmask(0x02);
		}
		//根据1366x768和砖块实际大小设计的砖块相对坐标摆放位置
		float xOffset = 8 + (x - 0.5)*(block->getContentSize().width);
		float yOffset = 688 - (y - 0.5)*(block->getContentSize().height);
		block->setPosition(xOffset, yOffset);
		block->setPhysicsBody(blockBody);
	}
	return block;
}
Blocks* Blocks::m_Blocks = NULL;
Blocks* Blocks::getInstance() {
	if (m_Blocks == NULL) {
		m_Blocks = new Blocks();
		if (m_Blocks && m_Blocks->init()) {
			m_Blocks->autorelease();
			m_Blocks->retain();
		}
		else{
			CC_SAFE_DELETE(m_Blocks);
			m_Blocks = NULL;
		}
	}
	return m_Blocks;
}
bool Blocks::init() {
	return true;
}