#! /bin/sh

sleep 1
echo "********* SoccerServer **********"
BIN="SoccerServer"
DES="/home/sepehr/MixReal/mr-soccer_istanbul"
cd $DES/soccer/soccer_server
xterm -e "./$BIN" &

sleep 1
echo "********* Graphic Module **********"
BIN="KoblenzGraphics"
cd $DES/graphics/trunk
xterm -e "./$BIN" &

sleep 2
echo "********* MR-Simulator **********"
BIN="MR_Simulator"
cd $DES/mr-simulator/soccer_simulator
xterm -e "./$BIN " &

sleep 0
echo "********* OperatorServer **********"
BIN="VisualOperator"
cd $DES/soccer/soccer_visual_operator
xterm -e "./$BIN" &

echo "***********finish************"


