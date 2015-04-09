
/*
 * Name        : Block.cpp
 * Author      : Pooya Moradi
 * Version     :
 * Copyright   : It's Under GPL3
 * Description : plan for defencers that blocks opponents
 */

#include <Block.h>

Block::Block(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
}

Block::Block()
{
}

void Block::initializeWorld(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
}

bool Block::canIBlock()
{
    Vector Me = pWorldData->me(true);
    Point me;

    me.x = Me.getX();
    me.y = Me.getY();

    Point nearstOppToMe = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(me)));

    if (nearstOppToMe.getDistance(me) > 200)
        return false;
    else
        return true;
}

/* this function find the block point */
void Block::targetFinder()
{
    //Point pole1 = pWorldData->basedCenter(pWorldData->mygoal_pole1());
    //Point pole2 = pWorldData->basedCenter(pWorldData->mygoal_pole2());
    Point pole1 = pWorldData->ourPole1;
    Point pole2 = pWorldData->ourPole2;

    Point goalCenter, me, oppPos;

    vector<int> teammates = pWorldData->mr_found_teammates();
    vector<int> opps = pWorldData->mr_found_opponents();

    Vector myVector = pWorldData->me(true);

    Line oppToGoalCenter;

    me.x = myVector.getX();
    me.y = myVector.getY();

    goalCenter.x = ((pole1.getX() + pole2.getX()) / 2);
    goalCenter.y = (pole1.getY() + pole2.getY()) / 2;

    if (opps.size() != 0)
        oppPos = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(me)));
    //oppPos = pWorldData->basedCenter(pWorldData->opponent(closestOppFinder()));

    cout << "[block] i want to block:  " << oppPos << endl;
    oppToGoalCenter.setByPoints(oppPos, goalCenter);


    /* math calculation for finding the block point */
    float a = oppToGoalCenter.getA();
    float b = oppToGoalCenter.getB();
    float c = oppToGoalCenter.getC();
    float xp = me.getX();
    float yp = me.getY();
    float m1 = getShib(oppPos, goalCenter);
    float m2 = -1 / m1;
    float targetX = ((b * b * xp) - (a * c) - (a * b * yp)) / (b * b + a * a);
    float targetY = ((-a / b) * targetX) - (c / b);

    target.x = targetX;
    target.y = targetY;

    /* if our dist from line be bigger than 35 and opp distance from goal center be bigger than 80 it means
     * we should block it. else we can't block it and so we don't go anywhere :)
     */
    if (oppToGoalCenter.getPointDist(me) > 35 && oppPos.getDistance(goalCenter) > 80)
        check = 100;
    else
        check = 101;

}

void Block::targetFinder(Point opp)
{
	//Point pole1 = pWorldData->basedCenter(pWorldData->mygoal_pole1());
	//Point pole2 = pWorldData->basedCenter(pWorldData->mygoal_pole2());
	Point pole1 = pWorldData->ourPole1;
	Point pole2 = pWorldData->ourPole2;

	Point goalCenter, me, oppPos;

	vector<int> teammates = pWorldData->mr_found_teammates();
	vector<int> opps = pWorldData->mr_found_opponents();

	Vector myVector = pWorldData->me(true);

	Line oppToGoalCenter;

	me.x = myVector.getX();
	me.y = myVector.getY();

	goalCenter.x = ((pole1.getX() + pole2.getX()) / 2);
	goalCenter.y = (pole1.getY() + pole2.getY()) / 2;


	cout << "[block] i want to block:  " << opp << endl;
	oppToGoalCenter.setByPoints(opp, goalCenter);


	/* math calculation for finding the block point */
	float a = oppToGoalCenter.getA();
	float b = oppToGoalCenter.getB();
	float c = oppToGoalCenter.getC();
	float xp = me.getX();
	float yp = me.getY();
	float m1 = getShib(oppPos, goalCenter);
	float m2 = -1 / m1;
	float targetX = ((b * b * xp) - (a * c) - (a * b * yp)) / (b * b + a * a);
	float targetY = ((-a / b) * targetX) - (c / b);

	target.x = targetX;
	target.y = targetY;

	    /* if our dist from line be bigger than 35 and opp distance from goal center be bigger than 80 it means
	     * we should block it. else we can't block it and so we don't go anywhere :)
	     */
	if (oppToGoalCenter.getPointDist(me) > 35 && oppPos.getDistance(goalCenter) > 80)
		check = 100;
	else
	    check = 101;

}

void Block::doDash()
{
    cout << "[block] i'm in doDash" << endl;
    if (check == 100)
    {
        cout << "[block] i'm dashing " << endl;
        pBasicPlayer->go(target.getX(), target.getY());
    }
    else
    {
        cout << "[block] i'm stopping" << endl;
        pBasicPlayer->set_velocity(0, 0);
    }
}
