#ifndef _BLOCK_H
#define _BLOCK_H


#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>

class Block {
public:
    Block();
    Block(WorldData* newWorldData, BasicPlayer* newBasicPlayer);

    bool canIBlock();
    void initializeWorld(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
    void doDash();
    void targetFinder();
    void targetFinder(Point opp);
    int closestOppFinder();

private:
    int check;
    Point target;
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;
};
#endif
