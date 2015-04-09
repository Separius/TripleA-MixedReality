#include <cuPrintf.cu>
#include "Pass-gpu.h"

gpuPass::gpuPass(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerId = pWorldData->my_id();
    passReachableRadius = 165;
    ballPos = pWorldData->basedCenter(pWorldData->ball());
    selectedId = -1 ;
}

gpuPass::gpuPass()
{
    selectedId = -1 ;
    passReachableRadius = 165;
}

void gpuPass::initializeData(WorldData* newWorldData, BasicPlayer* newBasicPlayer)
{
    pWorldData = newWorldData;
    pBasicPlayer = newBasicPlayer;
    callerId = pWorldData->my_id();
    ballPos = pWorldData->basedCenter(pWorldData->ball());
}

__global__ void bestTeammateFinder(float* oppPosX ,float* oppPosY ,int* oppNum ,int* caller ,float* ourPosX ,float* ourPosY ,int* ourNum ,float* ballX ,float* ballY ,int* ourIds ,float* ourGoalX ,float* values)
{
    int idx = blockIdx.x*blockDim.x + threadIdx.x;
    Line AB ;
}

Point gpuPass::getSelectedPoint()
{
    return selectedPoint ;
}

int gpuPass::getSelectedId()
{
    return selectedId ;
}

void gpuPass::gpuPassNow()
{
    float angle = 0;

    Vector shootVector;

    bestTeammateSelector();

    shootVector.setByPoints(ballPos, selectedPoint);

    if (selectedId != -1)
    {
        angle = pWorldData->bodyAngle() - shootVector.getDirection();
        pBasicPlayer->kick(angle * M_PI / 180, 1);
    }
    else
    {
        pBasicPlayer->kick(M_PI / 2, 1);
        //we should call Clear
    }
}

void gpuPass::kickIt()
{
    float angle = 0;

    Vector shootVector;

    shootVector.setByPoints(ballPos, selectedPoint);

    angle = pWorldData->bodyAngle() - shootVector.getDirection();
    pBasicPlayer->kick(angle * M_PI / 180, 1);
}

float gpuPass::passLastValue()
{
    return bestValue ;
}

void gpuPass::setBallPos(Point newBallPos)
{
    ballPos = newBallPos ;
}

void gpuPass::setReachableDist(float newReachable)
{
    passReachableRadius = newReachable ;
}

void gpuPass::setUid(int newId)
{
    callerId = newId ;
}

void gpuPass::bestTeammateSelector()
{
    //Host define & init:
    std::vector<int> tmmIDs = pWorldData->mr_found_teammates() ;
    std::vector<int> oppIDs = pWorldData->mr_found_opponents() ;
    float* oppPosX_h ;
    float* oppPosY_h ;
    int* oppNum_h = (int *)malloc(sizeof(int));
    int* caller_h = (int *)malloc(sizeof(int)) ;
    //int* myId_h = (int *)malloc(sizeof(int));
    float* ourPosX_h ;
    float* ourPosY_h ;
    int* ourNum_h = (int *)malloc(sizeof(int)) ;
    float* ballX_h = (float *)malloc(sizeof(float)) ;
    float* ballY_h = (float *)malloc(sizeof(float)) ;
    int* ourIds_h ;
    float* ourGoalX_h = (float *)malloc(sizeof(float)) ;
    float* values_h = (float *)malloc(sizeof(float) * 5 * 128) ;
    int maxValueId = -1 ;
    
    *oppNum_h = oppIDs.size() ;
    *ourNum_h = tmmIDs.size() + 1 ;
    oppPosX_h = (float *)malloc(sizeof(float) * *oppNum_h);
    oppPosY_h = (float *)malloc(sizeof(float) * *oppNum_h);
    ourPosX_h = (float *)malloc(sizeof(float) * *ourNum_h);
    ourPosY_h = (float *)malloc(sizeof(float) * *ourNum_h);
    ourIds_h = (int *)malloc(sizeof(int) * *ourNum_h);

    for(int i = 0 ; i < tmmIDs.size() ; i++)
        ourIds_h[i] = tmmIDs[i];
    ourIds_h[ tmmIDs.size() ] = pWorldData->my_id();

    std::sort(ourIds_h , ourIds_h + *ourNum_h);

    for(int i = 0 ; i < *ourNum_h ; i++)
    {
        if(ourIds_h[i] == pWorldData->my_id())
        {
            ourPosX_h[i] = pWorldData->me(true).getX() ;
            ourPosY_h[i] = pWorldData->me(true).getY() ;
        }
        else
        {
            ourPosX_h[i] = pWorldData->basedCenter(pWorldData->teammate(ourIds_h[i])).getX();
            ourPosY_h[i] = pWorldData->basedCenter(pWorldData->teammate(ourIds_h[i])).getY();
        }
    }

    for(int i = 0 ; i < *oppNum_h ; i++)
    {
        oppPosX_h[i] = pWorldData->basedCenter(pWorldData->opponent(oppIDs[i])).getX();
        oppPosY_h[i] = pWorldData->basedCenter(pWorldData->opponent(oppIDs[i])).getY();
    }

    *ourGoalX_h = pWorldData->ourPole1.getX() ;

    for (int i = 0 ; i < 5 * 128 ; i++ )
        values_h[i] = 0 ;

    //*myId_h = pWorldData->my_id() ;
    *caller_h = callerId ;

    *ballX_h = ballPos.getX() ;
    *ballY_h = ballPos.getY() ;
    
    //Device define :
    cudaPrintfInit();
    float* oppPosX_d ;
    float* oppPosY_d ;
    int* oppNum_d ;
    int* caller_d ;
    //int* myId_d ;
    float* ourPosX_d ;
    float* ourPosY_d ;
    int* ourNum_d = (int *)malloc(sizeof(int)) ;
    float* ballX_d = (float *)malloc(sizeof(float)) ;
    float* ballY_d = (float *)malloc(sizeof(float)) ;
    int* ourIds_d = (int *)malloc(sizeof(int) * *ourNum_h) ;
    float* ourGoalX_d = (float *)malloc(sizeof(float)) ;
    float* values_d = (float *)malloc(sizeof(float) * 5 * 128) ;

    cudaMalloc((void**)&oppPosX_d , sizeof(float) * *oppNum_h);
    cudaMalloc((void**)&oppPosY_d , sizeof(float) * *oppNum_h);
    cudaMalloc((void**)&oppNum_d , sizeof(int));
    cudaMalloc((void**)&caller_d , sizeof(int));
    //cudaMalloc((void**)&myId_d , sizeof(int));
    cudaMalloc((void**)&ourPosX_d , sizeof(float) * *ourNum_h);
    cudaMalloc((void**)&ourPosY_d , sizeof(float) * *ourNum_h);
    cudaMalloc((void**)&ourNum_d , sizeof(int));
    cudaMalloc((void**)&ballX_d , sizeof(float));
    cudaMalloc((void**)&ballY_d , sizeof(float));
    cudaMalloc((void**)&ourIds_d , sizeof(int) * *ourNum_h);
    cudaMalloc((void**)&ourGoalX_d , sizeof(float));
    cudaMalloc((void**)&values_d , sizeof(float) * 5 * 128);
    
    //HostToDevice :
    cudaMemcpy(oppPosX_d , oppPosX_h , sizeof(float) * *oppNum_h, cudaMemcpyHostToDevice);
    cudaMemcpy(oppPosY_d , oppPosY_h , sizeof(float) * *oppNum_h, cudaMemcpyHostToDevice);
    cudaMemcpy(oppNum_d , oppNum_h , sizeof(int) , cudaMemcpyHostToDevice);
    cudaMemcpy(caller_d , caller_h , sizeof(int) , cudaMemcpyHostToDevice);
    //cudaMemcpy(myId_d , myId_h , sizeof(int) , cudaMemcpyHostToDevice);
    cudaMemcpy(ourPosX_d , ourPosX_h , sizeof(float) * *ourNum_h , cudaMemcpyHostToDevice);
    cudaMemcpy(ourPosY_d , ourPosY_h , sizeof(float) * *ourNum_h , cudaMemcpyHostToDevice);
    cudaMemcpy(ourNum_d , ourNum_h , sizeof(int) , cudaMemcpyHostToDevice);
    cudaMemcpy(ballX_d , ballX_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(ballY_d , ballY_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(ourIds_d , ourIds_h , sizeof(int) *  *ourNum_h, cudaMemcpyHostToDevice);
    cudaMemcpy(ourGoalX_d , ourGoalX_h , sizeof(float) , cudaMemcpyHostToDevice);
    cudaMemcpy(values_d , values_h , sizeof(float) * 5 * 128 , cudaMemcpyHostToDevice);
    
    //Call the Kernel :
    dim3 dimGrid(5);
    dim3 dimBlock(128);
    bestTeammateFinder<<<dimGrid , dimBlock>>>(oppPosX_d , oppPosY_d , oppNum_d , caller_d , ourPosX_d , ourPosY_d , ourNum_d , ballX_d , ballY_d , ourIds_d , ourGoalX_d , values_d);
    cudaThreadSynchronize();
    //DeviceToHost :
    cudaMemcpy(values_h , values_d , sizeof(float) * 5 * 128 , cudaMemcpyDeviceToHost);
    //CompareValues :

                                    //not done yet

    //CleanUp :
    cudaFree(oppPosX_d);
    cudaFree(oppPosY_d);
    cudaFree(oppNum_d);
    cudaFree(caller_d);
    //cudaFree(myId_d);
    cudaFree(ourPosX_d);
    cudaFree(ourPosY_d);
    cudaFree(ourNum_d);
    cudaFree(ballX_d);
    cudaFree(ballY_d);
    cudaFree(ourIds_d);
    cudaFree(ourGoalX_d);
    cudaFree(values_d);

    free(oppPosX_h);
    free(oppPosY_h);
    free(oppNum_h);
    free(caller_h);
    //free(myId_h);
    free(ourPosX_h);
    free(ourPosY_h);
    free(ourNum_h);
    free(ballX_h);
    free(ballY_h);
    free(ourIds_h);
    free(ourGoalX_h);
    free(values_h);
    
    cudaPrintfDisplay(stdout, true);
    cudaPrintfEnd();
}