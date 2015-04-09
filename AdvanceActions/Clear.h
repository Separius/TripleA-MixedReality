#ifndef _CLEAR_H
#define _CLEAR_H

#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>
#include <Circle.h>
#include <Line.h>

class Clear {
public:
    Clear(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    Clear();
    float getLineValue(Point start, Point end); //it will return the value of the given angle
    void pointSelector(); //it will select the best end Point
    void clearNow();
    Point selectedPoint;
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    void setUid(int givenID);
    float clearLastValue();

private:
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
    int callerUid;
    Point ballPos;
};
#endif