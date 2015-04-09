#include <Goalie.h>

Goalie::Goalie(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
	pWorldData = newWorldData;
	pBasicPlayer = newBasicPlayer;
}

float Goalie::getPassValue()
{
	Pass ttestPass(pWorldData,pBasicPlayer);
	ttestPass.bestTeammateSelector();
	return ttestPass.passLastValue();
}

float Goalie::getClearValue()
{
	Clear ttestClear(pWorldData,pBasicPlayer);
	ttestClear.pointSelector();
	return ttestClear.clearLastValue();
}

void Goalie::doPositioning()
{
	/* abstract of goalie positioning algorithm
	 * we plot the circle that its center is center of our goal area
	 * and we plot the line from ball to center of our goal area
	 *	then we find the intersect point of that line and that circle and go there :)
	 */

	Point pole1 = pWorldData->ourPole1 ;//= pWorldData->basedCenter(pWorldData->mygoal_pole1());
	Point pole2 = pWorldData->ourPole2 ;//= pWorldData->basedCenter(pWorldData->mygoal_pole2());

	pole1.x *= -1;
	pole2.x *= -1;

	cout << "[goalie][positioning] pole1:  " << pole1 << endl;
	cout << "[goalie][positioning] pole2:  " << pole2 << endl;

	Point goalCenter;
	Point ballPos = pWorldData->basedCenter(pWorldData->ball());
	//Point nearstOppToBall = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(ballPos)));
	Point intersectPoint;

	//defining the radius of that circle
	float radius = pole1.getDistance(pole2) / 2;

	//defines the vector that includes intersects of line and circle
	vector <Point> intersects;

	Circle halfCircle;
	Line balltoCenterLine;

	goalCenter.x = pole1.getX();
	goalCenter.y = (pole1.getY() + pole2.getY()) / 2;

	//defining that circle and line
	balltoCenterLine.setByPoints(ballPos,goalCenter);
	halfCircle.setByCenterRadius(goalCenter,radius);

	//finding the intersects of circle and line
	halfCircle.getLineIntersect(balltoCenterLine,intersects);

	//the line has 2 intersect point with circle. we want to find the point that be in the field :)
	if(intersects[0].getDistance(ballPos) < intersects[1].getDistance(ballPos))
		intersectPoint = intersects[0];
	else
		intersectPoint = intersects[1];

	cout << "[goalie][positioning] intersectPoint:  " << intersectPoint << endl;
	pBasicPlayer->go(intersectPoint.getX() , intersectPoint.getY());
}

void Goalie::doGo()
{
	Point ball = pWorldData->basedCenter(pWorldData->ball());
	cout << "ball:  " << ball << endl;
	pBasicPlayer->go(ball.getX(),ball.getY());
}

void Goalie::decide()
{
	Clear testClear(pWorldData,pBasicPlayer);
	Pass testPass(pWorldData,pBasicPlayer);
	testClear.pointSelector();
	testPass.bestTeammateSelector();


	Point ball = pWorldData->basedCenter(pWorldData->ball());
	Point nearstOppToBall;
	Point me;

	vector<int> Opps= pWorldData->mr_found_opponents();
	if(Opps.size() != 0)
		pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(ball)));

	Vector Me = pWorldData->me(true);

	me.x = Me.getX();
	me.y = Me.getY();

	if(Opps.size() != 0)
	{
		int nearstOppToPointt = pWorldData->getNearstOppToPoint(ball);
		nearstOppToBall = pWorldData->basedCenter(pWorldData->opponent(nearstOppToPointt));

		//cout << "[goalie] my dist from ball:  " << me.getDistance(ball) << endl;
		//cout << "[goalie] nearstOppToBall dist from ball:  " << nearstOppToBall.getDistance(ball) << endl;
		//cout << "[goalie] nearstOppToBall:  " << nearstOppToPointt << endl;

		if(!pBasicPlayer->can_kick())
		{
			if(me.getDistance(ball) < nearstOppToBall.getDistance(ball))
			{
				cout << "[Goalie] going to ball" << endl;
				doGo();
			}
			else
			{
				cout << "[Goalie] positioning " << endl;
				doPositioning();
			}
		}
		else
		{
			if(getClearValue() > getPassValue())
			{
				cout << "[Goalie] clearing" << endl;
				testClear.clearNow();
			}
			if(getClearValue() == getPassValue())
			{
				cout << "[Goalie] i'm passing " << endl;
				testPass.passNow();
			}
			if(getClearValue() < getPassValue())
			{
				cout << "[Goalie] passing" << endl;
				testPass.passNow();
			}
		}
	}
	else
	{
		cout << "[Goalie] going to ball!" << endl;
		doGo();
	}
}
