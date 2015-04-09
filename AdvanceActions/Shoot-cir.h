#ifndef _SHOOT_H
#define _SHOOT_H
#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>
#include <Circle.h>
#include <Line.h>

using namespace std;

class Shoot {
public:
    Shoot(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    Shoot();
    float getLineValue(Point start, Point end);
    void pointSelector();
    void shootNow();
    void reachPoints();
    void goalChooser(char whichOne);
    Point selectedPoint;
    bool can_i_shoot;
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    void setUid(int givenID);
    float shootLastValue();
    //void selfCollision();
    void setBallPos(Point newBallPos);
    void setReachableDist(float newReachable);

private:

    Point firstReachablePoint;
    Point lastReachablePoint;
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
    char whichGoal;
    int callerUid;
    Point ballPos;
    float shootReachableDist;
    
};
#endif