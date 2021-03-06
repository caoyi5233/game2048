#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "NumSprite.h"
#include "ScoreSprite.h"
#include "NumSpriteMoveHelper.h"
#include "Config.h"
#include "NumSpriteArray.h"

class GameWorld : public cocos2d::Layer {
public:
    /**
     *  create game scene
     *
     *  @return scene
     */
    static cocos2d::Scene* createScene();

    /**
     *  init game data&ui
     *
     *  @return is init success
     */
    virtual bool init();

    /**
     *  call macro to auto create "create" function
     */
    CREATE_FUNC(GameWorld);

    /**
     *  touchBegan callback
     *
     *  @param touch touch description
     *  @param event event description
     *
     *  @return return value description
     */
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     *  touchEnded callback
     *
     *  @param touch touch description
     *  @param event event description
     */
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    int touchX, touchY;
    NumSpriteArray* numSpriteArray;
    ScoreSprite* scoreSprite;
    NumSpriteMoveHelper* numSpriteMoveHelper;
    bool isGameOver();
    void createNumSprite(cocos2d::Size size);
    void randomFill();
};

#endif
