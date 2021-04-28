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

    visibleSize = Director::getInstance()->getWinSize();
    origin = Director::getInstance()->getVisibleOrigin();
	xResize = visibleSize.width / 1280.0;	//Здесь я пытался сделать переменные для того, чтоб скейлить
	yResize = visibleSize.height / 720.0;	//под разные разрешения, но оно не получилось(
	averResize = (visibleSize.width * visibleSize.height) / (1280 * 720);

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

    labelDistance = Label::createWithTTF("Введите тормозной путь в метрах", "fonts/arial.ttf", 20 * averResize);
    labelSpeed = Label::createWithTTF("Введите скорость в км/ч", "fonts/arial.ttf", 20 * averResize);
    labelWeight = Label::createWithTTF("Введите вес в килограммах", "fonts/arial.ttf", 20 * averResize);
    labelTires = Label::createWithTTF("Введите радиус колёс в сантиметрах", "fonts/arial.ttf", 20 * averResize);

    textDistance = TextField::create("X", "fonts/arial.ttf", 20 * averResize);
    textSpeed = TextField::create("X", "fonts/arial.ttf", 20 * averResize);
    textWeight = TextField::create("X", "fonts/arial.ttf", 20 * averResize);
    textTires = TextField::create("X", "fonts/arial.ttf", 20 * averResize);

    ///---------- Свойства объектов ----------

    blueBackground->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    blueBackground->setScale(1.5 * averResize);

    roadPart1->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 250 * yResize));
	roadPart1->setScale(averResize);

    roadPart2->setPosition(Vec2(roadPart1->getPositionX() + roadPart2->getContentSize().width * averResize - 5, visibleSize.height / 2 + 250 * yResize));
	roadPart2->setScale(averResize);

    car->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50 * yResize));
	car->setScale(averResize);

    tireFront->setPosition(Vec2(car->getPositionX() + 290 * xResize, car->getPositionY() - 160 * yResize));

    tireBack->setPosition(Vec2(car->getPositionX() - 255 * xResize, car->getPositionY() - 160 * yResize));


    buttonStart->setPosition(Vec2(visibleSize.width - 160 * xResize, visibleSize.height - 50 * yResize));
    buttonStart->setScale(0.5 * averResize);

    buttonStop->setPosition(Vec2(visibleSize.width - 160 * xResize, visibleSize.height - 150 * yResize));
    buttonStop->setScale(0.5 * averResize);
    buttonStop->setEnabled(false);


    labelDistance->setPosition(Vec2(220 * xResize, visibleSize.height - 20 * yResize));
    labelDistance->setWidth(400);
    labelDistance->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelSpeed->setPosition(Vec2(220 * xResize, visibleSize.height - 80 * yResize));
    labelSpeed->setWidth(400);
    labelSpeed->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelWeight->setPosition(Vec2(220 * xResize, visibleSize.height - 140 * yResize));
    labelWeight->setWidth(400);
    labelWeight->enableOutline(Color4B(0, 0, 0, 255), 1);

    labelTires->setPosition(Vec2(220 * xResize, visibleSize.height - 200 * yResize));
    labelTires->setWidth(400);
    labelTires->enableOutline(Color4B(0, 0, 0, 255), 1);


    textDistance->setPosition(Vec2(labelDistance->getPositionX() - 50 * xResize, labelDistance->getPositionY() - 30 * yResize));
    textDistance->setMaxLength(10);
    textDistance->setEnabled(false);

    textSpeed->setPosition(Vec2(labelSpeed->getPositionX() - 50 * xResize, labelSpeed->getPositionY() - 30 * yResize));
    textSpeed->setMaxLength(10);

    textWeight->setPosition(Vec2(labelWeight->getPositionX() - 50 * xResize, labelWeight->getPositionY() - 30 * yResize));
    textWeight->setMaxLength(10);
    textWeight->setEnabled(false);

    textTires->setPosition(Vec2(labelTires->getPositionX() - 50 * xResize, labelTires->getPositionY() - 30 * yResize));
    textTires->setMaxLength(10);

    ///---------- Функции для кнопок и полей ввода ----------

    buttonStart->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {  //Нажатие на кноку "старт"
        if (type == Widget::TouchEventType::BEGAN) {
            int i = getRandDistWeight();    //Случайно выбирается тормозной путь и вес и вписываются в поля
            buttonStop->setEnabled(true);
            textDistance->setString(to_string(distWeight[i].distance));
            textWeight->setString(to_string(distWeight[i].weight));
			unschedule(schedule_selector(HelloWorld::roadStop));
			
			setStopTime(i); //Вычисляем время торможения

            this->schedule(schedule_selector(HelloWorld::roadMove));	//Запускаем постоянное движение дороги и колёс
        }
    });

    buttonStop->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::BEGAN) {
            buttonStop->setEnabled(false);
            unschedule(schedule_selector(HelloWorld::roadMove));	//Убираем постоянное движение

			stopTimeTmp = stopTimeFull;
			this->schedule(schedule_selector(HelloWorld::roadStop));	//Запускаем остановку
        }
    });

    textSpeed->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == TextField::EventType::DETACH_WITH_IME) {                            //Сразу после ввода скорости
            speed = stof(textSpeed->getString());                              //Изменяем значение переменной
            saveData();                                                                 //Также сохраняем все данные в виде json файла
        }
    });

    textTires->addEventListener([&](Ref* sender, TextField::EventType type) {
        if (type == ui::TextField::EventType::DETACH_WITH_IME) {                        //Сразу после ввода радиуса колёс
            tires = stof(textTires->getString());                                   //Изменяем значение переменной
            tireFront->setScale(averResize * 0.06 * stof(textTires->getString()) / 30);    //И меняем масштаб
            tireBack->setScale(averResize * 0.06 * stof(textTires->getString()) / 30);
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
        speed = 10;
        tires = 30;
                                //Выбор между
        //distWeightManual();   //Ручным
        distWeightRand();       //Случайным
                                //Вводом тормозного пути и веса
        textSpeed->setString(to_string(speed));
        textTires->setString(to_string(tires));
        tireFront->setScale(averResize * 0.06 * tires / 30);
        tireBack->setScale(averResize * 0.06 * tires / 30);
    }

    this->schedule(schedule_selector(HelloWorld::roadCircle));  //Постоянно выполняется метод зацикливания дороги

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender) {
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

void HelloWorld::roadCircle(float dt) { //Зацикливание дороги
    if (visibleSize.width / 2 - roadPart1->getPositionX() >= roadPart1->getContentSize().width * averResize)         //Если левая часть уходит далеко влево
        roadPart1->setPositionX(roadPart2->getPositionX() + roadPart2->getContentSize().width * averResize - 5);  //То она появляется справа
    else if (visibleSize.width / 2 - roadPart2->getPositionX() >= roadPart2->getContentSize().width * averResize)    //Если правая часть далеко уходит влево
        roadPart2->setPositionX(roadPart1->getPositionX() + roadPart1->getContentSize().width * averResize - 5);  //То она появляется справа
}

void HelloWorld::roadMove(float dt) {	//Равномерное движение дороги
    roadPart1->setPositionX(roadPart1->getPositionX() + roadSpeed());
    roadPart2->setPositionX(roadPart2->getPositionX() + roadSpeed());
    tireFront->setRotation(tireFront->getRotation() + distToDeg());
    tireBack->setRotation(tireBack->getRotation() + distToDeg());
}

void HelloWorld::roadStop(float dt) {	//Движение дороги с остановкой
	stopTimeTmp -= dt;
	if (stopTimeTmp <= 0)		//Если время вышло
		this->unschedule(schedule_selector(HelloWorld::roadStop));
	float tmp = stopTimeTmp / stopTimeFull;
	roadPart1->setPositionX(roadPart1->getPositionX() + roadSpeed() * tweenfunc::sineEaseOut(tmp));	//Уменьшение скорости
	roadPart2->setPositionX(roadPart2->getPositionX() + roadSpeed() * tweenfunc::sineEaseOut(tmp));	//По функции
	tireFront->setRotation(tireFront->getRotation() + distToDeg() * tweenfunc::sineEaseOut(tmp));	//sineEaseOut
	tireBack->setRotation(tireBack->getRotation() + distToDeg() * tweenfunc::sineEaseOut(tmp));
}

void HelloWorld::setStopTime(int i) {                                                       //Вычисление времени торможения
	stopTimeFull = M_PI_2 * distWeight[i].distance * distWeight[i].weight / ((speed / 3.6) * 1000);    //pi/2 * тормозной путь * вес / ((скорость / 3.6) * 1000)
}																							  //Вес / 1000 определяет коэффициент времени торможения от веса

float HelloWorld::roadSpeed() {         //Скорость перемещения дороги
    return -1 * speed;
}

float HelloWorld::distToDeg() {         //Скорость вращения колёс
    float deg;              //360 * Скорость движения дороги / длина окружности колеса
    deg = -roadSpeed() / (M_PI * (tireFront->getContentSize().width * tireFront->getScale()));
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
    delete distWeight;
    distWeight = new DistWeight[countDistWeight];
    for (int i = 0; i < countDistWeight; i++) { //Заполняем сам массив
        distWeight[i].distance = distances[i].GetFloat();
        distWeight[i].weight = weights[i].GetFloat();
    }
    speed = doc.FindMember("speed")->value.GetFloat();  //Получаем скорость и радиус колёс
    tires = doc.FindMember("tires")->value.GetFloat();

    textSpeed->setString(to_string(speed));  //Заполняем визуальные компоненты
    textTires->setString(to_string(tires));
    tireFront->setScale(averResize * 0.06 * tires / 30);
    tireBack->setScale(averResize * 0.06 * tires / 30);

    return true;
}

void HelloWorld::distWeightRand() {     //Случайное заполнение тормозного пути и веса
    countDistWeight = rand_0_1() * 19 + 5;   //Количество элементов
    delete distWeight;
    distWeight = new DistWeight[countDistWeight];
    for (int i = 0; i < countDistWeight; i++) {
        distWeight[i].distance = rand_0_1() * 100.0 + 10;
        distWeight[i].weight = rand_0_1() * 500.0 + 1000;
    }
}

void HelloWorld::distWeightManual() {   //Ручное заполнение тормозного пути и веса
    float tmpd[] = {45,     55,     16,     72,     21,     96};    //Сначала заполняются отдельные массивы
    float tmpw[] = {1369,   1176,   1485,   1163,   1290,   1393};  //А затем на их основе заполняется массив структур
    countDistWeight = sizeof(tmpd) / sizeof(float);
    delete distWeight;
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