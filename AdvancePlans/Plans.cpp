#include <Plans.h>

Plans::Plans(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    role_Clear.initializeData(pWorldData, pBasicPlayer);
    role_Pass.initializeData(pWorldData, pBasicPlayer);
    role_Shoot.initializeData(pWorldData, pBasicPlayer);
    role_dribble.initializeData(pWorldData, pBasicPlayer);
    role_Block.initializeWorld(pWorldData, pBasicPlayer);
    ballpos = pWorldData->basedCenter(pWorldData->ball());
    myUid = pWorldData->my_id();
    am_i_near = false;
    AmINear();
}

/*this is not completed yet*/
void Plans::setFormation()                                                                               //////////////////////////////////////
{
    int defenders[2]; //uids
    int defendersNum;

    int ofenders[2]; //uids
    int ofendersNum;

    defenders[0] = 1;
    defenders[1] = 2;
    pWorldData->mrRoles[1] = MR_ROLE_DEFENCE ;
    pWorldData->mrRoles[2] = MR_ROLE_DEFENCE ;
    defendersNum = 2;

    ofenders[0] = 3;
    ofenders[1] = 4;
    pWorldData->mrRoles[3] = MR_ROLE_OFFENCE ;
    pWorldData->mrRoles[4] = MR_ROLE_OFFENCE ;
    ofendersNum = 2;

    role_defence.initializeWorld(pWorldData, pBasicPlayer, defenders, defendersNum, 'r');
    role_offence.initializeWorld(pWorldData, pBasicPlayer, 'l', ofenders, ofendersNum);
}

void Plans::AmINear()
{
    if (ballpos.getDistance(pWorldData->me(true)) < 200)
        am_i_near = true;
    else
        am_i_near = false;
}

int Plans::whereIsHe(int oppId)
{
    Point opponentha[5];
    vector<int> foundOpps = pWorldData->mr_found_opponents();
    for (int i = 0; i < foundOpps.size(); i++)
    {
        opponentha[i] = pWorldData->oppPositions[foundOpps[i] % 5];
    }
    Point oppPos = opponentha[oppId % 5];

    char side;

    if (pWorldData->ourPole1.getX() > 0)
        side = 'r';
    else
        side = 'l';

    if (oppPos.getX() >= 0)
    {
        if (oppPos.getX() > pWorldData->realVectors[15].getX() / 2)
        {
            if (side == 'r')
                area = AREA_I;
            else if (side == 'l')
                area = AREA_IV;
        }
        else
        {
            if (side == 'r')
                area = AREA_II;
            else if (side == 'l')
                area = AREA_III;
        }
    }
    else
    {
        if (oppPos.getX() < pWorldData->realVectors[11].getX() / 2)
        {
            if (side == 'r')
                area = AREA_IV;
            else if (side == 'l')
                area = AREA_I;
        }
        else
        {

            if (side == 'r')
                area = AREA_III;
            else if (side == 'l')
                area = AREA_II;
        }
    }
    return area;
}

void Plans::setSituation()
{
    bool found = false;

    vector<int> teamms = pWorldData->mr_found_teammates();
    vector<int> opponts = pWorldData->mr_found_opponents();

    if (ballpos.getDistance(pWorldData->me(true)) <= 36)
    {
        situation = ROLE_SITUATION_MINE;
        found = true;
        pWorldData->owner = pWorldData->my_id();
    }

    if (found == false)
    {
        for (int i = 0; i < teamms.size(); i++)
        {
            if (ballpos.getDistance(pWorldData->tmmPositions[i]) <= 36)
            {
                found = true;
                pWorldData->owner = teamms[i];
                situation = ROLE_SITUATION_TEAMMATE;
                break;
            }
        }
    }

    if (found == false)
    {
        for (int i = 0; i < opponts.size(); i++)
        {

            if (ballpos.getDistance(pWorldData->oppPositions[i]) <= 36)
            {
                found = true;
                //pWorldData->owner = opponts[i];
                pWorldData->owner = -1;                                                                  //////////////////////////////////////////
                situation = ROLE_SITUATION_OPP;
                break;
            }
        }
    }

    if (found == false)
    {
        situation = ROLE_SITUATION_NOONE;
        pWorldData->owner = -1;
    }
}

int Plans::checkValue(int uid)
{
    role_Pass.setUid(uid);
    role_Pass.bestTeammateSelector();
    role_dribble.setUid(uid);
    role_dribble.targetFinder();
    role_Clear.setUid(uid);
    role_Clear.pointSelector();

    if (role_Pass.passLastValue() == 100 || role_Pass.passLastValue() >= role_dribble.dribbleLastValue() && role_Pass.passLastValue() >= 50)
    {
        return 1;
    }
    else if (role_dribble.dribbleLastValue() == 100 || role_dribble.dribbleLastValue() >= role_Clear.clearLastValue() && role_dribble.dribbleLastValue() >= 50)
    {
        return 2;
    }
    else
    {
        return 3;
    }

}

void Plans::shouldITry()
{
    vector<int> opps = pWorldData->mr_found_opponents();
    vector<int> tmms = pWorldData->mr_found_teammates();

    float minDist = 1000 ;

    role_Clear.pointSelector();
    
    //we should find the nearest and if the nearest is me then i should try
    for (int i = 0 ; i < tmms.size() ; i++)
    {
        if (pWorldData->tmmPositions[tmms[i] % 5].getDistance(role_Clear.selectedPoint) < minDist)
            minDist = pWorldData->tmmPositions[tmms[i] % 5].getDistance(role_Clear.selectedPoint);
    }
    
    if (pWorldData->me(true).getDistance(role_Clear.selectedPoint) < minDist && minDist <= 120)
    {
        pBasicPlayer->go(role_Clear.selectedPoint.getX(), role_Clear.selectedPoint.getY());
        pWorldData->teamAction[myUid] = ACTION_GO;
    }
    else
    {
        for (int i = 0; i < opps.size(); i++)
        {
            if (pWorldData->oppPositions[opps[i] % 5].getDistance(role_Clear.selectedPoint) < 170)
            {
                role_Block.targetFinder(ballpos);
                role_Block.doDash();
                pWorldData->teamAction[myUid] = ACTION_BLOCK ;
            }
        }
    }
}

void Plans::passOrDribble()
{
    role_Pass.bestTeammateSelector();
    if (role_Pass.bestTeammate != -1)
    {
        role_Pass.passNow();
        pWorldData->teamAction[myUid] = ACTION_PASS;
    }
    else
    {
        role_dribble.decide();
        pWorldData->teamAction[myUid] = ACTION_DRIBBLE;
    }
}

void Plans::positionMe()
{
    for (int i = 0; i < pWorldData->mr_number_found_teammates(); i++)
    {
        if (myUid == role_defence.resultId[i])
        {
            pBasicPlayer->go(role_defence.resultPos[i].x, role_defence.resultPos[i].y);
            pWorldData->teamAction[myUid] = ACTION_DEFPOS;
            break;
        }
        else if (myUid == role_offence.resultId[i])
        {
            pBasicPlayer->go(role_offence.resultPos[i].x, role_offence.resultPos[i].y);
            pWorldData->teamAction[myUid] = ACTION_OFSPOS;
            break;
        }
    }

}

void Plans::blockOrPos()
{
    if (pWorldData->mrRoles[myUid] == MR_ROLE_DEFENCE)
    {
        if (role_Block.canIBlock() == true)
        {
            pWorldData->teamAction[myUid] = ACTION_BLOCK ;
            role_Block.targetFinder();
            role_Block.doDash();
        }
        else
        {
            positionMe();
        }
    }
    else if (pWorldData->mrRoles[myUid] == MR_ROLE_OFFENCE)
    {
        positionMe();
    }

}

void Plans::start()
{
    /***************IF I AM NOT GOALER***************/
    if (pWorldData->mrRoles[myUid] != MR_ROLE_GOALIE)
    {
        /***********IF I POSSES THE BALL*****************/
        if (situation == ROLE_SITUATION_MINE)
        {
            if (pWorldData->mrRoles[myUid] == MR_ROLE_DEFENCE)
            {
                int tempValue = checkValue(myUid);

                if (tempValue == 1)
                {
                    role_Pass.passNow();
                    pWorldData->teamAction[myUid % 5] = ACTION_PASS ;
                }
                else if (tempValue == 2)
                {
                    pWorldData->teamAction[myUid % 5] = ACTION_BLOCK ;
                    role_dribble.decide();
                }
                else if (tempValue == 3)
                {
                    pWorldData->teamAction[myUid % 5] = ACTION_CLEAR ;
                    role_Clear.clearNow();
                }
            }
            else if (pWorldData->mrRoles[myUid % 5] == MR_ROLE_OFFENCE)
            {
                role_Shoot.pointSelector();
                if (role_Shoot.can_i_shoot == true)
                {
                    role_Shoot.shootNow();
                    pWorldData->teamAction[myUid % 5] = ACTION_SHOOT;
                }
                else
                {
                    int tempValue = checkValue(myUid); //moshakhas mikonad ke alan chikar mikonim (pass/dribble/clear)

                    if (tempValue == 1)
                    {
                        pWorldData->teamAction[myUid % 5] = ACTION_PASS ;
                        role_Pass.passNow();
                    }
                    else if (tempValue == 2)
                    {
                        pWorldData->teamAction[myUid % 5] = ACTION_BLOCK ;
                        role_dribble.decide();
                    }
                    else if (tempValue == 3)
                    {
                        pWorldData->teamAction[myUid % 5] = ACTION_CLEAR ;
                        role_Clear.clearNow();
                    }
                }
            }
        }
        else if (situation == ROLE_SITUATION_TEAMMATE)
        {
            if (am_i_near == false)
                positionMe();
            else if (pWorldData->mrRoles[(pWorldData->owner) % 5] == MR_ROLE_GOALIE)
            {
                role_Pass.setUid(pWorldData->owner);
                role_Pass.bestTeammateSelector();
                int bestTmmId = role_Pass.bestTeammate;
                if (bestTmmId == myUid)
                {
                    //do nothing (stay where you are)
                }
                else if (bestTmmId != -1)
                {
                    if (role_Block.canIBlock() == true)
                    {
                        role_Block.targetFinder();
                        role_Block.doDash();
                    }
                    else
                    {
                        positionMe();
                    }
                }
                else
                {
                    shouldITry();
                }
            }
            else if (pWorldData->mrRoles[(pWorldData->owner) % 5] == MR_ROLE_DEFENCE)
            {
                int tempValue = checkValue(pWorldData->owner);
                if (tempValue == 1)
                {
                    role_Pass.setUid(pWorldData->owner);
                    role_Pass.bestTeammateSelector();
                    int bestTmm = role_Pass.bestTeammate;
                    if (bestTmm == myUid)
                    {
                        //do nothing (stay where you are)
                    }
                    else if (bestTmm != -1)
                    {
                        if (pWorldData->mrRoles[myUid % 5] == MR_ROLE_DEFENCE)
                        {
                            if (role_Block.canIBlock() == true)
                            {
                                role_Block.targetFinder();
                                role_Block.doDash();
                            }
                            else
                            {
                                positionMe();
                            }
                        }
                        else
                        {
                            //what should i do if i am offence ?                                        ///////////////////////
                        }
                    }
                }
                else if (tempValue == 2)
                {
                    if (role_Block.canIBlock() == true)
                    {
                        role_Block.targetFinder();
                        role_Block.doDash();
                    }
                    else
                    {
                        positionMe();
                    }
                }
                else if (tempValue == 3)
                {
                    shouldITry();
                }
            }
            else if (pWorldData->mrRoles[pWorldData->owner] == MR_ROLE_OFFENCE)
            {
                role_Shoot.setUid(pWorldData->owner);
                role_Shoot.pointSelector();
                Point tempPoint = role_Shoot.selectedPoint;
                if (role_Shoot.getLineValue(ballpos, tempPoint) > 65)
                {
                    Line forbiddenLine;
                    forbiddenLine.setByPoints(ballpos, tempPoint);
                    //i should get away from that fuckin line
                }
                else
                {
                    int tempValue = checkValue(pWorldData->owner);
                    if (tempValue == 1)
                    {
                        role_Pass.setUid(pWorldData->owner);
                        role_Pass.bestTeammateSelector();
                        int bestTmm = role_Pass.bestTeammate;
                        if (bestTmm == myUid)
                        {
                            pBasicPlayer->set_velocity(0, 0);
                        }
                        else if (bestTmm != -1)
                        {
                            if (pWorldData->mrRoles[myUid] == MR_ROLE_OFFENCE)
                            {
                                positionMe();
                            }
                        }
                    }
                    else if (tempValue == 2)
                    {
                        if (role_Block.canIBlock() == true)
                        {
                            role_Block.targetFinder();
                            role_Block.doDash();
                        }
                        else
                        {
                            positionMe();
                        }
                    }
                    else if (tempValue == 3)
                    {
                        shouldITry();
                    }
                }
            }
        }
        else if (situation == ROLE_SITUATION_OPP)
        {
            if (am_i_near == false)
                positionMe();
            else
            {

                /**not completed yet**/                                             /////////////////////////////////////
                int oppArea = whereIsHe(pWorldData->owner);
                if (oppArea == AREA_I)
                {
                    role_Block.targetFinder();
                    role_Block.doDash();
                }
                else if (oppArea == AREA_II)
                {
                    role_Block.targetFinder();
                    role_Block.doDash();
                }
                else if (oppArea == AREA_III)
                {
                    role_Block.targetFinder();
                    role_Block.doDash();
                }
                else if (oppArea == AREA_IV)
                {
                    role_Block.targetFinder();
                    role_Block.doDash();
                }
            }
        }

        else //if no one owns the ball
        {
            if (ballpos.getDistance(pWorldData->me(true)) < 200)
                am_i_near = true;
            else
                am_i_near = false;

            if (am_i_near == false)
                positionMe();
            else
            {
                if (pWorldData->owner == myUid) //i WAS the owner
                {
                    if (pWorldData->teamAction[myUid] == ACTION_SHOOT)
                    {
                        pBasicPlayer->go(ballpos.getX(), ballpos.getY());
                    }
                    else if (pWorldData->teamAction[myUid] == ACTION_PASS || pWorldData->teamAction[myUid] == ACTION_CLEAR)
                    {
                        blockOrPos();
                    }
                    else if (pWorldData->teamAction[myUid] == ACTION_DRIBBLE)
                    {
                        role_dribble.doDash();
                    }
                }
                else if (pWorldData->owner != -1) // means we ownded it  if owner == -1 then they owned it but what if no one owns it ?
                {
                    if (pWorldData->teamAction[pWorldData->owner] == ACTION_SHOOT)
                    {
                        blockOrPos();
                    }
                    else if (pWorldData->teamAction[pWorldData->owner] == ACTION_PASS)
                    {
                        role_Pass.setUid(pWorldData->owner);
                        role_Pass.bestTeammateSelector();
                        if (role_Pass.bestTeammate == myUid)
                        {
                            pBasicPlayer->set_velocity(0, 0);
                        }
                        else //if it is not for me
                        {
                            blockOrPos();
                        }
                    }
                    else if (pWorldData->teamAction[pWorldData->owner] == ACTION_CLEAR)
                    {
                        shouldITry();
                    }
                    else if (pWorldData->teamAction[pWorldData->owner] == ACTION_DRIBBLE)
                    {
                        blockOrPos();
                    }
                }
                else if (pWorldData->owner == -1) //no one owns the ball
                {
                    if (am_i_near == false)
                        blockOrPos();
                    else if (am_i_near == true)
                    {
                        pBasicPlayer->go(ballpos.getX(), ballpos.getY());
                    }
                }
            }
        }
    }
}