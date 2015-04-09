#ifndef _PLANS_H
#define _PLANS_H

#include <iostream>
#include <WorldData.h>
#include <BasicPlayer.h>
#include <OffensePositioning.h>
#include <DefencePositioning.h>
#include <Goalie.h>
#include <Dribble.h>
#include <Block.h>

#define ROLE_SITUATION_MINE 1
#define ROLE_SITUATION_TEAMMATE 2
#define ROLE_SITUATION_OPP 3
#define ROLE_SITUATION_NOONE 4

//I STARTS FROM OUR GOAL
#define AREA_I 1
#define AREA_II 2
#define AREA_III 3
#define AREA_IV 4

class Plans {
public:
    Plans(WorldData* newWorldData, BasicPlayer* newBasicPlayer);

    void setFormation();
    void start();
    void setSituation();
    Point ballpos; //ball position
    int area; //area : I II III IV
    int situation; //last ball owner

private:
    WorldData* pWorldData;
    BasicPlayer* pBasicPlayer;

    Pass role_Pass;
    Shoot role_Shoot;
    Clear role_Clear;
    DefPos role_defence;
    OfsPos role_offence;
    Dribble role_dribble;
    Block role_Block;

    void passOrDribble();
    void positionMe();
    void blockOrPos();
    void shouldITry();
    int checkValue(int uid); // return 1 -> pass 2->dribble 3->clear
    int myUid;
    int whereIsHe(int oppId);
    bool am_i_near;
    void AmINear();
};
#endif