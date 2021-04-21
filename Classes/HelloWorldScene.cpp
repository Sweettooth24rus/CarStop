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

    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

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
    this->addChild(menu, 10);

    ///---------- Определения ----------

    speed = 5000;

    blueBackground = Sprite::create("BlueBackground.png");
    roadPart1 = Sprite::create("Road.png");
    roadPart2 = Sprite::create("Road.png");
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

    roadPart1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y));

    roadPart2->setPosition(Vec2(visibleSize.width / 2 + roadPart2->getContentSize().width - 1, visibleSize.height / 2 + origin.y));

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


    textDistance->setPosition(Vec2(labelDistance->getPositionX(), labelDistance->getPositionY() - 25));
    textDistance->setMaxLength(10);

    textSpeed->setPosition(Vec2(labelSpeed->getPositionX(), labelSpeed->getPositionY() - 25));
    textSpeed->setMaxLength(10);
    textSpeed->setText(to_string(speed / 100));

    textWeight->setPosition(Vec2(labelWeight->getPositionX(), labelWeight->getPositionY() - 25));
    textWeight->setMaxLength(10);

    textTires->setPosition(Vec2(labelTires->getPositionX(), labelTires->getPositionY() - 25));
    textTires->setMaxLength(10);
    textTires->setText(to_string(250 / ((car->getContentSize().width * car->getScale()) / (tireFront->getContentSize().width * tireFront->getScale()))));

    ///---------- Функции для кнопок ----------

    buttonStart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            roadPart1->stopAllActions();
            roadPart2->stopAllActions();
            tireFront->stopAllActions();
            tireBack->stopAllActions();

            roadPart1Move = MoveBy::create(10, Point(roadSpeed(), 0));
            roadPart2Move = MoveBy::create(10, Point(roadSpeed(), 0));
            tireFrontRotate = RotateBy::create(10, distToDeg());
            tireBackRotate = RotateBy::create(10, distToDeg());

            roadPart1->runAction(RepeatForever::create(roadPart1Move));
            roadPart2->runAction(RepeatForever::create(roadPart2Move));
            tireFront->runAction(RepeatForever::create(tireFrontRotate));
            tireBack->runAction(RepeatForever::create(tireBackRotate));
            this->schedule(schedule_selector(HelloWorld::testTire));
        }
    });

    buttonStop->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            roadPart1->stopAllActions();
            roadPart2->stopAllActions();
            tireFront->stopAllActions();
            tireBack->stopAllActions();

            int x = 30;

            auto roadPart1Movez = MoveBy::create(x, Point(roadSpeed() / (25.0 / x), 0));
            auto roadPart2Movez = MoveBy::create(x, Point(roadSpeed() / (25.0 / x), 0));
            auto tireFrontRotatez = RotateBy::create(x, distToDeg() / (25.0 / x));
            auto tireBackRotatez = RotateBy::create(x, distToDeg() / (25.0 / x));

            auto roadPart1Ease = EaseSineOut::create(roadPart1Movez);
            auto roadPart2Ease = EaseSineOut::create(roadPart2Movez);
            auto tireFrontEase = EaseSineOut::create(tireFrontRotatez);
            auto tireBackEase = EaseSineOut::create(tireBackRotatez);

            roadPart1->runAction(roadPart1Ease);
            roadPart2->runAction(roadPart2Ease);
            tireFront->runAction(tireFrontEase);
            tireBack->runAction(tireBackEase);

            //this->unschedule(schedule_selector(HelloWorld::testTire));
        }
    });

    textDistance->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireBack->setScale(tireBack->getScale() * stof(textDistance->getString()));
        }
    });

    textSpeed->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            speed = stof(textSpeed->getString()) * 100;
        }
    });

    textWeight->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireBack->setScale(tireBack->getScale() * stof(textWeight->getString()));
        }
    });

    textTires->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {
            tireFront->setScale(0.08 * stof(textTires->getString()) / 29.6117);
            tireBack->setScale(0.08 * stof(textTires->getString()) / 29.6117);
        }
    });

    ///---------- Добавление на экран ----------

    this->addChild(blueBackground, 0);
    this->addChild(roadPart1, 1);
    this->addChild(roadPart2, 1);
    this->addChild(car, 2);
    this->addChild(tireFront, 3);
    this->addChild(tireBack, 3);

    this->addChild(buttonStart, 2);
    this->addChild(buttonStop, 2);

    this->addChild(labelDistance, 2);
    this->addChild(labelSpeed, 2);
    this->addChild(labelWeight, 2);
    this->addChild(labelTires, 2);

    this->addChild(textDistance, 2);
    this->addChild(textSpeed, 2);
    this->addChild(textWeight, 2);
    this->addChild(textTires, 2);

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void HelloWorld::testTire(float dt) {
    if (visibleSize.width / 2 - roadPart1->getPositionX() >= roadPart1->getContentSize().width)
        roadPart1->setPositionX(roadPart2->getPositionX() + roadPart2->getContentSize().width - 1);
    else if (visibleSize.width / 2 - roadPart2->getPositionX() >= roadPart2->getContentSize().width)
        roadPart2->setPositionX(roadPart1->getPositionX() + roadPart1->getContentSize().width - 1);
}

float HelloWorld::roadSpeed() {
    return -1 * speed;
}

float HelloWorld::distToDeg() {
    float deg;
    deg = speed / (M_PI * (tireFront->getContentSize().width * tireFront->getScale()));
    deg *= 360;
    return deg;
}