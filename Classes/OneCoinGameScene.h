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
	void coinEntry();

	// �^�b�`����
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// �R�C���ɗ͂�������
	void applyForceCoin(float vecx, float power);

	// �X�P�W���[������
	void update(float delta);

	// �ڐG�C�x���g�̌��m
	bool onContactBegin(cocos2d::PhysicsContact& contact);

	// DEMO��ʗpupdate
	void demoUpdate(float delta);
private:
	cocos2d::Sprite	*_coin;	// �R�C��
	cocos2d::Sprite	*_lever[4];	// ���o�[

	// ���o�[�^�b�`�����p
	bool			_isTouchLever;
	int				_touchLeverNumber;
	cocos2d::Vec2	_touchlocation;

	// �f�����[�h
	bool			_isDemo;
	cocos2d::Sprite	*_demoSprite;
};

#endif // __ONECOINGAME_SCENE_H__
