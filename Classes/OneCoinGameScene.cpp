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
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = OneCoinGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

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

	// �R�C�����~���A��\���ɂ��Ă���
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


    return true;
}


// �R�C�������{�^��
void OneCoinGame::insertCoinCallback(cocos2d::Ref* pSender)
{
	// �R�C���̕\��
	_coin->setVisible(true);
}


