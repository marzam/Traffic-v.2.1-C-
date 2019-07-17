   /*
 *  CellularAutomata.cpp
 *  TCA
 *
 *  Created by Marcelo Zamith on 3/15/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <CellularAutomata.hpp>
//#include <TModelCA.hpp>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <ctime>
#include <climits>
#include <thread>
using namespace std;
/*
 * #include <thread>
 * unsigned int nthreads = std::thread::hardware_concurrency();
 */
/*
 *
 */
CellularAutomata::CellularAutomata():
pThreadClass::pThreadClass()
// mRules(NULL),
// mState(0),
// mpStep(0),
// mdGlobal(0.0f)
{ cout << "\nStarting application" << endl; }

/*
 *
 */
CellularAutomata::~CellularAutomata()
{ cout << "\nFinalizing application" << endl;  }


void CellularAutomata::initDefault(void){
/*
    if (mParam->modelName.compare ("TModel_ML_SYM") == 0)
      mRules = new TModel_ML_SYM();

    assert(mRules != NULL);
    //mRules = new TModel();
    mRules->setParam(mParam);
    mRules->setSensor(mSensor);
    mRules->setGrid(mGrid);



    mRules->getGrid()->allocVehicleType(mParam->nProfiles);
    mRules->getGrid()->getVehicleType(0)->percent = 1.0f;
    mRules->getSensor()->setSaveFiles(mRules->getParam()-> photo, mRules->getParam()->fixed);

    memset( mRules->getGrid()->getVehicleType(0)->param, 0x00, sizeof(double) * 5 ); //It indicates the number of parameters of beta function. Considering two policy (acceleration and safe distance)/
    strcpy(mRules->getGrid()->getVehicleType(0)->description, "Default");
    mRules->getGrid()->getVehicleType(0)->param[0] = 8;
    mRules->getGrid()->getVehicleType(0)->param[1] = 7;
    mRules->getGrid()->getVehicleType(0)->inc = 4;
    mRules->getGrid()->getVehicleType(0)->desc = 1;
    mRules->getGrid()->getVehicleType(0)->size = 5;
    mRules->getGrid()->getVehicleType(0)->vMax = 25;
    mRules->getGrid()->getVehicleType(0)->aheadInt = 6.0;
    mRules->getGrid()->getVehicleType(0)->safeDist = 2.0f;
    mRules->getGrid()->getVehicleType(0)->left_p = 1.0f;
    mRules->getGrid()->getVehicleType(0)->right_p = 1.0f;
    mRules->getGrid()->getVehicleType(0)->param[2] = gamaFunction(mRules->getGrid()->getVehicleType(0)->param[0]);
    mRules->getGrid()->getVehicleType(0)->param[3] = gamaFunction(mRules->getGrid()->getVehicleType(0)->param[1]);
    mRules->getGrid()->getVehicleType(0)->param[4] = gamaFunction(mRules->getGrid()->getVehicleType(0)->param[0]+mRules->getGrid()->getVehicleType(0)->param[1]);


    //--------------------------------------
    //Printing information about parameters

    std::cout << " Cellular automata parameters: \n";
    std::cout << " Model = " << mRules->getParam()->modelName << std::endl;
    std::cout << " cellX  = " << mRules->getParam()->cellX << std::endl;
    std::cout << " cellY  = " << mRules->getParam()->cellY << std::endl;
    std::cout << " DeltaH = " << mRules->getParam()->deltaH << std::endl;
    std::cout << " Dece   = " << mRules->getParam()->deceleration << std::endl;
    std::cout << " sTime        = " << mRules->getParam()->sTime << std::endl;
    std::cout << " dTime        = " << mRules->getParam()->dTime << std::endl;
    std::cout << " stTime       = " << mRules->getParam()->stTime << std::endl;
    //std::cout << " deltaTime    = " << mRules->getParam()->deltaTime << std::endl;
    std::cout << " iDensity     = " << fixed << setprecision(5) << mRules->getParam()->iDensity << std::endl;
    std::cout << " fDensity     = " << mRules->getParam()->fDensity << std::endl;
    std::cout << " dDensity     = " << mRules->getParam()->dDensity << std::endl;
    std::cout << " vMax         = " << mRules->getParam()->vMax << std::endl;
    std::cout << " deceleration = " << mRules->getParam()->deceleration << std::endl;
    std::cout << " spc. perc.   = " << fixed << setprecision(1) << mRules->getParam()->spacePerception << std::endl;

    std::cout << " Log files: " << std::endl;
    std::cout << " \tfixed           = " << std::boolalpha  << mRules->getParam()->fixed << std::endl;
    std::cout << " \tphoto           = " << std::boolalpha  <<  mRules->getParam()->photo << std::endl;
    std::cout << " \tlogCluster      = " << std::boolalpha  << mRules->getParam()->logCluster << std::endl;
    std::cout << " \tlast Vehicle(s) = " << std::noboolalpha  << mRules->getParam()->logVehicles << " *[negative values indicate no log/positives indicates number of last vehicles]" << std::endl;



    std::cout << "=====================================================" << std::endl << std::endl;
    std::cout << fixed << setprecision(0);
    std::cout << "Groups of vehicles: " << mRules->getGrid()->getVehiclesTypeSize() << std::endl;
    for (int i = 0; i < mRules->getGrid()->getVehiclesTypeSize(); i++){
        tpVehiclesType *type =  mRules->getGrid()->getVehicleType(i);
        std::cout << "Profile: " << type->description << " " \
        << "Percent: " << fixed << setprecision(2) << type->percent \
        << " Size: " << type->size \
        << " vMax: " << type->vMax \
        << " Aheadway: " << type->aheadInt \
        << " Safe dist.: " << type->safeDist \
        << " Alpha parameter ACC(" << setprecision(0) << type->param[0] << ", " << type->param[1] << ") "
                  << " Inc: " << setprecision(0) << type->inc \
        << " Desc: " << type->desc \
        << " Left prob.:" <<   setprecision(2) << type->left_p << " Right prob." << type->right_p \
        << std::endl;
        
        if (type->vehicleLog){
                std::cout << "\t Vehicle range: " << type->lo_ID << "," << type->hi_ID << " file sufix: " << type->sufix << std::endl;
        }
    }
    std::cout.flush();
      */
  



}


/**
 * Finalize and dealloc all variables.
 * It also saves log file
 */
void CellularAutomata::finalize(void){
/*
    mRules->getGrid()->clearMemories();
    mRules->finalizer();
    if (mRules != NULL)
        delete mRules;
*/
};

/*
 * Running the simulation
 */

void CellularAutomata::update(void){
/*
    if (mState == 0){


        mdGlobal = mRules->getParam()->iDensity;
        mpStep   = mRules->getParam()->sTime / 20;

        if (mpStep == 0)
            mpStep = 1;

        mState++;
        return;
    }

    if ((mState >= 1) && (mState <= 2)){
        if (mdGlobal <  mRules->getParam()->fDensity){
            if (mState == 1){

                mRules->initialCondition(mdGlobal);

                //if (mRules->getParam()->roadBlock)
//                   mRules->addONEObstables();

                mRules->getSensor()->clear();

                std::cout << std::endl << "Density: " << std::setiosflags(std::ios::fixed) << \
                    std::setprecision(4) << mdGlobal << "/" << mRules->getParam()->fDensity << \
                    std::endl;

                mRules->update(0, false, true);// update(0, dGlobal);

                START_STOPWATCH(mStopwatch);
                mStep = 1;
                mState++;
                return;
            }//end- if (mState == 1){

            if (mState == 2){
                if (mStep <= mRules->getParam()->sTime){
//-------------------------------------------------------------------
                    if ((mStep % mpStep) == 0){
                        std::cout << ".";
                        std::cout.flush();
                    }

                    if (mStep >= mRules->getParam()->dTime){
                        if (mStep == mRules->getParam()->dTime){
                            std::cout << "|";


                            mRules->setStatistic(true);
                        }




                    }


                    mRules->applyRules(mStep);
                    mStep++;
                    return;
//-------------------------------------------------------------------
                }else{
                   mState = 1;
                   mdGlobal += mRules->getParam()->dDensity;
                   mRules->finalCondition();
                   STOP_STOPWATCH(mStopwatch);
                   std::cout << "\t Elapsed time:" << std::fixed << std::setw(18) << std::setprecision(5) << mStopwatch.mElapsedTime;

                   return;
                }//end-if (mStep <= mRules->getParam()->sTime){
            }//end- if (mState == 2){


        }else{
            mState = 3;
            return;
        }//end-if (mdGlobal <  mRules->getParam()->fDensity){
    }//end-if ((mState >= 1) && (mState <= 2)){

    if (mState == 3){
        mRunning = false;
        cout << endl << "[End the game!]" << endl;
        return;
    }//end-if (mState == 3){

*/



};

void CellularAutomata::debugAllVehicles(void){
/*
  Entity *pEntity = mRules->getGrid()->getEntities();
    while (pEntity != NULL){
        std::cerr << "V1(ID= " << pEntity->ID << ",X= "
            <<  pEntity->x  << ",Y= "
            <<  pEntity->y  << std::endl;
        pEntity = pEntity->next;
    }
    */
}

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void CellularAutomata::debug(int time){
//     return;
 /*  
  fstream dLog;
    dLog.open("Debug.log", fstream::out | fstream::app);
    assert(dLog.is_open());
    dLog << "[" << time << "]";
    for (int j = 0; j < mRules->getParam()->cellY; j++){


    	for (int i = 0; i < mRules->getParam()->cellX;  i++){
            Entity *e = mRules->getGrid()->getGrid(i, j);
            if (e != NULL){
               if (e->whoAmI().compare("Vehicle") == 0){
                   Vehicle *v = dynamic_cast <Vehicle *>(e);
//                   dLog << "(" << v->ID << "," << v->vx << ")";
                   dLog << v->vx ;

               }else if (e->whoAmI().compare("Obstacle") == 0){

                   dLog << "#";


               }


            }else
               dLog << ".";


    	}
        dLog << endl;
    	//cerr << endl;

    }
    dLog << endl;
    dLog.close();
*/

};
void*CellularAutomata::execThread(void){
  cout << "Waiting...." << endl;
  pthread_barrier_wait(m_Barrier);
  cout << "Hello, I'm thread: " << mMyThread << endl;
  cout.flush();
  pthread_barrier_wait(m_Barrier);
  return NULL;
};

/*
* Master class that managers threads
*
*/
// Including an array of behaviors comming from external call !!!
MasterCellularAutomata::MasterCellularAutomata(tpParam param, tpVehiclesType *vt, int vtsize, int t):
mGrid(NULL),
mVehiclesType(NULL)
{
  mParam = param;



  if (t == 0)
    mNumberOfThreads = thread::hardware_concurrency();
  else
   mNumberOfThreads = t;

  cout << "\nNumber of threads: " << mNumberOfThreads << endl;
//  assert(posix_memalign((void**) &mPCA, ALIGN, mNumberOfThreads * sizeof(CellularAutomata)) == 0);
  mPCA = new CellularAutomata[mNumberOfThreads];
  assert(pthread_barrier_init(&m_Barrier, NULL, mNumberOfThreads + 1) == 0);
  
  if (mGrid != NULL)
    free(mGrid);
 
//Allocing memories
   assert(posix_memalign((void**) &mGrid, ALIGN, mParam.cellX * mParam.cellY *  sizeof(uintptr_t)) == 0);
   assert(mGrid != NULL);

  mNumTypes = vtsize;

  if (mVehiclesType != NULL){
    free(mVehiclesType);
  }

  assert(posix_memalign((void**) &mVehiclesType, ALIGN, mNumTypes *  sizeof(tpVehiclesType)) == 0);
  assert(mVehiclesType != NULL);
   
  mEntities = NULL;
  

  for (unsigned int i = 0; i < mNumberOfThreads; i++){
     //mVetThread[i] = pThreadClass();
     mPCA[i].m_Barrier = &m_Barrier;
     mPCA[i].create();
  }

  cin.get();
  pthread_barrier_wait(&m_Barrier);
  cin.get();
  pthread_barrier_wait(&m_Barrier);
  for (unsigned int i = 0; i < mNumberOfThreads; i++)
           mPCA[i].wait();
}

MasterCellularAutomata::~MasterCellularAutomata(void)
{
  cout << "MasterCellularAutomata::~MasterCellularAutomata(void)" << endl;
  delete[] mPCA;
  
  if (mGrid != NULL)  free(mGrid);
  if (mVehiclesType != NULL)  free(mVehiclesType);
  assert(pthread_barrier_destroy(&m_Barrier) == 0);
//  pThreadClass::~pThreadClass();
}

void MasterCellularAutomata::initialCondition(float dGlobal){
    //Define how many vehicles will be on the road
    Vehicle vehicle;
/*
 * Observation 2017, March 18th
 * For each vehicle Verifying if it is in a list of log vehicles vehicleLog
 * To do this: Verifying if there is a substring with the number.
 *             Set true vehicle parameter
 *             Put in Vehicle.cpp code to log the vehicle performance
 *
 */
    int totalVehicles = 0,
        *vehiclesTypes = new int[getGrid()->getVehiclesTypeSize()];
    mStatistic = false;

    for (int i = 0; i < getGrid()->getVehiclesTypeSize(); i++){
        tpVehiclesType *type  = getGrid()->getVehicleType(i);

        float dVehicle  = type->percent * dGlobal / static_cast<float> (type->size);

        int   nVehicles = static_cast <int> (dVehicle * static_cast <float> (mParam->cellX));

        if (nVehicles == 0) nVehicles = 1;

        totalVehicles += nVehicles;

        vehiclesTypes[i] = nVehicles;
    }
    totalVehicles *= mParam->cellY;

    getGrid()->clearMemories();

    if (mParam->logVehicles > 0)
      assert(posix_memalign((void**) &mLastVehicles, ALIGN, mParam->logVehicles *  sizeof(Vehicle*)) == 0);

    int lIndex = 0;
    for (int road = 0; road < mParam->cellY; road++){
        int cell = 0;
        for (int i = 0; i < getGrid()->getVehiclesTypeSize(); i++){
            tpVehiclesType *type  = getGrid()->getVehicleType(i);
            int length    = type->size;
            int nVehicles = vehiclesTypes[i];
            for(int iveicles = 0; iveicles < nVehicles; iveicles++){
                Vehicle *vehicle = new Vehicle();

                vehicle->ID = getGrid()->getID();

                vehicle->x = cell + length - 1;
                vehicle->vx = 0;
                vehicle->vxNew = 0;
                vehicle->y = road;
                vehicle->vy = 0;
                vehicle->yChange = false;
                vehicle->mLights = false;
                vehicle->lg = type->size;
                vehicle->myDensity = dGlobal;
                vehicle->mSensor = this->getSensor();
                vehicle->next = NULL;
                vehicle->prev = NULL;
                vehicle->type = getGrid()->getVehicleType(i);
                vehicle->defineLattice(mParam->cellX, mParam->cellY);
                vehicle->save = false;
                vehicle->mDV = 0;
                vehicle->mDIST = 0;
                vehicle->setFileSufix(mParam->modelName);
                if (mParam->logVehicles > 0){
                  if (iveicles >= (nVehicles - mParam->logVehicles)){
                      vehicle->save = true;
                      mLastVehicles[lIndex++] = vehicle;

                  }//end-if (iveicles == (nVehicles - 1 - mParam->logVehicles)){
                }//end-                if (mParam->logVehicles > 0){

                getGrid()->addEntity(vehicle);
                cell += length;

            }

        }
    }


    delete[] vehiclesTypes;
    vehiclesTypes = NULL;


    //Setting up sensor
    mSensor->setFileName(mParam->modelName);
    mSensor->setGrid(this->getGrid());
    mSensor->reset();


    mReAjusted = 0;



}
//--------- Functions
double gamaFunction (double n){

    unsigned long in1 = static_cast <unsigned long> (n-1),
    iacc = in1;

    if (n <= 2.0f) return 1.0f;

    for (unsigned long i = in1-1 ; i > 0 ; i--)
        iacc *= i;

    return static_cast <double> (iacc);
}
