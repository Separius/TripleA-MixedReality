#ifndef _DRIBBLE_H
#define _DRIBBLE_H
#include <WorldData.h>
#include <BasicPlayer.h>

class Dribble {
public:
    Dribble(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    Dribble();
    void setUid(int newID);
    void initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    void targetFinder();
    void doDash();
    void doKick();
    void decide();
    float dribbleLastValue();
    Line bestLine;

private:
    int callerUid;
    Point target;
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
};
#endif