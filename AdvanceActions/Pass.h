#ifndef _PASS_H
#define _PASS_H

#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>
#include <Circle.h>
#include <Line.h>

using namespace std;

class Pass {
public:
    Pass(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    Pass();
    float getLineValue(Point start, Point end); //it will return the value of the given line
    void bestTeammateSelector(); //it will set the bestteammate
    void passNow();
    void setUid(int givenID);
    //float nextStep(Point teammate);
    int bestTeammate;
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    float passLastValue();
    Point ballPos;

private:
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
    bool secondStep;
    int callerUid;
};
#endif