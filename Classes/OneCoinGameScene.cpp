//
//  OneCoinGameScene.cpp
//  10�~�Q�[���V�[��
//
//  2016/06/27
//  Create By Kaeru
//  http://surfrog.net/
//
//

#include "OneCoinGameScene.h"

#include <vector>
#include <fstream>

USING_NS_CC;

#pragma execution_character_set("utf-8")

// ���o�[�̐ڐG�����ݒ�
Rect leverArea[4] = {
	Rect(45, 410, 30, 30),
	Rect(285, 330, 30, 30),
	Rect(45, 270, 30, 30),
	Rect(285, 170, 30, 30),
};

Scene* OneCoinGame::createScene()
{
	// �V�[���𕨗��G���W�����g�p�ł���V�[���ɂ���
	auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto layer = OneCoinGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

	/////////////////////////////
	// �����G���W���p�ݒ�
	auto world = scene->getPhysicsWorld();
	// �����V�[���̌v�Z���x
	world->setSpeed(3.0f);
	// �����V�[���̃T�u�X�e�b�v��
	world->setSubsteps(5);
	// �����G���W���̃f�o�b�O�p�̗̈��\��
	world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool OneCoinGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// ���o�[�֘A��������
	_isTouchLever = false;
	_touchLeverNumber = 0;
	_touchlocation = Vec2(0.0f, 0.0f);

	/////////////////////////////
	// �^�b�`�C�x���g���g�p���邽�߂̏�������
	// �^�b�`�Ɋւ���C�x���g���X�i�[�𐶐�
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(OneCoinGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OneCoinGame::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OneCoinGame::onTouchEnded, this);
	// �^�b�`�C�x���g�����X�i�[�ɓo�^
	auto dip = Director::getInstance()->getEventDispatcher();
	dip->addEventListenerWithSceneGraphPriority(listener, this);

	/////////////////////////////
	// ➑̔w�i�摜�̕`��
	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(180.0f, 320.0f));
	this->addChild(background);

	/////////////////////////////
	// ➑̉摜�i�t���[���j�̕`��
	auto frame = Sprite::create("frame.png");
	frame->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	this->addChild(frame, 2);

	/////////////////////////////
	// �R�C���̕`��
	_coin = Sprite::create("coin.png");
	_coin->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5));
	this->addChild(_coin, 1);

	/////////////////////////////
	// �R�C���ɕ�����ݒ肷��
	// �}�e���A���̐ݒ�
	PhysicsMaterial coinMaterial;
	coinMaterial.density = 0.1f;		// ���x
	coinMaterial.restitution = 0.5f;	// �����W��
	coinMaterial.friction = 0.4f;		// ���C�W��

	// �~�`�̕����ݒ�
	auto coinPhysics = PhysicsBody::createCircle(12.5f, coinMaterial);
	coinPhysics->setMass(1.0);		// �d��
	coinPhysics->setMoment(1.0f);	// ���[�����g�B��]�����邽�߂̗́B�傫���قǉ�]���ɂ���

	// �R�C���ɍ��̂��֘A�t����
	_coin->setPhysicsBody(coinPhysics);

	// �R�C�����~���A��\���ɂ��Ă���
	_coin->getPhysicsBody()->setEnabled(false);
	_coin->setVisible(false);

	/////////////////////////////
	// �R�C���������i�{�^���j
	auto insertCoinItem = MenuItemImage::create(
		"coin_entry.png",	// �ʏ펞�̃{�^���摜
		"coin_entry.png",	// �I�����̃{�^���摜
		CC_CALLBACK_1(OneCoinGame::insertCoinCallback, this));

	insertCoinItem->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height*0.9));

	auto menu = Menu::create(insertCoinItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);
	

	/////////////////////////////
	// 10�~�Q�[��➑̓����̗̈�ݒ�
	{
		Vec2 vec[4] = {
			Vec2(60, 600),
			Vec2(60, 60),
			Vec2(300, 60),
			Vec2(300, 600),
		};
		auto wall = Node::create();
		wall->setPhysicsBody(PhysicsBody::createEdgeChain(vec, 4, PhysicsMaterial(1.0f, 0.5f, 0.2f)));
		wall->getPhysicsBody()->setDynamic(false);
		this->addChild(wall);
	}

	/////////////////////////////
	// rail�̕����ݒ�
	{
		std::vector<Vec2>	railData;

		std::string		filePath = FileUtils::getInstance()->fullPathForFilename("raildata.csv");
		std::ifstream	ifs(filePath.c_str());
		std::string		sheetLine;

		// �t�@�C�������݂��Ȃ��ꍇ
		if (!ifs) {
			CCLOG("raildata.csv file not found");
		}

		// �P�s���ǂݍ���
		while (getline(ifs, sheetLine))
		{
			std::istringstream sheetStream(sheetLine);
			std::string temp;

			std::vector<std::string> cellData;              // �P�s���̕�����̃��X�g
			while (getline(sheetStream, temp, ','))
			{
				// �P�Z�����̕���������X�g�ɒǉ�����
				cellData.push_back(temp);
			}

			auto pos = Vec2(std::stoi(cellData[0]), std::stoi(cellData[1]));
			if (0 == std::stoi(cellData[2])) {
				// �I�[�ȊO
				railData.push_back(pos);
			}
			else {
				// �I�[
				railData.push_back(pos);
				
				auto rail = Node::create();
				rail->setPhysicsBody(PhysicsBody::createEdgeChain(railData.data(), railData.size(),
					PhysicsMaterial(1.0f, 0.5f, 0.2f)));
				rail->getPhysicsBody()->setDynamic(false);
				this->addChild(rail);

				// vector���N���A����
				railData.clear();
				railData.shrink_to_fit();
			}

		}
	}

	/////////////////////////////
	// ���o�[�ݒ�
	{
		// ���o�[�ʒu�ݒ�
		Vec2 vec[4] = {
			Vec2(45, 400),
			Vec2(315, 321),
			Vec2(45, 255),
			Vec2(315, 160),
		};

		for (int i = 0; i < 4; i++) {
			// ���o�[�̔w�i
			auto leverBackground = Sprite::create("lever_background.png");
			leverBackground->setPosition(vec[i]);
			leverBackground->setAnchorPoint(Vec2(0.5f, 0.28f));
			this->addChild(leverBackground, 2);

			// ���o�[����
			_lever[i] = Sprite::create("lever.png");
			_lever[i]->setPosition(vec[i]);
			_lever[i]->setAnchorPoint(Vec2(0.5f, 0.28f));
			this->addChild(_lever[i], 2);
		}
	}

    return true;
}


// �R�C�������{�^��
void OneCoinGame::insertCoinCallback(cocos2d::Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// �R�C���ɓ����Ă��镨���̗͂����Z�b�g����
	_coin->getPhysicsBody()->resetForces();
	_coin->getPhysicsBody()->setVelocity(Vec2(0.0f, 0.0f));

	// �ʒu�̃��Z�b�g�i�������{�^���ɃZ�b�g����j
	_coin->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height*0.9));

	// �R�C���̕�����L���ɂ��ĕ\������
	_coin->getPhysicsBody()->setEnabled(true);
	_coin->setVisible(true);
}



// �^�b�`�J�n���̏���
bool OneCoinGame::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();
	bool isTouchLever = false;

	// �^�b�`�������o�[�̒���
	for (int i = 0; i < 4; i++) {
		Rect rect = _lever[i]->getBoundingBox();

		// �^�b�`�����_�����o�[�̈�����ǂ���
		if (rect.containsPoint(location)){
			isTouchLever = true;
			// �^�b�`�������o�[���L��
			_touchLeverNumber = i;
			// �^�b�`�����ʒu���L��
			_touchlocation = location;
		}
	}

	// ���o�[���^�b�`�����ꍇ�A���o�[�̃^�b�`��Ԃ�ݒ肷��
	if (isTouchLever == true) {
		_isTouchLever = true;
	}
	else {
		_isTouchLever = false;
	}

	// 

	return true;
}

// �^�b�`�ړ����̏���
void OneCoinGame::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();

	if (_isTouchLever) {
		float d = _touchlocation.getDistance(location);
		if (d >= 100.0f) {
			d = 100.0f;
		}

		// ���o�[�X�v���C�g�̍X�V
		if (_touchLeverNumber == 0 || _touchLeverNumber == 2) {
			_lever[_touchLeverNumber]->setRotation(-90.0f * (d / 100.0f)); // 0 - 90 max
		}
		else {
			_lever[_touchLeverNumber]->setRotation(90.0f * (d / 100.0f)); // 0 - 90 max
		}
	}

}

// �^�b�`�I�����̏���
void OneCoinGame::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();

	if (_isTouchLever) {
		float d = _touchlocation.getDistance(location);
		if (d >= 100.0f) {
			d = 100.0f;
		}
		// ���o�[�X�v���C�g�������ʒu�ɖ߂�
		_lever[_touchLeverNumber]->setRotation(0);

		// �R�C�������o�[�͈͂ɂ��邩�m�F
		if (leverArea[_touchLeverNumber].containsPoint(_coin->getPosition())) {
			// �͈͓��Ȃ�R�C���ɗ͂�������
			if (_touchLeverNumber == 0 || _touchLeverNumber == 2) {
				this->applyForceCoin(1.0f, (d / 100.0f));
			}
			else {
				this->applyForceCoin(-1.0f, (d / 100.0f));
			}

		}

		// ���o�[�̃^�b�`��Ԃ�����
		_isTouchLever = false;
	}

}


// �R�C���ɗ͊w�I�ȗ͂�������
void OneCoinGame::applyForceCoin(float vecx, float power)
{
	_coin->getPhysicsBody()->applyImpulse(Vec2(400.0f * power * vecx, 0.0f));
}

