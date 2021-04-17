/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    } else {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    ///---------- Определения ----------

    blueBackground = Sprite::create("BlueBackground.png");
    road = Sprite::create("Road.png");
    car = Sprite::create("Granta.png");
    tireFront = Sprite::create("Tire.png");
    tireBack = Sprite::create("Tire.png");

    buttonStop = Button::create("ButtonStopNormal.png", "ButtonStopSelected.png");
    buttonStart = Button::create("ButtonStartNormal.png", "ButtonStartSelected.png");

    labelDistance = Label::createWithTTF("Введите расстояние", "fonts/arial.ttf", 10);
    labelSpeed = Label::createWithTTF("Введите скорость", "fonts/arial.ttf", 10);
    labelWeight = Label::createWithTTF("Введите вес", "fonts/arial.ttf", 10);
    labelTires = Label::createWithTTF("Введите размер колёс", "fonts/arial.ttf", 10);

    textDistance = TextField::create("1", "fonts/arial.ttf", 10);
    textSpeed = TextField::create("2", "fonts/arial.ttf", 10);
    textWeight = TextField::create("3", "fonts/arial.ttf", 10);
    textTires = TextField::create("4", "fonts/arial.ttf", 10);

    ///---------- Свойства объектов ----------

    blueBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    road->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    car->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 - origin.y));
    car->setScale(1.5);

    tireFront->setPosition(Vec2(car->getPositionX() + 82, car->getPositionY() - 45));
    tireFront->setScale(0.08);

    tireBack->setPosition(Vec2(car->getPositionX() - 73, car->getPositionY() - 45));
    tireBack->setScale(0.08);


    buttonStart->setPosition(Vec2(visibleSize.width - 35, visibleSize.height + 10));
    buttonStart->setScale(0.5);

    buttonStop->setPosition(Vec2(visibleSize.width - 35, visibleSize.height - 10));
    buttonStop->setScale(0.5);


    labelDistance->setPosition(Vec2(80, visibleSize.height + 20));
    labelDistance->setWidth(150);
    labelDistance->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelSpeed->setPosition(Vec2(80, visibleSize.height - 30));
    labelSpeed->setWidth(150);
    labelSpeed->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelWeight->setPosition(Vec2(80, visibleSize.height - 80));
    labelWeight->setWidth(150);
    labelWeight->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelTires->setPosition(Vec2(80, visibleSize.height - 130));
    labelTires->setWidth(150);
    labelTires->enableOutline(Color4B(0, 0, 0, 255), 1);


    textDistance->setPosition(Vec2(labelDistance->getPositionX() - 70, labelDistance->getPositionY() - 25));

    textSpeed->setPosition(Vec2(labelSpeed->getPositionX() - 70, labelSpeed->getPositionY() - 25));

    textWeight->setPosition(Vec2(labelWeight->getPositionX() - 70, labelWeight->getPositionY() - 25));

    textTires->setPosition(Vec2(labelTires->getPositionX() - 70, labelTires->getPositionY() - 25));

    ///---------- Функции для кнопок ----------

    buttonStart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            tireFront->setScale(tireFront->getScale() * 2);
            tireBack->setScale(tireBack->getScale() * 2);
        }
    });

    buttonStop->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            tireFront->setScale(tireFront->getScale() / 2);
            tireBack->setScale(tireBack->getScale() / 2);
        }
    });

    textDistance->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireBack->setScale(tireBack->getScale() * stof(textDistance->getString()));
        }
    });

    textSpeed->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireBack->setScale(tireBack->getScale() * stof(textSpeed->getString()));
        }
    });

    textWeight->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireBack->setScale(tireBack->getScale() * stof(textWeight->getString()));
        }
    });

    textTires->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireBack->setScale(tireBack->getScale() * stof(textTires->getString()));
        }
    });

    ///---------- Добавление на экран ----------

    this->addChild(blueBackground);
    this->addChild(road);
    this->addChild(car);
    this->addChild(tireFront);
    this->addChild(tireBack);

    this->addChild(buttonStart);
    this->addChild(buttonStop);

    this->addChild(labelDistance);
    this->addChild(labelSpeed);
    this->addChild(labelWeight);
    this->addChild(labelTires);

    this->addChild(textDistance);
    this->addChild(textSpeed);
    this->addChild(textWeight);
    this->addChild(textTires);

    //this->schedule(schedule_selector(HelloWorld::respawnRoad), visibleSize.width / 10);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void HelloWorld::respawnRoad(float dt) {
    auto roadTmp = Sprite::create("Road.png");
    roadTmp->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 3));
    this->addChild(roadTmp);
    auto roadTmpMove = MoveBy::create(0.05 * Director::getInstance()->getVisibleSize().width, Point(-Director::getInstance()->getVisibleSize().width * 1.5, 0));
    roadTmp->runAction(roadTmpMove);
}
