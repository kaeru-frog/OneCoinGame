//
//  OneCoinGameScene.h
//  10�~�Q�[���V�[��
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

	// �R�C���������{�^���������̃R�[���o�b�N
	void insertCoinCallback(cocos2d::Ref* pSender);

	// �^�b�`����
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	cocos2d::Sprite	*_coin;	// �R�C��
	cocos2d::Sprite	*_lever[4];	// ���o�[

	// ���o�[�^�b�`�����p
	bool			_isTouchLever;
	int				_touchLeverNumber;
	cocos2d::Vec2	_touchlocation;
};

#endif // __ONECOINGAME_SCENE_H__
