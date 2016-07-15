//
//  OneCoinGameScene.cpp
//  10円ゲームシーン
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
	// シーンを物理エンジンが使用できるシーンにする
	auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
	auto layer = OneCoinGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

	/////////////////////////////
	// 物理エンジン用設定
	auto world = scene->getPhysicsWorld();
	// 物理シーンの計算速度
	world->setSpeed(3.0f);
	// 物理シーンのサブステップ数
	world->setSubsteps(5);
	// 物理エンジンのデバッグ用の領域を表示
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
	// 筐体背景画像の描画
	auto background = Sprite::create("background.png");
	background->setPosition(Vec2(180.0f, 320.0f));
	this->addChild(background);

	/////////////////////////////
	// 筐体画像（フレーム）の描画
	auto frame = Sprite::create("frame.png");
	frame->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	this->addChild(frame, 2);

	/////////////////////////////
	// コインの描画
	_coin = Sprite::create("coin.png");
	_coin->setPosition(Vec2(visibleSize.width*0.5, visibleSize.height*0.5));
	this->addChild(_coin, 1);

	/////////////////////////////
	// コインに物理を設定する
	// マテリアルの設定
	PhysicsMaterial coinMaterial;
	coinMaterial.density = 0.1f;		// 密度
	coinMaterial.restitution = 0.5f;	// 反発係数
	coinMaterial.friction = 0.4f;		// 摩擦係数

	// 円形の物理設定
	auto coinPhysics = PhysicsBody::createCircle(12.5f, coinMaterial);

	// コインに剛体を関連付ける
	_coin->setPhysicsBody(coinPhysics);

	// コインを停止し、非表示にしておく
	_coin->getPhysicsBody()->setEnabled(false);
	_coin->setVisible(false);

	/////////////////////////////
	// コイン投入口（ボタン）
	auto insertCoinItem = MenuItemImage::create(
		"coin_entry.png",	// 通常時のボタン画像
		"coin_entry.png",	// 選択時のボタン画像
		CC_CALLBACK_1(OneCoinGame::insertCoinCallback, this));

	insertCoinItem->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height*0.9));

	auto menu = Menu::create(insertCoinItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 2);
	

	/////////////////////////////
	// 10円ゲーム筐体内部の領域設定
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


// コイン投入ボタン
void OneCoinGame::insertCoinCallback(cocos2d::Ref* pSender)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// コインに働いている物理の力をリセットする
	_coin->getPhysicsBody()->resetForces();
	_coin->getPhysicsBody()->setVelocity(Vec2(0.0f, 0.0f));

	// 位置のリセット（投入口ボタンにセットする）
	_coin->setPosition(Vec2(visibleSize.width*0.7, visibleSize.height*0.9));

	// コインの物理を有効にして表示する
	_coin->getPhysicsBody()->setEnabled(true);
	_coin->setVisible(true);
}


