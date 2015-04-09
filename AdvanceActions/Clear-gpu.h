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

class gpuClear
{
public :
    gpuClear(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    gpuClear();
    void gpuClearNow();
    void pointSelector();
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    float clearLastValue();
    void setBallPos(Point newBallPos);
    void setReachableDist(int newReachable);
    Point getSelectedPoint();

private :
    Point selectedPoint;
    Point ballPos ;
    float bestValue ;
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
    int clearReachableRadius ;
};

#endif