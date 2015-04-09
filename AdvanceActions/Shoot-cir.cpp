#include <Shoot-cir.h>

/*
 To Do :
 * prevent Self Collision (decide then rotate || change the reachPoints() func || rotate at first so we can shoot to all of the reachable points)
 * prevent Teammate Collision
 * step kick
 * ball 's velocity before kick will change the angle and also ball reachable dist
 * there is collision if opps move
 * if we are beside the goal it will shoot to a goal pole !
 * circle radiuses are ass generated ! (they are actually good !)
 * This is the correct shape (it is not 2 circles :D) :
 *               ___
 *              |   |
 *             /|tm |\
 *            /  \ /  \
 *          /  op | op  \
 *          _____________
*/

// Value System :  (0 : there is collision with opp) , (30 to 41.25 : bad opp in the end circle) , (40 to 51.5 : bad opp in the first circle) , (100 : Clear Shoot)


Shoot::Shoot(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    selectedPoint .x = 0;
    selectedPoint .y = 0;
    whichGoal = 'o'; //t stands for teamGoal and o stands for oppGoal
    callerUid = pWorldData->my_id();
    ballPos = pWorldData->basedCenter(pWorldData->ball());
    shootReachableDist = 165;
}

Shoot::Shoot()
{
    selectedPoint .x = 0;
    selectedPoint .y = 0;
    whichGoal = 'o'; //t stands for teamGoal and o stands for oppGoal
    shootReachableDist = 165;
}

void Shoot::setReachableDist(float newReachable)
{
    shootReachableDist = newReachable;
}

void Shoot::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerUid = pWorldData->my_id();
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

void Shoot::setBallPos(Point newBallPos)
{
    ballPos = newBallPos;
}

void Shoot::setUid(int givenID)
{
    callerUid = givenID;
}

void Shoot::shootNow()
{
    float angle = 0;

    can_i_shoot = false;

    Vector shootVector;

    pointSelector();

    shootVector.setByPoints(ballPos, selectedPoint);

    if (can_i_shoot == true && selectedPoint.getX() != 0)
    {
        angle = pWorldData->bodyAngle() - shootVector.getDirection();
        pBasicPlayer->kick(angle * M_PI / 180, 1);
    }
    else
    {
        pBasicPlayer->kick(M_PI / 2, 1);
        //we should pass the ball !
    }
}

void Shoot::goalChooser(char whichOne)
{
    if (whichOne == 'o' || whichOne == 'O')
        whichGoal = 'o';
    else if (whichOne == 't' || whichOne == 'T')
        whichGoal = 't';
}

/*
void Shoot::selfCollision()
{
    Point me[5] ;
    Point ballPos ;
    
    Vector OAvec , OBvec , OCvec , ODvec ;

    ballPos = pWorldData->basedCenter(pWorldData->ball());
    
    /*
 *      4D               1A
 *          ///
 *              ///
 *              0O   *
 *              ///
 *          ///
 *      3C               2B
    
    me[0].x = pWorldData->me(true).getX();
    me[0].y = pWorldData->me(true).getY();
    me[1].x = pWorldData->me(true).getX() + 20;
    me[1].y = pWorldData->me(true).getY() + 10;
    me[2].x = pWorldData->me(true).getX() + 20;
    me[2].y = pWorldData->me(true).getY() - 10;
    me[3].x = pWorldData->me(true).getX() - 20;
    me[3].y = pWorldData->me(true).getY() - 10 ;
    me[4].x = pWorldData->me(true).getX() - 20;
    me[4].y = pWorldData->me(true).getY() + 10;

    OAvec.setByPoints(me[0],me[1]);
    OAvec.rotate(pWorldData->bodyAngle());
    OBvec.setByPoints(me[0],me[2]);
    OBvec.rotate(pWorldData->bodyAngle());
    OCvec.setByPoints(me[0],me[3]);
    OCvec.rotate(pWorldData->bodyAngle());
    ODvec.setByPoints(me[0],me[4]);
    ODvec.rotate(pWorldData->bodyAngle());

    me[1].x = me[0].getX() + OAvec.getX() ;
    me[1].y = me[0].getY() + OAvec.getY() ;
    me[2].x = me[0].getX() + OBvec.getX() ;
    me[2].y = me[0].getY() + OBvec.getY() ;
    me[3].x = me[0].getX() + OCvec.getX() ;
    me[3].y = me[0].getY() + OCvec.getY() ;
    me[4].x = me[0].getX() + ODvec.getX() ;
    me[4].y = me[0].getY() + ODvec.getY() ;

}
 */

float Shoot::getLineValue(Point start, Point end)
{
    Line shootLine;
    Circle startCircle, endCircle;
    Vector shootVector;
    Vector tempVector;
    vector <int> opps = pWorldData->mr_found_opponents();

    shootVector.setByPoints(start, end);
    shootLine.setByPoints(start, end);
    tempVector = (shootVector / 16);

    Point startCenter = start + (tempVector * 5);
    Point endCenter = end;
    Point thisOppCenter;

    float d = start.getDistance(end);
    float startRadius = 5 * d / 16;
    float endRadius = d / 2;
    float startZaribOpp = 1;
    float endZaribOpp = 0.5;

    startCircle.setByCenterRadius(startCenter, startRadius);
    endCircle.setByCenterRadius(endCenter, endRadius);

    float minValue = 100;

    for (int i = 0; i < opps.size(); i++)
    {
        thisOppCenter = pWorldData->oppPositions[i];

        //34.5 = players max kickable area radius
        //this is completly correct if the opponents won't move !

        if (startCircle.checkPointInArea(thisOppCenter))
        {
            if (shootLine.getPointDist(thisOppCenter) < 40)
            {
                minValue = 0;
                break;
            }
            if ((shootLine.getPointDist(thisOppCenter) * startZaribOpp) < minValue)
            {
                minValue = shootLine.getPointDist(thisOppCenter) * startZaribOpp;
            }
        }

        if (endCircle.checkPointInArea(thisOppCenter))
        {
            if (shootLine.getPointDist(thisOppCenter) < 60)
            {
                minValue = 0;
                break;
            }
            if ((shootLine.getPointDist(thisOppCenter) * endZaribOpp) < minValue)
            {
                minValue = shootLine.getPointDist(thisOppCenter) * endZaribOpp;
            }
        }
    }

    return minValue;
}

void Shoot::reachPoints()
{
    float distance = 0;
    float i;

    Point tempPoint;
    Point goalPole1;
    Point goalPole2;

    if (whichGoal == 'o')
    {
        goalPole1 = pWorldData->oppPole1;
        goalPole2 = pWorldData->oppPole2;
    }
    if (whichGoal == 't')
    {
        goalPole1 = pWorldData->ourPole1;
        goalPole2 = pWorldData->ourPole2;
    }

    //this is for knowing that we can shoot or we can not ?
    firstReachablePoint.x = 0;
    lastReachablePoint.x = 0;

    tempPoint.x = goalPole1.getX();

    // - 15 is for not shooting directly to the pole
    for (i = goalPole1.y - 15; i >= (goalPole1.y + goalPole2.y) / 2; i -= 5)
    {
        tempPoint.y = i;
        distance = ballPos.getDistance(tempPoint);
        if (distance <= shootReachableDist)
        {
            firstReachablePoint = tempPoint;
            break;
        }
    }

    // + 15 is for not shooting directly to the pole
    for (i = goalPole2.y + 15; i <= (goalPole1.y + goalPole2.y) / 2; i += 5)
    {
        tempPoint.y = i;
        distance = ballPos.getDistance(tempPoint);
        if (distance <= shootReachableDist)
        {
            lastReachablePoint = tempPoint;
            break;
        }
    }
}

void Shoot::pointSelector()
{
    Point thisPoint;

    float lineValue;
    float highestValue = 0;
    float i;
    float dis2Point = 9999;

    if (whichGoal == 'o')
    {
        thisPoint.x = pWorldData->oppPole1.getX();
    }
    if (whichGoal == 't')
    {
        thisPoint.x = pWorldData->ourPole1.getX();
    }

    selectedPoint = (0,0);

    reachPoints();

    //this is for checking that there is actually a selected points :
    if (firstReachablePoint.getX() == 0 && lastReachablePoint.getX() == 0)
    {
        selectedPoint.x = 0;
        selectedPoint.y = 0;
        can_i_shoot = false;
    }
    else
    {
        for (i = firstReachablePoint.getY(); i >= lastReachablePoint.getY(); i -= 3)
        {
            thisPoint.y = i;

            lineValue = getLineValue(ballPos, thisPoint);
             
            if (lineValue == 100 && ballPos.getDistance(thisPoint) < dis2Point)
            {
                selectedPoint = thisPoint;
                dis2Point = ballPos.getDistance(thisPoint);
                highestValue = 100;
            }
            if (lineValue > highestValue)
            {
                highestValue = lineValue;
                selectedPoint = thisPoint;
            }
        }
    }
    if (selectedPoint == (0 ,0))
        can_i_shoot = false ;
    else if (getLineValue(ballPos, selectedPoint) == 0)
        can_i_shoot = false;
    else
        can_i_shoot = true;
}

float Shoot::shootLastValue()
{
    if (can_i_shoot == 1)
        return getLineValue(ballPos, selectedPoint);
    else
        return 0;
}