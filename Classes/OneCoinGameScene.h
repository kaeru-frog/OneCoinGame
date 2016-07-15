//
//  OneCoinGameScene.h
//  10円ゲームシーン
//
//  2016/06/27
//  Create By Kaeru
//  http://surfrog.net/
//
//

#ifndef __ONECOINGAME_SCENE_H__
#define __ONECOINGAME_SCENE_H__

#include "cocos2d.h"

class OneCoinGame : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
	CREATE_FUNC(OneCoinGame);

	// コイン投入口ボタン押下時のコールバック
	void insertCoinCallback(cocos2d::Ref* pSender);
	void coinEntry();

	// タッチ処理
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// コインに力を加える
	void applyForceCoin(float vecx, float power);

	// スケジュール処理
	void update(float delta);

	// 接触イベントの検知
	bool onContactBegin(cocos2d::PhysicsContact& contact);

	// DEMO画面用update
	void demoUpdate(float delta);
private:
	cocos2d::Sprite	*_coin;	// コイン
	cocos2d::Sprite	*_lever[4];	// レバー

	// レバータッチ処理用
	bool			_isTouchLever;
	int				_touchLeverNumber;
	cocos2d::Vec2	_touchlocation;

	// デモモード
	bool			_isDemo;
	cocos2d::Sprite	*_demoSprite;
};

#endif // __ONECOINGAME_SCENE_H__
