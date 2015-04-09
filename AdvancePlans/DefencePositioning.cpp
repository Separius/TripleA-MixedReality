#include <DefencePositioning.h>

/* IMPORTANT
 * about uidList[] and uid array:
 * you have to give it an array with 6 blocks
 * and fill it with defencers uid's from [0]
 * IMPORTANT
 */
using namespace std;

DefPos::DefPos()
{
}

DefPos::DefPos(WorldData* newWorldData, BasicPlayer* newBasicPlayer,
        int uids[], int defenersNum, char teamField)
{
    initializeWorld(newWorldData, newBasicPlayer, uids, defenersNum, teamField);

}

void
DefPos::initializeWorld(WorldData* newWorldData, BasicPlayer* newBasicPlayer, int uids[], int defenersNum, char teamField)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;

    //size of the field
    //*/ fieldTool = pWorldData->fieldTool();
    //*/ fieldArz = pWorldData->fieldArz();
    //   cout<<"field tool   "<<fieldTool<<endl;
    // cout<<"field arz    "<<fieldArz<<endl;
    //team e ma dar kodam field ast
    area = teamField;

    //tedade nafarati ke barashoon Point moshakhas kardam
    resultnum = 0;
    //shomare e uid haye defencer ha
    //uid list ye pointer ast

    for (int i = 0; i < defenersNum; i++)
    {
        uidList[i] = uids[i];
        // cout << "uidlist[i] == " << uidList[i] << endl;
    }

    //tedad e defencer ha
    botnum = defenersNum;

    //khali kardan e list e uid haye estefade shode
    for (int i = 0; i < 6; i++)
    {
        usedUids[i] = 99;
    }
    //seting up flags for use
    setflags();

    //Max 7 Min makene momken baraye defencer ha
    //in do baraye taiin e availableTool estefade mishavand
    double minX;
    double maxX;

    if (area == 'l' || area == 'L')
    {
        //*/ minX = pWorldData->basedCenter(pWorldData->mrTopLeftSmallArea()).getX();
        //*/ maxX = pWorldData->basedCenter(pWorldData->mrMiddleField()).getX();
        minX = realVectors[14].getX();
        maxX = realVectors[9].getX();
    }
    else if (area == 'r' || area == 'R')
    {
        //*/ maxX = pWorldData->basedCenter(pWorldData->mrTopRightSmallArea()).getX();
        //*/ minX = pWorldData->basedCenter(pWorldData->mrMiddleField()).getX();
        maxX = realVectors[18].getX();
        minX = realVectors[9].getX();
    }
    aviliableTool = maxX - minX;
    //    cout<<"avaailable tool : "<<aviliableTool<<endl;
    //based center e ball moshkel dare

    //tabdil e origin az markaz e zamin be gooshe ye bala chap
    double tempX;
    //*/ tempX = pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).getX();
    ballX = pWorldData->basedCenter(pWorldData->ball()).getX();
    tempX = realVectors[11].getX();
    ballX = ballX - tempX;
    //agar biroon e zamin bood haman 0 dar nazar migirimash
    if (ballX < 0)
    {
        ballX = 0;
    }
    // cout<<"ballx top left(0,0):  "<<ballX<<endl;


    double tempY; //*/= pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).getY();
    tempY = realVectors[11].getY();
    ballY = pWorldData->basedCenter(pWorldData->ball()).getY();
    //    cout<<"<ball>\n"<<"<y>\n"<<"<basedcenter>"<<ballY<<"</basedcenter>\n";
    ballY = tempY - ballY;
    //agar biroon e zamin bood haman 0 dar nazar migirimash
    if (ballY < 0)
    {
        ballY = 0;
    }
    //cout<<"ball pos : "<<ballX<<" , "<<ballY<<endl;
    //    cout<<"ball "<<ballX<<" , "<<ballY<<endl;
    // cout<<"<topleft>"<<ballY<<"<topleft>\n"<<"</y>\n"<<"</ball>\n";

    if (pWorldData->time() > 0.0)
    {
        for (int i = 0; i < 4; i++)
            prePoint[i] = (Point) (1001, 1001);
    }
}

void DefPos::setflags()
{
    /* DO NOT CALL THIS METHOD :: PRIVATE */

    //in ha ye vector az 0,0 be flag ha hastand.
    fieldTool = 781.81; //defining width of field
    fieldArz = 487.475; //defining height of field

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

/* tain e meakan e 'X' khate defence
 * -------------------------------
 * agar samte chap e zamin boodim
 * az mrLeftSmallArea ta miidle of the field
 * agar ballX == 0 bashad khate defence bayad rooye small area bashad
 * agar ballX == fieldTool bashad khate Defence bayad rooye khate middle bashad
 * -----------------------------------
 * agar samte rast boodim az middle of the field
 * ta mrRightAmallArea
 */
void DefPos::decideXLine()
{

    /*
     * inja ball based center nist  ta ba field tool khonsa shavad
     */

    if (area == 'l' || area == 'L')
    {
        xLine = (aviliableTool / fieldTool) * ballX + realVectors[14].getX();
        xLine -= 100;
        //*/ xLine = (aviliableTool / fieldTool) * ballX + pWorldData->basedCenter(pWorldData->mrTopLeftSmallArea()).getX();
    }
    else if (area == 'r' || area == 'R')
    {
        xLine = (aviliableTool / fieldTool) * ballX + realVectors[9].getX();
        xLine += 100;
        //*/ xLine = (aviliableTool / fieldTool) * ballX + pWorldData->basedCenter(pWorldData->mrMiddleField()).getX();
    }
    /*cout << "middlefield: " << pWorldData->mrMiddleField().length << endl;
     cout << "middelefield.getx  : " << pWorldData->basedCenter(
     pWorldData->mrMiddleField()).getX() << endl;
     cout << "field tool:" << fieldTool << endl;
     cout << "avtool : " << aviliableTool << endl;
     cout << "ball: " << ballX << endl;*/
    //cout << "Xline : " << xLine << endl;

}

// tool e defenceLine ro taiin mikonea>:

void DefPos::decideLength()
{
    botSize = 15;
    Point temp1;
    Point temp2;

    double minTool;
    double maxTool;

    /* tavajoh::
     * in temp1 & temp2 be 'area' bastegi nadarad
     * chon faghat tool e darvaze ro bedast miavarim */
    //*/ temp1 = pWorldData->basedCenter(pWorldData->mrTopLeftPole());
    //*/ temp2 = pWorldData->basedCenter(pWorldData->mrBottomLeftPole());

    temp1 = (Point) realVectors[13];
    temp2 = (Point) realVectors[3];

    //min tool baraye vaghti ast ke x e DefenceLine rooye kamtarin meghdar (nazdik be darvaze) bashad
    minTool = temp1.getDistance(temp2);

    //*/ temp1 = pWorldData->basedCenter(pWorldData->mrTopCenter());
    //*/ temp2 = pWorldData->basedCenter(pWorldData->mrBottomCenter());
    temp1 = (Point) realVectors[10];
    temp2 = (Point) realVectors[0];

    //yani dar vasat e zamin Max e  length e defenceLine kamtar az fieldArz abshad
    //addad e 50 ke az khodam daravarodam hamin ro neshoon mideh
    maxTool = temp1.getDistance(temp2) - 100;
    if (area == 'l' || area == 'L')
    {

        availableArz = ((maxTool - minTool) / fieldTool) * ballX + minTool;
    }
    else if (area == 'r' || area == 'R')
    {
        availableArz = maxTool - ((maxTool - minTool) / fieldTool) * ballX;
    }
    //agar dar availablearz e taiin shode jaye kefi baraye istadan e robot ha nabashad
    //be andaze ye kafi arz ro zia kon
    if (availableArz < botnum * botSize)
        availableArz += (botnum * botSize) - availableArz;
    availableArz -= 100;

    //cout << "availableArz : " << availableArz << endl;

}

void DefPos::decideYLine()
{
    /* agar ballY == 0  bashad y1Line ke makane balaye line ro taiin mikonad
     * bayad barabar e 10 bashad
     * albate '10 ro az khodam dar avordam va
     * karbordesh in ast ke defencer ha biroon e zamin naravand
     * ----------------------------------------------------------------------
     * agar ballY == fieldArz bashad y2Line bayad 10 (10 ro az khodam dar avordam)
     * vahed (px) balatar az fieldArz bashad ta defencer biroon e zamin  naravd
     * dar naije y1Line bayad barabae e meghdar e zir bashad
     */

    //y1 bar asas e top left ast (0,0)
    y1Line = (((fieldArz - (availableArz + 20)) / fieldArz) * ballY) + 10;

    //cout << "<yline>" << y1Line<<"</yline>"<< endl;

}

void DefPos::definePoints()
{
    //cout << "<lunched>definepoints</lunched>" << endl;
    // cout << "Uidlist start of  definePoints0:     " << uidList[0] << endl;
    // cout << "Uidlist start of  definePoints1:     " << uidList[1] << endl;
    // cout << "***************************************************" << endl;
    //int uid;
    usednum = 0;
    // cout << "in define Points used num (bayad =0)==" << usednum << endl;
    //  cout << "botnum" << botnum << endl;
    //   cout<<"<availaaleArz>"<<availableArz<<"</availaaleArz>"<<endl;
    for (int i = 0; i < botnum; i++)
    {
        Point temp;
        temp.x = xLine;
        //bar asas e top left corner (0,0)
        temp.y = ((availableArz / (botnum - 1)) * i) + y1Line;
        /* cout<<"<Defenser>"<<endl;
         cout<<"<i> "<<i<<"</i>"<<endl;
         cout<<"<id>"<<uidList[i]<<"</id>"<<endl;
         cout<<"<y>"<<temp.y<<"</y>"<<endl;
         cout<<"</Defenser>"<<endl;*/
        //tabdil e noghte az mokhtasat bar asas e goosheye bala samte chap be real center of the field
        //tempX va tempY mokhtasat e reaal center bar asas e gooshye chap va bala hastand
        //        double centerX;
        double centerY;
        //        centerX = pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).x * -1;
        //*/ centerY = pWorldData->basedCenter(pWorldData->mrTopLeftCorner()).y;
        centerY = realVectors[11].getY();
        //        temp.x = temp.getX() - centerX;
        temp.y = centerY - temp.getY();

        avp[i] = temp;

        //  cout << "lunching nearestplayer(pos)" << endl;
        // cout << "temp is:       " << temp << endl;
        // uid = nearestPlayer(temp);
        // cout << "nearest player ID : " << uid << " Player POS" << temp << endl;
        //goToPoint(uid, temp);

    }
    //    for(int i=0; i<botnum;i++)
    //    {
    //        cout<<"<i> "<<i<<"</i>"<<endl;
    //        cout<<"avp[i] : "<<avp[i]<<endl;
    //    }

    computeTotal();
    //Ptp *DATA;
    Ptp PosId;
    //DATA = (Ptp*) malloc (sizeof(Ptp) * totalDATA);
    Ptp DATA[6];
    fill(DATA);
    computePtpSum(DATA);
    selectIds(DATA, &PosId);

    for (int i = 0; i < botnum; i++)
    {
        cout << "<pos before>" << PosId.pos[i] << "</pos before>" << endl;
    }

    noiseCancell(&PosId);
    for (int i = 0; i < botnum; i++)
    {
        cout << "<pos after>" << PosId.pos[i] << "</pos after before>" << endl;
        goToPoint(uidList[i], PosId.pos[i]);
    }
    //     noiseCancell();


}

void DefPos::check()
{
    // vector_t m;
    cout << "<check>" << endl;
    cout << "<lunched>" << "check" << "</lunched>" << endl;
    cout << "<myid>" << pWorldData->my_id() << "</myid>" << endl;
    cout << "<uid>\n" << "<0>" << uidList[0] << "</0>\n" << "<1>" << uidList[1]
            << "</1>\n" << "</uid>" << endl;
    for (int j = 0; j < botnum; j++)
    {
        cout << "<j>" << j << "</j>" << endl;
        //  m = pWorldData->teammate(uidList[1]);
        cout << "<m type = 'angle'> " << pWorldData->teammate(uidList[j]).angle
                << " </m>" << endl;
        cout << "<m type = 'lenght'> "
                << pWorldData->teammate(uidList[j]).length << " </m>" << endl;
    }
    cout << "/<check>" << endl;

}

void DefPos::goToPoint(int pId, Point pos)
{
    resultId[resultnum] = pId;
    resultPos[resultnum] = pos;
    resultnum++;
    // cout<<"result num : "<<resultnum -1<<endl;
    //for(int i=0; i<botnum;i++)
    //{
    //  cout<<"<i> "<<i<<"</i>"<<endl;
    // cout<<"point[i] : "<<resultPos[resultnum -1]<<endl;
    //}
    //IMPORTANT
    //must return it to pworlddata
    //IMPORTANT
}

void DefPos::defencePosNOW()
{
    decideXLine();
    decideLength();
    decideYLine();
    definePoints();
}

void DefPos::fill(Ptp * DATA)
{
    //cout<<"<lunched>"<<"fill"<<"</lunched>"<<endl;
    if (botnum == 1)
    {
        DATA[0].pos[0] = avp[0];
    }
    else if (botnum == 2)
    {
        for (int i = 0; i <= 1; i++)
        {
            DATA[i].pos[0] = avp[0 + i];
            DATA[i].pos[1] = avp[1 - i];
        }
    }
    else if (botnum == 3)
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

void DefPos::computePtpSum(Ptp * DATA)
{
    //cout<<"<lunched>"<<"computePtpSum"<<"</lunched>"<<endl;
    for (int i = 0; i < totalDATA; i++)
    {
        DATA[i].sum = 0;
        for (int j = 0; j < botnum; j++)
        {
            DATA[i].sum += pWorldData->basedCenter(pWorldData->teammate(
                    uidList[j])).getDistance(DATA[i].pos[j]);
        }
    }
}

void DefPos::selectIds(Ptp* DATA, Ptp* resultPtp)
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

void DefPos::computeTotal()
{
    //cout<<"<lunched>"<<"computeTotal"<<"</lunched>"<<endl;
    if (botnum == 1)
        totalDATA = 1;
    else if (botnum == 2)
        totalDATA = 2;
    else if (botnum == 3)
        totalDATA = 6;

}

void DefPos::noiseCancell(Ptp * DATA)
{
    cout << "******************noid cancell lunched ***********" << endl;
    //	cout << "resultsum = " << resultnum << endl;
    for (int i = 0; i < botnum; i++)
    {
        cout << "<Points>\n<input>" << endl;
        cout << "<i>" << i << "</i>" << endl;
        cout << "resultPos[i] : " << DATA->pos[i] << endl;
        cout << "prePoint[i] : " << prePoint[i] << endl;
        cout << "</input>\n</Points>\n";
        if (prePoint[i].x == 1001 && prePoint[i].y == 1001)
        {
            cout << "************NOTSET***************" << endl;
            prePoint[i] = DATA->pos[i];
            //			cout << "<1000>lunched</1000>";
        }
        else if (fabs(DATA->pos[i].x - prePoint[i].x) < 2.0 && fabs(DATA->pos[i].y - prePoint[i].y) < 2.0)
        {
            cout << "************cancelling noise************" << endl;
            DATA->pos[i] = prePoint[i];
        }
        else
        {
            cout << "******************updating cash ************************" << endl;
            prePoint[i] = DATA->pos[i];
        }
        //		cout << "<Points>\n<output>" << endl;
        //		cout << "resultPos[i] : " << resultPos[i] << endl;
        //		cout << "</output>\n</Points>" << endl;

    }
}

