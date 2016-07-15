//
//  OneCoinGameScene.cpp
//  10‰~ƒQ[ƒ€ƒV[ƒ“
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

    return true;
}
