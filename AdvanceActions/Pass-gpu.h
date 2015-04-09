#ifndef _CLEARGPU_H
#define _CLEARGPU_H
#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>
#include <Circle.h>
#include <Line.h>
#include <cmath>
#include <stdlib.h>
#include <cstdio>

class gpuPass
{
public :
    gpuPass(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    gpuPass();
    void gpuPassNow();
    void bestTeammateSelector();
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    float passLastValue();
    void setUid(int newId);
    void setBallPos(Point newBallPos);
    void setReachableDist(float newReachable);
    Point getSelectedPoint();
    int getSelectedId();
    void kickIt();

private :
    Point selectedPoint; //for indirect passing
    unsigned short int callerId ;
    unsigned short int selectedId ;
    Point ballPos ;
    float bestValue ;
    WorldData* pWorldData ;
    BasicPlayer* pBasicPlayer ;
    float passReachableRadius ;
};

#endif