#!/bin/bash

host=localhost
port_team=3310

#1 stands for blue, 2 for yellow
team_id=0

if [ "$port_team" = 3310 ]; then
	team_id="TripleA" # blue
else if [ "$port_team" = 3311 ]; then
	team_id="virOpp" # yellow
fi
fi
echo "yor team running as : ${team_id} "
# set your robot control ID's here
rc_id_1=0
rc_id_2=1
rc_id_3=2
rc_id_4=3
rc_id_5=4
# set your robots vision ID's here
vt_id_1=0
vt_id_2=1
vt_id_3=2
vt_id_4=3
vt_id_5=4
# define time delay between running agents
delay=1
# enter name of your team binary here
binary_name="ESKILASAgent"

echo "Connecting ..."
# xterm -e  "./$binary_name  -t ${team_id} -p ${port_team} -r ${rc_id_1} -v ${vt_id_1} " & 
#sleep ${delay}
# xterm -e  "./$binary_name  -t ${team_id} -p ${port_team} -r ${rc_id_2} -v ${vt_id_2} " & 
#sleep ${delay}
# xterm -e  "./$binary_name  -t ${team_id} -p ${port_team} -r ${rc_id_3} -v ${vt_id_3} " &
#sleep ${delay}
# xterm -e  "./$binary_name  -t ${team_id} -p ${port_team} -r ${rc_id_4} -v ${vt_id_4} " & 
#sleep ${delay}
# xterm -e  "./$binary_name  -t ${team_id} -p ${port_team} -r ${rc_id_5} -v ${vt_id_5} " & 
#sleep ${delay}

echo " Enjoy the game "
