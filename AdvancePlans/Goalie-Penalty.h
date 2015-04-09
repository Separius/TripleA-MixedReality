#include <WorldData.h>
#include <BasicPlayer.h>
#include <Block.h>
#include <Clear.h>

class GoaliePen
{
	public:

		GoaliePen(WorldData* newWorldData, BasicPlayer* newBasicPlayer );
		void decide();
		void doBlock();
		void doClear();
		void doGo();



	private:

		WorldData* pWorldData;
		BasicPlayer* pBasicPlayer;
};



