#include <iostream>
#include <BasicPlayer.h>
#include <WorldData.h>
#include <Dribble.h>
#include <Shoot-cir.h>

class PenaltyPlan
{
	public:
	PenaltyPlan(WorldData* newWorldData, BasicPlayer* newBasicPlayer);
	void doDribble();
	void doShoot();
	void decide();

	private:
	WorldData* pWorldData;
	BasicPlayer* pBasicPlayer;

};
