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


#ifndef WORLDDATA_H_
#define WORLDDATA_H_

#include <string>
#include <vector>

#include "BasicUtils.h"
#include "Connection.h"
#include "Xml_ServerMessage.h"
#include "AgentList.h"
#include <Vector.h>
#include <Line.h>

/*
 * These are DEFINEs created to help fine-tuning the base client
 */

#define MR_STRONGEST_KICK 1
#define MR_STRONG_KICK 0.85
#define MR_AVERAGE_KICK 0.7
#define MR_WEAK_KICK 0.55
#define MR_WEAKEST_KICK 0.3


//The actual max_distance is within soccerServer...
#define MR_TOKICK_MAX_DISTANCE 34.5


#define MR_NONE_WHEEL_SPEED 0.0
#define MR_SLOW_WHEEL_SPEED 31.0
#define MR_NORMAL_WHEEL_SPEED 60.0
#define MR_FAST_WHEEL_SPEED 100.0
#define MR_FASTEST_WHEEL_SPEED 130.0

#define VEL_RODA_LENTA 10.0
#define VEL_RODA_MEDIA 65.0
#define VEL_RODA_RAPIDA 130.0

//goto
#define MR_DISTANCE_VERY_NEAR 45
#define MR_DISTANCE_NEAR  70
#define MR_DISTANCE_FAR 130
#define MR_DISTANCE_VERY_FAR 200

//Roles
#define MR_ROLE_UNKNOWN 0
#define MR_ROLE_GOALIE 1
#define MR_ROLE_DEFENCE 2
#define MR_ROLE_OFFENCE 3

#define ACTION_NON 0
#define ACTION_SHOOT 1
#define ACTION_PASS 2
#define ACTION_CLEAR 3
#define ACTION_BLOCK 4
#define ACTION_DRIBBLE 5
#define ACTION_DEFPOS 6
#define ACTION_OFSPOS 7
#define ACTION_GO 8


enum side_t
{
	BOTTOM,
	TOP
};

enum flag_t
{
    TOP_LEFT_POLE,
    BOTTOM_LEFT_POLE,
    TOP_RIGHT_POLE,
    BOTTOM_RIGHT_POLE,
    TOP_LEFT_CORNER,
    BOTTOM_LEFT_CORNER,
    TOP_RIGHT_CORNER,
    BOTTOM_RIGHT_CORNER,
    TOP_LEFT_GOAL_AREA,
    BOTTOM_LEFT_GOAL_AREA,
    TOP_RIGHT_GOAL_AREA,
    BOTTOM_RIGHT_GOAL_AREA,
    TOP_LEFT_SMALL_AREA,
    BOTTOM_LEFT_SMALL_AREA,
    TOP_RIGHT_SMALL_AREA,
    BOTTOM_RIGHT_SMALL_AREA,
    TOP_CENTER,
    CENTER,
    BOTTOM_CENTER,
};

class WorldData
{
public:
	WorldData(int sleep, int tries, mrTeam team);
	virtual ~WorldData();

	int getISleep();

        int mrRoles[5]; //Number of team members
        int teamAction[5];
        int owner;//owner of the ball

	void setConnection(Connection * newConnection);

	/**
	 * Returns a polar vector poiting towards the
     * ball in the field.
     *
     * @return the position of the ball
     * @throw ClientException if the agent is lost
     */
        
     Vector realVectors[19];

     void setRealVectors();

     vector_t ball();

    /* Returns a vector pointing towards the goal
     * poles in the field.
     */
     
     //float getBallVelocity();

     vector_t mygoal_pole1();
     vector_t mygoal_pole2();
     vector_t opgoal_pole1();
     vector_t opgoal_pole2();


    /* Returns a vector pointing towards the field
     * corners.
     */
     vector_t mycorner_flag1();
     vector_t mycorner_flag2();
     vector_t opcorner_flag1();
     vector_t opcorner_flag2();

     /*
      * Given a flag Id, returns a polar vector to that flag.
      */
     vector_t mrFlag(std::string flagId);

     /*gWorldData
      * Return polar vectors to the goal poles
      */
     vector_t mrTopLeftPole();
     vector_t mrBottomLeftPole();
     vector_t mrTopRightPole();
     vector_t mrBottomRightPole();

     /*
      * Return polar vectors to the corner flags
      */
     vector_t mrTopLeftCorner();
     vector_t mrBottomLeftCorner();
     vector_t mrTopRightCorner();
     vector_t mrBottomRightCorner();

     /*
      * Return polar vectors to the goal area flags
      */
     vector_t mrTopLeftGoalArea();
     vector_t mrBottomLeftGoalArea();
     vector_t mrTopRightGoalArea();
     vector_t mrBottomRightGoalArea();

     /*
      * Return polar vectors to the small area flags
      */
     vector_t mrTopLeftSmallArea();
     vector_t mrBottomLeftSmallArea();
     vector_t mrTopRightSmallArea();
     vector_t mrBottomRightSmallArea();

     /*
      * Return polar vector to central flags
      */
     vector_t mrTopCenter();
     vector_t mrMiddleField();
     vector_t mrBottomCenter();
     

     /***
      *
      * This function return an agentVector
      * corresponding the specified uid
      *
      * @param An agent uid
      *
      * @return The agentVector of the
      * specified agent
      *
      * @throw ClientException in case the agent
      * specified is lost or don't exist
      *
      */
     agentVector mr_agent(int uid);

    /**
     * This function returns the polar vector that
     * gives the estimate position of a given
     * teammate player
     *
     * @param uid The <b>found</b> teammate uid
     *
     * @return a polar vector of the specified
     * teammate
     *
     * @throw ClientException in case the agent
     * specified is lost or don't exist
     *
     */
     vector_t teammate(int uid);

    /**
     * This function returns the polar vector that
     * gives the estimate position of a given
     * opponent player
     *
     * @param uid The opponents <b>found</b> uid
     *
     * @return a polar vector position
     * of the specified uid
     *
     * @throw ClientException in case the
     * agent specified is lost, or don't
     * exist
     */
     vector_t opponent(int uid);

    
    
    /**
    * This Function Returns the Real Body Angle based Real Center of Field
    */
    float bodyAngle();    
    
    
    
    /**
    * This Function Returns The Position of Agent Based Real Center of Field
    */
    Vector me(bool a);    
    
    
    
    /**
    * This Fucntion Returns The Tool of Field
    */
    float fieldTool();
    
    
    
    /**
    * This Function Returns the Arz of Field
    */
    float fieldArz();
    
    
    
    /**
    * This Fucntions Convert Vectors Based Me to Vectors Based Real Center Of Field
    */
    Point basedCenter(vector_t v);
    
    Point oppPositions[5],ourPole1,ourPole2,oppPole1,oppPole2;
    Point tmmPositions[4];
    
    float getNearstOppDistFromLine(Line line);
    int getNearstOppToPoint(Point point);
    int getNearstOppToLine(Line line);
    float getOppNearestDistance(Line line);
    /**
     * Returns the vector pointing to the self. This
     * always length zero, angle zero, but is put
     * here for using combined with other functions
     * like closest_teammate_to(me()) or more
     * phylosophical ones like escape_from(me())
     *
     * @return A vector pointing to one's self
     */
    
    
    
    
     vector_t me();

    /**
     * This function returns the uid of the closest
     * teammate to a certain position given as
     * argument
     *
     * @param v a vector containing the position
     *
     * @return The uid of the closest teammate
     * not including one's self or -1 in case
     * no teammate was found
     */
     int closest_teammate_to(vector_t v);
	
    /**
     * This function returns the uid of the closest
     * opponent to a certain position given as
     * argument
     *
     * @param v A vector containing a valid position
     *
     * @return The uid of the opponent or -1 in case
     * no opponent was found
     */
     int closest_opponent_to(vector_t v);

    /**
     * This function returns the uid of the closest
     * teammate, not including self, to the given
     * opponent
     *
     * @param the uid of the specified <b>found</b>
     * opponent
     *
     * @return the uid of the teammate, or -1 if
     * no teammate exist
     *
     * @throw ClientException when the specified
     * argument uid was not found by the server
     * or don't exist
     */
     int closest_teammate_to_opponent(int uid);

    /**
     *  This function returns the uid of the closest
     * teammate to the given teammate
     *
     * @param uid the <b>found</b> teammate UID
     *
     * @return The uid of the closest teammate
     * to the specified teammate or -1 if no other
     * teammate was found
     *
     * @throw ClientException if the argument uid
     * is invalid or was not found by the server
     */
     int closest_teammate_to_teammate(int id);

    /**
     *  This function returns the uid of the closest
     * opponent to the given teammate
     *
     * @param uid The <b>found</b> teammate UID
     *
     * @return The Opponents UIDs or -1 in case
     * no opponent was found
     *
     * @throw ClientException when the client of
     * the UID parameter was not found by the server
     * or don't exist
     */
     int closest_opponent_to_teammate(int uid);

    /**
     * This function returns the uid of the closest
     * teammate to the given teammate
     *
     * @param uid A <b>found</b> opponent's uid
     *
     * @return The UID of the closest opponent
     * or -1 in case there is no other opponent
     *
	 * @throw ClientException when the client of
     * the UID parameter was not found by the server
     * or don't exist
     */
     int closest_opponent_to_opponent(int uid);


     /**
      *
      * Returns the number of found teammates
      * in the current cycle
      *
      * @return An integer specifying the
      * number of found teammates
      *
      */
     int mr_number_found_teammates();

     /**
      *
      * Returns the number of found teammates
      * in the current cycle
      *
      * @return An integer specifying the
      * number of found teammates
      *
      */
     int mr_number_found_opponents();

     /**
      * Return all <b>found</b> players uids
      *
      * @return a vector containing the values
      *
      */
     std::vector<int> mr_found_players();

     /**
      * Return all <b>found</b> opponents uids
      *
      * @return a vector containing the values
      *
      */
     std::vector<int> mr_found_opponents();

     /**
      * Return all <b>found</b> teammates uids,
      * not including self
      *
      * @return a vector containing the values
      *
      */
     std::vector<int> mr_found_teammates();

     /**
      * Returns the teammate uid based on the
      * argument nickname
      *
      * @param nickname a string containing the
      * agent nickname
      *
      * @throw ClientException if no agent with
      * such nickname exists
      *
      */
     int mr_teammate_by_nickname(const std::string& nickname);

    /**
     * This function returns the id of the closest
     * teammate to the ball
     *
     * @return The teammate's UID or -1 in case no
     * teammates were found
     */
     int closest_teammate_to_ball();

    /**
     * This function returns the id of the closest
     * opponent to the ball
     *
     * @return The opponent's UID or -1 in case
     * no opponents were found
     */
     int closest_opponent_to_ball();

    /**
     * Finds the closest player. If no players
     * were found, this method returns
     * a vector_t zero-initialized
     *
     * @return a vector_t containing
     * the found player.
     *
     * @throw ClientException if self
     * it lost
     */
     vector_t closest_player();

    /**
     * Return the uid of one's self
     *
     * @return The player's uid
     */
     int my_id();

    /**
     * Tells whether you are lost or not. Being lost
     * happens when the robot is removed from the field
     * or if the camera cannot find it for any reason.
     *
     * @return <i>true</i> in case the agent was
     * not found, and <i>false</i> otherwise
     *
     */
     bool i_am_lost();

    /**
     * Tells if a given oponent was found in
     * the field
     *
     * @param uid the opponent uid
     * @return <i>true</i> in case it was found
     * and <i>false</i> otherwise
     */
     bool opponent_is_found(int uid);

    /**
     * Tells if a given teammate, including
     * you, was found in the field
     *
     * @param uid the teammate uid
     * @return <i>true</i> in case it was found
     * and <i>false</i> otherwise
     */
     bool teammate_is_found(int uid);

    /**
     * Returns the time
     * of last update
     * in the server clock, in seconds,
     *
     * @return the time in seconds
     */
     double time();

    /**
     * Returns 1 if the game is on
     * or zero if not
     *
     * @param <b>true</b> if the game is on
     * </b>false</b> otherwise
     */
     bool is_game_on();
     
     vector_t nearestTmmToMe();

    /**
     * Returns current/last playmode received
     * from match's server.
     */
     std :: string mrPlayMode();

     /**
      * Return the given flag
      * @param flag the flag id
      * @return The given flag position
      */
     vector_t mr_get_flag(flag_t flag);


    /**
     * Should be called at the beginning of every cicle.
     * Updates information sent by the soccer server.
     *
     * @return <b>true</b> in case the update was successful
     */
    bool updateAll();

    WorldDataApplication mXmlWorldData;

private:

	int ClosestInList(const vector_t& me, const AgentList& positions);

    /*
     * WorldData contains all information about the world.
     * Such information is stored inside the xml derived
     * data structure WorldDataApplication.
     */

    Connection * pConnection;

    /**
     * Tells how much time we should sleep, during updating
     *
     **/
    int iSleep;

    /**
     *
     * Tells how many times should we try to get a message from the server.
     **/
    int iTries;

    mrTeam mAgentTeam;
};

#endif /*WORLDDATA_H_*/