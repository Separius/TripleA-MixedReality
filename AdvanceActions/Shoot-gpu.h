#ifndef _SHOOTGPU_H
#define _SHOOTGPU_H
#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>
#include <Circle.h>
#include <Line.h>
#include <cmath>
#include <stdlib.h>
#include <cstdio>

class gpuShoot
{
public :
    gpuShoot(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    gpuShoot();
    void gpuShootNow();
    void gpuKickNow(Point target);
    void pointSelector();
    void goalChooser(char whichOne);
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    void setUid(int givenID);
    float shootLastValue();
    void setBallPos(Point newBallPos);
    void setReachableDist(float newReachable);
    Point getSelectedPoint();
    
private :
    char whichGoal;
    int callerUid;
    Point selectedPoint;
    Point ballPos ;
    float bestValue ;
    bool canShoot ;
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
    float shootReachableRadius ;
};

#endif