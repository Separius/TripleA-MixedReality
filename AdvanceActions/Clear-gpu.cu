#include "Clear-gpu.h"

gpuClear::gpuClear(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    selectedPoint.x = 0;
    selectedPoint.y = 0;
    clearReachableRadius = 165;
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

gpuClear::gpuClear()
{
    selectedPoint.x = 0;
    selectedPoint.y = 0;
    clearReachableRadius = 165;
}

Point gpuClear::getSelectedPoint()
{
    return selectedPoint ;
}

void gpuClear::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    ballPos = pWorldData->basedCenter(pWorldData->ball());
    pBasicPlayer = newBasicPlayer;
}

void gpuClear::setBallPos(Point newBallPos)
{
    ballPos = newBallPos ;
}

void gpuClear::setReachableDist(int newReachable)
{
    clearReachableRadius = newReachable ;
}

float gpuClear::clearLastValue()
{
    return bestValue ;
}

void gpuClear::gpuClearNow()
{
    float angle = 0;

    Vector clearVector;

    pointSelector();

    clearVector.setByPoints(ballPos, selectedPoint);

    angle = pWorldData->bodyAngle() - clearVector.getDirection();
    pBasicPlayer->kick(angle * M_PI / 180, 1);
}

__global__ void bestPointSelector(int* oppNum , float* oppPosX ,float* oppPosY ,float* ballPosX ,float* ballPosY ,float* values ,float* goalPosTmmX ,float* goalPosOppX ,float* topY ,float* downY ,int reachAble)
{
    int idx = blockIdx.x*blockDim.x + threadIdx.x;
    float temp , minValue = 99 ;
    float xMyPoint , yMyPoint ;

    if (idx < reachAble * 4)
    {
        xMyPoint = int(idx / 2) - reachAble ;
        yMyPoint = sqrt(reachAble * reachAble - xMyPoint * xMyPoint);
        xMyPoint = *ballPosX + xMyPoint ;
        if (idx % 2 == 0)
            yMyPoint = *ballPosY + yMyPoint ;
        else
            yMyPoint = *ballPosY - yMyPoint ;

        Point start , end ;
        start.x = *ballPosX ;
        start.y = *ballPosY ;
	
        Line clearLine;
        Circle startCircle, endCircle;
        Vector tempVector;
        Vector clearVector;

        clearVector.x = xMyPoint - start.x ;
        clearVector.y = yMyPoint - start.y ;
        clearVector.magnitude = hypot(clearVector.x , clearVector.y) ;
        temp = atan2(clearVector.y , clearVector.x) * 180 / M_PI ;
        while (temp < -180) temp += 360 ;
        while (temp > 180) temp -= 360 ;
        clearVector.direction = temp ;

	if (xMyPoint == start.x)
	{
		clearLine.b = 0;
		clearLine.a = 1;
		clearLine.c = 0 - start.x;
		clearLine.m = 0xFFFFFF;
	}
	else
	{
		clearLine.m = (yMyPoint - start.y) / (xMyPoint - start.x);
		clearLine.a = start.y - yMyPoint;
		clearLine.b = xMyPoint - start.x;
		clearLine.c = start.y * (start.x - xMyPoint) + start.x * (yMyPoint - start.y);
	}
	clearLine.direction = atan2(yMyPoint - start.y , xMyPoint - start.x) * 180 / M_PI ;

	tempVector.x = clearVector.x / 16.f ;
	tempVector.y = clearVector.y / 16.f ;
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

        Point endCenter;
	endCenter.x = xMyPoint ;
	endCenter.y = yMyPoint ;
        Point thisOppCenter;

        float d = hypot(start.x - xMyPoint , start.y - yMyPoint);
	float dist2Goal = hypot(*goalPosTmmX - xMyPoint , 0 - yMyPoint);

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

		Line tempLine;
		Point interPoint;

		temp = clearLine.direction + 90 ;
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

		float a1 = clearLine.a, b1 = clearLine.b, c1 = clearLine.c;
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
                        minValue = dist2Line * startZaribOpp + (dist2Goal / 100);
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
                        minValue = dist2Line * endZaribOpp + (dist2Goal / 100);
                    }
                }
            }
            
            if (yMyPoint >= *topY || yMyPoint <= *downY)
	    {
		temp = 80;
		if (temp < minValue)
		{
		    minValue = temp + (dist2Goal / 100) ;
		}
	    }
	    
	    if (*goalPosTmmX < 0) //we are in the left side
	    {
	        if (xMyPoint <= *goalPosTmmX)
	        {
	            if (hypot(*goalPosTmmX - xMyPoint , 0 - yMyPoint) < 100) //then it is a goal to ourteam :D
	            {
	                minValue = 0;
	            }
	            else
	            {
	                temp = 20;
	                if (temp < minValue)
	                {
	                    minValue = temp + (dist2Goal / 100) ;
	                }
	            }
        	}
		else if (xMyPoint >= *goalPosOppX)
		{
		    temp = 60;
		    if (temp < minValue)
		    {
			minValue = temp + (dist2Goal / 100) ;
		    }
		}
	    }
	    else if (*goalPosTmmX > 0) //we are in the right side
	    {
	        if (xMyPoint >= *goalPosTmmX)
	        {
		    if (hypot(*goalPosTmmX - xMyPoint , 0 - yMyPoint) < 100) //then it is a goal to ourteam :D
	            {
	                minValue = 0;
	            }
	            else
	            {
	                temp = 20;
	                if (temp < minValue)
	                {
	                    minValue = temp + (dist2Goal / 100) ;
	                }
	            }
        	}
        	else if (xMyPoint <= *goalPosOppX)
	        {
	            temp = 60;
	            if (temp < minValue)
	            {
	                minValue = temp + (dist2Goal / 100) ;
	            }
        	}
	    }            
            if (minValue == 99)
                minValue += (dist2Goal / 100) ;

            values[idx] = minValue;
        }
    }
}

void gpuClear::pointSelector()
{
    //Host Things :
    cudaPrintfInit();
    int maxValueId = 0 ;
    int temp = pWorldData->mr_number_found_opponents();
    int* oppNum_h = &temp ;
    float* oppPosX_h = (float *)malloc(temp * sizeof(float)) ;
    float* oppPosY_h = (float *)malloc(temp * sizeof(float)) ;
    float* ballPosX_h = (float *)malloc(sizeof(float)) ;
    float* ballPosY_h = (float *)malloc(sizeof(float)) ;
    float* values_h ;
    float* goalPosTmmX_h = (float *)malloc(sizeof(float)) ;
    float* goalPosOppX_h = (float *)malloc(sizeof(float)) ;
    float* topY_h = (float *)malloc(sizeof(float)) ;
    float* downY_h = (float *)malloc(sizeof(float)) ;

    *topY_h = pWorldData->realVectors[10].getY();
    *downY_h = pWorldData->realVectors[0].getY();
    *ballPosX_h = ballPos.getX();
    *ballPosY_h = ballPos.getY();

    *goalPosTmmX_h = pWorldData->basedCenter(pWorldData->mygoal_pole1()).getX();
    *goalPosOppX_h = pWorldData->oppPole1.getX();

    values_h = (float *)malloc( clearReachableRadius * 4 * sizeof(float));

    
    for (int i = 0 ; i < *oppNum_h ; i++)
    {
        oppPosX_h[i] = pWorldData->oppPositions[i].getX() ;
        oppPosY_h[i] = pWorldData->oppPositions[i].getY() ;
    }
    
    for (int i = 0 ; i < 4 * clearReachableRadius ; i++)
        values_h[i] = 0 ;

    //Device Things :
    int* oppNum_d ;
    float* oppPosX_d ;
    float* oppPosY_d ;
    float* ballPosX_d;
    float* ballPosY_d;
    float* values_d;
    float* goalPosTmmX_d;
    float* goalPosOppX_d ;
    float* topY_d ;
    float* downY_d ;

    cudaMalloc((void**)&oppNum_d , sizeof(int));
    cudaMalloc((void**)&oppPosX_d , temp * sizeof(float));
    cudaMalloc((void**)&oppPosY_d , temp * sizeof(float));
    cudaMalloc((void**)&ballPosX_d , sizeof(float));
    cudaMalloc((void**)&ballPosY_d , sizeof(float));
    cudaMalloc((void**)&values_d , clearReachableRadius * 4 * sizeof(float));
    cudaMalloc((void**)&goalPosTmmX_d , sizeof(float));
    cudaMalloc((void**)&goalPosOppX_d , sizeof(float));
    cudaMalloc((void**)&topY_d , sizeof(float));
    cudaMalloc((void**)&downY_d , sizeof(float));


    //Copy From Host to Device :
    cudaMemcpy(oppNum_d , oppNum_h , sizeof(int) , cudaMemcpyHostToDevice);
    cudaMemcpy(oppPosX_d , oppPosX_h , temp * sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(oppPosY_d , oppPosY_h , temp * sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(ballPosX_d , ballPosX_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(ballPosY_d , ballPosY_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(values_d , values_h , sizeof(float) * 4 * clearReachableRadius , cudaMemcpyHostToDevice);
    cudaMemcpy(goalPosTmmX_d , goalPosTmmX_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(goalPosOppX_d , goalPosOppX_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(topY_d , topY_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(downY_d , downY_h , sizeof(float) , cudaMemcpyHostToDevice);


    //Call the Kernel :
    dim3 dimGrid(4);
    dim3 dimBlock(192);
    bestPointSelector<<<dimGrid , dimBlock>>>(oppNum_d , oppPosX_d , oppPosY_d , ballPosX_d , ballPosY_d , values_d , goalPosTmmX_d , goalPosOppX_d , topY_d , downY_d , clearReachableRadius);
    cudaThreadSynchronize();

    //Copy values back to Host :
    cudaMemcpy(values_h , values_d ,clearReachableRadius * 4 * sizeof(float) , cudaMemcpyDeviceToHost);
    //Compare Values and set the Point :
    for(int i = 1 ; i < clearReachableRadius * 4 ; i++)
    {
        if (values_h[i] > values_h[maxValueId])
        {
            maxValueId = i ;
            bestValue = values_h[i] ;
        }
    }
    
    selectedPoint.x = int(maxValueId / 2) - clearReachableRadius ;
    selectedPoint.y = sqrt(clearReachableRadius * clearReachableRadius - selectedPoint.x * selectedPoint.x);
    selectedPoint.x += *ballPosX_h ;
    if (maxValueId % 2 == 0)
        selectedPoint.y += *ballPosY_h ;
    else
        selectedPoint.y = *ballPosY_h - selectedPoint.y ;

    cudaFree(oppNum_d);
    cudaFree(oppPosX_d);
    cudaFree(oppPosY_d);
    cudaFree(ballPosX_d);
    cudaFree(ballPosY_d);
    cudaFree(values_d);
    cudaFree(goalPosTmmX_d);
    cudaFree(goalPosOppX_d);
    cudaFree(topY_d);
    cudaFree(downY_d);

    free(oppPosX_h);
    free(oppPosY_h);
    free(ballPosX_h);
    free(ballPosY_h);
    free(values_h);
    free(goalPosTmmX_h);
    free(goalPosOppX_h);
    free(topY_h);
    free(downY_h);
}