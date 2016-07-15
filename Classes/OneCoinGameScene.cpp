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

USING_NS_CC;

#pragma execution_character_set("utf-8")

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


