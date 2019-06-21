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
 *
 */
CellularAutomata::CellularAutomata():
mRules(NULL),
mState(0),
mRunning(true),
mpStep(0),
mdGlobal(0.0f)
{ cout << "\nStarting application" << endl; }

/*
 *
 */
CellularAutomata::~CellularAutomata()
{ cout << "\nFinalizing application" << endl;  }


void CellularAutomata::initDefault(void){

    mParam.modelName = "DefaultModel";
    mRules = new TModel();
    mRules = new TModel_ML_SYM();
    mRules->setParam(&mParam);
    mRules->setSensor(&mSensor);
    mRules->setGrid(&mGrid);

    mParam.logVehicles = 0;
    mParam.deceleration = 3;
    mParam.spacePerception = 12;
    mParam.vMax = 25;
    mParam.cellX = 10000;
    mParam.cellY = 1;
    mParam.deltaH = 1.5f;
    mParam.defaultSize = 7.5;
    mParam.sTime = 3600 * 5;
    mParam.dTime = 3600;
    mParam.stTime = 120;
    mParam.iDensity = 0.01f;
    mParam.dDensity = 0.01f;
    mParam.fDensity = 0.95f;
    mParam.photo = true;
    mParam.fixed = true;
    mParam.logCluster = true;
    mParam.nProfiles = 1;

    mRules->getGrid()->allocVehicleType(mParam.nProfiles);
    mRules->getGrid()->getVehicleType(0)->percent = 1.0f;
    mRules->getSensor()->setSaveFiles(mRules->getParam()-> photo, mRules->getParam()->fixed);

    assert(posix_memalign((void**) &mGrid.grid, ALIGN, mParam.cellX * mParam.cellY *  8) == 0);
    mGrid.allocGrid(mParam.cellX, mParam.cellY);



    memset( mRules->getGrid()->getVehicleType(0)->param, 0x00, sizeof(double) * 5 ); //It indicates the number of parameters of beta function. Considering two policy (acceleration and safe distance)/
    strcpy(mRules->getGrid()->getVehicleType(0)->description, "Default");
    mRules->getGrid()->getVehicleType(0)->param[0] = 8;
    mRules->getGrid()->getVehicleType(0)->param[1] = 7;
    mRules->getGrid()->getVehicleType(0)->inc = 4;
    mRules->getGrid()->getVehicleType(0)->desc = 1;
    mRules->getGrid()->getVehicleType(0)->size = 5;
    mRules->getGrid()->getVehicleType(0)->vMax = 25;
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
        << " Alpha parameter ACC(" << setprecision(0) << type->param[0] << ", " << type->param[1] << ") "
                  << " Inc: " << setprecision(0) << type->inc \
        << " Desc: " << type->desc \
        << " Left prob.:" <<   setprecision(2) << type->left_p << " Right prob." << type->right_p \
        << std::endl;
        /*
        if (type->vehicleLog){
                std::cout << "\t Vehicle range: " << type->lo_ID << "," << type->hi_ID << " file sufix: " << type->sufix << std::endl;
        }
        */
    }
    std::cout.flush();




}


/**
 * Finalize and dealloc all variables.
 * It also saves log file
 */
void CellularAutomata::finalize(void){

    mRules->getGrid()->clearMemories();
    mRules->finalizer();
    if (mRules != NULL)
        delete mRules;

};

/*
 * Running the simulation
 */

void CellularAutomata::update(void){

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





};

void CellularAutomata::debugAllVehicles(void){
    Entity *pEntity = mRules->getGrid()->getEntities();
    while (pEntity != NULL){
        std::cerr << "V1(ID= " << pEntity->ID << ",X= "
            <<  pEntity->x  << ",Y= "
            <<  pEntity->y  << std::endl;
        pEntity = pEntity->next;
    }
}

/*
 * It is a method that depends on what king of information I would show.
 * It means: I can see on the screen or saved into a file.
 */
void CellularAutomata::debug(int time){
//     return;
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


};
