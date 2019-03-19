#include "Blocks.h"

USING_NS_CC;

//����ש������͵������಻ͬ�ò�ͬ����ͼƬ��ʼ�������ò�ͬtag
//x �����Ͻ����ש������ϵx����
//y �����Ͻ����ש������ϵy����
//type ש������
//item ��������
Sprite* Blocks::createBlockSprite(int x, int y, int type, int item) {
	Sprite* block;
	//����tag: ʮλ����ʾ���� 0��ʾû�е���; ��λ����ʾש���� 0��ʾ��ɫש�� 1��ɫ 2��ɫ 3����ľ�� 4����һ�κ��ľ�� 5�������κ��ľ�� 6����
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
	//�ж��Ƿ����˾���
	if (block != NULL) {
		auto blockBody = PhysicsBody::createBox(block->getContentSize(), PHYSICSBODY_MATERIAL_DEFAULT);
		blockBody->getShape(0)->setDensity(10.0f);
		blockBody->getShape(0)->setFriction(0.0f);
		blockBody->getShape(0)->setRestitution(1.0f);
		blockBody->setDynamic(false);
		//ש��������趨ʹש��ש����ײ�Ȳ�����ʵ������Ч��Ҳ��������ײ�¼�
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
		//����1366x768��ש��ʵ�ʴ�С��Ƶ�ש���������ڷ�λ��
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