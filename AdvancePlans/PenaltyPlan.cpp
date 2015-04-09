#include <PenaltyPlan.h>

PenaltyPlan::PenaltyPlan(WorldData* newWorldData,BasicPlayer* newBasicPlayer)
{
	pWorldData = newWorldData;
	pBasicPlayer = newBasicPlayer;
}

void PenaltyPlan::doDribble()
{
	cout << "i'm dribbling!" << endl;
	Dribble penaltyDribble(pWorldData,pBasicPlayer);
	penaltyDribble.decide();
}

void PenaltyPlan::doShoot()
{
	cout << "i'm shooting!" << endl;
	Shoot penaltyShoot(pWorldData,pBasicPlayer);
	penaltyShoot.shootNow();
}

void PenaltyPlan::decide()
{
	vector <int> opps = pWorldData->mr_found_opponents();

	Vector Me = pWorldData->me(true);

	Point ballPos = pWorldData->basedCenter(pWorldData->ball());
	Point me(Me.getX() , Me.getY());
	Point nearstOppToMe;

	if(opps.size() != 0)
			nearstOppToMe = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(Me)));

	Point pole1 = pWorldData->oppPole1;
	Point pole2 = pWorldData->oppPole2;
	Point goalCenter((pole1.getX() + pole2.getX()) / 2, (pole1.getY() + pole2.getY()) / 2);

	if(opps.size() != 0)
	{
		if(!pBasicPlayer->can_kick())
		{
			cout << "[PenaltyPlan] i'm dribbling" << endl;
			doDribble();
		}
		else
		{
			if(me.getDistance(goalCenter) > 190)
			{
				cout << "[PenaltyPlan] i'm dribbling" << endl;
				doDribble();
			}
			else
			{
			    cout << "[PenaltyPlan] my dist from goalCenter:  " << me.getDistance(goalCenter) << endl;
				cout << "[PenaltyPlan] i'm shooting" << endl;
				doShoot();
			}
		}
	}

}
