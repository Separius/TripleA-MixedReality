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
                                                                                
/*
 * ATENTION!
 *
 * This class uses some methods early defined in Rodrigo Guerra's
 * pv-soccer-client. The reason to use them is provide an
 * already known interface to the teams, and also make good
 * use of what already existed of ealier work in MR-league.
 *
 * The main idea of this class is provide the basic methods that an agent
 * must have, in order to be able to play. To get information about
 * opponents, teammates and stuff, the agent would use "WorldData" methods.
 */

#ifndef BASICPLAYER_H_
#define BASICPLAYER_H_

#include <sstream>

#include <BasicUtils.h>
#include <Xml_CommandInterface.h>
#include <WorldData.h>
#include <Circle.h>
#include <HalfLine.h>
#include <Types.h>
#include <FullLine.h>

class BasicPlayer
{
public:
	BasicPlayer(mrTeam myTeam);
	virtual ~BasicPlayer();

	void setConnection(Connection* newConnection);
	void setWorldData(WorldData* newWorldData);

	/* Sends command stored in cmd to the soccer server.
	 * Must be called one time per cicle.
	 */
	void flush();

	/* This function makes the robot kick the
     * ball. The angle gives the direction of the
     * kick and the strength.
     */
    void kick(double angle, double strength);

    /* Tells whether the robot can kick or not.
     */
    bool can_kick();

	/* Set the velocity of the wheels.
     */
    void set_velocity(double left, double right);

    /* Makes the robot go straight with same
     * velocity for both wheels.
     */
    void go_straight(double speed);

    /* Stops the robot
     */
    void stop();

    /* Makes the robot turn left (rapido)
     */
    void quick_turn_left();

    /* Makes the robot turn right (rapido)
     */
    void quick_turn_right();

    /* Makes the robot turn left
     */
    void turn_left();

    /* Makes the robot turn right
     */
    void turn_right();

    /* Makes the robot turn left (lento)
     */
    void slow_turn_left();

    /* Makes the robot turn right (lento)
     */
    void slow_turn_right();

    /* Makes the robot go in a curve with a
     * trajectory of an arc to the left
     */
    void curve_left();

    /* Makes the robot go in a curve with a
     * trajectory of an arc to the right
     */
    void curve_right();

    /* This makes the robot go toward a vector,
     * turning and making curves when necessary.
     */
    void go_to_original(vector_t v);
    void go_to(vector_t v);
    void go(float x,float y);        // this function lead to agent go to the special point
    void goForbidden(float x,float y,float recX1,float recY1,float recX2,float recY2);   //this function lead to agent go to special point and do not cross in special forbiden area
    void goForbidden(Point target,Point point,float dangerDistance);  //this function lead to agent go to the special point and do not cross near the special point
    bool isForbidden(float x,float y,float recX1,float recY1,float recX2,float recY2);   //this function says that do we cross in forbidden area
    bool isForbidden(Point target,Point point,float dangerDistance);              //this function says that do we cross near the forbidden point 
    void curve_left2(int back);
    void curve_right2(int back);
    void chase(vector_t v);

    /* This is the oposite of go_to, making
     * the robot escape away from the given
     * vector.
     */
    void escape_from(vector_t v);

    /* Returns agent's team.
     */
    mrTeam getAgentTeam();


private:

	Connection* pConnection;

    /* Receives the commands and generates the xml code.
     */
    Command mCmd;
    mrTeam mTeam;
    WorldData* mWorldData;
};

#endif /*BASICPLAYER_H_*/