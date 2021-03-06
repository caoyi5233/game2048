#include "GameWorld.h"
#include "NumSprite.h"
#include "DataUtil.h"
USING_NS_CC;
Scene* GameWorld::createScene()
{
    Scene* scene = Scene::create();
    Layer* layer = GameWorld::create();
    scene->addChild(layer);
    return scene;
}

bool GameWorld::init()
{
    if (!Layer::init()) {
        return false;
    }

    //init inner data
    numSpriteArray = new NumSpriteArray();

    //fetch evi param
    Size visiableSize = Director::getInstance()->getVisibleSize();

    //init bg
    LayerColor* layerColorBg = LayerColor::create(Color4B(180, 170, 160, 255));
    this->addChild(layerColorBg);

    //init scoreSprite
    scoreSprite = ScoreSprite::create(0, Point(visiableSize.width / 2 + 120, visiableSize.height - 150));
    this->addChild(scoreSprite);

    //init event listener
    EventListenerTouchOneByOne* touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    //init numSpriteArray
    createNumSprite(visiableSize);

    //init spriteMoveHelper
    numSpriteMoveHelper = NumSpriteMoveHelper::create(numSpriteArray);
    numSpriteMoveHelper->retain();
    
    return true;
}

bool GameWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    Point touchPoint = touch->getLocation();
    touchX = touchPoint.x;
    touchY = touchPoint.y;
    return true;
}
void GameWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    int offest = 5;
    int moveScore = 0;
    Point touchPoint = touch->getLocation();
    int endX = touchX - touchPoint.x;
    int endY = touchY - touchPoint.y;
    if (abs(endX) > abs(endY)) {
        if (endX + offest > 0) {
            moveScore = numSpriteMoveHelper->moveLeft();
        } else {
            moveScore = numSpriteMoveHelper->moveRight();
        }
    } else {
        if (endY + offest > 0) {
            moveScore = numSpriteMoveHelper->moveDown();
        } else {
            moveScore = numSpriteMoveHelper->moveUp();
        }
    }
    if (moveScore > 0) {
        randomFill();
        scoreSprite -> setScore(scoreSprite -> getScore() + moveScore);
        if (isGameOver()) {
            Director::getInstance()->replaceScene(TransitionFade::create(1, GameWorld::createScene()));
        }
    }
    
}

void GameWorld::createNumSprite(cocos2d::Size size)
{
    int margin = 28;
    int singleMargin = 20;
    int singleLon = (size.width - margin) / Config::LINE_COUNT;
    for (int i = 0; i < Config::LINE_COUNT; i++) {
        for (int j = 0; j < Config::LINE_COUNT; j++) {
            Point position = Point(singleLon * i + singleMargin, singleLon * j + singleMargin + size.height / 6);
            NumSprite* cardSprite = NumSprite::create(0, singleLon, singleLon, position);
            numSpriteArray->numSprites[i][j] = cardSprite;
            addChild(cardSprite);
        }
    }
    randomFill();
}

void GameWorld::randomFill()
{
    int emptyCount = 0;
    for (int i = 0; i < Config::LINE_COUNT; i++) {
        for (int j = 0; j < Config::LINE_COUNT; j++) {
            if (numSpriteArray->numSprites[i][j]->getNumber() <= 0) {
                emptyCount++;
            }
        }
    }
    if (emptyCount == 0) {
        return;
    }
    int randomIndex = DataUtil::getRand(0, emptyCount - 1);
    emptyCount = 0;
    for (int i = 0; i < Config::LINE_COUNT; i++) {
        for (int j = 0; j < Config::LINE_COUNT; j++) {
            if (numSpriteArray->numSprites[i][j]->getNumber() <= 0) {
                if (emptyCount == randomIndex) {
                    numSpriteArray->numSprites[i][j]->setNumber(DataUtil::getRand(1, 2) * 2);
                    return;
                }
                emptyCount++;
            }
        }
    }
}

bool GameWorld::isGameOver()
{
    for (int i = 0; i < Config::LINE_COUNT; i++) {
        for (int j = 0; j < Config::LINE_COUNT; j++) {
            NumSprite* numSprite = numSpriteArray->numSprites[i][j];
            if (numSprite->getNumber() <= 0) {
                return false;
            } else if (i > 0 && numSprite->getNumber() == numSpriteArray->numSprites[i - 1][j]->getNumber()) {
                return false;
            } else if (j > 0 && numSprite->getNumber() == numSpriteArray->numSprites[i][j - 1]->getNumber()) {
                return false;
            }
        }
    }
    return true;
};
