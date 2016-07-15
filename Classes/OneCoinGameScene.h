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
private:
	cocos2d::Sprite	*_coin;	// �R�C��
};

#endif // __ONECOINGAME_SCENE_H__
