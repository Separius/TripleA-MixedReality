#include <Pass.h>

/*
 To Do :
 * indirect passing
 * it passes to ball to the center of the teammates !
 * kick power = ?
 * i should call a clear if i can not pass
 * complte nextStep function (and also next step circle radiuses are bigger)
 * prevent Self Collision (decide then rotate || only check tmms that do not make collision || rotate to the degree that we can pass most tmms without collision)
 * step kick
 * ball 's velocity before kick will change the angle and also ball reachable dist
 * there is collision if opps move
 * circle radiuses are ass generated !
 * This is the correct shape : (it is not 2 circles :D)
 *               ___
 *              |   |
 *             /|tm |\
 *            /  \ /  \
 *          /  op | op  \
 *          _____________
 */

/* ValueSystem : (0 : it can not pass the ball to anyone) () (100 : it is a safePass)  */

Pass::Pass(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    bestTeammate = -1;
    callerUid = pWorldData->my_id();
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

Pass::Pass()
{
    bestTeammate = -1;
}

void Pass::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    ballPos = pWorldData->basedCenter(pWorldData->ball());
    callerUid = pWorldData->my_id();
}

void Pass::setUid(int givenID)
{
    callerUid = givenID;
}

float Pass::getLineValue(Point start, Point end)
{
    Line passLine;
    Circle startCircle, endCircle;
    Vector passVector;

    vector <int> opps = pWorldData->mr_found_opponents();

    passVector.setByPoints(start, end);
    passLine.setByPoints(start, end);

    Point startCenter = start + ((passVector) / 4);
    Point endCenter = end;

    float d = start.getDistance(end);
    float startRadius = (5 * d) / 16;
    float endRadius = d / 2;
    float startZarib = 1;
    float endZarib = 0.5;
    float safeValue = 100;

    startCircle.setByCenterRadius(startCenter, startRadius);
    endCircle.setByCenterRadius(endCenter, endRadius);

    float minValue = 9999;
    for (int i = 0; i < opps.size(); i++)
    {
        Point thisOpp = pWorldData->oppPositions[i];

        if (startCircle.checkPointInArea(thisOpp))
        {
            if (passLine.getPointDist(thisOpp) < 40)
            {
                minValue = 0;
                break;
            }
            if ((passLine.getPointDist(thisOpp) * startZarib) < minValue)
                minValue = passLine.getPointDist(thisOpp) * startZarib;
        }
        else if (endCircle.checkPointInArea(thisOpp))
        {
            if (passLine.getPointDist(thisOpp) < 60)
            {
                minValue = 0;
                break;
            }
            if ((passLine.getPointDist(thisOpp) * endZarib) < minValue)
                minValue = passLine.getPointDist(thisOpp) * endZarib;
        }
    }

    if (minValue == 9999)
    {
        minValue = safeValue;
    }

    return minValue;
}

void Pass::bestTeammateSelector()
{
    float maxValue = 0;
    //float zaribNextStep = 1;
    float zaribLineValue = 1;
    float thisValue;
    //163 = kick dist with power 1 and 35 = teammate kickable radius
    float passReachableDist = 163.927 + 35;
    float safeValue = 100;
    float maxDistFromGoal = 0;

    vector <int> tmms = pWorldData->mr_found_teammates();

    Point thisTeammate;
    Point ourGoal;

    ourGoal.x = pWorldData->ourPole1.getX();
    ourGoal.y = (pWorldData->ourPole1.getY() + pWorldData->ourPole2.getY()) / 2;

    bestTeammate = -1;

    for (int i = 0; i < tmms.size(); i++)
    {
        if (tmms[i] != callerUid)
        {
            thisTeammate = pWorldData->tmmPositions[i];

            if (ballPos.getDistance(thisTeammate) > passReachableDist)
            {
                thisValue = 0;
            }
            else
            {
                thisValue = zaribLineValue * getLineValue(ballPos, thisTeammate);

                if (thisValue == safeValue * zaribLineValue && ourGoal.getDistance(thisTeammate) > maxDistFromGoal)
                {
                    bestTeammate = tmms[i];
                    maxValue = thisValue;
                    maxDistFromGoal = ourGoal.getDistance(thisTeammate);
                }

                else if (thisValue > maxValue && getLineValue(ballPos, thisTeammate) != 0)
                {
                    maxValue = thisValue;
                    bestTeammate = tmms[i];
                }
            }
        }
    }

    //if we call the pass for another uid it won't check myID (for passing) so we have to do it by ourself
    if (callerUid != pWorldData->my_id())
    {
        thisTeammate.x = pWorldData->me(true).getX();
        thisTeammate.y = pWorldData->me(true).getY();
        if (ballPos.getDistance(thisTeammate) > passReachableDist)
        {
            thisValue = 0;
        }
        else
        {
            thisValue = zaribLineValue * getLineValue(ballPos, thisTeammate);

            if (thisValue == safeValue * zaribLineValue && ourGoal.getDistance(thisTeammate) > maxDistFromGoal)
            {
                bestTeammate = pWorldData->my_id();
                maxDistFromGoal = ourGoal.getDistance(thisTeammate);
            }

            else if (thisValue > maxValue && getLineValue(ballPos, thisTeammate) != 0)
            {
                maxValue = thisValue;
                bestTeammate = pWorldData->my_id();
            }
        }
    }
}

void Pass::passNow()
{
    bestTeammateSelector();

    if (bestTeammate != -1) //if there are no good passes it won't pass !
    {
        float angle;

        Vector passVector;

        passVector.setByPoints(ballPos, pWorldData->basedCenter(pWorldData->teammate(bestTeammate)));

        angle = pWorldData->bodyAngle() - passVector.getDirection();
        pBasicPlayer->kick(angle * M_PI / 180, 1);
    }
    else
    {
        pBasicPlayer->kick(M_PI / 2, 1);
        //we should call clear 
    }
}

float Pass::passLastValue()
{
    if (bestTeammate != -1)
    {
        return getLineValue(ballPos, pWorldData->basedCenter(pWorldData->teammate(bestTeammate)));
    }
    else
    {
        return 0;
    }
}