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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "time.h"
#include "fstream"
#include "external/json/document.h"
#include "external/json/ostreamwrapper.h"
#include "external/json/istreamwrapper.h"
#include "external/json/writer.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
using namespace rapidjson;

class HelloWorld : public cocos2d::Scene {
private:

    ///---------- Структуры ----------

    struct DistWeight {
        float distance;
        float weight;
    };

    ///---------- Константы ----------

    const string FILENAME = "test.json";

    ///---------- Переменные ----------

    Size visibleSize;
    Vec2 origin;

    Sprite *blueBackground;
    Sprite *roadPart1;
    Sprite *roadPart2;
    Sprite *car;
    Sprite *tireFront;
    Sprite *tireBack;

    Button *buttonStop;
    Button *buttonStart;

    Label *labelDistance;
    Label *labelSpeed;
    Label *labelWeight;
    Label *labelTires;

    TextField *textDistance;
    TextField *textSpeed;
    TextField *textWeight;
    TextField *textTires;

    MoveBy *roadPart1Move;
    MoveBy *roadPart2Move;
    RotateBy *tireFrontRotate;
    RotateBy *tireBackRotate;

    EaseSineOut *roadPart1Ease;
    EaseSineOut *roadPart2Ease;
    EaseSineOut *tireFrontEase;
    EaseSineOut *tireBackEase;

    int countDistWeight;
    DistWeight *distWeight;
    float speed;
    float stopTime;
    float stopTimeWeight;
    float tires;

    ///---------- Функции ----------

    void roadCircle(float dt);

    void setStopTime(int i);

    float roadSpeed();

    float roadSpeedEase();

    float distToDeg();

    float distToDegEase();

    void saveData();

    bool loadData();

    void distWeightRand();

    void distWeightManual();

    int getRandDistWeight();

public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    void menuCloseCallback(cocos2d::Ref* pSender);

    CREATE_FUNC(HelloWorld);
};

#endif __HELLOWORLD_SCENE_H__
