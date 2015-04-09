/*                                                                       
 * Written by Juliana Fajardini <jufajardini@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */


#include "WorldData.h"

#include <unistd.h>
#include <sstream>
#include <Properties.h>

WorldData::WorldData ( int sleep,int tries,mrTeam team)
{


	    /* defining the real place of flags(based center of field) */

    iSleep = sleep;
    iTries = tries;
    mAgentTeam = team;
    setRealVectors();
}


WorldData::~WorldData()
{
}

int
WorldData::getISleep(){
    return iSleep;
}

void
WorldData::setRealVectors()
{
//in ha ye vector az 0,0 be flag ha hastand.
	float fieldTool = 781.81; //defining width of field
	float fieldArz = 487.475; //defining height of field

	/* defining the real place of flags(based center of field) */

	realVectors[0].setAsCartesian(0, -1 * (fieldArz / 2));
	//bottom center

	realVectors[1].setAsCartesian((-1 * fieldTool) / 2, (-1 * fieldArz) / 2); //bottom left corner

	realVectors[2].setAsCartesian((0.25 * fieldTool) - (fieldTool / 2), (0.19
			* fieldArz) - (fieldArz / 2));
	// bottom left goal area

	realVectors[3].setAsCartesian(-1 * (fieldTool / 2), (0.35 * fieldArz)
			- (fieldArz / 2));
	//bottom left pole

	realVectors[4].setAsCartesian((0.15 * fieldTool) - (fieldTool / 2), (0.35
			* fieldArz) - (fieldArz / 2));
	//bottom left small area

	realVectors[5].setAsCartesian(fieldTool / 2, (-1 * fieldArz) / 2);
	//bottom right corner

	realVectors[6].setAsCartesian((fieldTool / 2) - (0.25 * fieldTool), (0.19
			* fieldArz) - (fieldArz / 2));
	//bottom right goal area

	realVectors[7].setAsCartesian(fieldTool / 2, (0.35 * fieldArz) - (fieldArz
			/ 2));
	//bottom right pole

	realVectors[8].setAsCartesian((fieldTool / 2) - (0.15 * fieldTool), (0.35
			* fieldArz) - (fieldArz / 2));
	//bottom right small area

	realVectors[9].setAsCartesian(0, 0);
	//middle center

	realVectors[10].setAsCartesian(0, fieldArz / 2);
	//top center

	realVectors[11].setAsCartesian((-1 * fieldTool) / 2, fieldArz / 2);
	//top left corner

	realVectors[12].setAsCartesian((0.25 * fieldTool) - (fieldTool / 2),
			(fieldArz / 2) - (0.19 * fieldArz));
	//top left goal area

	realVectors[13].setAsCartesian(-1 * (fieldTool / 2), (fieldArz / 2) - (0.35
			* fieldArz)); //top left pole

	realVectors[14].setAsCartesian((0.15 * fieldTool) - (fieldTool / 2),
			(fieldArz / 2) - (0.35 * fieldArz));
	//top left small area

	realVectors[15].setAsCartesian(fieldTool / 2, fieldArz / 2);
	//top right corner

	realVectors[16].setAsCartesian((fieldTool / 2) - (0.25 * fieldTool),
			(fieldArz / 2) - (0.19 * fieldArz));
	//top right goal area

	realVectors[17].setAsCartesian(fieldTool / 2, (fieldArz / 2) - (0.35
			* fieldArz)); //top right pole

	realVectors[18].setAsCartesian((fieldTool / 2) - (0.15 * fieldTool),
			(fieldArz / 2) - (0.35 * fieldArz));
	//top right small area
}

void
WorldData::setConnection(Connection* newConnection)
{
    pConnection = newConnection;
}


/* Returns a polar vector poiting towards the
 * ball in the field.
 */
vector_t
WorldData :: ball()
{
	if (i_am_lost())
		throw ClientException("World::ball() request on lost agent");

    vector_t auxBall;
    auxBall.angle = mXmlWorldData.mBall.mAngle;
    auxBall.length = mXmlWorldData.mBall.mDist;
    return auxBall;
}

/* Returns the time of last update
 * in the match's server clock, in seconds
 */
double
WorldData :: time()
{
   return mXmlWorldData.mTime;
}

/* Returns 1 if the game is on
 * or zero if not
 */
bool
WorldData ::  is_game_on()
{
    if (mXmlWorldData.mPlayMode == "play on"
    	|| mXmlWorldData.mPlayMode == "warn ending")
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
 * Returns current/last playmode received
 * from match's server.
 */
std :: string
WorldData ::  mrPlayMode()
{
    return mXmlWorldData.mPlayMode;
}

/* Returns a vector pointing towards the goal
 * poles in the field.
 */
vector_t
WorldData ::  mygoal_pole1()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrTopRightPole();
    }
    else
        return mrTopLeftPole();
}
vector_t
WorldData ::  mygoal_pole2()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrBottomRightPole();
    }
    else
        return mrBottomLeftPole();
}

vector_t
WorldData ::  opgoal_pole1()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrTopLeftPole();
    }
    else {
        return mrTopRightPole();
    }
}

vector_t
WorldData ::  opgoal_pole2()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrBottomLeftPole();
    }
    else {
        return mrBottomRightPole();
    }
}

/* Returns a vector pointing towards the field
 * corners.
 */
vector_t
WorldData ::  mycorner_flag1()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrTopRightCorner();
    }
    else
        return mrTopLeftCorner();
}
vector_t
WorldData ::  mycorner_flag2()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrBottomRightCorner();
    }
    else
        return mrBottomLeftCorner();
}
vector_t
WorldData ::  opcorner_flag1()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrTopLeftCorner();
    }
    else
        return mrTopRightCorner();
}
vector_t
WorldData ::  opcorner_flag2()
{
    if (mAgentTeam == TEAM_BLUE){
        return mrBottomLeftCorner();
    }
    else
        return mrBottomRightCorner();
}

/* Given an flagId, returns a polar vector with
 * agent centered pointing to specified flag.
 * If no such id is found, returns a polar vector
 * that cannot really occur in mr soccer world.
 */
vector_t
WorldData :: mrFlag(std::string flagId)
{
    return mXmlWorldData.getVector(mXmlWorldData.getFlag(flagId));
}

/*
 * Return polar vectors to the goal poles
 */

vector_t
WorldData :: mrTopLeftPole()
{
    return mrFlag("top_left_pole");
}

vector_t
WorldData :: mrBottomLeftPole()
{
    return mrFlag("bottom_left_pole");
}

vector_t
WorldData :: mrTopRightPole()
{
    return mrFlag("top_right_pole");
}

vector_t
WorldData :: mrBottomRightPole()
{
    return mrFlag("bottom_right_pole");
}

/*
 * Return polar vectors to the corner flagssum
 */

vector_t
WorldData :: mrTopLeftCorner()
{
    return mrFlag("top_left_corner");
}

vector_t
WorldData :: mrBottomLeftCorner()
{
    return mrFlag("bottom_left_corner");
}

vector_t
WorldData :: mrTopRightCorner()
{
    return mrFlag("top_right_corner");
}

vector_t
WorldData :: mrBottomRightCorner()
{
    return mrFlag("bottom_right_corner");
}

/*
 * Return polar vectors to the goal area flags
 */

vector_t
WorldData :: mrTopLeftGoalArea()
{
    return mrFlag("top_left_goal");
}

vector_t
WorldData :: mrBottomLeftGoalArea()
{
    return mrFlag("bottom_left_goal");
}

vector_t
WorldData :: mrTopRightGoalArea()
{
    return mrFlag("top_right_goal");
}

vector_t
WorldData :: mrBottomRightGoalArea()
{
    return mrFlag("bottom_right_goal");
}

vector_t
WorldData :: mrTopLeftSmallArea()
{
    return mrFlag("top_left_small_area");
}

vector_t
WorldData :: mrBottomLeftSmallArea()
{
    return mrFlag("bottom_left_small_area");
}

vector_t
WorldData :: mrTopRightSmallArea()
{
    return mrFlag("top_right_small_area");
}

vector_t
WorldData :: mrBottomRightSmallArea()
{
    return mrFlag("bottom_right_small_area");
}

vector_t
WorldData :: mrTopCenter()
{
    return mrFlag("top_center");
}

vector_t
WorldData :: mrMiddleField()
{
    return mrFlag("middle_center");
}

vector_t
WorldData :: mrBottomCenter()
{
    return mrFlag("bottom_center");
}

vector_t
WorldData :: teammate(int id)
{
    if ( id == my_id() )
    {
        vector_t vectorMe ;

        vectorMe.angle = 0 ;
        vectorMe.length = 0 ;

        return vectorMe ;
    }

    else
    {
        return mXmlWorldData
                    .Teammates()
                    .ByUid(id)
                    .position()
                    .position;
    }
}

vector_t
WorldData :: opponent(int id)
{
    return mXmlWorldData.Opponents()
			.ByUid(id)
			.position()
			.position;
}

agentVector
WorldData :: mr_agent(int uid)
{
	return mXmlWorldData
			.All()
			.WithFound(true)
			.ByUid(uid)
			.position();
}

/* Return the id (a.k.a. t-shirt number)
 * of one's self
 */
int
WorldData :: my_id()
{
    return mXmlWorldData.Me().uid();
}

int
WorldData :: closest_teammate_to(vector_t v)
{
	return ClosestInList(v, mXmlWorldData.Teammates()
								.WithFound(true));
}

int
WorldData :: closest_opponent_to(vector_t v)
{
	return ClosestInList(v, mXmlWorldData.Opponents()
								.WithFound(true));
}

float
WorldData::getNearstOppDistFromLine(Line line)
{
	float dist = 9999;
        vector <int> oppUids = mr_found_opponents();
	for(int i = 0;i < oppUids.size();i++)
		if(line.getPointDist(oppPositions[i]) < dist)
			dist = line.getPointDist(oppPositions[i]);

	return dist;
}

int
WorldData::getNearstOppToLine(Line line)
{
	float dist = 9999;
	int id = -1;
	vector <int> opponentIds = mr_found_opponents();

	for(int i = 0;i < opponentIds.size();i++)
		if(line.getPointDist(oppPositions[i]) < dist)
		{
			dist = line.getPointDist(oppPositions[i]);
			id = opponentIds[i];
		}
	return id;
}

int
WorldData::getNearstOppToPoint(Point point)
{
	float dist = 9999;
	int id = -1;
	vector<int> oppIds = mr_found_opponents();
	for(int i = 0;i < oppIds.size();i++)
		if(oppPositions[i].getDistance(point) < dist)
		{
			id = oppIds[i];
			dist = oppPositions[i].getDistance(point);
		}

	return id;
}


vector_t WorldData :: nearestTmmToMe()
{
	vector_t me = mXmlWorldData.Me().position().position;
	int uid = WorldData::closest_teammate_to(me); //DANGER : i changed it to WorldData instead of mXmlWorldData
	vector_t closest_tm = WorldData::teammate(uid); //DANGER : i changed it to WorldData instead of mXmlWorldData

}

vector_t
WorldData :: closest_player()
{
	if (i_am_lost())
		throw ClientException("World::closest_player() request when agent is lost");

	vector_t me = mXmlWorldData.Me().position().position;
	AgentList all = mXmlWorldData.All();

	int uid = ClosestInList(me, all.WithFound(true));

	if (uid == -1) {
		return me;
	}

	return all.ByUid(uid)
			.position()
			.position;
}

bool
WorldData :: i_am_lost()
{
    return !mXmlWorldData.Me().found();
}





float 
WorldData::bodyAngle()
{
    Vector nesbiVectors[19];  //arraye of flags nesbi(based me) and real(based center field)
    
    for(int i = 0;i < mXmlWorldData.mFlags.size();i++)    //mXmlworldData.mflags is a map or vector of the field flags so we should use it to defining all flags
    {
            vector_t vector = mXmlWorldData.getVector(mXmlWorldData.getFlag(mXmlWorldData.mFlags[i].mId)); //function getVector gives the flag and get me a vector to that
            
            /* convert from polar to cartesian */
            double x = cos(vector.angle) * (double)vector.length;
            double y = sin(vector.angle) * (double)vector.length;
            
            /* setting as cartesian */
            nesbiVectors[i].setAsCartesian(x,y);
    } 
    
    
    /* finding the bodyAngle */
    Vector zero(0,0);
    
    /* these for statements are moveing between flags */
    for(int i = 0;i < mXmlWorldData.mFlags.size();i++)
        for(int j = 0;j < mXmlWorldData.mFlags.size();j++)
        {
            if(i != j)
            {
                Vector a = realVectors[i] - realVectors[j]; //subtracting real vectors(based center of field)
                Vector b = nesbiVectors[i] - nesbiVectors[j]; //subtracting nessbi vectors(based me)
                
                zero += Vector(1, a.getDirection() - b.getDirection(), true);  //formula for finding body angle
            }
        }

    return -1 * zero.getDirection();        
}

Vector WorldData::me(bool a)
{
    /* for decreasing noise we use all flags to find our place and then get average of them . the result is soso correct */ 
    float sumX,sumY = 0;
    
    float bodyAngleReserved = bodyAngle();

    for(int i = 0;i < mXmlWorldData.mFlags.size();i++)
    {
        vector_t toFlag = mXmlWorldData.getVector(mXmlWorldData.getFlag(mXmlWorldData.mFlags[i].mId));
        Vector ttoFlag;
        
        float x = cos(toFlag.angle) * (double)toFlag.length;
        float y = sin(toFlag.angle) * (double)toFlag.length;
        
        
        ttoFlag.setAsCartesian(x,y);
        ttoFlag.rotate(-1 * bodyAngleReserved);
        
        sumX += ttoFlag.getX() + realVectors[i].getX();
        sumY += ttoFlag.getY() + realVectors[i].getY();
        
    }
    
    /* getting average from diffrent x and y */
    sumX /= mXmlWorldData.mFlags.size();
    sumY /= mXmlWorldData.mFlags.size();
    
    Vector body;
    body.setAsCartesian(-1 * sumX,sumY);

    return body;
}

float
WorldData :: fieldTool()
{
    vector_t topRight = mrTopRightCorner();
    vector_t topLeft = mrTopLeftCorner();
    
    Point a(cos(topRight.angle) * (double)topRight.length, sin(topRight.angle) * (double)topRight.length);
    Point b(cos(topLeft.angle) * (double)topLeft.length, sin(topLeft.angle) * (double)topLeft.length);
    
    return a.getDistance(b);
    
}

float 
WorldData :: fieldArz()
{
    vector_t topRight = mrTopRightCorner();
    vector_t bottomRight = mrBottomRightCorner();
    
    Point a(cos(topRight.angle) * (double)topRight.length, sin(topRight.angle) * (double)topRight.length);
    Point b(cos(bottomRight.angle) * (double)bottomRight.length, sin(bottomRight.angle) * (double)bottomRight.length);
    
    return a.getDistance(b);
}

Point
WorldData :: basedCenter(vector_t v)
{
    float firstAngle = -1 * ((v.angle * 180) / M_PI) ;
    float angle = 0;
    float bA = bodyAngle();
    if(bA >= 0 && firstAngle >= 0)
    {
	angle = bA + firstAngle;	
    }
    if(bA >= 0 && firstAngle <= 0)
    {
	angle = firstAngle + bA;
    }
    if(bA <= 0 && firstAngle >= 0)
    {
	angle = firstAngle + bA;
    }
    if(bA <= 0 && firstAngle <= 0)
    {
	angle = firstAngle + bA;
    }
    
    vector_t newVector;
    
    newVector.length = v.length;
    newVector.angle = (angle * M_PI) / 180;
 
    Vector Me = me(true);
    
    Point mme;
    
    mme.x = Me.getX();
    mme.y = Me.getY();
    Point newPoint(mme.getX() + (cos(newVector.angle) * newVector.length), mme.getY() + (sin(newVector.angle) * newVector.length));
    
    return newPoint; 
}


float WorldData :: getOppNearestDistance(Line line)
{
    
}

vector_t
WorldData ::me()
{
    return mXmlWorldData.Me().position().position;
}

int
WorldData ::closest_teammate_to_opponent(int id)
{
    return closest_teammate_to(mXmlWorldData
				.Opponents()
				.ByUid(id)
				.position()
				.position);
}

int
WorldData :: closest_teammate_to_teammate(int id)
{
    return closest_teammate_to(mXmlWorldData
				.Teammates()
				.ByUid(id)
				.position()
				.position);
}

int
WorldData :: closest_opponent_to_teammate(int id)
{
    return closest_opponent_to(mXmlWorldData
				.Teammates()
				.ByUid(id)
				.position()
				.position);
}

int
WorldData :: closest_opponent_to_opponent(int id)
{
    return closest_opponent_to(mXmlWorldData
				.Opponents()
				.ByUid(id)
				.position()
				.position);
}

int
WorldData :: closest_teammate_to_ball()
{
    return closest_teammate_to(ball());
}

int
WorldData :: closest_opponent_to_ball()
{
	return ClosestInList(ball(), mXmlWorldData
			.Opponents()
			.WithFound(true));
}


bool
WorldData :: opponent_is_found(int uid)
{
    return mXmlWorldData
				.Opponents()
				.ByUid(uid)
				.found();
}


bool
WorldData :: teammate_is_found(int uid)
{
    if (uid != my_id())
    {
        return mXmlWorldData.Teammates().ByUid(uid).found();
   
    }
    return mXmlWorldData.Me().found();
}


int
WorldData :: mr_number_found_teammates()
{
	return mXmlWorldData
			.Teammates()
			.WithFound(true)
			.size();
}

int
WorldData :: mr_number_found_opponents()
{
	return mXmlWorldData
			.Opponents()
			.WithFound(true)
			.size();
}

std::vector<int>
WorldData :: mr_found_opponents()
{
	std::vector<int> agents;
	AgentList l = mXmlWorldData
					.Opponents()
					.WithFound(true);

	for (AgentList::iterator it = l.begin(); it != l.end(); ++it)
	{
		agents.push_back(it->uid());
	}

	return agents;
}

std::vector<int>
WorldData :: mr_found_players()
{
	std::vector<int> agents;
	AgentList l = mXmlWorldData
					.All()
					.WithFound(true);

	for (AgentList::iterator it = l.begin(); it != l.end(); ++it)
	{
		agents.push_back(it->uid());
	}
	return agents;
}

std::vector<int>
WorldData :: mr_found_teammates()
{
	std::vector<int> agents;
	AgentList l = mXmlWorldData
					.Teammates()
					.WithFound(true);

	for (AgentList::iterator it = l.begin(); it != l.end(); ++it)
	{
		agents.push_back(it->uid());
	}
	return agents;
}

int
WorldData :: mr_teammate_by_nickname(const std::string& nickname)
{
	Agent a = mXmlWorldData.Teammates()[nickname];
	return a.uid();
}

int WorldData::ClosestInList(const vector_t& me, const AgentList& positions)
{
	int uid = -1, min;

	if (!positions.empty())
	{
		// Start the minimum as the first element
		vector_t ag = positions[0].position().position;
		min = subtract(ag, me).length;
		uid = positions[0].uid();

		AgentList::const_iterator it = positions.begin();
		for (; it != positions.end(); ++it)
		{

			Agent other = *it;
			ag = other.position().position;
			double dist = subtract(ag, me).length;
			if (dist < min)
			{
				uid = it->uid();
				min = dist;
			}
		}
	}

	return uid;
}

bool
WorldData :: updateAll(){
    int tries = iTries;
    while(tries){
        if(pConnection->Receive()){
            std::string aux = pConnection->getData();
            // std::cout << aux << std::endl;
            std::stringstream data(aux);
            XMLItem root = XMLSerializer::documentToRoot(&data);
            mXmlWorldData = WorldDataApplication();
            if (!mXmlWorldData.read(root))
            {
            	std::cout << "Error processing XML data" << std::endl;
            	return false;
            }

            return true;
        }
        tries--;

        usleep(iSleep);
    }

    std::cout << "updated false" << std::endl;
    return false;
}