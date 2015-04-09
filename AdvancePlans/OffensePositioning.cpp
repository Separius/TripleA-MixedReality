#include <OffensePositioning.h>
#include <time.h>

/***************FINDING OPPONENTS DEFENCERS*******************************/
void OppDefencers::setData(WorldData* newWorldData,
        BasicPlayer* newBasicPlayer, char areaofopp)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    area = areaofopp;
}

/*
 * miad batavajoh be in ke harif to kdoom nime ye zamin ast
 * yek mostatil (az ye meghdar ghabl az goal area ta tah zamin )ro baresi mikone
 * va uid haye defencer haye doshman va goaler ro too ye moteghayer haye marboote mirize */
void OppDefencers::findGoaler()
{
    //	cout<<"...................find goaler lunched ..............\n";
    double tempX;
    double highestX;
    if (area == 'r' || area == 'R')
    {
        highestX = -900;
    }
    else if (area == 'l' || area == 'L')
    {
        highestX = 900;
    }
    for (int i = 0; i < pWorldData->mr_found_opponents().size(); i++)
    {
        tempX = pWorldData->oppPositions[i].getX();
        if (area == 'r' || area == 'R')
        {
            if (tempX > 0 && tempX > highestX)
            {
                highestX = tempX;
                goaler = i;
            }
        }
        else if (area == 'l' || area == 'L')
        {
            if (tempX < 0 && tempX < highestX)
            {
                highestX = tempX;
                goaler = i;
            }
        }

    }
    // goaler += 5; // yani shomare opponent ha az sefr shroo mishavad
}

void OppDefencers::findthem()
{
    //	cout<<"...................find them lunched ..............\n";
    double tempX;
    //*/ tempX = pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).getX();
    float ballX = pWorldData->basedCenter(pWorldData->ball()).getX();
    tempX = pWorldData->realVectors[11].getX();
    ballX = ballX - tempX;
    //agar biroon e zamin bood haman 0 dar nazar migirimash
    if (ballX < 0)
    {
        ballX = 0;
    }

    uidnumbers = 0;

    Point ras1;
    Point ras2;
    Point minPos;
    Point maxPos;

    if (area == 'l' || area == 'L')
    {
        /* adad e 100 ro az ye jaim va hesi dar avordam
         * in addad bayad besoorate tajrobi dar biad
         * ehtemalan baraye har tim motafavete
         */

        //*/  ras1.x = pWorldData->basedCenter(pWorldData->mrTopLeftGoalArea()).getX() + 100;

        /*	cout<<"<topcenter>"<< realVectors[11].getX() <<"</topcenter>"<<endl;
         cout<<"<center>"<< realVectors[9].getX() <<"</center>"<<endl;
         cout<<"<ball>"<< ballX <<"</ball x>"<<endl;
         cout<<"<tool>"<< pWorldData->fieldTool() <<"</tool>"<<endl;*/

        maxPos.x = pWorldData->realVectors[11].getX()
                + ((pWorldData->realVectors[9].getX()
                - pWorldData->realVectors[11].getX() - 300)
                / pWorldData->fieldTool()) * ballX + 250;
        maxPos.y = pWorldData->realVectors[11].getY();
        ///ras1.x = realVectors[12].getX() + 100;
        ///ras1.y = 0.0;

        //*/ ras2.x = pWorldData->basedCenter(pWorldData->mrTopLeftPole()).getX();
        ///ras2.x = realVectors[13].getX();
        ///ras2.y = pWorldData->fieldArz();
        minPos.x = maxPos.x - 250;
        minPos.y = pWorldData->realVectors[1].getY();
    }
    else if (area == 'r' || area == 'R')
    {

        //*/ ras1.x = pWorldData->basedCenter(pWorldData->mrTopRightGoalArea()).getX() - 100;
        ///	ras1.x = realVectors[16].getX() - 100;
        //*/*/	ras1.y = 0.0;
        ///	ras1.y = 245;
        maxPos.x = pWorldData->realVectors[9].getX()
                + ((pWorldData->realVectors[15].getX()
                - pWorldData->realVectors[9].getX() - 300)
                / pWorldData->fieldTool()) * ballX + 300;
        maxPos.y = pWorldData->realVectors[11].getY();

        minPos.x = maxPos.x - 250;
        minPos.y = pWorldData->realVectors[1].getY();

        //*/ ras2.x = pWorldData->basedCenter(pWorldData->mrTopRightGoalArea()).getX();
        //*/*/ ras2.y = pWorldData->fieldArz();
        ///	ras2.y = -235;
    }
    /***************************************************/
    //    cout << "<rectangle>\n" << "<max>" << maxPos << "</max>\n" << "<min>"
    //            << minPos << "</min>\n" << "</rectangle>" << endl;
    Rectangle mostatil(minPos, maxPos);
    //	cout<<"<uidNUM>"<<uidnumbers<<"</uidNUM>"<<endl;
    //	cout<<"<current_num>"<<pWorldData->mr_number_found_teammates() +1<<"</current_num>/n";
    //	cout<<"<max_num_avalable>"<<pWorldData->mr_number_found_opponents() + pWorldData->mr_number_found_teammates()<<"</max_num_avalable>\n";
    for (int i = 0; i < pWorldData->mr_found_opponents().size(); i++)
    {
        //		cout<<"<findthem>\n";
        //		cout<<"<i>"<<i<<"</i>"<<endl;
        if (i == goaler)
        {
            //			cout<<".....goaler ! continuing....\n";
            continue;
        }
        if (mostatil.isInRectangle(pWorldData->oppPositions[i]) == true)
        {
            //				cout<<"<IN>"<<endl;
            //				cout<<"<i>"<<i<<"</i>"<<endl;
            //				cout<<"</IN>"<<endl;
            uidList[uidnumbers] = i;
            uidnumbers++;
        }
        //		cout<<"</findthem>\n";
    }
    //	cout<<"...................find them ended ..............\n";
}

/*****************************************************************************/

/**********************Ofense Positioning Iran Open***************************/
OfsPos::OfsPos()
{
}

OfsPos::OfsPos(WorldData* newWorldData, BasicPlayer* newBasicPlayer,
        char areaofOpponent, int uids[], int offensersNum)
{
    initializeWorld(newWorldData, newBasicPlayer, areaofOpponent, uids,
            offensersNum);
}

void OfsPos::initializeWorld(WorldData* newWorldData,
        BasicPlayer* newBasicPlayer, char areaofOpponent, int uids[],
        int offensersNum)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    area = areaofOpponent;
    offencer_num = offensersNum;
    uidStart = uids;
    //pointer array to array
    for (int i = 0; i < offensersNum; i++)
    {
        uidList[i] = *uidStart;
        uidStart++;
    }

    // OppDefencers myOppTemp(pWorldData , pBasicPlayer , area);
    myOpp.setData(pWorldData, pBasicPlayer, area);
    myOpp.findGoaler();
    myOpp.findthem();
}

void OfsPos::setPoints()
{
    resultnum = 0;
    if (offencer_num == 1)
    {
        goToPoint(uidList[0], avp[0]);
    }
    else
    {
        if (offencer_num == 2)
            totalDATA = 2;
        else if (offencer_num == 3)
            totalDATA = 6;

        //	cout << "\n<debug>" << endl;
        //	cout << "<oppNum> " << myOpp.uidnumbers << " </oppNum>" << endl;
        //	cout << "<avp>" << endl;
        //	cout << "<0> " << avp[0] << " </0>" << endl;
        //	cout << "<1> " << avp[1] << " </1>" << endl;
        //	cout << "<2> " << avp[2] << " </2>" << endl;
        //	cout << "</avp>" << endl;
        //	cout << "<totalDATA> " << totalDATA << " </totalDATA>" << endl;
        //computeTotal();
        //Ptp *DATA;
        Ptp PosId;
        //DATA = (Ptp*) malloc (sizeof(Ptp) * totalDATA);
        Ptp DATA[6];
        fill(DATA);
        //	cout << "<fill> done </fill>" << endl;
        computePtpSum(DATA);
        cout << "<computePtpSum> done </computePtpSum>" << endl;
        selectIds(DATA, &PosId);
        //	cout << "<selectIds> done </selectIds>" << endl;
        //	cout << "<Result>" << endl;
        for (int i = 0; i < offencer_num; i++)
        {
            cout << "<offense>" << endl;
            cout << "<id> " << uidList[i] << " </id>" << endl;
            cout << "<pos> " << PosId.pos[i] << " </pos>" << endl;
            cout << "<offense>" << endl;
            goToPoint(uidList[i], PosId.pos[i]);
        }
        //	cout << "</Result>" << endl;
        //	cout << "</debug>\n\n" << endl;
    }
}

void OfsPos::fill(Ptp * DATA)
{
    //faghat ta 3 ta support mikone ! bayad 4 ham ezafe beshe
    //cout<<"<lunched>"<<"fill"<<"</lunched>"<<endl;
    //	if (botnum == 1) {
    //		DATA[0].pos[0] = avp[0];
    //	} else
    if (offencer_num == 2)
    {
        for (int i = 0; i <= 1; i++)
        {
            DATA[i].pos[0] = avp[0 + i];
            DATA[i].pos[1] = avp[1 - i];
        }
    }
    else if (offencer_num = 3)
    {
        for (int i = 0; i <= 2; i++)
        {
            for (int j = 0; j <= 1; j++)
            {
                DATA[j + i * 2].pos[0] = avp[0];
                DATA[j + i * 2].pos[1] = avp[1 + j];
                DATA[j + i * 2].pos[2] = avp[2 - j];
            }
            Point exchange = avp[0];
            avp[0] = avp[1];
            avp[1] = avp[2];
            avp[2] = exchange;
        }
    }
}

void OfsPos::computePtpSum(Ptp * DATA)
{
    //cout<<"<lunched>"<<"computePtpSum"<<"</lunched>"<<endl;
    for (int i = 0; i < totalDATA; i++)
    {
        DATA[i].sum = 0;
        for (int j = 0; j < myOpp.uidnumbers + 1; j++)
        {
            DATA[i].sum += pWorldData->tmmPositions[uidList[j]].getDistance(DATA[i].pos[j]);
        }
    }
}

void OfsPos::selectIds(Ptp* DATA, Ptp* resultPtp)
{
    //cout<<"<lunched>"<<"selectIds"<<"</lunched>"<<endl;
    int selected;

    double min = DATA[0].sum;
    selected = 0;

    for (int i = 1; i < totalDATA; i++)
    {
        if (DATA[i].sum < min)
        {
            min = DATA[i].sum;
            selected = i;
        }
    }
    *resultPtp = DATA[selected];
}

void OfsPos::goToPoint(int pId, Point pos)
{
    //    cout<<"result num : "<<resultnum - 1<<endl;
    //    cout<<"..............goto point\n";
    resultId[resultnum] = pId;
    //    cout<<"..............After ID\n";
    resultPos[resultnum] = pos;
    //    cout<<"..............goto......................\n";
    resultnum++;
    //     cout<<"result num : "<<resultnum - 1<<endl;
    //    for(int i=0; i<;i++)
    //    {
    //      cout<<"<i> "<<i<<"</i>"<<endl;
    //      cout<<"point[i] : "<<resultPos[resultnum -1]<<endl;
    //    }
    //IMPORTANT
    //must return it to pworlddata
    //IMPORTANT
}

double
OfsPos::initializeX()
{
    double tempX;
    //*/ tempX = pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).getX();
    ballX = pWorldData->basedCenter(pWorldData->ball()).getX();
    tempX = pWorldData->realVectors[11].getX();
    ballX = ballX - tempX;
    //agar biroon e zamin bood haman 0 dar nazar migirimash
    if (ballX < 0)
    {
        ballX = 0;
    }
    // cout<<"ballx top left(0,0):  "<<ballX<<endl;


    double tempY; //*/= pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).getY();
    tempY = pWorldData->realVectors[11].getY();
    ballY = pWorldData->basedCenter(pWorldData->ball()).getY();
    //    cout<<"<ball>\n"<<"<y>\n"<<"<basedcenter>"<<ballY<<"</basedcenter>\n";
    ballY = tempY - ballY;
    //agar biroon e zamin bood haman 0 dar nazar migirimash
    if (ballY < 0)
    {
        ballY = 0;
    }
    /*****************************************/
    /*****************************************/
    double minX;
    double maxX;

    //*/ maxX = pWorldData->basedCenter(pWorldData->mrTopRightSmallArea()).getX();
    //*/ minX = pWorldData->basedCenter(pWorldData->mrMiddleField()).getX();
    maxX = pWorldData->realVectors[18].getX();
    minX = pWorldData->realVectors[9].getX();

    aviliableTool = maxX - minX;
    double xLine;

    if (area == 'l' || area == 'L')
    {
        xLine = (aviliableTool / pWorldData->fieldTool()) * ballX + pWorldData->realVectors[14].getX();
        //xLine += 100;
        //*/ xLine = (aviliableTool / fieldTool) * ballX + pWorldData->basedCenter(pWorldData->mrTopLeftSmallArea()).getX();
    }
    else if (area == 'r' || area == 'R')
    {
        xLine = (aviliableTool / pWorldData->fieldTool()) * ballX + pWorldData->realVectors[9].getX();
        //xLine -= 100;
        //*/ xLine = (aviliableTool / fieldTool) * ballX + pWorldData->basedCenter(pWorldData->mrMiddleField()).getX();
    }
    return xLine;
}

void OfsPos::position()
{
    cout << ".....................................................POSITION...........................................\n";
    myOpp.setData(pWorldData, pBasicPlayer, area);
    myOpp.findGoaler();
    myOpp.findthem();
    //    cout << "<oppNum>" << myOpp.uidnumbers << "</oppNum>" << endl;
    //    cout << "<offncerNum>" << offencer_num << "</offncerNum>" << endl;
    /********************/
    Shoot myShoot(pWorldData, pBasicPlayer);
    myShoot.setReachableDist(300);
    /*******************/
    Pass myPass(pWorldData, pBasicPlayer);

    if (offencer_num <= 2 && offencer_num > 0)
    {
        //        cout << "<lunched>offencer_num <= 2</lunched>\n";
        //danger zone az yekhorde balatar az pole bala ta ye khorde paiin tar az pole paiin

        Point tempPoint;
        float tempValue;
        float PassValue;
        float shootValue;
        Point selectedPoint;
        bool found = false;


        if (myOpp.uidnumbers == 0)
        {
            tempValue = initializeX();
        }
        else
        {
            if (area == 'r' || area == 'R')
                tempValue = pWorldData->realVectors[7].getX() - 75;
            else if (area == 'l' || area == 'L')
                tempValue = pWorldData->realVectors[3].getX() + 75;
            for (int i = 0; i < myOpp.uidnumbers; i++)
            {
                //                cout << "<lunched>Xsetup based on opp</lunched>\n";
                tempPoint = myOpp.uidList[i];
                if (area == 'r' || area == 'R')
                {
                    if (tempValue < tempPoint.getX())
                        tempValue = tempPoint.getX();
                }
                else if (area == 'l' || area == 'L')
                {
                    if (tempValue > tempPoint.getX())
                        tempValue = tempPoint.getX();
                }
            }
        }
        tempPoint.x = tempValue;
        //        cout << "<X>" << tempPoint.x << "</X>\n";
        /*************************************************/
        /*************************************************/
        //        cout << "................................<lunched1>Shoot check</lunched1>...............................\n";

        //whole lenghth check
        for (int i = pWorldData->realVectors[13].getY(); i
                >= pWorldData->realVectors[3].getY(); i -= 40)
        {
            //            cout << "<i>" << i << "</i>\n";
            float TShootValue;
            tempPoint.y = i;
            myShoot.setBallPos(tempPoint);
            myShoot.pointSelector();
            TShootValue = myShoot.shootLastValue();

            if (TShootValue > shootValue)
            {
                shootValue = TShootValue;
                selectedPoint = tempPoint;
                if (shootValue == 100)
                {
                    found = true;
                    break;
                }
            }
        }
        //scale selection
        Point defultPoint = selectedPoint;
        for (int j = 40; j > 10; j = j / 2)
        {
            if (found == false)
            {
                for (int i = defultPoint.getY() + j; i
                        >= defultPoint.getY() - j; i -= j / 2)
                {
                    cout << "<j>" << i << "</j>\n";
                    float TShootValue;
                    tempPoint.y = i;
                    myShoot.setBallPos(tempPoint);
                    myShoot.pointSelector();
                    TShootValue = myShoot.shootLastValue();

                    if (TShootValue > shootValue)
                    {
                        shootValue = TShootValue;
                        selectedPoint = tempPoint;
                        if (shootValue == 100)
                        {
                            found = true;
                            break;
                        }
                    }
                }
            }
            defultPoint = selectedPoint;
        }
        cout << "<Shoot Value>" << shootValue << "</Shoot Value>\n";
        cout << "<finished>Shoot check</finished>\n";
        avp[0] = selectedPoint;
        cout << "<avp[0]>" << avp[0] << "</avp[0]>\n";
        found = false;

        if (offencer_num == 2)
        {
            found = false;
            PassValue = 0;
            //whole check
            //            cout << ".....................<lunched 2>Shoot check</lunched 2>.................\n";
            for (int i = pWorldData->realVectors[13].getY(); i
                    >= pWorldData->realVectors[3].getY(); i -= 40)
            {
                //                cout << "<m>" << i << "</m>\n";
                if (i == avp[0].getY())
                {
                    //                    cout << "continuing......\n";
                    continue;
                }
                tempPoint.y = i;
                if (tempPoint.getY() != avp[0].getY())
                {
                    float TShootValue; //value
                    float TPassValue;
                    myShoot.setBallPos(tempPoint);
                    myShoot.pointSelector();
                    TShootValue = myShoot.shootLastValue();
                    TPassValue = myPass.getLineValue(tempPoint, avp[0]);

                    if (PassValue + shootValue < TPassValue + TShootValue)
                    {
                        selectedPoint = tempPoint;
                        PassValue = TPassValue;
                        shootValue = TShootValue;
                    }

                    //                    cout << "<VALUE>" << PassValue + shootValue << "</VALUE>\n";
                    //                    cout << "<Pass Value>" << PassValue << "</Pass Value>\n";
                    //                    cout << "<TempPass Value>" << TPassValue << "</TempPass Value>\n";
                    //                    cout << "<shoot Value>" << shootValue << "</shoot Value>\n";
                    //                    cout << "<TempShoot Value>" << TShootValue << "</TempShoot Value>\n";

                    if (PassValue + shootValue == 200)
                    {
                        found = true;
                        break;
                    }
                }
            }
            //selection check
            //            cout << "................<lunching>2M2</lunching>...........\n";
            defultPoint = selectedPoint;
            for (int j = 40; j > 10; j = j / 2)
            {
                //                cout << "<J>" << j << "</J>\n";
                if (found == false)
                {
                    for (int i = defultPoint.getY() + j; i
                            >= defultPoint.getY() - j; i -= j / 2)
                    {
                        //                        cout << "<2m2>" << i << "</2m2>\n";
                        if (i == avp[0].getY())
                        {
                            //                            cout << "continuing......\n";
                            continue;
                        }
                        tempPoint.y = i;
                        if (tempPoint.getY() != avp[0].getY())
                        {
                            float TShootValue; //value
                            float TPassValue;
                            myShoot.setBallPos(tempPoint);
                            myShoot.pointSelector();
                            TShootValue = myShoot.shootLastValue();
                            TPassValue = myPass.getLineValue(tempPoint, avp[0]);
                            if (PassValue + shootValue < TPassValue + TShootValue)
                            {
                                selectedPoint = tempPoint;
                                PassValue = TPassValue;
                                shootValue = TShootValue;
                            }


                            //                            cout << "<VALUE>" << PassValue + shootValue << "</VALUE>\n";
                            //                            cout << "<tempPoint>" << tempPoint << "<tempPoint>\n";
                            //                            cout << "<Pass Value>" << PassValue << "</Pass Value>\n";
                            //                            cout << "<shoot Value>" << shootValue << "</shoot Value>\n";

                            if (PassValue + shootValue == 200)
                            {
                                found = true;
                                break;
                            }
                        }

                    }
                }
                defultPoint = selectedPoint;
            }

            avp[1] = selectedPoint;
            //            cout << "<avp[1]>" << avp[1] << "</avp[1]>\n";
            //            cout << ".............................<finished 2>Shoot check</finished 2>.............................................\n";
            found = false;

        }
    }









    else if (offencer_num == 3)
    {
        //        cout << ".....................<lunched>offencer_num == 3</lunched>.................................\n";
        Point oppPos1; //position of opponent 1
        Point oppPos2; //position of opponent 2
        Point tempPoint;
        int tempId;
        double temp;
        Line visionLine;
        Pass visionL(pWorldData, pBasicPlayer);
        //        cout << "...............After declerations....................\n";
        //int down; //defines which robot is in the lowwer part of the field (higher y)

        if (myOpp.uidnumbers != 0)
            oppPos1 = pWorldData->oppPositions[myOpp.uidList[0]];
        if (myOpp.uidnumbers >= 2)
            oppPos2 = pWorldData->oppPositions[myOpp.uidList[1]];



        //        cout << "<1_OFFENCER>\n";
        if (myOpp.uidnumbers == 2)
        {
            //            cout << "<OppsFound>2</OppsFound>\n";
            oppPos2 = pWorldData->oppPositions[myOpp.uidList[1]];

            /************************************************/
            /***********************FIRST OFFENCER*********************/
            if (area == 'l' || area == 'L')
            {
                if (oppPos1.getX() > oppPos2.getX())
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos1.getX() + 30;
                }
                else
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos2.getX() + 30;
                }

            }
            else if (area == 'r' || area == 'R')
            {
                if (oppPos1.getX() > oppPos2.getX())
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos2.getX() - 30;
                }
                else
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos1.getX() - 30;
                }

            }
            //            cout << "<X>" << tempPoint.getX() << "</X>\n";
        }
        else if (myOpp.uidnumbers == 1)
        {
            if (area == 'l' || area == 'L')
                tempPoint.x = oppPos1.getX() + 30;
            else if (area == 'r' || area == 'R')
                tempPoint.x = oppPos1.getX() - 30;

            //            cout << "<X>" << tempPoint.getX() << "</X>\n";
        }
        else if (myOpp.uidnumbers == 0)
        {
            /* if (area == 'l' || area == 'L')
                 tempPoint.x = pWorldData->realVectors[13].getX() + 150; //150 addade tajrobi ast ke byada ba tajrobe bedast byad
             else if (area == 'r' || area == 'R')
                 tempPoint.x = pWorldData->realVectors[17].getX() - 150;
             cout << "<X>" << tempPoint.getX() << "</X>\n";*/
            tempPoint.x = initializeX();
        }

        if (myOpp.uidnumbers == 2)
        {
            //az nazar e y beyne 2 ta opp bashad
            tempPoint.y = (oppPos1.getY() + oppPos2.getY()) / 2;
            //            cout << "<Y>" << tempPoint.getY() << "</Y>\n";
        }
        else if (myOpp.uidnumbers == 1)
            tempPoint.y = oppPos1.getY();
        else if (myOpp.uidnumbers == 0)
        {
            myShoot.setReachableDist(400);
            float shootValue = 0;
            Point selectedPoint;
            //45 addad e tajrobi ast
            for (int i = pWorldData->realVectors[7].getY() + 45; i
                    <= pWorldData->realVectors[77].getY() - 45; i++)
            {
                tempPoint.y = i;

                myShoot.setBallPos(tempPoint);
                myShoot.pointSelector();

                float TShootValue;
                TShootValue = myShoot.shootLastValue();

                if (TShootValue > shootValue)
                    //                    cout << "<lunched>for</lunched>\n";
                {
                    shootValue = TShootValue;
                    selectedPoint = tempPoint;
                }
            }
            tempPoint.y = selectedPoint.getY();
        }
        avp[0] = tempPoint;
        //        cout << "<finalPOINT>" << avp[0] << "</finalPOINT>\n";
        //        cout << "</1_OFFENCER>\n";


        /************************************************/
        /***********************SECOND OFFENCER*********************/
        //        cout << "<2_OFFENCER>\n";
        //tempPoint.x bayad yekam nazdiktar az opponent bashad

        //XXXXXXXXXXXXXXXxxxxxxxxxxxxxxxxxxxxxxxXXXXXXXXXXXXXXXXXXXXXXX
        //XXXXXXXXXXXXXXXxxxxxxxxxxxxxxxxxxxxxxxXXXXXXXXXXXXXXXXXXXXXXX
        if (myOpp.uidnumbers == 2)
        {
            if (area == 'l' || area == 'L')
            {

                if (oppPos1.getX() > oppPos2.getX())
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos2.getX() - 30;
                }
                else
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos1.getX() - 30;
                }


            }
            else if (area == 'r' || area == 'R')
            {
                if (oppPos1.getX() > oppPos2.getX())
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos1.getX() + 30;
                }
                else
                {
                    //yani az dortarin opp defencer yekhordefasle begir va beist
                    tempPoint.x = oppPos2.getX() + 30;
                }
            }
        }
        else if (myOpp.uidnumbers == 1)
        {
            if (area == 'l' || area == 'L')
                tempPoint.x = oppPos1.getX() - 30;
            else if (area == 'r' || area == 'R')
                tempPoint.x = oppPos1.getX() + 30;
        }
        else if (myOpp.uidnumbers == 0)
        {

            if (area == 'l' || area == 'L')
                tempPoint.x = avp[0].getX() - 70;
            else if (area == 'r' || area == 'R')
                tempPoint.x = avp[0].getX() + 70;
        }

        //x tempPoint nabayad bishtar az dore zamin birron bezanad :
        if (tempPoint.x < pWorldData->realVectors[11].getX() + 20)
        {
            tempPoint.x = pWorldData->realVectors[11].getX() + 20;
        }
        else if (tempPoint.x > pWorldData->realVectors[15].getX() - 20)
        {
            tempPoint.x = pWorldData->realVectors[15].getX() - 20;
        }

        //YYYYYYYYYYYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYY
        //YYYYYYYYYYYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYY
        //YYYYYYYYYYYYYYYYYYyyyyyyyyyyyyyyyyyyyyyyyyYYYYYYYYYYYYYYYYYYYYYYY
        /***************************************/
        //peyda kardan e paiin tarin (y) robot
        if (myOpp.uidnumbers == 2)
        {
            if (oppPos1.getY() > oppPos2.getY())
            {
                temp = oppPos2.getY();
                //down = 2;
            }
            else
            {
                temp = oppPos1.getY();
                //down = 1;
            }
        }
        else if (myOpp.uidnumbers == 1)
        {
            temp = oppPos1.getY();
        }

        double value = 0;
        double selectedY;
        // for() az bad az opp defencer ta ja i ke visionline e motma'en ijad she .
        //bar asase fasele az opponent defencer va goaler va pole darvaze y e offenser ro tain mikonim :
        if (myOpp.uidnumbers == 2)
        {
            for (int i = temp; i <= temp + 100; i++)
            {
                double faseleAzPole;
                double lineValue; // temprory line value for pass line value

                //visionLine.setByPoints(avp[0], tempPoint);
                tempPoint.y = i;
                lineValue = visionL.getLineValue(avp[0], tempPoint);
                lineValue -= pWorldData->oppPositions[
                        myOpp.goaler].getDistance(tempPoint) * 6;

                if (area == 'l' || area == 'L') //fasele az pole e darvaze baraye in ke fasele az darvaze check beshe
                    faseleAzPole = pWorldData->realVectors[3].getDistance(
                        tempPoint);
                    //= pWorldData->basedCenter(pWorldData->mrBottomLeftPole()).getDistance(tempPoint);
                else if (area == 'r' || area == 'R')
                    faseleAzPole = pWorldData->realVectors[7].getDistance(
                        tempPoint);
                //= pWorldData->basedCenter(pWorldData->mrBottomRightPole()).getDistance(tempPoint);
                lineValue -= faseleAzPole * 4;

                if (lineValue > value)
                {
                    value = lineValue;
                    selectedY = i;
                }
            }
        }
        else if (myOpp.uidnumbers == 1)
        {
            for (int i = temp; i <= temp + 100; i++)
            {
                double faseleAzPole;
                double lineValue; // temprory line value for pass line value

                //visionLine.setByPoints(avp[0], tempPoint);
                tempPoint.y = i;
                lineValue = visionL.getLineValue(avp[0], tempPoint);
                lineValue -= pWorldData->oppPositions[
                        myOpp.goaler].getDistance(tempPoint) * 6;

                if (area == 'l' || area == 'L') //fasele az pole e darvaze baraye in ke fasele az darvaze check beshe
                    faseleAzPole = pWorldData->realVectors[3].getDistance(
                        tempPoint);
                    //= pWorldData->basedCenter(pWorldData->mrBottomLeftPole()).getDistance(tempPoint);
                else if (area == 'r' || area == 'R')
                    faseleAzPole = pWorldData->realVectors[7].getDistance(
                        tempPoint);
                //= pWorldData->basedCenter(pWorldData->mrBottomRightPole()).getDistance(tempPoint);
                lineValue -= faseleAzPole * 4;

                if (lineValue > value)
                {
                    value = lineValue;
                    selectedY = i;
                }
            }
        }
        else if (myOpp.uidnumbers == 0)
        {
            selectedY = avp[0].getY() + 75;
        }

        tempPoint.y = selectedY;


        avp[1] = tempPoint;

        cout << "<finalPOINT>" << avp[1] << "</finalPOINT>\n";
        cout << "</2_OFFENCER>\n";

        /************************************************/
        /***********************THIRD OFFENCER*********************/

        /**************************************************/
        cout << "<3_OFFENCER>\n";
        //bejaye if haye tooye second player az oon ja ke down va temp set shodand

        if (oppPos1.getY() > oppPos2.getY())
        {
            temp = oppPos1.getY();
        }
        else
        {
            temp = oppPos2.getY();
        }

        tempPoint.y = temp;

        // for() az bad az opp defencer ta ja i ke visionline e motma'en ijad she .
        //bar asase fasele az opponent defencer va goaler va pole darvaze y e offenser ro tain mikonim :
        tempPoint.y = temp;
        //			double value = 0;
        value = 0;
        //			double selectedY;
        // for() az bad az opp defencer ta ja i ke visionline e motma'en ijad she .
        //bar asase fasele az opponent defencer va goaler va pole darvaze y e offenser ro tain mikonim :

        /***************************************/
        //peyda kardan e paiin tarin (y) robot
        if (myOpp.uidnumbers == 2)
        {
            if (oppPos1.getY() > oppPos2.getY())
            {
                temp = oppPos2.getY();
                //down = 2;
            }
            else
            {
                temp = oppPos1.getY();
                //down = 1;
            }
        }
        else if (myOpp.uidnumbers == 1)
        {
            temp = oppPos1.getY();
        }

        //		double value = 0;
        //		double selectedY;
        // for() az bad az opp defencer ta ja i ke visionline e motma'en ijad she .
        //bar asase fasele az opponent defencer va goaler va pole darvaze y e offenser ro tain mikonim :
        if (myOpp.uidnumbers == 2)
        {
            for (int i = temp; i >= temp - 100; i--)
            {

                double faseleAzPole;
                double lineValue; // temprory line value for pass line value

                //visionLine.setByPoints(avp[0], tempPoint);
                tempPoint.y = i;
                if (avp[1] != i)
                {
                    lineValue = visionL.getLineValue(avp[0], tempPoint);
                    lineValue -= pWorldData->oppPositions[myOpp.goaler].getDistance(tempPoint) * 6;

                    if (area == 'l' || area == 'L') //fasele az pole e darvaze baraye in ke fasele az darvaze check beshe
                        faseleAzPole = pWorldData->realVectors[3].getDistance(
                            tempPoint);
                        //= pWorldData->basedCenter(pWorldData->mrBottomLeftPole()).getDistance(tempPoint);
                    else if (area == 'r' || area == 'R')
                        faseleAzPole = pWorldData->realVectors[7].getDistance(
                            tempPoint);
                    //= pWorldData->basedCenter(pWorldData->mrBottomRightPole()).getDistance(tempPoint);
                    lineValue -= faseleAzPole * 4;

                    if (lineValue > value)
                    {
                        value = lineValue;
                        selectedY = i;
                    }
                }
            }
        }
        else if (myOpp.uidnumbers == 1)
        {
            for (int i = temp; i >= temp - 100; i--)
            {
                double faseleAzPole;
                double lineValue; // temprory line value for pass line value

                //visionLine.setByPoints(avp[0], tempPoint);
                tempPoint.y = i;
                lineValue = visionL.getLineValue(avp[0], tempPoint);
                lineValue -= pWorldData->oppPositions[myOpp.goaler].getDistance(tempPoint) * 6;

                if (area == 'l' || area == 'L') //fasele az pole e darvaze baraye in ke fasele az darvaze check beshe
                    faseleAzPole = pWorldData->realVectors[3].getDistance(
                        tempPoint);
                    //= pWorldData->basedCenter(pWorldData->mrBottomLeftPole()).getDistance(tempPoint);
                else if (area == 'r' || area == 'R')
                    faseleAzPole = pWorldData->realVectors[7].getDistance(
                        tempPoint);
                //= pWorldData->basedCenter(pWorldData->mrBottomRightPole()).getDistance(tempPoint);
                lineValue -= faseleAzPole * 4;

                if (lineValue > value)
                {
                    value = lineValue;
                    selectedY = i;
                }
            }
        }
        else if (myOpp.uidnumbers == 0)
        {
            selectedY = avp[0].getY() - 75;
        }

        tempPoint.y = selectedY;

        avp[2] = tempPoint;
        //        cout << "<finalPOINT>" << avp[2] << "</finalPOINT>\n";
        //        cout << "</3_OFFENCER>\n";
    }
}