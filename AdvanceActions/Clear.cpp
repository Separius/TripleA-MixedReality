#include <Clear.h>
#include <fstream>

/*
 To DO :
 * i should use a better algorithm
 * prevent self collision
 * what if we could not clear
 * ball 's velocity before kick will change the angle and also ball reachable dist
 * circle radiuses are ass generated !
 * step kick
 * there is collision if opps move
 * the if es for out and ... numbers are not correct
 * the goal center (Point) is not correct (Position)
 * This is the correct shape (it is not 2 circles :D) :
 *               ___
 *              |   |
 *             /     \
 *            /  opp  \
 *          /           \
 *          _____________
 */

/* ValueSystem : (0 : we should keep the ball) (20 : tmmCorner) (30 to 41 : opp in the second circle) (40 to 51 : opp in the first circle) (65 : oppCorner) (80 : out) (100 : safeClear) */

Clear::Clear(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerUid = pWorldData->my_id(); //this will be used for self collision detection
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

Clear::Clear()
{
    
}

void Clear::setUid(int givenID)
{
    callerUid = givenID;
}

void Clear::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerUid = pWorldData->my_id();
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

float Clear::getLineValue(Point start, Point end)
{
    Line clearLine;

    Circle startCircle, endCircle;

    Vector clearVector;
    Vector tempVector;

    vector <int> opps = pWorldData->mr_found_opponents();

    clearVector.setByPoints(start, end);
    clearLine.setByPoints(start, end);
    tempVector = (clearVector / 16);

    Point startCenter = start + (tempVector * 5);
    Point endCenter = end;
    Point thisOpp;
    Point goalCenter;
    //Point oppPole1 = pWorldData->basedCenter(pWorldData->opgoal_pole1());

    float d = start.getDistance(end);
    float startRadius = 7 * d / 16;
    float endRadius = 3 * d / 4;
    float startZaribOpp = 1;
    float endZaribOpp = 0.5;
    float outValue = 80;
    float oppcornerValue = 60;
    float tmmcornerValue = 20;
    float minValue = 9999;
    float tempValue;
    float clearValue = 100;

    goalCenter.x = pWorldData->ourPole1.getX();
    goalCenter.y = (pWorldData->ourPole1.getY() + pWorldData->ourPole2.getY()) / 2;

    startCircle.setByCenterRadius(startCenter, startRadius);
    endCircle.setByCenterRadius(endCenter, endRadius);

    for (int i = 0; i < opps.size(); i++)
    {
        thisOpp = pWorldData->oppPositions[i];

        if (startCircle.checkPointInArea(thisOpp))
        {
            if (clearLine.getPointDist(thisOpp) < 50)
            {
                minValue = 0;
                return 0;
            }
            tempValue = clearLine.getPointDist(thisOpp) * startZaribOpp;
            if (tempValue < minValue)
            {
                minValue = tempValue;
            }
        }
        if (endCircle.checkPointInArea(thisOpp))
        {
            if (clearLine.getPointDist(thisOpp) < 70)
            {
                minValue = 0;
                return 0;
            }
            tempValue = clearLine.getPointDist(thisOpp) * endZaribOpp;
            if (tempValue < minValue)
            {
                minValue = tempValue;
            }
        }
    }

    if (end.getY() >= pWorldData->realVectors[10].getY() || end.getY() <= pWorldData->realVectors[0].getY())
    {
        tempValue = outValue;
        if (tempValue < minValue)
        {
            minValue = tempValue;
        }
    }
    if (goalCenter.getX() < 0) //we are in the left side
    {
        if (end.getX() <= goalCenter.getX())
        {
            if (goalCenter.getDistance(end) < 100) //then it is a goal to ourteam :D
            {
                minValue = 0;
                return 0;
            }
            else
            {
                tempValue = tmmcornerValue;
                if (tempValue < minValue)
                {
                    minValue = tempValue;
                }
            }
        }
        //if (end.getX() >= pWorldData->basedCenter(pWorldData->opgoal_pole1()).getX())
        if (end.getX() >= pWorldData->oppPole1.getX())
        {
            tempValue = oppcornerValue;
            if (tempValue < minValue)
            {
                minValue = tempValue;
            }
        }
    }
    else if (goalCenter.getX() > 0) //we are in the right side
    {
        if (end.getX() >= goalCenter.getX())
        {
            if (goalCenter.getDistance(end) < 100) //then it is a goal to ourteam :D
            {
                minValue = 0;
                return minValue;
            }
            else
            {
                tempValue = tmmcornerValue;
                if (tempValue < minValue)
                {
                    minValue = tempValue;
                }
            }
        }
        //if (end.getX() <= pWorldData->basedCenter(pWorldData->opgoal_pole1()).getX())
        if (end.getX() <= pWorldData->oppPole1.getX())
        {
            tempValue = oppcornerValue;
            if (tempValue < minValue)
            {
                minValue = tempValue;
            }
        }
    }
    if (minValue == 9999)
    {
        minValue = clearValue;
    }

    return minValue;
}

void Clear::pointSelector()
{
    Point tempPointUp, tempPointDown;
    Point goalCenter;

    float clearRadius = 163.927;
    float x, y;
    float maxValue = 0;
    float tempValueUp, tempValueDown;

    //this is only for test (this is correct only and anly if we are in the right side of the field):
    goalCenter.x = pWorldData->ourPole1.getX();
    goalCenter.y = (pWorldData->ourPole1.getY() + pWorldData->ourPole2.getY()) / 2;

    for (x = -1 * clearRadius; x <= clearRadius; x += 5)
    {
        y = sqrt(clearRadius * clearRadius - x * x);

        tempPointUp.x = ballPos.getX() + x;
        tempPointDown.x = ballPos.getX() + x;
        tempPointUp.y = ballPos.getY() + y;
        tempPointDown.y = ballPos.getY() - y;

        tempValueUp = getLineValue(ballPos, tempPointUp);
        tempValueDown = getLineValue(ballPos, tempPointDown);

        if (tempValueUp == maxValue && maxValue != 0 && goalCenter.getDistance(tempPointUp) > goalCenter.getDistance(selectedPoint))
        {
            selectedPoint = tempPointUp;
        }
        else if (tempValueUp > maxValue)
        {
            selectedPoint = tempPointUp;
            maxValue = tempValueUp;
        }

        if (tempValueDown == maxValue && maxValue != 0 && goalCenter.getDistance(tempPointDown) > goalCenter.getDistance(selectedPoint))
        {
            selectedPoint = tempPointUp;
        }
        else if (tempValueDown > maxValue)
        {
            selectedPoint = tempPointDown;
            maxValue = tempValueDown;
        }
    }
}

void Clear::clearNow()
{
    Vector clearVector;
    
    float angle = 0;

    pointSelector();
    
    if ((getLineValue(ballPos, selectedPoint)) != 0)
    {
        clearVector.setByPoints(ballPos, selectedPoint);
        angle = pWorldData->bodyAngle() - clearVector.getDirection();
        pBasicPlayer->kick(angle * M_PI / 180, 1);
    }
    else
    {
        //maybe we should keep the ball
    }
}

float Clear::clearLastValue()
{
    return getLineValue(ballPos, selectedPoint);
}