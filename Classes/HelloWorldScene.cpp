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

    srand((unsigned)time(NULL));

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

    blueBackground = Sprite::create("BlueBackground.png");
    roadPart1 = Sprite::create("Road.png"); //Дорога состоит из двух последовательных частей
    roadPart2 = Sprite::create("Road.png");
    car = Sprite::create("Granta.png");
    tireFront = Sprite::create("Tire.png");
    tireBack = Sprite::create("Tire.png");

    buttonStop = Button::create("ButtonStopNormal.png", "ButtonStopSelected.png");
    buttonStart = Button::create("ButtonStartNormal.png", "ButtonStartSelected.png");

    labelDistance = Label::createWithTTF("Введите тормозной путь в метрах", "fonts/arial.ttf", 10);
    labelSpeed = Label::createWithTTF("Введите скорость в км/ч", "fonts/arial.ttf", 10);
    labelWeight = Label::createWithTTF("Введите вес в килограммах", "fonts/arial.ttf", 10);
    labelTires = Label::createWithTTF("Введите радиус колёс в сантиметрах", "fonts/arial.ttf", 10);

    textDistance = TextField::create("X", "fonts/arial.ttf", 10);
    textSpeed = TextField::create("", "fonts/arial.ttf", 10);
    textWeight = TextField::create("X", "fonts/arial.ttf", 10);
    textTires = TextField::create("", "fonts/arial.ttf", 10);

    ///---------- Свойства объектов ----------

    blueBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    roadPart1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + origin.y));

    roadPart2->setPosition(Vec2(visibleSize.width / 2 + roadPart2->getContentSize().width - 1, visibleSize.height / 2 + origin.y));

    car->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 - origin.y));
    car->setScale(1.5);

    tireFront->setPosition(Vec2(car->getPositionX() + 82, car->getPositionY() - 45));

    tireBack->setPosition(Vec2(car->getPositionX() - 73, car->getPositionY() - 45));


    buttonStart->setPosition(Vec2(visibleSize.width - 35, visibleSize.height + 10));
    buttonStart->setScale(0.5);

    buttonStop->setPosition(Vec2(visibleSize.width - 35, visibleSize.height - 10));
    buttonStop->setScale(0.5);
    buttonStop->setEnabled(false);


    labelDistance->setPosition(Vec2(110, visibleSize.height + 20));
    labelDistance->setWidth(200);
    labelDistance->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelSpeed->setPosition(Vec2(110, visibleSize.height - 30));
    labelSpeed->setWidth(200);
    labelSpeed->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelWeight->setPosition(Vec2(110, visibleSize.height - 80));
    labelWeight->setWidth(200);
    labelWeight->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelTires->setPosition(Vec2(110, visibleSize.height - 130));
    labelTires->setWidth(200);
    labelTires->enableOutline(Color4B(0, 0, 0, 255), 1);


    textDistance->setPosition(Vec2(labelDistance->getPositionX() - 75, labelDistance->getPositionY() - 25));
    textDistance->setMaxLength(10);
    textDistance->setEnabled(false);

    textSpeed->setPosition(Vec2(labelSpeed->getPositionX() - 75, labelSpeed->getPositionY() - 25));
    textSpeed->setMaxLength(10);

    textWeight->setPosition(Vec2(labelWeight->getPositionX() - 75, labelWeight->getPositionY() - 25));
    textWeight->setMaxLength(10);
    textWeight->setEnabled(false);

    textTires->setPosition(Vec2(labelTires->getPositionX() - 75, labelTires->getPositionY() - 25));
    textTires->setMaxLength(10);

    ///---------- Функции для кнопок и полей ввода ----------

    buttonStart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {  //Нажатие на кноку "старт"
        if (type == Widget::TouchEventType::BEGAN) {
            int i = getRandDistWeight();    //Случайно выбирается тормозной путь и вес
            buttonStop->setEnabled(true);
            textDistance->setString(to_string(distWeight[i].distance / 10));
            textWeight->setString(to_string(distWeight[i].weight));

            roadPart1->stopAllActions();    //Убираем все действия
            roadPart2->stopAllActions();
            tireFront->stopAllActions();
            tireBack->stopAllActions();

            setStopTime(i); //Вычисляем время торможения

            roadPart1Move = MoveBy::create(stopTimeWeight, Point(roadSpeed(), 0));  //Задаём движение с постоянной скоростью
            roadPart2Move = MoveBy::create(stopTimeWeight, Point(roadSpeed(), 0));
            tireFrontRotate = RotateBy::create(stopTimeWeight, distToDeg());
            tireBackRotate = RotateBy::create(stopTimeWeight, distToDeg());

            roadPart1->runAction(RepeatForever::create(roadPart1Move)); //Добавляем объектам движение на бесконечность времени
            roadPart2->runAction(RepeatForever::create(roadPart2Move));
            tireFront->runAction(RepeatForever::create(tireFrontRotate));
            tireBack->runAction(RepeatForever::create(tireBackRotate));
        }
    });

    buttonStop->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            buttonStop->setEnabled(false);

            roadPart1->stopAllActions();
            roadPart2->stopAllActions();
            tireFront->stopAllActions();
            tireBack->stopAllActions();

            roadPart1Move = MoveBy::create(stopTimeWeight, Point(roadSpeedEase(), 0));  //Вычисляем новые скорости движения
            roadPart2Move = MoveBy::create(stopTimeWeight, Point(roadSpeedEase(), 0));  //Как-то опытным путём они получились в два раза меньше обычных
            tireFrontRotate = RotateBy::create(stopTimeWeight, distToDegEase());
            tireBackRotate = RotateBy::create(stopTimeWeight, distToDegEase());

            roadPart1Ease = EaseSineOut::create(roadPart1Move); //Создаём "плавное торможение"
            roadPart2Ease = EaseSineOut::create(roadPart2Move);
            tireFrontEase = EaseSineOut::create(tireFrontRotate);
            tireBackEase = EaseSineOut::create(tireBackRotate);

            roadPart1->runAction(roadPart1Ease);    //Добавляем объектам движение на время торможения
            roadPart2->runAction(roadPart2Ease);
            tireFront->runAction(tireFrontEase);
            tireBack->runAction(tireBackEase);
        }
    });

    textSpeed->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == TextField::EventType::DETACH_WITH_IME) {                            //Сразу после ввода скорости
            speed = stof(textSpeed->getString()) * 10;                              //Изменяем значение переменной
            saveData();                                                                 //Также сохраняем все данные в виде json файла
        }
    });

    textTires->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {                        //Сразу после ввода радиуса колёс
            tires = stof(textTires->getString());                                   //Изменяем значение переменной
            tireFront->setScale(0.08 * stof(textTires->getString()) / 30);    //И меняем масштаб
            tireBack->setScale(0.08 * stof(textTires->getString()) / 30);
            saveData();                                                                  //Сохраняем
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

    if (!loadData()) {    //Если json файл отсутствует, то заполняем дефолтными значениями
        speed = 100;
        tires = 30;
                                //Выбор между
        //distWeightManual();   //Ручным
        distWeightRand();       //Случайным
                                //Вводом тормозного пути и веса
        textSpeed->setString(to_string(speed / 10.0));
        textTires->setString(to_string(tires));
        tireFront->setScale(0.08 * tires / 30);
        tireBack->setScale(0.08 * tires / 30);
    }

    this->schedule(schedule_selector(HelloWorld::roadCircle));  //Постоянно выполняется метод зацикливания дороги

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void HelloWorld::roadCircle(float dt) { //Зацикливание дороги
    if (visibleSize.width / 2 - roadPart1->getPositionX() >= roadPart1->getContentSize().width)         //Если левая часть уходит далеко влево
        roadPart1->setPositionX(roadPart2->getPositionX() + roadPart2->getContentSize().width - 1);  //То она появляется справа
    else if (visibleSize.width / 2 - roadPart2->getPositionX() >= roadPart2->getContentSize().width)    //Если правая часть далеко уходит влево
        roadPart2->setPositionX(roadPart1->getPositionX() + roadPart1->getContentSize().width - 1);  //То она появляется справа
}

void HelloWorld::setStopTime(int i) {                                                       //Вычисление времени торможения
    stopTime = 2 * distWeight[i].distance / speed;                                          //2 * тормозной путь / скорость
    stopTimeWeight = 2 * distWeight[i].distance * distWeight[i].weight / (speed * 1000);    //2 * тормозной путь * вес / (скорость / 1000)
}

float HelloWorld::roadSpeed() {         //Обычная скорость перемещения дороги
    return -1 * speed * stopTime;
}

float HelloWorld::roadSpeedEase() {     //Скорость перемещения дороги при торможении
    return roadSpeed() / 2.0;
}

float HelloWorld::distToDeg() {         //Обычная скорость вращения колёс
    float deg;              //360 * Скорость движения дороги / длина окружности колеса
    deg = -roadSpeed() / (M_PI * (tireFront->getContentSize().width * tireFront->getScale()));
    deg *= 360;
    return deg;
}

float HelloWorld::distToDegEase() {     //Скорость вращения колёс при торможении
    float deg;
    deg = -roadSpeedEase() / (M_PI * (tireFront->getContentSize().width * tireFront->getScale()));
    deg *= 360;
    return deg;
}

void HelloWorld::saveData() {   //Сохранение данных в файл
    rapidjson::Document document;
    rapidjson::Value distances(kArrayType);
    rapidjson::Value weights(kArrayType);
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    ofstream saveFile(FileUtils::getInstance()->getWritablePath() + FILENAME);
    if (!saveFile.is_open())                                                    //Если файл не открылся, выходим из функции
        return;
    OStreamWrapper oSaveFile(saveFile);
    Writer<OStreamWrapper> writer(oSaveFile);

    document.SetObject();
    for (int i = 0; i < countDistWeight; i++) {                     //В цикле заполняем каждый массив значениями
        distances.PushBack(distWeight[i].distance, allocator);
        weights.PushBack(distWeight[i].weight, allocator);
    }
    document.AddMember("distance", distances, allocator);   //Добавляем элементы в json строку
    document.AddMember("weight", weights, allocator);
    document.AddMember("speed", speed, allocator);
    document.AddMember("tires", tires, allocator);

    document.Accept(writer);    //Записываем
}

bool HelloWorld::loadData() {   //Получение данных из файла
    rapidjson::Document doc;
    ifstream  file(FileUtils::getInstance()->getWritablePath() + FILENAME); //Открываем файл
    IStreamWrapper ifile(file);
    if (!file.is_open())    //Если не открылся, то возвращаем false
        return false;
    doc.ParseStream(ifile);
    rapidjson::Value &distances = doc["distance"];
    rapidjson::Value &weights = doc["weight"];

    countDistWeight = distances.Size();     //Заполняем количество элементов массива
    distWeight = new DistWeight[countDistWeight];
    for (rapidjson::SizeType i = 0; i < countDistWeight; i++) { //Заполняем сам массив
        distWeight[i].distance = distances[i].GetFloat();
        distWeight[i].weight = weights[i].GetFloat();
    }
    speed = doc.FindMember("speed")->value.GetFloat();  //Получаем скорость и радиус колёс
    tires = doc.FindMember("tires")->value.GetFloat();

    textSpeed->setString(to_string(speed / 10.0));  //Заполняем визуальные компоненты
    textTires->setString(to_string(tires));
    tireFront->setScale(0.08 * tires / 30);
    tireBack->setScale(0.08 * tires / 30);

    return true;
}

void HelloWorld::distWeightRand() {     //Случайное заполнение тормозного пути и веса
    countDistWeight = rand_0_1() * 9 + 5;   //Количество элементов
    distWeight = new DistWeight[countDistWeight];
    for (int i = 0; i < countDistWeight; i++) {
        distWeight[i].distance = rand_0_1() * 1000.0 + 100;
        distWeight[i].weight = rand_0_1() * 500.0 + 1000;
    }
}

void HelloWorld::distWeightManual() {   //Ручное заполнение тормозного пути и веса
    float tmpd[] = {45,     55,     16,     72,     21,     96};    //Сначала заполняются отдельные массивы
    float tmpw[] = {1369,   1176,   1485,   1163,   1290,   1393};  //А затем на их основе заполняется массив структур
    countDistWeight = sizeof(tmpd) / sizeof(float);
    distWeight = new DistWeight[countDistWeight];
    for (int i = 0; i < countDistWeight; i++) {
        distWeight[i].distance = tmpd[i];
        distWeight[i].weight = tmpw[i];
    }
}

int HelloWorld::getRandDistWeight() {   //Получение случайного пути и веса
    float sum = 0;
    float r;
    for (int i = 0; i < countDistWeight; i++)   //Веса складываются
        sum += distWeight[i].weight;
    r = rand_0_1() * (sum - 1.0) + 1;   //Получается какое-то случайное значение между 1 и суммой весов
    sum = 0;
    for (int i = 0; i < countDistWeight; i++) { //Выбирается нужный вес
        if (sum >= r)
            return i;
        sum += distWeight[i].weight;
    }
    return countDistWeight - 1;
}