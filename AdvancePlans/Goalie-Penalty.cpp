#include <Goalie-Penalty.h>

GoaliePen::GoaliePen(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
	pWorldData = newWorldData;
	pBasicPlayer = newBasicPlayer;
}

void GoaliePen::doClear()
{
	cout << "[GoaliePen] i'm clearing" << endl;
	Clear myClear(pWorldData,pBasicPlayer);
	myClear.clearNow();
}

void GoaliePen::doGo()
{
	cout << "[Goalie-Penalty] i'm going to ball" << endl;
	Point ballPos = pWorldData->basedCenter(pWorldData->ball());
	pBasicPlayer->go(ballPos.getX(),ballPos.getY());
}

void GoaliePen::doBlock()
{
	cout << "[Goalie-Penalty] i'm blocking " << endl;

	Point pole1 = pWorldData->ourPole1;
	Point pole2 = pWorldData->ourPole2;

	pole1.x *= -1;
	pole2.x *= -1;

	Point myPoint1,myPoint2;
	Line firstLine,secondLine;
	Point ballPos = pWorldData->basedCenter(pWorldData->ball());
	Point nearstOppToBall = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(ballPos)));
	Point goalCenter((pole1.getX() + pole2.getX()) / 2, (pole1.getY() + pole2.getY()) / 2);
	Point blockPoint;


	if(pole1.getX() < 0)
	{
		myPoint1.x = pole1.getX() + 100;
		myPoint1.y = pole1.getY();

		myPoint2.x = pole2.getX() + 100;
		myPoint2.y = pole2.getY();
	}
	else
	{
		myPoint1.x = pole1.getX() - 100;
		myPoint1.y = pole1.getY();

		myPoint2.x = pole2.getX() - 100;
		myPoint2.y = pole2.getY();

	}

	firstLine.setByPoints(myPoint1,myPoint2);
	secondLine.setByPoints(nearstOppToBall,goalCenter);

	firstLine.getLineIntersect(secondLine,blockPoint);

	cout << "[Goalie-Penalty][doBlock] blockPoint:  " << blockPoint << endl;

	pBasicPlayer->go(blockPoint.getX(),blockPoint.getY());
}

void GoaliePen::decide()
{
	Vector Me;

	vector <int> opps = pWorldData->mr_found_opponents();

	Point ballPos = pWorldData->basedCenter(pWorldData->ball());
	Point closestOppToBalll;
	if(opps.size() != 0)
		pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(ballPos)));
	Point me;

	me.x = Me.getX();
	me.y = Me.getY();

	if(pBasicPlayer->can_kick())
		doClear();
	else
	{
		if(me.getDistance(closestOppToBalll) > ballPos.getDistance(closestOppToBalll) && opps.size() != 0)
			doBlock();
		else
			doGo();
	}


}


