/*
 * Written by Juliana Fajardini <jufajardini@gmail.com>
 *
 * This program is free software; you can refdistribute it and/or
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


#include "BasicPlayer.h"
#include "Xml_CommandInterface.h"
#include "sstream"
#include "Degree.h"
#include "fstream"

BasicPlayer::BasicPlayer (mrTeam myTeam)
{
    mTeam = myTeam;
}

BasicPlayer::~BasicPlayer()
{
}

void
BasicPlayer::setConnection(Connection* newConnection)
{
    pConnection = newConnection;
}

void
BasicPlayer::setWorldData(WorldData* newWorldData)
{
    mWorldData = newWorldData;
}

/*
 * Sends command stored in cmd to the soccer server.
 * Must be called once per cicle.
 */
void
BasicPlayer :: flush()
{
    stringstream cmdMsg;
    XMLSerializer::writeNestedNode(&cmdMsg, &mCmd);
    pConnection->Send(cmdMsg.str());

}

/* Returns agent's team.
 */
mrTeam
BasicPlayer :: getAgentTeam()
{
   return mTeam;
}

/* Tells whether the robot can kick or not.
 * Distance is given in milimmeters.
 */
bool
BasicPlayer :: can_kick()
{
    vector_t t = mWorldData->ball();
    /* Length is already in milimmeters.
     */
    if (t.length < MR_TOKICK_MAX_DISTANCE)
        return true;
    else
        return false;
}


/* This function makes the robot kick the
 * ball. The angle gives the direction of the
 * kick.
 */
void
BasicPlayer ::  kick(double angle, double strength)
{
    mCmd.setKick(angle, strength);
}

/* Set the velocity of the wheels.
 */
void
BasicPlayer :: set_velocity(double left, double right)
{
    mCmd.setVelocity(left, right);
}

/* Makes the robot go straight with same
 * velocity for both wheels.
 */
void
BasicPlayer :: go_straight(double speed)
{
    set_velocity(speed,speed);
}

/* Stops the robot
 */
void
BasicPlayer :: stop()
{
    set_velocity(0,0);
}

/* TODO all these methods that use some velocity must be checked
 * and updated to new environment compatible values.*/

/* Makes the robot turn left
 */
void
BasicPlayer :: quick_turn_left()
{
    set_velocity(-MR_FASTEST_WHEEL_SPEED, MR_FASTEST_WHEEL_SPEED);
}

/* Makes the robot turn right
 */
void
BasicPlayer :: quick_turn_right()
{
    set_velocity(MR_FASTEST_WHEEL_SPEED, -MR_FASTEST_WHEEL_SPEED);
}

/* Makes the robot turn left
 */
void
BasicPlayer :: turn_left()
{
    set_velocity(-MR_NORMAL_WHEEL_SPEED, MR_NORMAL_WHEEL_SPEED);
}

/* Makes the robot turn right
 */
void
BasicPlayer :: turn_right()
{
    set_velocity(MR_NORMAL_WHEEL_SPEED, -MR_NORMAL_WHEEL_SPEED);
}

/* Makes the robot turn left
 */
void
BasicPlayer :: slow_turn_left()
{
    set_velocity(-MR_SLOW_WHEEL_SPEED, MR_SLOW_WHEEL_SPEED);
}

/* Makes the robot turn right
 */
void
BasicPlayer :: slow_turn_right()
{
    set_velocity(MR_SLOW_WHEEL_SPEED,-MR_SLOW_WHEEL_SPEED);
}


/* Makes the robot go in a curve with a
 * trajectory of an arc to the left
 */
void
BasicPlayer ::  curve_left()
{
    set_velocity(MR_SLOW_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
}

/* Makes the robot go in a curve with a
 * trajectory of an arc to the right
 */
void
BasicPlayer ::  curve_right()
{
    set_velocity(MR_FASTEST_WHEEL_SPEED,MR_SLOW_WHEEL_SPEED);
}

/* This makes the robot go toward a vector,
 * turning and making curves when necessary.
 */
//MUST review velocities values, based on real tests
void
BasicPlayer :: go_to_original(vector_t v)
{
    /* Adjust the angle to be between -180
     * and 180 degrees.
     */
    BOUND_ANGLE(v.angle)

    /* If the angle error is bigger than
     * 90 degrees let's just turn
     */
    if (v.angle > M_PI/2) turn_right();
    else if (v.angle < -M_PI/2) turn_left();
    /* Otherwise let's check the distance
     */
    else if (v.length > 150)
    {
        /* We are still far, let's
         * not care much about being
         * completely aligned.
         */
        if (v.angle > DEG2RAD(20.0))
            curve_right();
        else if (v.angle < DEG2RAD(-20.0))
            curve_left();
        else if (v.length > 200)
            /*
             * The idea here is having a really fast velocity.
             * But - which one it's going to be?
             */
            go_straight(MR_FASTEST_WHEEL_SPEED);
        else
            go_straight(MR_FASTEST_WHEEL_SPEED);
    } else if (v.length > 50) {
        /* Okay we are closer, let's
         * narrow the angle
         */
        if (v.angle > DEG2RAD(10.0))
            curve_right();
        else if (v.angle < DEG2RAD(-10.0))
            curve_left();
        /* Angle seems OK, let's keep going
         * straight ahead
         */
        else
            go_straight(MR_SLOW_WHEEL_SPEED);
    } else if (v.length > 10) {
        /* Okay we are closer, let's
         * narrow the angle
         */
        if (v.angle > DEG2RAD(5.0))
            set_velocity(MR_SLOW_WHEEL_SPEED,0);
        else if (v.angle < DEG2RAD(-5.0))
            set_velocity(0,MR_SLOW_WHEEL_SPEED);
        /* Angle seems OK, let's keep going
         * straight ahead
         */
        else
            go_straight(MR_SLOW_WHEEL_SPEED);
    } else {
        /* Good enough! We stop here.
         */
        stop();
    }
}


/* This makes the robot go toward a vector,
 * turning and making curves when necessary.
 */
//MUST review velocities values, based on real tests
void
BasicPlayer :: go(float x,float y)
{
    Point target(x,y);
    Point man(0,0);
    
    Vector Man = mWorldData->me(true);

    man.x = Man.getX();
    man.y = Man.getY();
    man = Man.asPoint();

    if(man.getDistance(target) > 10)
    {
	Vector meTarget;
	
	meTarget.setByPoints(man,target);
  
	float bodyAngle = mWorldData->bodyAngle();
	cout << "[go] bodyAngle:  " << bodyAngle << endl;
	float sensAngle = 30 - ((man.getDistance(target)) / 65);
	cout << "[go] sensAngle:  " << sensAngle << endl;

	if(fabs(Degree::normalizeAngle((bodyAngle - meTarget.getDirection()))) > sensAngle)
	{
		/* turning */
        
            /* options : */
         
	    if(meTarget.getDirection() >= 0 && meTarget.getDirection() <= 90)
	    {
	        if(bodyAngle >= 0 && bodyAngle <= 90)
	        {
    	            if(bodyAngle > meTarget.getDirection())
    	                set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
                
        	    else
            	        set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
                    
        	}
            
        	if(bodyAngle > 90 && bodyAngle <= 180)
        	{
            	    if(bodyAngle > meTarget.getDirection())
                	set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
                
            	    else
                	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
                    
        	}
            
        	if(bodyAngle < 0 && bodyAngle >= -90)
        	{
        		set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
        	}
            
        	if(bodyAngle < -90 && bodyAngle> -180)
        	{
        	    if(fabs(bodyAngle) + fabs(meTarget.getDirection()) <= 180)
        	        set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
            	    else
                	set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
        	}
               
	    }
        
    	if(meTarget.getDirection() > 90 && meTarget.getDirection() <= 180)
	    {
    		if(bodyAngle >= 0 && bodyAngle <= 90)
    	       	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);

    	    if(bodyAngle > 90 && bodyAngle <= 180)
    	    {
    	    	if(bodyAngle > meTarget.getDirection())
                	set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);

            	else
                	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
    	    }
            
        	if(bodyAngle < 0 && bodyAngle >= -90)
        	{
            	    if(fabs(meTarget.getDirection()) + fabs(bodyAngle) <= 180)
                	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
                
            	    else
                	set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
	        }
            
        	if(bodyAngle < -90 && bodyAngle > -180)
        	{
            	    set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
        	}
            
    	}
        
    	if(meTarget.getDirection() < 0 && meTarget.getDirection() >= -90)
    	{
        	if(bodyAngle >= 0 && bodyAngle <= 90)
        	{
            	    set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
        	}   	
            
        	if(bodyAngle > 90 && bodyAngle <= 180)
        	{
            	    if(fabs(meTarget.getDirection()) + fabs(bodyAngle) <= 180)
                	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);

            	    else
                	set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
        	}
            
        	if(bodyAngle < 0 && bodyAngle >= -90)
        	{
            	    if(bodyAngle > meTarget.getDirection())
            	        set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
                
            	    else
                	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
        	}
            
        	if(bodyAngle < -90 && bodyAngle > -180)
        	{	
            	    set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
        	}
            
    	}
        
    	if(meTarget.getDirection() < -90 && meTarget.getDirection() > -180)
    	{
        	if(bodyAngle >= 0 && bodyAngle <= 90)
        	{
            	if(fabs(meTarget.getDirection()) + fabs(bodyAngle) <= 180)
            		set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
            	else
                    set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
        	}
            
        	if(bodyAngle > 90 && bodyAngle <= 180)
        	{
        	    set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
        	}
            
        	if(bodyAngle < 0 && bodyAngle >= -90)
        	{
        	    set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
        	}
            
        	if(bodyAngle < -90 && bodyAngle > -180)
        	{
        		if(bodyAngle > meTarget.getDirection())
                	set_velocity(MR_FASTEST_WHEEL_SPEED,-MR_FASTEST_WHEEL_SPEED);
            	else
                	set_velocity(-MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
        	}
    	}
        


	}
    
	else
	{
	    /* moving */
	    set_velocity(MR_FASTEST_WHEEL_SPEED,MR_FASTEST_WHEEL_SPEED);
	}
}
}

void
BasicPlayer :: goForbidden(float x,float y,float recX1,float recY1,float recX2,float recY2)
{
    
}

void 
BasicPlayer :: goForbidden(Point target,Point point,float dangerDistance)
{
   /* mavade avalie :)   */
    
    Point me = mWorldData->me(true);
    Point newTarget = point;
    
     
    /* configuring mavade avalie :) */
    if(me.getY() < point.getY())
        newTarget.y -= 3 * dangerDistance;                    
    else
        newTarget.y += 3 * dangerDistance;
    
    if(me.getDistance(point) < 30)
    {
        go(newTarget.getX(),newTarget.getY());
    }
    
    else
    {
        go(target.getX(),target.getY());
    }
        
       
}

bool
BasicPlayer :: isForbidden(float x,float y,float recX1,float recY1,float recX2,float recY2)
{ 
    Point Me(mWorldData->me(true).getX(),mWorldData->me(true).getY());
    Point target(x,y);
    Point a(recX1,recY1);
    Point b(recX2,recY2);
    
    Circle forbiddenCircle;
    
    forbiddenCircle.setByCenterRadius((a + b) / 2, a.getDistance(b));
       
    HalfLine meTarget;
    
    Vector meTar;
    
    meTar.setByPoints(Me,target);
    meTarget.setByPointDir(Me,meTar.getDirection());
    
    vector <Point> intersect;
    
    meTarget.getCircleIntersect(forbiddenCircle,intersect);
    if(intersect.size() == 0)
        return false;
    else
        return true;
    
}

bool
BasicPlayer :: isForbidden(Point target,Point point,float dangerDistance)
{
    FullLine meTarget(mWorldData->me(true),target);
    float distance = meTarget.getDistance(point);
    if(distance <= dangerDistance)
        return true;
    else
        return false;            
}

void
BasicPlayer :: go_to(vector_t v)
{
     /* Adjust the angle to be between -180
	 * and 180 degrees.
	 */
	BOUND_ANGLE(v.angle)

   	int back = 0 ;

	if((v.angle < -M_PI/3 && v.angle > -(2*M_PI/3))
	  || (v.angle > M_PI/3 && v.angle < (2*M_PI/3) ))
	{
		go_to_original(v); // ou turn_left();
	}
	else
	{
		/* If the angle error is bigger than
		 * 90 degrees let's flag it and reduce the angle
		 */
		if (v.angle > M_PI/2) {
			back = 1;
			v.angle = v.angle-M_PI;
		}
		else if (v.angle < -M_PI/2) {
			back = 1;
			v.angle = v.angle+M_PI;
		}
		/* Now let's check the distance
		 */
		if (v.length > 150)
		{
			/* We are still far, let's
			 * not care much about being
			 * completely aligned.
			 */
			if (v.angle > DEG2RAD(20.0))
				curve_right2(back);
			else if (v.angle < DEG2RAD(-20.0))
				curve_left2(back);
			else if (v.length > 120) {
				if (back) go_straight(-MR_FASTEST_WHEEL_SPEED);
				else go_straight(MR_FASTEST_WHEEL_SPEED);
			}
			else {
				go_straight(back ?
						  -MR_FASTEST_WHEEL_SPEED
						: MR_FASTEST_WHEEL_SPEED);
			}
		} else if (v.length > 50) {
			/* Okay we are closer, let's
			 * narrow the angle
			 */
			if (v.angle > DEG2RAD(10.0))
				curve_right2(back);
			else if (v.angle < DEG2RAD(-10.0))
				curve_left2(back);
			/* Angle seems OK, let's keep going
			 * straight ahead
			 */
			else
				go_straight(back ? -MR_SLOW_WHEEL_SPEED: MR_SLOW_WHEEL_SPEED);
		} else if (v.length > 10) {
			/* Okay we are closer, let's
			 * narrow the angle
			 */
			if (v.angle > DEG2RAD(5.0)) {
				if (back) set_velocity(0,-MR_SLOW_WHEEL_SPEED);
				else set_velocity(MR_SLOW_WHEEL_SPEED,0);
			}
			else if (v.angle < DEG2RAD(-5.0)) {
				if (back) set_velocity(-MR_SLOW_WHEEL_SPEED,0);
				else set_velocity(0,MR_SLOW_WHEEL_SPEED);
			}
			/* Angle seems OK, let's keep going
			 * straight ahead
			 */
			else
				go_straight(back ? -MR_NORMAL_WHEEL_SPEED : MR_NORMAL_WHEEL_SPEED);
		} else {
			/* Good enough! We stop here.
			 */
			stop();
		}
	}
}


/* Makes the robot go in a curve with a
 * trajectory of an arc to the left
 */
void
BasicPlayer :: curve_left2(int back) {
  if (back) set_velocity(-MR_NORMAL_WHEEL_SPEED,-MR_SLOW_WHEEL_SPEED);
  else set_velocity(MR_SLOW_WHEEL_SPEED,MR_NORMAL_WHEEL_SPEED);
}

/* Makes the robot go in a curve with a
 * trajectory of an arc to the right
 */
void
BasicPlayer :: curve_right2(int back) {
  if (back) set_velocity(-MR_SLOW_WHEEL_SPEED,-MR_NORMAL_WHEEL_SPEED);
  else set_velocity(MR_NORMAL_WHEEL_SPEED,MR_SLOW_WHEEL_SPEED);
}

/* This makes the robot go toward a vector,
 * turning and making curves when necessary.
 */
void
BasicPlayer ::  chase(vector_t v)
{
    go_to(v);
}

/* This is the oposite of go_to, making
 * the robot escape away from the given
 * vector.
 */
void
BasicPlayer ::  escape_from(vector_t v)
{
    v.angle += M_PI; go_to(v);
}