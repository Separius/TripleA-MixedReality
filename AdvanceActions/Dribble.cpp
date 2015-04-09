#include <Dribble.h>

Dribble::Dribble(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerUid = pWorldData->my_id();
}

Dribble::Dribble()
{
    
}

void Dribble::setUid(int newID)
{
    callerUid = newID;
}

void Dribble::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerUid = pWorldData->my_id();
}

void Dribble::targetFinder()
{

    Vector Me = pWorldData->me(true);
    Vector meToOppGoalCenter, tempGoalCenterVector;
    Point me, goalCenter, tempGoalCenter, bestPoint, nearstOppToMe;
    Point pole1 = pWorldData->basedCenter(pWorldData->opgoal_pole1());
    Point pole2 = pWorldData->basedCenter(pWorldData->opgoal_pole2());

    Line dribbleLine;
    float dist = 0;
    vector<int> opps = pWorldData->mr_found_opponents();
    me.x = Me.getX();
    me.y = Me.getY();
    goalCenter.x = (pole1.getX() + pole2.getX()) / 2;
    goalCenter.y = (pole1.getY() + pole2.getY()) / 2;

    meToOppGoalCenter.setByPoints(me, goalCenter);
    bestPoint = goalCenter;
    if (opps.size() != 0)
        nearstOppToMe = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(me)));
    if (opps.size() != 0 && nearstOppToMe.getDistance(me) < 150)
    {
        if (pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(me))).getDistance(goalCenter) < me.getDistance(goalCenter))
        {
            cout << "[dribble]opponent dribbling" << endl;
            //	cout << "[dribble] i'll go to for loop" << endl;
            for (int i = -35; i <= 35; i++)
            {
                tempGoalCenterVector = meToOppGoalCenter;
                tempGoalCenterVector.rotate(i);

                tempGoalCenter.x = me.getX() + tempGoalCenterVector.getX();
                tempGoalCenter.y = me.getY() + tempGoalCenterVector.getY();

                dribbleLine.setByPoints(me, tempGoalCenter);

                if (pWorldData->getNearstOppDistFromLine(dribbleLine) > dist)
                {
                    cout << "dist1:  " << pWorldData->getNearstOppDistFromLine(dribbleLine) << endl;
                    cout << "tempGoalCenter:  " << tempGoalCenter << endl;
                    cout << "nearst opp id:  " << pWorldData->getNearstOppToLine(dribbleLine) << endl;
                    bestLine = dribbleLine;
                    bestPoint = tempGoalCenter;
                    dist = pWorldData->getNearstOppDistFromLine(dribbleLine);
                }
            }
        }
        else
            cout << "[dribble]normal dribbling" << endl;
    }

    target = bestPoint;
}

void Dribble::doDash()
{
    Point ball = pWorldData->basedCenter(pWorldData->ball());
    pBasicPlayer->go(ball.getX(), ball.getY());
}

void Dribble::doKick()
{
    Vector ballTarget;
    Point ball = pWorldData->basedCenter(pWorldData->ball());
    Vector Me = pWorldData->me(true);
    Point me(Me.getX(),Me.getY());
    
    float angle;

    ballTarget.setByPoints(ball, target);
    angle = pWorldData->bodyAngle() - ballTarget.getDirection();
    
    Point nearstOppToMe = pWorldData->basedCenter(pWorldData->opponent(pWorldData->getNearstOppToPoint(me)));
    
    if(me.getDistance(nearstOppToMe) > 300)
        pBasicPlayer->kick((angle * M_PI) / 180, 0.75);
    else
	pBasicPlayer->kick((angle * M_PI) / 180,0.2);
}

float Dribble::dribbleLastValue()
{
    float nearstOppToLineDist = pWorldData->getNearstOppDistFromLine(bestLine);

    if (nearstOppToLineDist / 2 > 100)
        return 100;
    else
        return nearstOppToLineDist / 2;
}

void Dribble::decide()
{
    targetFinder();
    cout << "[dribble] target:  " << target << endl;
    if (!pBasicPlayer->can_kick())
        doDash();
    else
        doKick();
}
