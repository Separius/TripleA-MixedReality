#ifndef __Defence_Positioning_H
#define __Defence_Positioning_H

#include <WorldData.h>
#include <BasicUtils.h>
#include <BasicPlayer.h>
#include <cstring>

class DefPos {
public:
	DefPos(WorldData* newWorldData, BasicPlayer* newBasicPlayer, int uids[],int defenersNum, char teamField);
        DefPos();
        void initializeWorld(WorldData* newWorldData, BasicPlayer* newBasicPlayer, int uids[],int defenersNum, char teamField);
	void defencePosNOW();
	//deciders::
	void decideXLine();
	void decideLength();
	void decideYLine();
	void definePoints();
	void setflags();

	//   int nearestPlayer(Point place);
	void goToPoint(int pId, Point pos);

	int resultnum; //number of ordered robots
	int resultId[6];
	Point resultPos[6];

	int botnum; //number of robots

	int usednum; //number of used uids
	int usedUids[6];
	int uidList[6];

private:

	WorldData* pWorldData;
	BasicPlayer* pBasicPlayer;

	double botSize; //andazeye har robot

	char area;

	double fieldTool;
	double fieldArz;
	double aviliableTool; //yani dar kol (ba position haye mokhtalefe ball)
	//defenceline cheghadr mitavanad jabejaii dashte bashad

	double availableArz; //yani defence line chand vahed (ehtemalan px) ast (az bala be paiin)

	double ballX;
	double ballY;

	// int usedUids[6];


	Vector realVectors[19];

	double xLine;
	//balatarin makan e Y
	double y1Line;

private:
	struct Ptp //put to point
	{
		Point pos[4];
		double sum;
	};

	Point avp[4]; //computed points for defensers


public:
	void fill(Ptp * DATA);
	void computePtpSum(Ptp * DATA);
	void selectIds(Ptp *DATA, Ptp * resultPtp);
	void computeTotal();
	void check();
	int totalDATA;

	void noiseCancell(Ptp *DATA);
	Point prePoint[4];


};
#endif