#include <Shoot-gpu.h>

gpuShoot::gpuShoot(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    selectedPoint.x = 0;
    selectedPoint.y = 0;
    whichGoal = 'o'; //t stands for teamGoal and o stands for oppGoal
    callerUid = pWorldData->my_id();
    shootReachableRadius = 165;
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

gpuShoot::gpuShoot()
{
    selectedPoint.x = 0;
    selectedPoint.y = 0;
    whichGoal = 'o'; //t stands for teamGoal and o stands for oppGoal
    shootReachableRadius = 165;
}

Point gpuShoot::getSelectedPoint()
{
    return selectedPoint ;
}
void gpuShoot::goalChooser(char whichOne)
{
    whichGoal = whichOne ;
}

void gpuShoot::gpuKickNow(Point target)
{
    float angle = 0;

    Vector shootVector;

    shootVector.setByPoints(pWorldData->basedCenter(pWorldData->ball()), target);

    angle = pWorldData->bodyAngle() - shootVector.getDirection();
    pBasicPlayer->kick(angle * M_PI / 180, 1);
}

void gpuShoot::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    callerUid = pWorldData->my_id();
    ballPos = pWorldData->basedCenter(pWorldData->ball());
    pBasicPlayer = newBasicPlayer;
}

void gpuShoot::setBallPos(Point newBallPos)
{
    ballPos = newBallPos ;
}

void gpuShoot::setReachableDist(float newReachable)
{
    shootReachableRadius = newReachable ;
}

void gpuShoot::setUid(int givenID)
{
    callerUid = givenID ;
}

float gpuShoot::shootLastValue()
{
    return bestValue ;
}

void gpuShoot::gpuShootNow()
{
    float angle = 0;
    
    Vector shootVector;

    pointSelector();

    shootVector.setByPoints(ballPos, selectedPoint);

    if (selectedPoint.getX() != 0)
    {
        angle = pWorldData->bodyAngle() - shootVector.getDirection();
        pBasicPlayer->kick(angle * M_PI / 180, 1);
    }
    else
    {
        pBasicPlayer->kick(M_PI / 2, 1);
        //we should pass the ball !
    }
}

__global__ void bestPointSelector(int* goalPolDist , int* oppNum , float* oppPosX , float* oppPosY , float* ballPosX , float* ballPosY , float* values , float* goalPos1X , float* goalPos1Y , float* goalPos2X , float* goalPos2Y)
{
    int idx = blockIdx.x*blockDim.x + threadIdx.x;
    int reachAble = 165 ;
    float temp , minValue = 99 ;
    float endY = *goalPos2Y + 15.0 + idx ;
    
    if (endY < (*goalPos1Y - 15.0))
    {
        Point start , end ;
        start.x = *ballPosX ;
        start.y = *ballPosY ;
        end.x = *goalPos1X ;
        end.y = endY ;
        Line shootLine;
        Circle startCircle, endCircle;
        Vector tempVector;
        Vector shootVector;	

        shootVector.x = end.x - start.x ;
        shootVector.y = end.y - start.y ;
        shootVector.magnitude = hypot(shootVector.x , shootVector.y) ;
        temp = atan2(shootVector.y , shootVector.x) * 180 / M_PI ;
        while (temp < -180) temp += 360 ;
        while (temp > 180) temp -= 360 ;
        shootVector.direction = temp ;

	if (end.x == start.x)
	{
		shootLine.b = 0;
		shootLine.a = 1;
		shootLine.c = 0 - start.x;
		shootLine.m = 0xFFFFFF;
	}
	else
	{
		shootLine.m = (end.y - start.y) / (end.x - start.x);
		shootLine.a = start.y - end.y;
		shootLine.b = end.x - start.x;
		shootLine.c = start.y * (start.x - end.x) + start.x * (end.y - start.y);
	}
	shootLine.direction = atan2(end.y - start.y , end.x - start.x) * 180 / M_PI ;

	tempVector.x = shootVector.x / 16.f ;
	tempVector.y = shootVector.y / 16.f ;
	tempVector.magnitude = hypot(tempVector.x, tempVector.y);
        temp = atan2(tempVector.y, tempVector.x) * 180 / M_PI ;
	while (temp < -180) temp += 360 ;
        while (temp > 180) temp -= 360 ;
        tempVector.direction = temp ;

	tempVector.x *= 5 ;
	tempVector.y *= 5 ;
	tempVector.magnitude = hypot(tempVector.x, tempVector.y);
	temp = atan2(tempVector.y, tempVector.x) * 180 / M_PI ;
	while (temp < -180) temp += 360 ;
        while (temp > 180) temp -= 360 ;
        tempVector.direction = temp ;
        
        Point startCenter ;
	startCenter.x = start.x + tempVector.x ;
	startCenter.y = start.y + tempVector.y ;

        Point endCenter = end;
        Point thisOppCenter;

        float d = hypot(start.x - end.x , start.y - end.y);
        
        if (d > reachAble)
            minValue = 0 ;
        else
        {
            float startRadius = 5 * d / 16;
            if (startRadius < 35)
                startRadius = 40 ;
            float endRadius = d / 2;
            if (endRadius < 35)
                endRadius = 40 ;
            float startZaribOpp = 1;
            float endZaribOpp = 0.5;

            startCircle.centeralPoint = startCenter ;
	    startCircle.radius = startRadius ;
	    endCircle.centeralPoint = endCenter ;
	    endCircle.radius = endRadius ;

            for (int i = 0; i < *oppNum; i++)
            {
                thisOppCenter.x = oppPosX[i];
                thisOppCenter.y = oppPosY[i];

                //34.5 = players max kickable area radius
                //this is completly correct if the opponents won't move !

		Line tempLine;
		Point interPoint;

		temp = shootLine.direction + 90 ;
		while (temp < -180) temp += 360 ;
		while (temp > 180) temp -= 360 ;
		
		float cs, sn, n;
		if (((temp + 90) / 180) == (int)((temp + 90) / 180))
		{
			tempLine.b = 0;
			tempLine.a = 1;
			tempLine.c = 0 - thisOppCenter.x;
			tempLine.m = 0xFFFFFF;
		}
		else
		{
			tempLine.m = tan(temp * M_PI/180);
			n = thisOppCenter.y - (tempLine.m * thisOppCenter.x);
			sn = sin(temp * M_PI/180);
			cs = cos(temp * M_PI/180);
			tempLine.a = 0 - sn;
			tempLine.b = cs;
			tempLine.c = 0 - (cs * n);
		}
		tempLine.direction = temp;
		
		float a1 = shootLine.a, b1 = shootLine.b, c1 = shootLine.c;
		float a2 = tempLine.a, b2 = tempLine.b, c2 = tempLine.c;
		interPoint.x = ((b1 * c2) - (b2 * c1)) / ((a1 * b2) - (a2 * b1));
		interPoint.y = ((a2 * c1) - (a1 * c2)) / ((a1 * b2) - (a2 * b1));

		float dist2Line = hypot(thisOppCenter.x - interPoint.x , thisOppCenter.y - interPoint.y) ;
                
		float centerToPointDist = sqrt((pow((startCircle.centeralPoint.x - thisOppCenter.x), 2) + pow((startCircle.centeralPoint.y - thisOppCenter.y), 2)));
		
                if (centerToPointDist < startCircle.radius)
                {
                    if (dist2Line < 40)
                    {
                        minValue = 0;
                        break;
                    }
                    if ((dist2Line * startZaribOpp) < minValue)
                    {
                        minValue = dist2Line * startZaribOpp + (1 - ( d / reachAble));
                    }
                }

                centerToPointDist = sqrt((pow((endCircle.centeralPoint.x - thisOppCenter.x), 2) + pow((endCircle.centeralPoint.y - thisOppCenter.y), 2)));
		if (centerToPointDist < endCircle.radius)
                {
                    if (dist2Line < 60)
                    {
                        minValue = 0;
                        break;
                    }
                    if ((dist2Line * endZaribOpp) < minValue)
                    {
                        minValue = dist2Line * endZaribOpp + (1 - ( d / reachAble));
                    }
                }
            }
            if (minValue == 99)
                minValue += (1 - ( d / reachAble)) ;
            
            values[idx] = minValue;
        }
    }
}

void gpuShoot::pointSelector()
{
    //Host Things :
    int maxValueId = 0 ;
    int *goalPolDist_h = (int *)malloc(sizeof(int)) ;
    int temp = pWorldData->mr_number_found_opponents();
    int* oppNum_h = &temp ;
    float* oppPosX_h ;
    float* oppPosY_h ;
    float* ballPosX_h = (float *)malloc(sizeof(float)) ;
    float* ballPosY_h = (float *)malloc(sizeof(float)) ;
    float* values_h ;
    float* goalPos1X_h = (float *)malloc(sizeof(float)) ;
    float* goalPos1Y_h = (float *)malloc(sizeof(float)) ;
    float* goalPos2X_h = (float *)malloc(sizeof(float)) ;
    float* goalPos2Y_h = (float *)malloc(sizeof(float)) ;

    *ballPosX_h = ballPos.getX();
    *ballPosY_h = ballPos.getY();
    if (whichGoal == 'o')
    {
        oppPosX_h = (float *)malloc(temp * sizeof(float)) ;
        oppPosY_h = (float *)malloc(temp * sizeof(float)) ;
        *goalPos1X_h = pWorldData->oppPole1.getX();
        *goalPos1Y_h = pWorldData->oppPole1.getY();
        *goalPos2X_h = pWorldData->oppPole2.getX();
        *goalPos2Y_h = pWorldData->oppPole2.getY();
    }
    if (whichGoal == 't')
    {
        temp = pWorldData->mr_number_found_teammates() + 1 ;
        oppPosX_h = (float *)malloc(temp * sizeof(float)) ;
        oppPosY_h = (float *)malloc(temp * sizeof(float)) ;
        *goalPos1X_h = pWorldData->basedCenter(pWorldData->mygoal_pole1()).getX();
        *goalPos1Y_h = pWorldData->basedCenter(pWorldData->mygoal_pole1()).getY();
        *goalPos2X_h = pWorldData->basedCenter(pWorldData->mygoal_pole2()).getX();
        *goalPos2Y_h = pWorldData->basedCenter(pWorldData->mygoal_pole2()).getY();
    }
    
    *goalPolDist_h = (int)(*goalPos1Y_h - *goalPos2Y_h) - 30 ;

    values_h = (float *)malloc( *goalPolDist_h * sizeof(float));

    if(whichGoal == 'o')
    {
        for (int i = 0 ; i < *oppNum_h ; i++)
        {
            oppPosX_h[i] = pWorldData->oppPositions[i].getX() ;
            oppPosY_h[i] = pWorldData->oppPositions[i].getY() ;
        }
    }
    if (whichGoal == 't')
    {
        for(int i = 0 ; i < pWorldData->mr_number_found_teammates() ; i++)
        {
            oppPosX_h[i] = pWorldData->tmmPositions[i].getX() ;
            oppPosY_h[i] = pWorldData->tmmPositions[i].getY() ;
        }
    }

    for (int i = 0 ; i < *goalPolDist_h ; i++)
        values_h[i] = 0 ;

    //Device Things :
    int* goalPolDist_d ;
    int* oppNum_d ;
    float* oppPosX_d ;
    float* oppPosY_d ;
    float* ballPosX_d;
    float* ballPosY_d;
    float* values_d;
    float* goalPos1X_d;
    float* goalPos1Y_d ;
    float* goalPos2X_d ;
    float* goalPos2Y_d ;

    cudaMalloc((void**)&goalPolDist_d , sizeof(int));
    cudaMalloc((void**)&oppNum_d , sizeof(int));
    cudaMalloc((void**)&oppPosX_d , temp * sizeof(float));
    cudaMalloc((void**)&oppPosY_d , temp * sizeof(float));
    cudaMalloc((void**)&ballPosX_d , sizeof(float));
    cudaMalloc((void**)&ballPosY_d , sizeof(float));
    cudaMalloc((void**)&values_d , *goalPolDist_h * sizeof(float));
    cudaMalloc((void**)&goalPos1X_d , sizeof(float));
    cudaMalloc((void**)&goalPos1Y_d , sizeof(float));
    cudaMalloc((void**)&goalPos2X_d , sizeof(float));
    cudaMalloc((void**)&goalPos2Y_d , sizeof(float));
    

    //Copy From Host to Device :
    cudaMemcpy(goalPolDist_d , goalPolDist_h , sizeof(int) , cudaMemcpyHostToDevice);
    cudaMemcpy(oppNum_d , oppNum_h , sizeof(int) , cudaMemcpyHostToDevice);
    cudaMemcpy(oppPosX_d , oppPosX_h , temp * sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(oppPosY_d , oppPosY_h , temp * sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(ballPosX_d , ballPosX_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(ballPosY_d , ballPosY_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(values_d , values_h , sizeof(float) * *goalPolDist_h , cudaMemcpyHostToDevice);
    cudaMemcpy(goalPos1X_d , goalPos1X_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(goalPos1Y_d , goalPos1Y_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(goalPos2X_d , goalPos2X_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(goalPos2Y_d , goalPos2Y_h , sizeof(float) , cudaMemcpyHostToDevice);
    

    //Call the Kernel :
    dim3 dimGrid(1);
    dim3 dimBlock(192);
    bestPointSelector<<<dimGrid , dimBlock>>>(goalPolDist_d , oppNum_d , oppPosX_d , oppPosY_d , ballPosX_d , ballPosY_d , values_d , goalPos1X_d , goalPos1Y_d , goalPos2X_d , goalPos2Y_d);
    cudaThreadSynchronize();

    //Copy values back to Host :
    cudaMemcpy(values_h , values_d ,*goalPolDist_h * sizeof(float) , cudaMemcpyDeviceToHost);
    //Compare Values and set the Point :
    for(int i = 1 ; i < *goalPolDist_h ; i++)
    {
        if (values_h[i] > values_h[maxValueId])
        {
            maxValueId = i ;
            bestValue = values_h[i] ;
        }
    }
    if (values_h[maxValueId] == 0)
    {
        //can not shoot ;
        selectedPoint.x = 0 ;
	selectedPoint.y = 0 ;
    }
    else
    {
        selectedPoint.x = pWorldData->oppPole1.getX();
        selectedPoint.y = pWorldData->oppPole2.getY() + 15 + maxValueId ;
    }
    
    cudaFree(goalPolDist_d);
    cudaFree(oppNum_d);
    cudaFree(oppPosX_d);
    cudaFree(oppPosY_d);
    cudaFree(ballPosX_d);
    cudaFree(ballPosY_d);
    cudaFree(values_d);
    cudaFree(goalPos1X_d);
    cudaFree(goalPos1Y_d);
    cudaFree(goalPos2X_d);
    cudaFree(goalPos2Y_d);

    free(goalPolDist_h);
    free(oppPosX_h);
    free(oppPosY_h);
    free(ballPosX_h);
    free(ballPosY_h);
    free(values_h);
    free(goalPos1X_h);
    free(goalPos1Y_h);
    free(goalPos2X_h);
    free(goalPos2Y_h);
}