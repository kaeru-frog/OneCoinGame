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

#include <vector>
#include <fstream>

USING_NS_CC;

#pragma execution_character_set("utf-8")

// レバーの接触部分設定
Rect leverArea[4] = {
	Rect(45, 410, 30, 30),
	Rect(285, 330, 30, 30),
	Rect(45, 270, 30, 30),
	Rect(285, 170, 30, 30),
};
// 穴の矩形領域
Rect coinHole[] = {
	Rect(245, 430, 30, 30),
	Rect(67, 335, 30, 30),
	Rect(213, 335, 30, 30),
	Rect(260, 258, 30, 30),
	Rect(182, 136, 30, 30),
	Rect(61, 67, 30, 30),
};
// ゴールの矩形領域
Rect goalHole = {
	Rect(117, 67, 30, 30),
};

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
	// レバー関連初期処理
	_isTouchLever = false;
	_touchLeverNumber = 0;
	_touchlocation = Vec2(0.0f, 0.0f);

	/////////////////////////////
	// タッチイベントを使用するための初期処理
	// タッチに関するイベントリスナーを生成
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(OneCoinGame::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(OneCoinGame::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(OneCoinGame::onTouchEnded, this);
	// タッチイベントをリスナーに登録
	auto dip = Director::getInstance()->getEventDispatcher();
	dip->addEventListenerWithSceneGraphPriority(listener, this);

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
	coinPhysics->setMass(1.0);		// 重さ
	coinPhysics->setMoment(1.0f);	// モーメント。回転させるための力。大きいほど回転しにくい
	coinPhysics->setCategoryBitmask(0x00000001);	// 物体のカテゴリ
	coinPhysics->setCollisionBitmask(0x00000001);	// 接触する物体のカテゴリを指定

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
		wall->getPhysicsBody()->setCategoryBitmask(0x00000001);		// 物体のカテゴリ
		wall->getPhysicsBody()->setCollisionBitmask(0x00000001);	// 接触する物体のカテゴリを指定
		this->addChild(wall);
	}

	/////////////////////////////
	// railの物理設定
	{
		std::vector<Vec2>	railData;

		std::string		filePath = FileUtils::getInstance()->fullPathForFilename("raildata.csv");
		std::ifstream	ifs(filePath.c_str());
		std::string		sheetLine;

		// ファイルが存在しない場合
		if (!ifs) {
			CCLOG("raildata.csv file not found");
		}

		// １行ずつ読み込む
		while (getline(ifs, sheetLine))
		{
			std::istringstream sheetStream(sheetLine);
			std::string temp;

			std::vector<std::string> cellData;              // １行分の文字列のリスト
			while (getline(sheetStream, temp, ','))
			{
				// １セル分の文字列をリストに追加する
				cellData.push_back(temp);
			}

			auto pos = Vec2(std::stoi(cellData[0]), std::stoi(cellData[1]));
			if (0 == std::stoi(cellData[2])) {
				// 終端以外
				railData.push_back(pos);
			}
			else {
				// 終端
				railData.push_back(pos);
				
				auto rail = Node::create();
				rail->setPhysicsBody(PhysicsBody::createEdgeChain(railData.data(), railData.size(),
					PhysicsMaterial(1.0f, 0.5f, 0.2f)));
				rail->getPhysicsBody()->setDynamic(false);
				rail->getPhysicsBody()->setCategoryBitmask(0x00000001);		// 物体のカテゴリ
				rail->getPhysicsBody()->setCollisionBitmask(0x00000001);	// 接触する物体のカテゴリを指定
				this->addChild(rail);

				// vectorをクリアする
				railData.clear();
				railData.shrink_to_fit();
			}

		}
	}

	/////////////////////////////
	// レバー設定
	{
		// レバー位置設定
		Vec2 vec[4] = {
			Vec2(45, 400),
			Vec2(315, 321),
			Vec2(45, 255),
			Vec2(315, 160),
		};

		for (int i = 0; i < 4; i++) {
			// レバーの背景
			auto leverBackground = Sprite::create("lever_background.png");
			leverBackground->setPosition(vec[i]);
			leverBackground->setAnchorPoint(Vec2(0.5f, 0.28f));
			this->addChild(leverBackground, 2);

			// レバー部分
			_lever[i] = Sprite::create("lever.png");
			_lever[i]->setPosition(vec[i]);
			_lever[i]->setAnchorPoint(Vec2(0.5f, 0.28f));
			this->addChild(_lever[i], 2);
		}
	}

	/////////////////////////////
	// 景品口の物理領域
	{
		Vec2 vec[4] = {
			Vec2(202, 65),	// 左上
			Vec2(202, 23),	// 左下
			Vec2(301, 23),	// 右下
			Vec2(301, 65),	// 右上
		};
		auto wall = Node::create();
		wall->setPhysicsBody(PhysicsBody::createEdgeChain(vec, 4, PhysicsMaterial(1.0f, 0.5f, 0.2f)));
		wall->getPhysicsBody()->setDynamic(false);
		wall->getPhysicsBody()->setCategoryBitmask(0x00000010);		// 物体のカテゴリ
		wall->getPhysicsBody()->setCollisionBitmask(0x00000010);	// 接触する物体のカテゴリを指定
		this->addChild(wall);
	}

	/////////////////////////////
	// スケジュール処理の開始
	this->scheduleUpdate();

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



// タッチ開始時の処理
bool OneCoinGame::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();
	bool isTouchLever = false;

	// タッチしたレバーの調査
	for (int i = 0; i < 4; i++) {
		Rect rect = _lever[i]->getBoundingBox();

		// タッチした点がレバー領域内かどうか
		if (rect.containsPoint(location)){
			isTouchLever = true;
			// タッチしたレバーを記憶
			_touchLeverNumber = i;
			// タッチした位置を記憶
			_touchlocation = location;
		}
	}

	// レバーをタッチした場合、レバーのタッチ状態を設定する
	if (isTouchLever == true) {
		_isTouchLever = true;
	}
	else {
		_isTouchLever = false;
	}

	// 

	return true;
}

// タッチ移動時の処理
void OneCoinGame::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();

	if (_isTouchLever) {
		float d = _touchlocation.getDistance(location);
		if (d >= 100.0f) {
			d = 100.0f;
		}

		// レバースプライトの更新
		if (_touchLeverNumber == 0 || _touchLeverNumber == 2) {
			_lever[_touchLeverNumber]->setRotation(-90.0f * (d / 100.0f)); // 0 - 90 max
		}
		else {
			_lever[_touchLeverNumber]->setRotation(90.0f * (d / 100.0f)); // 0 - 90 max
		}
	}

}

// タッチ終了時の処理
void OneCoinGame::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();

	if (_isTouchLever) {
		float d = _touchlocation.getDistance(location);
		if (d >= 100.0f) {
			d = 100.0f;
		}
		// レバースプライトを初期位置に戻す
		_lever[_touchLeverNumber]->setRotation(0);

		// コインがレバー範囲にあるか確認
		if (leverArea[_touchLeverNumber].containsPoint(_coin->getPosition())) {
			// 範囲内ならコインに力を加える
			if (_touchLeverNumber == 0 || _touchLeverNumber == 2) {
				this->applyForceCoin(1.0f, (d / 100.0f));
			}
			else {
				this->applyForceCoin(-1.0f, (d / 100.0f));
			}

		}

		// レバーのタッチ状態を解除
		_isTouchLever = false;
	}

}


// コインに力学的な力を加える
void OneCoinGame::applyForceCoin(float vecx, float power)
{
	_coin->getPhysicsBody()->applyImpulse(Vec2(400.0f * power * vecx, 0.0f));
}


// フレーム毎の処理
void OneCoinGame::update(float delta)
{
	/////////////////////////////
	// コインがはずれ穴とゴールに入っている場合の処理
	// コインの速度の取得
	auto velocity = _coin->getPhysicsBody()->getVelocity();
	float speed = velocity.getDistance(Vec2::ZERO);
	// コインの速度が一定速度以下かつコインの物理が有効でかどうか
	if (speed < 3.0f && _coin->getPhysicsBody()->isEnabled()) {
		// はずれ穴に入っているかどうかの確認
		for (int i = 0; i < 6; i++) {
			if (coinHole[i].containsPoint(_coin->getPosition())) {
				// コインがはずれ穴に落ちた時の処理
				// コインの物理動作を停止し、非表示にする
				_coin->getPhysicsBody()->setEnabled(false);
				_coin->setVisible(false);
			}
		}

		// ゴールに入っているかどうかの確認
		if (goalHole.containsPoint(_coin->getPosition())) {
			// コインがゴールに到達した時の処理
			// コインの物理動作を停止し、非表示にする
			_coin->getPhysicsBody()->setEnabled(false);
			_coin->setVisible(false);

			/////////////////////////////////
			// 景品を出す処理
			auto gift = Sprite::create("gift.png");
			gift->setPosition(Vec2(250.0f, 74.0f));
			this->addChild(gift, 1);

			// 景品の物理設定
			auto giftPhysics = PhysicsBody::createBox(Size(26.0f, 14.0f), PhysicsMaterial(0.6f, 0.3f, 6.0f));
			giftPhysics->setMass(1.0f);			// 重さ
			giftPhysics->setMoment(10.0f);		// モーメント(大きいほど回転しにくい)
			giftPhysics->setCategoryBitmask(0x00000010);	// 物体のカテゴリ
			giftPhysics->setCollisionBitmask(0x00000010);	// 接触する物体のカテゴリを指定
			gift->setPhysicsBody(giftPhysics);
		}


	}

}

